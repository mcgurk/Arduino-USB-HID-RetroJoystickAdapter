## PS/2 keyboard as 4 USB-joysticks

![PS2-adapter](https://raw.githubusercontent.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/master/Images/ps2-keyboard-adapter.jpg)

Needs Atmega32u4 (e.g. Arduino Leonardo).

Needs custom ps2-library (PS2Keyboard_mcgurk.zip, unzip to libraries-folder).

Clock-signal must be in pin 2 (because of interrupts).

Edit button assignments manually. Assignments are in the end of source file.

`#define DEBUG` if you want to see codes in console.

Joystick 1:
```
arrows = up/down/left/right
R-ctrl = a
alt gr = b
enter = start
R-shift = select
```

Joystick 2:
```
W/S/A/D = up/down/left/right
L-ctrl = a
L-alt = b
tab = start
L-shift = select
```

## Links

http://playground.arduino.cc/Main/PS2Keyboard

http://www.computer-engineering.org/ps2protocol/

http://www.computer-engineering.org/ps2keyboard/scancodes2.html
