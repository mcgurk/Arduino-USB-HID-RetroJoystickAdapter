I'm gathering stuff for BMC64 emulator USB input devices here.

Objective: USB-adapter for BMC64 with two fire support:

´´´
BMC64/theC64maxi/theVICmaxi compatible keyboard from Commodore keyboard:
https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/tree/master/C64_keyboard

If you use Arduino IDE, CDC device must be disabled or device doesn't work.

Objective: USB-adapter for BMC64 with two fire support:
Second fire uses pin 9: https://wiki.icomp.de/wiki/DE-9_Joystick

Disabling CDC:
31.3.2025: Arduino IDE 2.3.4.
C:\Users\[USER]\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.8.6\cores\arduino\USBDesc.h
Add line to start of file:
#define CDC_DISABLED

https://github.com/MickGyver/DaemonBite-CD32-USB



´´´
