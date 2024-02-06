# Poor man's "Keyrah" to connect C64 keyboard to USB (for BMC64 emulator)

![Controller](https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/raw/master/C64_keyboard/Soarer_controller_for_C64.jpg)

## Building
- Solder all keyboard connector pins to Arduino Pro Micro.
- You can solder Restore key (connector pin "I") paraller to some other pin. I soldered "I" and "G" to Arduino pin A8. (this is actually not needed, because there is one free pin left). [Pins I used](https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/blob/master/C64_keyboard/README.md#pins-i-used).
- You can also connect LED of C64 to VCC/GND of Arduino. I used 220ohm resistor.

## Firmware/flashing
- Download Soarer controller firmware (Soarer_Controller_v1.20_beta4.zip): https://geekhack.org/index.php?topic=50437.0 (or https://deskthority.net/workshop-f7/soarer-s-keyboard-controller-firmware-t6767.html)
- Extract /firmware/Soarer_Controller_v1.20_beta4_atmega32u4.hex from Soarer_Controller_v1.20_beta4.zip
- Install Arduino IDE
- Connect RST to GND couple of times to get Arduino Pro Micro to programming mode (notice that com-port is different in programming mode in Windows)
- Flash firmware
```
# old: & "C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avrdude" -C"C:\Program Files (x86)\Arduino\hardware\tools\avr/etc/avrdude.conf" -v -p m32u4 -c avr109 -P COM5 -b 57600 -U flash:w:firmware\Soarer_Controller_v1.20_beta4_atmega32u4.hex:i
# 11.2.2023:
& "$ENV:LOCALAPPDATA\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\bin\avrdude.exe" -C"$ENV:LOCALAPPDATA\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\etc\avrdude.conf" -v -p m32u4 -c avr109 -P COM9 -b 57600 -U flash:w:firmware\Soarer_Controller_v1.20_beta4_atmega32u4.hex:i
```

## Setup
- Download [config file](https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/raw/master/C64_keyboard/C64_matrix.sc). Check pin order to match your setup.
- Compile and upload config file to Arduino Pro Micro
```
.\scas C64_matrix.sc C64_matrix.bin
.\scwr C64_matrix.bin
```


## Testing
- https://keycode.info/

## Pins I used
```
//   Pin    Label
//   20(A)  2(D2/PD1)
//   19(B)  3(D3/PD0)
//   18(C)  4(D4/PD4)
//   17(D)  5(D5/PC6)
//   16(E)  6(D6/PD7)
//   15(F)  7(D7/PE6)
//   14(G)  8(D8/PB4)
//   13(H)  9(D9/PB5)
//   12(0)  10(D10/PB6)
//   11(1)  16(D16/PB2)
//   10(2)  14(D14/PB3)
//   9(3)   15(D15/PB1)
//   8(4)   A0(A0/PF7)
//   7(5)   A1(A1/PF6)
//   6(6)   A2(A2/PF5)
//   5(7)   A3(A3/PF4)
//   4        N/C
//   3(8)   TX(D1/PD3)
//   2        N/C
//   1(I)   8(D8/PB4)
```
## Links

- https://geekhack.org/index.php?topic=50437.0
- https://deskthority.net/workshop-f7/soarer-s-keyboard-controller-firmware-t6767.html
- Newest I found was Soarer_Controller_v1.20_beta4.zip from 26.10.2013
- Soarer config file documentation: https://deskthority.net/download/file.php?id=8833 (Soarer_Converter_v1.12_docs.zip)
- Keynames used in config file from https://deskthority.net/download/file.php?id=8833 (Soarer_Converter_v1.12_docs.zip): /docs/codes.html
- https://www.waitingforfriday.com/wp-content/uploads/2017/01/C64_Keyboard_Schematics_PNG.png
- http://kookye.com/wp-content/uploads/2016/02/Pinout-ProMicro.jpg
- I used this as starting point: https://github.com/abzman/Keyboard-config-file/blob/master/C64_matrix.sc
- C64-emulator for Raspberry Pi 2/3: http://accentual.com/bmc64/
- For bigger matrices use Arduino Micro (24 usable IO pins) https://www.40percent.club/2017/10/green-arduino-micro.html (The largest matrix you can do with a Pro Micro with its 18 pins is 9x9, 81 keys.)
- Example macros: https://sharktastica.co.uk/guides/soarers_2

## Macro without modifier key
Even though modifier key is mandatory, it can be replaced with unpressed modifiers. This writes shifted A when only a-key is pressed:
```
macroblock
  macro A -ALL
    SET_META LSHIFT
	PRESS A
	CLEAR_ALL
  endmacro
endblock
```

## TheC64 Maxi Orange Pi PC
Buttons that needs modification:
```
+ 12
- 13
@ 26
* 27
^ 111
: 39
; 40
= 43
£ 110
C= 29
CTRL 15
RESTORE 104
INS/DEL 14
```
Copy theC64-sym-CLASSIC.vkm to /usr/lib/vice/C64/theC64-sym-CLASSIC.vkm in ext4 partition of firmware-image. (THEC64_for_OPI.img or THE_VIC_20_for_OPI.img)

## OBSOLETE: Issues (all issues are fixed in newer BMC64 versions)

### Issue with Menu key (fixed in newer BMC64 versions)
- .crt-files doesn't work with C= + F7, so as workaround LSHIFT + F7 is F12.

### Issue with shift (fixed in newer BMC64 versions)

- "+", "-", "£", "@" and "*" doesn't produce GFX-chars when shifted

#### "Repair" rpi_sym.vkm (1.0.9)?
- at 5 6 0 -> 5 6 8
- minus 5 3 0 -> 5 3 8
- plus 5 0 0 -> 5 0 8
- sterling 6 0 0 -> 6 0 8
- asterisk 6 1 0 -> 6 1 8
```
sed -i 's/5 6 0/5 6 8/g; s/5 3 0/5 3 8/g; s/5 0 0/5 0 8/g; s/6 0 0/6 0 8/g; s/6 1 0/6 1 8/g' rpi_sym.vkm
```

## Stuff
Ultimate keybindings?
```
F9 = shift+return
F10 = Ultimate menu
F11 = Freeze
F12 = Restore
LCTRL = Control
LWIN = CBM
INS = Inst
HOME = Clr/home
PAUSE = run/stop
SCROLL_LOCK = Ultimate menu
BACKSPACE = Inst/del
LSHIFT+LCTRL+LWIN+LALT = Reset

Buttons: Freeze-Menu-Reset
```
