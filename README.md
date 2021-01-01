# SunriseAlarmClock
The Sunrise Alarm Clock project is designed to create a mood-lighting solution that simulates sunrise and sunset using internet-connected, Arduino-compatible LED boards ('NodeLED' project). It gets bright enough to awaken around the 15 minute mark, reaches full brightness ('daylight') at 1 hour, and fades off about 15 minutes after that. 

# Setup
1. Set up the board connected to Power with a barrel-jack power adapter, and the PC via a data-capable microUSB-USB cable
2. In the Arduino IDE, add in the NODEMCU ESP8266 board as a compatible board https://www.instructables.com/Steps-to-Setup-Arduino-IDE-for-NODEMCU-ESP8266-WiF/
3. Add in the depedency libraries (neopixelbus, NTPclient) library via Arduino library manager
4. Clone the repository and open in Arduino IDE
5. Make a copy of the config-template.h file in the Sunrise folder and rename it to just 'config.h'. Then define the values for your personal setup (i.e wifi password, start times, etc)
6. Upload Sunrise\Sunrise.ino to the board

When a flashed board receives power, it wil flash a single LED a dim white until the wifi connects (usually 4-5 seconds). If this doesn't stop, it's likely incorrect wifi config values.

# Hardware
This project uses Whale Electronic's custom-designed LED board, which is a single board combining the open-source, Arduino IDE compatible NODEMCU ESP8266 microcontroller with a neopixelbus-compatible array of RGB LEDs (Product listing and schematics: https://www.tindie.com/products/whalelec/nodeled/)

# Software
Software used is Arduino-IDE written C++ code. Main script is Sunrise\Sunrise.ino, and config values are stored in an untracked (gitignored) config.h file. The main transition sequences are represented in the code as an array of key frames that have a color and target time, and will (linearly) transition from one key frame to the next using elapsed time from Start time as the indicator. 
Dependencies:
-NeoPixelBus library (documentaiton: https://github.com/Makuna/NeoPixelBus/wiki )
-NTP client (https://github.com/arduino-libraries/NTPClient) which references the board-stock wifi libraries
-Arduino time library (https://playground.arduino.cc/Code/Time/)

# Domain
Specturm and timeline used from https://www.cambridgeincolour.com/tutorials/sunrise-sunset-calculator.htm
A snippet of the day spectrum was converted into MS Paint and pixels were sampled every 100 pixels (see the 'documentation' folder). Brightness on the spectrum scale was manually adjusted for each selection, but approximates a perceived linear ramp in brightness. 
