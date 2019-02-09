C64:
- https://geekhack.org/index.php?topic=50437.0
- https://deskthority.net/workshop-f7/soarer-s-keyboard-controller-firmware-t6767.html
- Soarer_Controller_v1.20_beta4.zip (26.10.2013)
- https://www.waitingforfriday.com/wp-content/uploads/2017/01/C64_Keyboard_Schematics_PNG.png
- http://kookye.com/wp-content/uploads/2016/02/Pinout-ProMicro.jpg
- https://github.com/abzman/Keyboard-config-file/blob/master/C64_matrix.sc
- https://github.com/dabonetn/C64USBKey/blob/master/C64USBKeyboard2.ino
- https://raw.githubusercontent.com/abzman/Keyboard-config-file/master/C64_matrix.sc

```
//KeyBoard Arduino Pro Micro 
//  Pin     Pin     Label			GPIO	pin (kirjaimet = columns (rivit), numerot = rows (otsikot))
//   20       5       2 - SDA		PD1		A
//   19       6       3 - SCL		PD0		B
//   18       7       4 - A6		PD4		C
//   17       8       5				PC6		D
//   16       9       6 - A7		PD7		E
//   15       10      7 - 			PE6		F
//   14       11      8 - A8 		PB4		G
//   13       12      9 - A9 		PB5		H
//   12       13      10 - A10 		PB6		0
//   11       14      16 - MOSI 	PB2		1
//   10       15      14 - MISO 	PB3		2
//   9        16      15 - SCLK 	PB1		3
//   8        17      18 - A0 		PF7		4
//   7        18      19 - A1 		PF6		5
//   6        19      20 - A2 		PF5		6
//   5        20      21 - A3 		PF4		7
//   4        N/C
//   3        1       1 - TX		PD3		8
//   2        N/C
//   1        11      8 - A8 		PB4		1
```
```
	sense			PB6			PB2		PB3			PB1		PF7		PF6		PF5		PF4		PD3
	strobe	PD1		1			ESC		LCTRL		PAUSE	SPACE	LGUI	Q		2		UNASSIGNED
	strobe	PD0		3			W		A			LSHIFT	Z		S		E		4		UNASSIGNED
	strobe	PD4		5			R		D			X		C		F		T		6		UNASSIGNED
	strobe	PC6		7			Y		G			V		B		H		U		8		UNASSIGNED
	strobe	PD7		9			I		J			N		M		K		O		0		UNASSIGNED
	strobe	PE6		LANG_1		P		L			COMMA	PERIOD	LANG_3	LANG_4	MINUS	UNASSIGNED
	strobe	PB4		BACKSLASH	LANG_2	SEMICOLON	SLASH	RSHIFT	EQUAL	INSERT	HOME	LANG_5
	strobe	PB5		BACKSPACE	ENTER	RIGHT		DOWN	F1		F3		F5		F7		UNASSIGNED
```
Reset to GND couple of times, then (notice that com-port is different in programming mode):
```
& "C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avrdude" -C"C:\Program Files (x86)\Arduino\hardware\tools\avr/etc/avrdude.conf" -v -p m32u4 -c avr109 -P COM5 -b 57600 -U flash:w:firmware\Soarer_Controller_v1.20_beta4_atmega32u4.hex:i
```
```
.\scas C64_matrix.sc C64_matrix.bin
.\scwr C64_matrix.bin
```
Tallettuu vaikka firmiksen uudelleenflashais!!!
http://www.keyboardtester.com/tester.html

