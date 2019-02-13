```
13.2.2019:
Koko ATmega32U4 tuntuu puuttuvan...
Löytyy kyllä kun tekee uuden projektin...
Examplet puuttuu kaikki...
Tools->Extensions and Updates...->Lufa
Nyt löytyy.
Lufa->New Example Project->FourWalledCubicle - Dean Camera->Joystick HID Device Demo (Class Driver APIs) - AVR8 Architecture
Project->Properties->Change Device...->ATmega32U4 (rutkuttaa jotain, mutta suostuu silti)

Tools->External tools
Title:
Arduino_via_bootloader
command:
C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avrdude.exe
Arguments:
-C"C:\Program Files (x86)\Arduino\hardware\tools\avr/etc/avrdude.conf" -v -patmega32u4 -cavr109 -PCOM5 -b57600 -D -Uflash:w:"$(BinDir)\$(TargetName).hex":i
Use Output window

Pari kertaa GND->RST, sitten Tools->Arduino_via_bootloader
C:\Users\lehti\Documents\Atmel Studio\7.0\CLASS_JOYSTICK1\CLASS_JOYSTICK1\Debug\CLASS_JOYSTICK1.hex

Tulee "LUFA Joystick Demo", 2 nappia ja kolme axista (X,Y,Z).
Näkyy Device managerissa "HID-compliant game controller".
0x03EB, 0x2043, Atmel Corp., LUFA Joystick Demo Application.

VID/PID ja "LUFA Joystick Demo": Descriptors.c
suuntien ja nappien määrä ehkä structissa: Joystick.h
Ja myös: src/LUFA/Drivers/USB/Class/Common/HIDClassCommon.h (HID_DESCRIPTOR_JOYSTICK)
Pois: HID_RI_USAGE(8, 0x32)
Muokkaa: HID_RI_REPORT_COUNT(8, 3) -> 2

Koodi: Joystick.c

GitHub-integraation vois tehdä external tools:lla.

PB6,2,3,1
PB5,4
```
## Joystick.c
SetupHardware (before USB_INIT();):
```
	DDRB = 0x0; //PORTB=input
	PORTB = 0xff; //PORTB=pullup
```
  
CALLBACK_HID_Device_CreateHIDReport (before report size):
```
	if (!(in & (1 << 6))) JoystickReport->Y = -100;
	if (!(in & (1 << 2))) JoystickReport->Y = 100;
	if (!(in & (1 << 3))) JoystickReport->X = -100;
	if (!(in & (1 << 1))) JoystickReport->X = 100;
	if (!(in & (1 << 5))) JoystickReport->Button |= (1 << 0);
	if (!(in & (1 << 4))) JoystickReport->Button |= (1 << 1);
```
