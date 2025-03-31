# Objective: USB-adapter for Atari Joysticks for BMC64 with two fire support

I'm gathering stuff for BMC64 emulator USB input devices here.

```
If you use Arduino IDE, CDC device must be disabled or device doesn't work.

Second fire uses pin 9: https://wiki.icomp.de/wiki/DE-9_Joystick

Disabling CDC (IMPORTANT!):
(works 31.3.2025 with Arduino IDE 2.3.4)
C:\Users\[USER]\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.8.6\cores\arduino\USBDesc.h
Add line to start of file:
#define CDC_DISABLED

I used this:
Adapter that works with original CD32-controller (works with normal joysticks too with 1 or 2 fires):
https://github.com/MiSTer-devel/Retro-Controllers-USB-MiSTer/tree/master/CD32ControllerUSB
Pinout and connections is front of source file:
https://github.com/MiSTer-devel/Retro-Controllers-USB-MiSTer/blob/master/CD32ControllerUSB/CD32ControllerUSB.ino

Another version of CD32ControllerUSB. Some differences, but I don't know if they are important:
https://github.com/MickGyver/DaemonBite-CD32-USB
Pinout and connections is front of source file:
https://github.com/MickGyver/DaemonBite-CD32-USB/blob/master/CD32ControllerUSB/CD32ControllerUSB.ino
(might support two controllers with one Arduino Pro Micro, but I haven't tested that with BMC64)

Another possibility is use theC64mini (this adapter works with theC64mini too):
https://github.com/rainisto/arcade2usb-converter
Pinout and connections is front of source file:
https://github.com/rainisto/arcade2usb-converter/blob/master/source/c64mini-arcade2usb-converter/c64mini-arcade2usb-converter.ino

```

BMC64/theC64maxi/theVICmaxi compatible keyboard from Commodore keyboard:
https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/tree/master/C64_keyboard

