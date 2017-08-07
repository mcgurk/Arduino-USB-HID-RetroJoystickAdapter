## USB mouse -> 1351 (C64) mouse adapter

#### Requirements

- Arduino Pro Micro 16MHz 5V
- USB Host Shield (USB Host Shield Library 2.0 from Arduino IDE Library Manager)
- 2pcs 10k resistors
- Level shifter for 6 pins

#### Pin 9 and 10 needed for TIMER1 - Move SS and INT of USB host shield library away

C:\Users\xxxxx\Documents\Arduino\libraries\USB_Host_Shield_Library_2.0\UsbCore.h:
```
//typedef MAX3421e<P10, P9> MAX3421E; // default pin asignments
//Arduino Pro Micro SS=A0(P18), INT=A1(P19):
typedef MAX3421e<P18, P19> MAX3421E;
//Arduino Uno SS=A0(P14), INT=A1(P15):
//typedef MAX3421e<P14, P15> MAX3421E;
```

#### Joystick port -> Arduino
- 9 (POTX) -> 10k resistor -> 9 (OC1A)
- 9 (POTX) -> Arduino Pro Micro: 4, Arduino Uno: 8 (ICP1)
- 5 (POTY) -> 10k resistor -> 10 (OC1B)
- 6 (FIRE, left mouse button) -> 5
- 1 (UP, right mouse button) -> 6
- 8 GND -> GND

#### USB Host Shield (3.3V) -> Arduino
- SS -> A0 (Arduino Pro Micro), A0 (Arduino Uno) (check UsbCore.h) (3.3V!)
- INT -> A1 (Arduino Pro Micro), A1 (Arduino Uno) (check UsbCore.h) (3.3V!)
- MOSI -> 16 (Arduino Pro Micro), 11 (Arduino Uno) (3.3V!)
- MISO -> 14 (Arduino Pro Micro), 12 (Arduino Uno) (3.3V!)
- CLK -> 15 (Arduino Pro Micro), 13 (Arduino Uno) (3.3V!)
- RST -> RST (3.3V!) (Pro Micro: beware of GND in place of RST!)
- GND -> GND
- VCC -> 3.3V
- VBUS -> 5V (cut trace from VBUS-pad to resistor!)

!: MOSI, MISO and CLK pins are wrongly marked in USB Host Shield DuinoFun UHS mini v2 -module!

#### Links
- http://asdasd.rpg.fi/~svo/%5bm%5douse/
- http://www.zimmers.net/anonftp/pub/cbm/documents/projects/interfaces/mouse/Mouse.html
- https://www.google.com/patents/US4886941
- https://ist.uwaterloo.ca/~schepers/MJK/pics/joyports.gif
- Trace cut from USB Host Shield mini: https://geekhack.org/index.php?topic=80421.0
- https://gammon.com.au/interrupts
- http://www.avrbeginners.net/architecture/timers/timers.html
- https://github.com/felis/USB_Host_Shield_2.0/blob/master/avrpins.h

### Hardware

I used Arduino Pro Micro and module which have AMS1117 3.3V regulator and 8 bidirectional voltage converters.

#### Arduino
![Arduino Pro Micro](https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/raw/master/Images/Arduino_ProMicro.jpg)

#### USB Host Shield
![USB Host Shield DuinoFun UHS mini v2](https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/raw/master/Images/USB_Host_Shield_DuinoFun_UHS_mini_v2.jpg)

#### Level converter with regulator
![Level converter with AMS1117](https://raw.githubusercontent.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/master/Images/Levelconverter_with_AMS1117.jpg)
