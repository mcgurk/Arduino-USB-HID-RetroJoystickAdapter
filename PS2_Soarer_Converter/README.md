# Soarer PS/2 to USB keyboard converter
https://deskthority.net/workshop-f7/xt-at-ps2-terminal-to-usb-converter-with-nkro-t2510.html

#### Keyboard protocols supported:
- XT (scan code set 1)
- AT (scan code set 2)
- PS/2 (MF2) (extended scan code set 2)
- Terminal e.g. 3179/318x/319x (scan code set 3)
#### Configurable Features:
- Remapping
- Layers
- Macros
- On-the-fly Config Selection
#### Other Features:
- Full NKRO, if the keyboard supports it (even on Macs!)
- Boot mode support (even with faulty BIOS!)
- Auto-detection of the keyboard type
- XT and AT boards are remapped correctly for PrtSc etc.
- 1000Hz polling using Full Speed USB
- Suspend and resume support
- Media and Power key support
- Jump to bootloader function (update firmware without pressing the reset button) (v1.0+)



## Hardware
You need
- ATMega32U4 Microcontroller/Arduino. E.g. Arduino Pro Micro
- PS/2 female connector

PS/2 -> Arduino
- Data (green or blue) -> PD0 (Pro Micro: 3)
- CLK (white or purple) -> PD1 (Pro Micro: 2)
- 5V (red) -> 5V
- GND (black) -> GND

##### Arduino Pro Micro pinout
http://www.pighixxx.com/test/wp-content/uploads/2016/07/pro_micro_pinout_v1_0_red.png

##### PS/2 pinout
http://ezcontents.org/sites/default/files/ps2_pinout.png

## Firmware
https://deskthority.net/workshop-f7/xt-at-ps2-terminal-to-usb-converter-with-nkro-t2510.html

Soarer_Converter_v1.12_update.zip: https://deskthority.net/resources/file/8295

#### Flasher
https://sourceforge.net/projects/winavr/
#### Flashing
```
avrdude -p m32u4 -b 57600 -P com5 -c avr109 -U flash:w:Soarer_at2usb_v1.12_atmega32u4.hex
```
Bootloader mode: RST to GND two times. You have couple of seconds to start flashing. Serial port can be different than in normal mode.


## Testing/Debugging
hid_listen.exe: https://www.pjrc.com/teensy/hid_listen.html

## Settings/Tools/Docs
Tools (Soarer_Converter_v1.10.zip):
- https://deskthority.net/workshop-f7/xt-at-ps2-terminal-to-usb-converter-with-nkro-t2510.html
- https://deskthority.net/download/file.php?id=6142

Docs (Soarer_Converter_v1.12_docs.zip):
- https://deskthority.net/workshop-f7/xt-at-ps2-terminal-to-usb-converter-with-nkro-t2510.html
- https://deskthority.net/download/file.php?id=8833
- Key names/codes: docs/codes.html

Tools from Soarer_Controller_v1.20_beta4.zip doesn't work!:
```
protocol version check: converter=1.00, scwr=1.00: ok
settings version check: converter=1.01, file=1.03: failed
```

Compile text file to binary
```
scas xarcade.txt xarcade.bin
```
Upload config (effective immediately)
```
scwr xarcade.bin
```

For deleting all settings, make empty file and assemble and upload it.

### Compile tools with Linux (e.g. Raspberry Pi, Orange Pi)
```
sudo apt install build-essential
unzip Soarer_Converter_v1.10.zip
cd tools
unzip Soarer_sctools_v1.10_source.zip
cd build/linux
make
```


## Links
- https://geekhack.org/index.php?topic=17458.0
- http://www.computer-engineering.org/ps2protocol/
- http://www.computer-engineering.org/ps2keyboard/
- http://www.computer-engineering.org/ps2keyboard/scancodes2.html
- test: https://www.microsoft.com/appliedsciences/KeyboardGhostingDemo.mspx
- test: https://keycode.info/

## Keyboard controller
- https://geekhack.org/index.php?topic=50437.0
- https://deskthority.net/workshop-f7/soarer-s-keyboard-controller-firmware-t6767.html
Soarer_Controller_v1.20_beta4.zip


## X-Arcade (xarcade.txt)
```
# scas xarcade.txt xarcade.bin
# scwr xarcade.bin

# important!:
force set2ext

# dir1 keypad 8 (75),2 (72),4 (6B),6 (74)
# sel1 3
# start1 1
# A,B,C: left shift (12),Z,X
# X,Y,Z: left ctrl (14),left alt (11),space (29)
# lowerbuttons: c,5

remapblock
# left controller
	PAD_8	W
	PAD_2	S
	PAD_4	A
	PAD_6	D
	LSHIFT	SPACE
	
	Z	F
	X	G
	LCTRL	R
	LALT	T
	SPACE	Y
	C	V # "normal"
	# C	RCTRL # "commando"
	5	B
	3	Q
	1	E

# right controller
	R	PAD_8
	F	PAD_2
	D	PAD_4
	G	PAD_6
	W	RCTRL

	E	K
	LEFT_BRACE	L
	A	I
	S	O
	Q	P
	RIGHT_BRACE	N # "normal"
	# RIGHT_BRACE	SPACE # "commando"
	6	M
	2	U
	4	J
endblock

# dir2 r,f,d,g
# sel2 4
# start2 2
# A,B,C: w,e,}(å)(54="[")
# X,Y,Z: a,s,q
# lowerbuttons: ;(¨)(5B="]"),6
```


## X-Arcade BMC64 emulator
- xarcade_bmc64.txt

## IBM model F XT

Colors: https://geekhack.org/index.php?topic=17458.msg616598#msg616598
```
Brown = Vcc
Red = Ground
White = Data
Black = Clock
```

Soarer_Converter_v1.12_docs/docs/hardware.html/hardware.html
```
GND	GND
Vcc/+5V	VCC
Data	PD0
Clock	PD1
```

Conlusion:
```
Brown = Vcc (5) -> VCC
Red = Ground (4) -> GND
White = Data (2) -> PD0 (Pro Micro: 3)
Black = Clock (1) -> PD1 (Pro Micro: 2)
```

## X-Arcade With Mister FPGA
- xarcade_mister.txt
[xarcade_mister.txt](https://github.com/mcgurk/Arduino-USB-HID-RetroJoystickAdapter/blob/master/PS2_Soarer_Converter/xarcade_mister.txt)
