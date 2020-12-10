#include <Time.h>
#include <TimeLib.h>
#include "Config.h" //password and time values in local folder
#include <NTPClient.h>
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
#include <WiFiUdp.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>


//************ GLOBAL VALUES ********************
//Wifi values
const char *ssid     = CONFIG_WIFI_SSID; //pulled from Config file - update to your implementation
const char *password = CONFIG_WIFI_PASSWORD;
WiFiUDP ntpUDP;

// Sets the time server pool and the offset, (in seconds) and you can specify the update interval (in milliseconds).
NTPClient timeClient(ntpUDP, "pool.ntp.org", -28800, 60000); //offset value is Pacific Winter time

// Alternatively you can use the default settings )'pool.ntp.org' is used, with 60 seconds cache update interval, and
// no timezone offset
//NTPClient timeClient(ntpUDP);

//Board values
const uint16_t PixelCount = 46; // the number of pixeles in the strip
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);

// The struct represents a color at a point in time
// Basically what ever you need inside the animation update function
struct KeyFrame
{
    RgbColor color;
    long targetTime; //when (time measured in elapsed seconds)that the frame's color will be shown exactly on the board
};

long startedTime =0; //the UNIX timestamp at which the sequence began (i.e sunrise)

//The following array represents the sunrise sequence
const int numOfSunriseKeyFrames = 8;
struct KeyFrame sunriseArray[numOfSunriseKeyFrames]={
  {RgbColor(0,0,0),0}, //start black
  {RgbColor(5,0,0),100}, //fade up to dark red over a minute
  {RgbColor(45,10,0),900},// dark orange
  {RgbColor(98,30,1),1800}, //strong orange
  {RgbColor(150,45,5),2700},//move towards yellow
  {RgbColor(225,100,25),3600}, //full bright (with yellow tinge)
  {RgbColor(225,100,25),4500}, //hold full bright
  {RgbColor(30,30,10),4700}, //fade out
};
// 1 hr is 3,600 seconds, and 900 seconds is 15 minutes. 



//**************METHODS ***********************
//Applies the specified color to the board
void updateBoard(RgbColor colorToSet){
    for (uint16_t i = 0; i < PixelCount; i += 1)
        {
            strip.SetPixelColor(i, colorToSet);
        }

   strip.Show();
}

//Returns the calculated color to display, given the target array and elapsed time (linear progression)
RgbColor pickColorToSet(KeyFrame frameArray[], long elapsedTime){
  if(elapsedTime<1){
    elapsedTime = 1; //to stop divide-by-zero errors
  }
  if(elapsedTime<frameArray[0].targetTime){ //if the time for the first frame hasn't arrived yet
    return RgbColor::LinearBlend(RgbColor(0,0,0), frameArray[0].color, float(frameArray[0].targetTime/(float)elapsedTime)); //fade in first frame from black (off)
  }
  
  for(int i=1; i<numOfSunriseKeyFrames; i++){
   if(elapsedTime<frameArray[i].targetTime){
    long millisSinceLastFrameSwitch = elapsedTime - frameArray[i-1].targetTime;
    long elapsedTimeBetweenFrames = frameArray[i].targetTime-frameArray[i-1].targetTime;
    float progress = ((float)millisSinceLastFrameSwitch/(float)(elapsedTimeBetweenFrames));
    return RgbColor::LinearBlend(frameArray[i-1].color, frameArray[i].color, progress);
   }
  }
  
  return RgbColor(0,0,0); //return black (off) when elapsed time is over the last frame target
}

void setup() {
// set up Serial
    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

// set board; resets all the neopixels to an off state 
    strip.Begin();
    strip.Show();


//set up wifi and NTP
    WiFi.begin(ssid, password);
    updateBoard(RgbColor(0,0,0));
    while ( WiFi.status() != WL_CONNECTED ) { 
      strip.SetPixelColor(5,RgbColor(10,10,10)); //indicator LED to confirm power and Wifi connection
      strip.Show();
      delay ( 250 );
      Serial.print ( "." );
      strip.SetPixelColor(5,RgbColor(0,0,0));
      strip.Show();
      delay (250);
    }
    updateBoard(RgbColor(0,0,0));
    timeClient.begin();
    timeClient.update();
    

    Serial.println();
    Serial.println("Running...");

    delay(2000);
    startedTime = 0; //will set the elapsed time to be so long (>24 hours) that the board will be over the last frame and black (off)
    Serial.println(startedTime);
    Serial.println(timeClient.getFormattedTime());
    setTime(timeClient.getEpochTime());
    Serial.println("time lib: "+String(hour())+" "+String(minute())+" "+String(second()));
}



void loop() {

 //sync time twice a day. Could add an error indicator (make board go full red?)
  if(String(hour()).equals("5") && String(minute()).equals("30")){
     timeClient.update(); //uses cache time specified in constructor so won't hammer. 
     setTime(timeClient.getEpochTime());
  }
  if(String(hour()).equals("20") && String(minute()).equals("30")){
     timeClient.update(); //uses cache time specified in constructor so won't hammer. 
     setTime(timeClient.getEpochTime());
  }

  //Set the 'started time' to current time (which resets the elapsed time to 0 and starts the sequence)
  if(String(hour()).equals(CONFIG_SUNRISE_START_HOUR) && String(minute()).equals(CONFIG_SUNRISE_START_MINUTE)){
    startedTime = timeClient.getEpochTime(); //start the sunrise sequence
    Serial.println("starting");
    delay(1000);
  }

  // THIS IS THE KEY LINE - for each iteration, set the board to be the next color in the sequence (determined by elapsed time since StartedTime)
  updateBoard(pickColorToSet(sunriseArray,(timeClient.getEpochTime()-startedTime)));
}
