# Arduino_X-Arcade_USB-adapter

"Old" PS/2 -version of X-arcade.
![X-Arcade](https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/raw/master/Images/x-arcade-dual-joystick.jpg)

```
D9-connector -> Arduino Pro Micro
6 -> 3 (data)
5 -> 2 (clk)
1 -> Vcc (5V)
2 -> Vcc (PS/2-mode)
9 -> Vcc (5V)
Shield -> GND

Or

PS/2 -> Arduino Pro Micro
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
