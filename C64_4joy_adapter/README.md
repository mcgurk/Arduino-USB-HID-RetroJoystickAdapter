- https://www.protovision.games/hardw/4_player.php?language=en

```
& 'C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avr-objdump.exe' -S "C:\\Users\\lehti\\AppData\\Local\\Temp\\arduino_build_208947/c64_4joystick-adapter.ino.elf" > c:\temp\koe.txt
```

```
 cli // 1 clock
 PORTB = *ptr; // is this atomic? probably, because ptr is 6-bit pointer. nope...
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
mov r30, gpior0
ld	r24, Z
out	0x05, r24
sei
``` 
375ns

## using i/o space
- Can be used with in/out (i/o space) or lds (data space = i/o address+0x20)?
- Compiler can detect what space is used and uses correct address?
- in/out is 1 cycle? lds 2 cycles?

## avr asm
- http://www.nongnu.org/avr-libc/user-manual/inline_asm.html

## register as variable
- https://forum.arduino.cc/index.php?topic=43760.5
- http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_regbind
- volatile register unsigned char my_register asm("r2");
- https://www.avrfreaks.net/forum/binding-variable-register

## running code from ram
- https://forum.arduino.cc/index.php?topic=425962.0
- https://forum.arduino.cc/index.php?topic=470631.0
- "AVRs are Harvard architecture CPUs, so they CANNOT run code out of RAM, so that directive can't possibly do anything of any value whatsoever."
- "Yes, Harvard IS the reason.  A Harvard CPU, by definition, has separate code and data memory spaces.  They execute code from one memory, and fetch data from a different memory.  What you're seeing is most likely the compiler pretending the directive can do what you want, but the linker doing the only thing it can do - putting that code in FLASH."
