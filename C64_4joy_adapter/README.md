```
& 'C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-objdump.exe' -S "C:\\Users\\Lehtinen\\AppData\\Local\\Temp\\arduino_build_119176/test.ino.elf" > c:\temp\koe.txt
```

```
 cli // 1 clock
 PORTB = *ptr; // is this atomic? probably, because ptr is 6-bit pointer.
 f98:	e0 91 26 01 	lds	r30, 0x0126	; 0x800126 <__data_end> // 2 clocks
 f9c:	f0 91 27 01 	lds	r31, 0x0127	; 0x800127 <__data_end+0x1> // 2 clocks
 fa0:	80 81       	ld	r24, Z // 1 clock
 fa2:	85 b9       	out	0x05, r24	; 5 // 1 clock
 sei // 1 clock
``` 
500ns

```
cli
mov r31, r1
mov r30, gpior2
ld	r24, Z
out	0x05, r24
sei
``` 
375ns
