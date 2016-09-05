# Arduino-USB-HID-RetroJoystickAdapter

This code supports Atari, SMS, NES, SNES and Genesis (3 and 6 button) controllers. Select your controller by commenting and uncommenting stuff from beginning of source code and give your IO-pin numbers.
Every one uses same button numbering, so if you make multiple adapters for different controllers, you have to teach buttons to RetrOrangePi only once. (of course this is also handy with every other emulators and platforms than RetrOrangePi)
You can also enable or disable Konami-code-shortcut (works only with Genesis 6-button controller (X+Y+Z+UP (or DOWN)) and SNES-controller (X+Y+R1+UP (or DOWN)).

Download and install Arduino IDE:
https://www.arduino.cc/en/Main/Software

Install joystick library (instructions for installing library: todo)
https://github.com/MHeironimus/ArduinoJoystickLibrary


ATmega32U4 Arduino Board. There is two versions of ATmega32U4, IO-logic with 5V or 3.3V. Many controllers (like basic Atari-controller) doesn't include any logic, so 5V and 3.3V versions are fine. Most of controllers with some logic needs 5V (NES, SNES, Genesis) and some rare ones needs 3.3V (Gamecube). Make sure that Arduino includes USB-port. There are some versions which doesn't include USB-port and then you have to add USB-connector for it.

Arduino UNO R3 includes ATmega16U2 and it can be used too, but it needs some extra work.
https://github.com/NicoHood/HoodLoader2
