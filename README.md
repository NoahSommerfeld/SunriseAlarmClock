# SunriseAlarmClock
The Sunrise Alarm Clock project is designed to create a mood-lighting solution that simulates sunrise and sunset using internet-connected, Arduino-compatible LED boards. 

# Hardware
This project uses Ben's custom-designed LED board, which is a single board combining the open-source, Arduino IDE compatible NODEMCU ESP8266 microcontroller with a neopixelbus-compatible array of RGB LEDs

# Software
Software used is Arduino-IDE written C++ code. ADD DETAILS ON CODE ARCH. HERE
Dependencies:
-NeoPixelBus library (documentaiton: https://github.com/Makuna/NeoPixelBus/wiki )
-NTP client (https://github.com/arduino-libraries/NTPClient) which references the board-stock wifi libraries

#Domain
Specturm and timeline used from https://www.cambridgeincolour.com/tutorials/sunrise-sunset-calculator.htm

# Setup
1. Set up the board connected to Power with a barrel-jack power adapter, and the PC via a data-capable microUSB-USB cable
2. In the Arduino IDE, add in the NODEMCU ESP8266 board as a compatible board https://www.instructables.com/Steps-to-Setup-Arduino-IDE-for-NODEMCU-ESP8266-WiF/
3. Add in the depedency libraries (neopixelbus, NTPclient) library via Arduino library manager
4. Clone the repository and upload MAIN.ino to the board

