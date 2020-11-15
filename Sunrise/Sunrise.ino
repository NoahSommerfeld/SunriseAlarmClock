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

//red, green, blue
struct KeyFrame start = {RgbColor(0,0,0),0};
struct KeyFrame frame1 = {RgbColor(100,0,0),1000};
struct KeyFrame frame2 = {RgbColor(0,150,0),4000};
struct KeyFrame endFrame = {RgbColor(0,0,35),7000};

void updateBoard(RgbColor colorToSet){
    for (uint16_t i = 0; i < PixelCount; i += 1)
        {
            strip.SetPixelColor(i, colorToSet);
        }

   strip.Show();
}

void setup() {
  
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

}

void loop() {
  // put your main code here, to run repeatedly:
  long currentTime = millis();
  RgbColor toSet;
  if(currentTime<frame1.targetTime){
    float progress = ((float)currentTime/(float)frame1.targetTime); //will be at 100% when elapsed time reaches frame1's targettime
    toSet = RgbColor::LinearBlend(start.color, frame1.color, progress);
  }
  else if(currentTime<frame2.targetTime){
    long millisSinceLastFrameSwitch = currentTime - frame1.targetTime;
    float progress = ((float)millisSinceLastFrameSwitch/(float)frame2.targetTime);
    toSet = RgbColor::LinearBlend(frame1.color, frame2.color, progress);
    
  }
  else if(currentTime<endFrame.targetTime){
    long millisSinceLastFrameSwitch = currentTime - frame2.targetTime;
    float progress = ((float)millisSinceLastFrameSwitch/(float)endFrame.targetTime);
    toSet = RgbColor::LinearBlend(frame2.color, endFrame.color, progress);
  }
  else{
    toSet = endFrame.color;
  }

  updateBoard(toSet);
  delay(50); // avoid hammering the board
}
