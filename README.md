# Arduino-USB-HID-RetroJoystickAdapter

## Short version
- ATmega32U4 Arduino Board
- Arduino IDE
- Joystick library
- Select controller and edit pin-numbers (if needed) from source
- Upload

## What you need

### Arduino board
ATmega32U4 Arduino Board. There is two versions of ATmega32U4, IO-logic with 5V or 3.3V. Many controllers (like basic Atari-controller) doesn't include any logic, so 5V and 3.3V versions are fine. Most of controllers with some logic needs 5V (NES, SNES, Genesis) and some rare ones needs 3.3V (Gamecube). Make sure that Arduino includes USB-port. There are some versions which doesn't include USB-port and then you have to add USB-connector for it. Arduino Leonardo includes ATmega32U4, but cheapest and smallest ones are named Pro Micro. Arduino Pro Micro and ATmega32U4 are good words to use when searching from Ebay etc. Cheapest starts from about $4.

Example of Arduino Pro Micro:

![Arduino Pro Micro](https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/raw/master/Arduino_ProMicro.jpg)


### Computer with Arduino IDE
Arduino IDE works with Windows, OS X and Linux and ARM (beta). If you use small card computer like Rasperry Pi or Orange Pi, you can program adapter even with them (tested with Orange Pi PC and RetrOrangePi 2.0). That way you don't even have to unplug adapter from your emulator/game-platform if you want reprogram adapter.

### Joystick library for Arduino IDE
Install joystick library (instructions for installing library: todo)
https://github.com/MHeironimus/ArduinoJoystickLibrary

### Joystick(s) or gamepad(s) and way to connect them to Arduino
Some joystick/gamepads uses their own connectors and original connectors can be very pricey. Sometimes you can find extension cord cheaper than plain connector and you can take cable and plug from that.
You can use any IO-pins, but try to avoid pins 0 and 1. They are serial port TX and RX pins and even though they are not used right now, they are handy for debugging and other uses.

## Setup

Download and install Arduino IDE:
https://www.arduino.cc/en/Main/Software

- Tools->Board->Arduino Leonardo
In Arduino IDE you have to select board. On my case some reason "Arduino Leonardo" selections works better than "Arduino/Genuino Micro". With Leonardo-setting RX and TX leds are normally off and blinks when data is transfered to/from USB. With Micro-setting LED's are normally on and they blinks to off when data is transferring.

- Tools->Port->COMx
In Windows Arduino IDE shows only available com-ports. In most cases it's biggest numbered COM-port. COM-port number can change when you start to upload program first time (COM-port number can be different after joystick-features are enabled). If this happens, just change COM-port and start upload again. When I tested with RetrOrangePi (Raspbian Linux), port was ttyACM0.

Install joystick library (instructions for installing library: todo):
https://github.com/MHeironimus/ArduinoJoystickLibrary

## Code
This code supports Atari, SMS, NES, SNES and Genesis (3 and 6 button) controllers. Select your controller by commenting and uncommenting stuff from beginning of source code and give your IO-pin numbers.
Every one uses same button numbering, so if you make multiple adapters for different controllers, you have to teach buttons to RetrOrangePi only once. (of course this is also handy with every other emulators and platforms than RetrOrangePi)


### Konami code
You can also enable or disable Konami-code-shortcut. For now it works only with Genesis 6-button controller (X+Y+Z+UP (or DOWN)) and SNES-controller (X+Y+R1+UP (or DOWN)). Some games demands that not other buttons are pressed before code and that's why only last pressed button can be button which have some function in emulator.

https://en.wikipedia.org/wiki/Konami_Code
https://en.wikipedia.org/wiki/List_of_Konami_code_games

## Misc
Arduino UNO R3 includes ATmega16U2 (along with ATmega328P) and it can be used too, but it needs some extra work (code doesn't work without modifications with UNO):
https://github.com/NicoHood/HoodLoader2
