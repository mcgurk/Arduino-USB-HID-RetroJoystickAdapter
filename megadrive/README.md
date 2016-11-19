# MegaDrive / Genesis Manual

## What do I need?
- You will need one [Atmega32u4](https://pt.aliexpress.com/item/New-Pro-Micro-ATmega32U4-5V-16MHz-Module-with-2-row-pin-header-For-Leonardo-best-quality/32273120508.html?spm=2114.13010608.0.0.Uv843y&detailNewVersion=&categoryId=400103) (e.g. Arduino Leonardo).
- [Two DB9 ports](https://pt.aliexpress.com/item/Type-DR9-90-degrees-bend-DR9-male-head-needle-serial-port-and-seat-DB9-RS232-9/32706812789.html?spm=2114.13010608.0.0.1YtDRt), better if you find a 90º version.
- Original Mega Drive Controller
- Follow step by step the how-to [`Long version`](https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter)
- Use the file [RetroJoystickAdapter_Megadrive.ino](https://raw.githubusercontent.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/master/RetroJoystickAdapter_Megadrive.ino), comment the fist set of pins, and uncomment the second that says `// if you use two DB9 connectors solded back to back on your ATmega32u4, you should use this inputs`.

## Schematics and pinout

You sholud print this schema to guide you during the soldering process.

![megadrive-adapter](https://raw.githubusercontent.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/master/megadrive/pinout.jpg)



## Final product

Here are Adriano's examples of his assembly.

![solded-adapter](https://raw.githubusercontent.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/master/megadrive/atmega_solded.jpg)
![solded-back-adapter](https://raw.githubusercontent.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/master/megadrive/atmega_solded_back.jpg)
![testing-adapter](https://raw.githubusercontent.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/master/megadrive/atmega_testing.jpg)

Here are the Jarno's examples of his assembly.

![Hardware_Atari-SMS-Genesis](https://raw.githubusercontent.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/master/Hardware/Hardware_Atari-SMS-Genesis.jpg)
![sega_genesis_adapter](https://raw.githubusercontent.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/master/Images/sega_genesis_adapter.jpg)
