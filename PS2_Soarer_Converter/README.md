# Soarer PS/2 to USB keyboard converter
You need
- ATMega32U4 Microcontroller/Arduino. E.g. Arduino Pro Micro
- PS/2 female connector

https://deskthority.net/workshop-f7/xt-at-ps2-terminal-to-usb-converter-with-nkro-t2510.html
Ohjeet:
https://geekhack.org/index.php?topic=17458.0
https://deskthority.net/resources/file/8295
Soarer_Converter_v1.12_update.zip (14.6.2013)
https://deskthority.net/resources/file/8833
Soarer_Converter_v1.12_docs.zip
Tools:
https://deskthority.net/resources/file/6142
Soarer_Converter_v1.10.zip

### Hardware
- Data -> PD0 (Pro Micro: 3)
- CLK -> PD1 (Pro Micro: 2)
https://github.com/iflowfor8hours/6112884-pro-micro-controller

### Testing
https://www.pjrc.com/teensy/hid_listen.html

### Settings

### Flashing firmware
https://sourceforge.net/projects/winavr/
avrdude -p m32u4 -b 57600 -P com5 -c avr109 -U flash:w:Soarer_at2usb_v1.12_atmega32u4.hex
Kaks kertaa RST GND:hen, jotta pääsee bootloader-modeen.


## Keyboard controller
https://geekhack.org/index.php?topic=50437.0
https://deskthority.net/workshop-f7/soarer-s-keyboard-controller-firmware-t6767.html
Soarer_Controller_v1.20_beta4.zip (26.10.2013)
Tools-kansiossa eri käyttiksille työkaluexet.
avrdude -p m32u4 -b 57600 -P com5 -c avr109 -U flash:w:Soarer_Controller_v1.20_beta4_atmega32u4.hex
