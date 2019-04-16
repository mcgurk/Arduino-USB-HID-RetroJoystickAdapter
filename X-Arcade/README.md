# Arduino_X-Arcade_USB-adapter

## 2 Joysticks

"Old" PS/2 -version of X-arcade.
![X-Arcade](https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/raw/master/Images/x-arcade-dual-joystick.jpg)

```
D9-connector -> Arduino Pro Micro
6 (data) -> 3
5 (clk) -> 2
1 (5V) -> Vcc
2 (PS/2-mode) -> Vcc
9 (5V) -> Vcc
Shield -> GND

Or

PS/2-connector -> Arduino Pro Micro
Data (green or blue) -> 3
CLK (white or purple) -> 2
5V (red) -> 5V
GND (black) -> GND

```
#### Library
PS2KeyRaw

Arduino Pro Micro (select Arduino Leonardo from Arduino IDE).

PS/2 keyboard not needed.

Takes only about 10mA.

## Keyboard
https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/tree/master/PS2_Soarer_Converter
