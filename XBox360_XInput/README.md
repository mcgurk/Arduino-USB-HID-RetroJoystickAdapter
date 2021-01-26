
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
