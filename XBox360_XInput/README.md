
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

PSX | SPI | Uno | Pro Micro
--- | --- | --- | ---
DATA/2 (brown ![#brown](https://via.placeholder.com/10/c68c53/000000?text=+)) | MISO | D12 (PB4) | D14 (PB3)
CMD/3 (orange ![#orange](https://via.placeholder.com/10/ff8000/000000?text=+)) | MOSI | D11 (PB3) | D16 (PB2)
CLK/5 (blue ![#blue](https://via.placeholder.com/10/0000ff/000000?text=+)) | SCK | D13 (PB5) | D15 (PB1)
ATT/4 (yellow ![#yellow](https://via.placeholder.com/10/ffff00/000000?text=+)) | SS | D10 (PB2) | D17 (PB0/RX_LED)

