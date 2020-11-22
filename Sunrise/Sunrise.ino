#include <Time.h>
#include <TimeLib.h>

#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

const char *ssid     = "Charterfelds";
const char *password = "855VancouverStreet";

WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds cache update interval and
// no timezone offset
//NTPClient timeClient(ntpUDP);

// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
NTPClient timeClient(ntpUDP, "pool.ntp.org", -28800, 60000);

const uint16_t PixelCount = 46; // the number of pixeles in the strip
long startTime =0;
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);

// what is stored for state is specific to the need, in this case, the colors.
// Basically what ever you need inside the animation update function
struct KeyFrame
{
    RgbColor color;
    long targetTime; //elapsed time (in millis) that this frame should be 100%
};

const int numOfSunriseKeyFrames = 7;
struct KeyFrame sunriseArray[numOfSunriseKeyFrames]={
  {RgbColor(0,0,0),0}, //start black
  {RgbColor(15,0,10),100}, //fade up to dark red over a minute
  {RgbColor(100,35,0),900},// dark orange
  {RgbColor(156,95,1), 1800}, //strong orange
  {RgbColor(239,150,1),2700},//move towards yellow
  {RgbColor(255,200,100),3600}, //full bright (with yellow tinge)
  {RgbColor(30,30,10),4500}, //fade out
};
// 1 hr is 3,600,000 milliseconds
struct KeyFrame start = {RgbColor(0,0,0),0};
struct KeyFrame frame1 = {RgbColor(0,0,35),1000};
struct KeyFrame frame2 = {RgbColor(0,200,0),4000};
struct KeyFrame endFrame = {RgbColor(40,40,40),7000};

void updateBoard(RgbColor colorToSet){
    for (uint16_t i = 0; i < PixelCount; i += 1)
        {
            strip.SetPixelColor(i, colorToSet);
        }

   strip.Show();
}

RgbColor pickColorToSet(KeyFrame frameArray[], long elapsedTime){
  if(elapsedTime<1){
    elapsedTime = 1; //to stop divide-by-zero errors
  }
  if(elapsedTime<frameArray[0].targetTime){
    return RgbColor::LinearBlend(RgbColor(0,0,0), frameArray[0].color, float(frameArray[0].targetTime/(float)elapsedTime)); //fade in first frame from black
  }
  
  for(int i=1; i<numOfSunriseKeyFrames; i++){
   if(elapsedTime<frameArray[i].targetTime){
    long millisSinceLastFrameSwitch = elapsedTime - frameArray[i-1].targetTime;
    long elapsedTimeBetweenFrames = frameArray[i].targetTime-frameArray[i-1].targetTime;
    float progress = ((float)millisSinceLastFrameSwitch/(float)(elapsedTimeBetweenFrames));
    return RgbColor::LinearBlend(frameArray[i-1].color, frameArray[i].color, progress);
   }
  }
  return RgbColor(0,0,0); //return black/off when elapsed time is over the last frame target
}

void setup() {
//****** set up Serial ************
    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

//****** set up wifi and NTP ************
    WiFi.begin(ssid, password);

    while ( WiFi.status() != WL_CONNECTED ) {
      delay ( 500 );
      Serial.print ( "." );
    }
    timeClient.begin();
    timeClient.update();
//****** set board; resets all the neopixels to an off state  ************
    strip.Begin();
    strip.Show();

    Serial.println();
    Serial.println("Running...");

    delay(2000);
    startTime = 0; //will force sunrise default to last frame
    Serial.println(startTime);
    Serial.println(timeClient.getFormattedTime());
    setTime(timeClient.getEpochTime());
    Serial.println("time lib: "+String(hour())+" "+String(minute())+" "+String(second()));
}



void loop() {

 //update time twice a day
  if(String(hour()).equals("5") && String(minute()).equals("30")){
     timeClient.update(); //uses cache time specified in constructor so won't hammer. 
     setTime(timeClient.getEpochTime());
  }
  if(String(hour()).equals("20") && String(minute()).equals("30")){
     timeClient.update(); //uses cache time specified in constructor so won't hammer. 
     setTime(timeClient.getEpochTime());
  }
  if(String(hour()).equals("8") && String(minute()).equals("45")){
    startTime = timeClient.getEpochTime(); //start the sunrise sequence
  }

  // put your main code here, to run repeatedly:
  updateBoard(pickColorToSet(sunriseArray,(timeClient.getEpochTime()-startTime)));
}
