# Objective: USB-adapter for Atari Joysticks for BMC64 with +5V and two fire support

## TL;DR
- Read this and build hardware: https://github.com/MiSTer-devel/Retro-Controllers-USB-MiSTer/tree/master/CD32ControllerUSB
- (if you are not going to use CD32-controller, you can leave 220Ω resistor out and you don't have to connect pin5 from D9-connector)
- Get avrdude.exe (if you have installed Arduino IDE, you already have that)
- Download [CD32ControllerUSB.ino.with_bootloader.bin](https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/raw/refs/heads/master/BMC64/CD32ControllerUSB.ino.with_bootloader.bin)
- Check what COM-port you have and flash firmware to Arduino Pro Micro from PowerShell:
```
& "$ENV:LOCALAPPDATA\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\bin\avrdude.exe" -C "$ENV:LOCALAPPDATA\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\etc\avrdude.conf" -v -patmega32u4 -c avr109 -U flash:w:"CD32ControllerUSB.ino.with_bootloader.bin":r -P com7
```
- If CDC is already disabled, you have to put Arduino flashing mode manually:

Connect RST to GND couple of times to get Arduino Pro Micro to programming mode (notice that COM-port is different in programming mode in Windows)


## I'm gathering stuff for BMC64 emulator USB input devices here.

```
If you use Arduino IDE, CDC device must be disabled or device doesn't work.

Second fire uses pin 9: https://wiki.icomp.de/wiki/DE-9_Joystick

Disabling CDC (IMPORTANT!):
(works 31.3.2025 with Arduino IDE 2.3.4)
C:\Users\[USER]\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.8.6\cores\arduino\USBDesc.h
Add this line to start of file:
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

