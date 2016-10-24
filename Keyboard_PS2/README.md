## PS/2 keyboard as 4 USB-joysticks

![PS2-adapter](https://raw.githubusercontent.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/master/Images/ps2-keyboard-adapter.jpg)

Needs Atmega32u4 (e.g. Arduino Leonardo).

Needs custom ps2-library (PS2Keyboard_mcgurk.zip, unzip to libraries-folder).

Clock-signal must be in pin 2 (because of interrupts).

Edit button assignments manually. Assignments are in the end of source file.

`#define DEBUG` if you want to see codes in console.

Joystick 0:
```
arrows = up/down/left/right
R-ctrl = a
alt gr = b
enter = start
R-shift = select
```

Joystick 1:
```
W/S/A/D = up/down/left/right
L-ctrl = a
L-alt = b
tab = start
L-shift = select
```

Joystick 2:
```
I/K/J/L = up/down/left/right
M = a
N = b
O = start
U = select
```

Joystick 3:
```
keypad 8/5/4/6 = up/down/left/right
keypad 0 = a
keypad , = b
keypad enter = start
keypad + = select
```

## Links

http://playground.arduino.cc/Main/PS2Keyboard

http://www.computer-engineering.org/ps2protocol/

http://www.computer-engineering.org/ps2keyboard/scancodes2.html
