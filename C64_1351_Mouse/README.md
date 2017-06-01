#### Pin 9 and 10 needed for TIMER1. Move SS and INT of USB host shield away

C:\Users\xxxxx\Documents\Arduino\libraries\USB_Host_Shield_Library_2.0\UsbCore.h:
```
//typedef MAX3421e<P10, P9> MAX3421E;
//Arduino Pro Micro SS=A0, INT=A1:
typedef MAX3421e<P18, P19> MAX3421E;
```
https://ist.uwaterloo.ca/~schepers/MJK/pics/joyports.gif

#### Joystick port -> Arduino
- 9 (POTX) -> 1k -> 9 (OC1A)
- 9 (POTX) -> (ICP1) Arduino Pro Micro: 4, Arduino Uno: 8
- 5 (POTY) -> 1k -> 10 (OC1B)
- 6 (FIRE) -> 5
- 1 (UP) -> 6

#### USB Host Shield -> Arduino
- SS -> A0 (Arduino Pro Micro) (check UsbCore.h)
- INT -> A1 (Arduino Pro Micro) (check UsbCore.h)
- MOSI -> 11 (Arduino Uno), 16 (Arduino Pro Micro)
- MISO -> 12 (Arduino Uno), 14 (Arduino Pro Micro)
- CLK -> 13 (Arduino Uno), 15 (Arduino Pro Micro)
- RST -> RST
- GND -> GND
- VCC -> 3.3V
- VBUS -> 5V (cut trace from VBUS-pad to resistor!)


