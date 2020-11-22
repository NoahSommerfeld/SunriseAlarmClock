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
NTPClient timeClient(ntpUDP);

const uint16_t PixelCount = 46; // the number of pixeles in the strip

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
  {RgbColor(85,0,10),6000}, //fade up to dark red over a minute
  {RgbColor(110,49,0),90000},// dark orange
  {RgbColor(156,95,1), 180000}, //strong orange
  {RgbColor(239,150,1),270000},//move towards yellow
  {RgbColor(255,200,100),360000}, //full bright (with yellow tinge)
  {RgbColor(30,30,10),450000}, //fade out
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
  return RgbColor(0,0,0); //return black/off
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

//****** set board; resets all the neopixels to an off state  ************
    strip.Begin();
    strip.Show();

    Serial.println();
    Serial.println("Running...");

}

void loop() {
  timeClient.update(); //uses cache time specified in constructor
  Serial.println(timeClient.getFormattedTime());

  // put your main code here, to run repeatedly:
  updateBoard(pickColorToSet(sunriseArray,millis()));
}
