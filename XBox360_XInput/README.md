
Based on https://github.com/dmadison/ArduinoXInput

- Install Arduino IDE hardware:
https://github.com/dmadison/ArduinoXInput_AVR

- Install Arduino IDE library from Library manager:
XInput by David Madison

- Tester from Windows Store:
Game Controller Tester

- Doesn't go to flashing mode automatically. Connect RST to GND to get Arduino Pro Micro to programming mode.


- Arduino Pro Micro needs PB0/SS-pin to work. And it is not broken out. So no SPI for PSX-controller I guess? 
https://forum.arduino.cc/index.php?topic=427828.0
- It could be used as pin 17 in code, if led could be disabled?

## PsxNewLib
- https://github.com/SukkoPera/PsxNewLib
- https://github.com/SukkoPera/PsxControllerShield
- https://github.com/SukkoPera/PsxControllerShield/blob/master/doc/schematics.pdf
- (my green dualshock analog controller: SCPH-1200)
- https://store.curiousinventor.com/guides/PS2

PSX | SPI | Uno | Pro Micro
--- | --- | --- | ---
1 DATA (brown ![#brown](https://via.placeholder.com/10/c68c53/000000?text=+)) | MISO | D12 (PB4) | D14 (PB3)
2 CMD (orange ![#orange](https://via.placeholder.com/10/ff8000/000000?text=+)) | MOSI | D11 (PB3) | D16 (PB2)
3 Vibration power (7.2-9V) (grey ![#grey](https://via.placeholder.com/10/999999/000000?text=+)) | | |
4 GND (black ![#black](https://via.placeholder.com/10/000000/000000?text=+)) | | GND | GND
5 VCC (3.3V) (red ![#red](https://via.placeholder.com/10/ff0000/000000?text=+)) | | VCC | VCC
6 ATT (yellow ![#yellow](https://via.placeholder.com/10/ffff00/000000?text=+)) | SS | D10 (PB2) | D17 (PB0/RX_LED)
7 CLK (blue ![#blue](https://via.placeholder.com/10/0000ff/000000?text=+)) | SCK | D13 (PB5) | D15 (PB1)
8 unknown (white ![#white](https://via.placeholder.com/10/ffffff/000000?text=+)) | | |
9 ACK (green ![#green](https://via.placeholder.com/10/00ff00/000000?text=+)) | | |
 | | | |
1 VCC (3.3V) | | |
2 SCL | | A5 (PC5) | D3 (PD0)
3 Detect device (3.3V) | - | - | -
4 NC | | |
5 SDA | | A4 (PC4) | D2 (PD1)
6 GND | | |

