#### Pin 9 and 10 needed for TIMER1. Move SS and INT of USB host shield away

C:\Users\xxxxx\Documents\Arduino\libraries\USB_Host_Shield_Library_2.0\UsbCore.h:
```
//typedef MAX3421e<P10, P9> MAX3421E;
//Arduino Pro Micro SS=A0, INT=A1:
typedef MAX3421e<P18, P19> MAX3421E;
```

#### Joystick port -> Arduino
- 9 (POTX) -> 1k -> 9 (OC1A)
- 9 (POTX) -> (ICP1) Arduino Pro Micro: 4, Arduino Uno: 8
- 5 (POTY) -> 1k -> 10 (OC1B)
- 6 (FIRE) -> 5
- 1 (UP) -> 6

#### USB Host Shield (3.3V) -> Arduino
- SS -> A0 (Arduino Pro Micro) (check UsbCore.h) (3.3V<->5V!)
- INT -> A1 (Arduino Pro Micro) (check UsbCore.h) (3.3V<->5V!)
- MOSI -> 11 (Arduino Uno), 16 (Arduino Pro Micro) (3.3V<->5V!)
- MISO -> 12 (Arduino Uno), 14 (Arduino Pro Micro) (3.3V<->5V!)
- CLK -> 13 (Arduino Uno), 15 (Arduino Pro Micro) (3.3V<->5V!)
- RST -> RST (3.3V<->5V!)
- GND -> GND
- VCC -> 3.3V
- VBUS -> 5V (cut trace from VBUS-pad to resistor!)

#### Links
- http://www.zimmers.net/anonftp/pub/cbm/documents/projects/interfaces/mouse/Mouse.html
- http://asdasd.rpg.fi/~svo/%5bm%5douse/
- https://ist.uwaterloo.ca/~schepers/MJK/pics/joyports.gif

I used Arduino Pro Micro and module which have AMS1117 3.3V regulator and 8 bidirectional voltage converters.
