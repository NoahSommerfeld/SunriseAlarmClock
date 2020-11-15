#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

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
  {RgbColor(5,0,20),3000}, //fade up to dark purple
  {RgbColor(63,22,24),8000},// move to pink
  {RgbColor(125,62,5),12000}, //strong orange
  {RgbColor(255,234,108),16000},//yellow of the morning
  {RgbColor(255,255,170),20000}, //full bright (with yellow tinge)
  {RgbColor(30,30,30),29000}, //fade out
};

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
  sunriseArray[0].color = RgbColor(0,0,0);

    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();


    Serial.println();
    Serial.println("Running...");
    //pickColorToSet(sunriseArray, millis());
}

void loop() {
  
  // put your main code here, to run repeatedly:
  updateBoard(pickColorToSet(sunriseArray,millis()));
}
