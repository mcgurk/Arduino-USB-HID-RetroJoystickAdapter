# Poor man's "Keyrah" to connect C64 keyboard to USB

## Building
- Solder all keyboard connector pins to Arduino Pro Micro.
- You can solder Restore key (connector pin "I") paraller to some other pin. I soldered "I" and "G" to Arduino pin A8. (this is actually not needed, because there is one free pin left)
- You can also connect LED of C64 to VCC/GND of Arduino. I used 220ohm resistor.

## Firmware/flashing
- Download Soarer controller firmware: https://geekhack.org/index.php?topic=50437.0
- Install Arduino IDE
- Connect RST to GND couple of times to get Arduino Pro Micro to programming mode (notice that com-port is different in programming mode in Windows)
- Flash firmware
```
& "C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avrdude" -C"C:\Program Files (x86)\Arduino\hardware\tools\avr/etc/avrdude.conf" -v -p m32u4 -c avr109 -P COM5 -b 57600 -U flash:w:firmware\Soarer_Controller_v1.20_beta4_atmega32u4.hex:i
```

## Setup
- Download configfile. Check pin order to match your setup.
- Compile and upload config file to Arduino Pro Micro
```
.\scas C64_matrix.sc C64_matrix.bin
.\scwr C64_matrix.bin
```
## Testing
- https://keycode.info/

## Pins I used
```
//KeyBoard Arduino Pro Micro 
//  Pin     Pin     Label			GPIO	pin (kirjaimet = columns (rivit), numerot = rows (otsikot))
//   20       5       2 - SDA		PD1		A
//   19       6       3 - SCL		PD0		B
//   18       7       4 - A6		PD4		C
//   17       8       5			PC6		D
//   16       9       6 - A7		PD7		E
//   15       10      7 - 		PE6		F
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
//   1        11      8 - A8 		PB4		I


## Links
- [https://geekhack.org/index.php?topic=50437.0]
- [https://deskthority.net/workshop-f7/soarer-s-keyboard-controller-firmware-t6767.html]
- Soarer_Controller_v1.20_beta4.zip (26.10.2013)
- [https://www.waitingforfriday.com/wp-content/uploads/2017/01/C64_Keyboard_Schematics_PNG.png]
- [http://kookye.com/wp-content/uploads/2016/02/Pinout-ProMicro.jpg]
- I used this as starting point: [https://github.com/abzman/Keyboard-config-file/blob/master/C64_matrix.sc]
