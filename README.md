# Arduino-USB-HID-RetroJoystickAdapter
## What is this?
Very cheap and quite easy way to connect many non-USB joysticks and gamepads or DIY arcade cabinet controller to almost any machine with USB-port and HID-support. Arduino board with ATmega32u4 chip with help of Joystick-library does all the USB-work.
Tested with Windows and Linux desktop computers and ARM based computers like Raspberry Pi and Orange Pi. Also worked with Samsung Galaxy S4 Android phone and Samsung Galaxy Tab 10.1 Android tablet with OTG-USB cable.

ATmega32u4 can be easily changed to act as keyboard and/or mouse too.

## Short version
- Get ATmega32u4 Arduino Board
- Connect joystick or gamepad to Arduino (in most situations soldering is required)
- Install Arduino IDE
- Install Joystick library (https://github.com/MHeironimus/ArduinoJoystickLibrary)
- Open sourcecode to Arduino IDE and select controller by commenting and uncommenting and edit pin-numbers (if needed)
- Upload
- Done

## Long version
### What you need

#### Arduino board
Starting point is ATmega32u4 based Arduino board. There is two versions of ATmega32u4, IO-logic with 5V or 3.3V. Many controllers (like basic Atari-controller) doesn't include any logic, so 5V and 3.3V versions are fine. Most of controllers with some logic needs 5V (NES, SNES, Genesis) and some rare ones needs 3.3V (Gamecube). Make sure that Arduino includes USB-port. There are some versions which doesn't include USB-port and then you have to add USB-connector for it. Arduino Leonardo includes ATmega32u4, but cheapest and smallest ones are named Pro Micro. Arduino Pro Micro and ATmega32u4 are good words to use when searching from Ebay etc. Cheapest starts from about $4.

Example of Arduino Pro Micro:

![Arduino Pro Micro](https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/raw/master/Images/Arduino_ProMicro.jpg)


#### Computer with Arduino IDE
Arduino IDE works with Windows, OS X and Linux and ARM (beta). If you use small card computer like Rasperry Pi or Orange Pi, you can program adapter even with them (tested with Orange Pi PC and RetrOrangePi 2.0). That way you don't even have to unplug adapter from your emulator/game-platform if you want reprogram adapter.

#### Joystick library for Arduino IDE
Install joystick library (instructions for installing library: todo)
https://github.com/MHeironimus/ArduinoJoystickLibrary

#### Joystick(s) or gamepad(s) and way to connect them to Arduino
Some joystick/gamepads uses their own connectors and original connectors can be very pricey. Sometimes you can find extension cord cheaper than plain connector and you can take cable and plug from that.
You can use any IO-pins, but try to avoid pins 0 and 1. They are serial port TX and RX pins and even though they are not used right now, they are handy for debugging and other uses.

### Setup

- Download and install Arduino IDE: https://www.arduino.cc/en/Main/Software

- `Tools->Board->Arduino Leonardo`

  In Arduino IDE you have to select board. In my case some reason "Arduino Leonardo" selections works better than "Arduino/Genuino Micro". With Leonardo-setting RX and TX leds are normally off and blinks when data is transfered to/from USB. With Micro-setting LED's are normally on and they blinks to off when data is transferring.

- `Tools->Port->COMx`

  Arduino IDE shows available COM-ports. In most cases it's biggest numbered COM-port. COM-port number can change when you start to upload program first time (COM-port number can be different after joystick-features are enabled). If this happens, just change COM-port and start upload again. When I tested with RetrOrangePi (Raspbian Linux), port was `/dev/ttyACM0`.

- Test your programming environment and upload

  You can now test your setup. One basic example is "blink". With this board, it doesn't blink, because there is no led in pin 13. Nevertheless you can use it as a test that your IDE and Arduino works. Select `File -> Examples -> 01.Basics -> Blink` and press `Upload` (Arrow to right -symbol top of code windows). If you get no errors, you are good to go.

- Install joystick library

  Go to https://github.com/MHeironimus/ArduinoJoystickLibrary and press `Clone or download -> Download ZIP`.
  Extract Joystick2 folder from zip to `%HOMEPATH%\Documents\Arduino\libraries` (Windows) or `~/Arduino/libraries` (Linux).
  
  After extraction check that you have files `%HOMEPATH%\Documents\Arduino\libraries\Joystick2\Joystick2.h and Joystick2.cpp` or `~/Arduino/libraries/Joystick2/Joystick2.h and Joystick2.cpp`.
  Close and start Arduino IDE again so it searches and finds library.

- Load RetroJoystickAdapter.ino to Arduino IDE

  Select controller by commenting and uncommenting and edit pin-numbers (if needed).

- Press Upload

- If you got no errors, test in Windows with `Control Panel -> Game Controllers` or in Linux with `jstest`

### Code
This code supports Atari, SMS, NES, SNES and Genesis (3 and 6 button) controllers. Select your controller by commenting and uncommenting stuff from beginning of source code and give your IO-pin numbers.
Every one uses same button numbering, so if you make multiple adapters for different controllers, you have to teach buttons to emulator/game (or whole emulation system like RetroPie or RetrOrangePi) only once.

#### Konami code
You can also enable or disable Konami-code-shortcut. For now it works only with Genesis 6-button controller (X+Y+Z+UP (or DOWN)) and SNES-controller (X+Y+R1+UP (or DOWN)). Some games demands that not other buttons are pressed before code and that's why only last pressed button can be button which have some function in emulator.

https://en.wikipedia.org/wiki/Konami_Code
https://en.wikipedia.org/wiki/List_of_Konami_code_games

## Misc
Arduino UNO R3 includes ATmega16u2 (along with ATmega328p) and it can be used too, but it needs some extra work (code doesn't work without modifications with UNO):
https://github.com/NicoHood/HoodLoader2

There is simple tutorial in tutorial folder (https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/tree/master/Tutorial). I tried to make it as clear as possible, so even if you are not familiar with coding, you may can do modifications to it (example add a button).

## Todo
- example (D-connectors straight to Arduino)
- structure of code, sends only when something changes
- button numbering (genesis C as select)
- changing maximum buttons from joystick library (maybe pointless)
- some words of analog controllers?
- some words of Arduino acting same time as serial port
- very simple test. test without joystick and test to connect one IO-pin to GND.
- other ideas - PIR example to misc
