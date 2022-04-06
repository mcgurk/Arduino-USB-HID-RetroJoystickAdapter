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

## avr
- https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf
- https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
- http://ww1.microchip.com/downloads/en/devicedoc/atmel-0856-avr-instruction-set-manual.pdf

## using data/i/o space
- lds can load from io (if +0x20 added to address) or data space
- sts can store from register to io (if +0x20 added to address) or data space
- in can load from io to register
- out can write from register to io
- only one io/data address space access within one instruction?
- lds/sts takes 2 cycles and in/out takes 1 cycle? ldi takes 1 cycle?
- lds/sts: "M" \_SFR_MEM_ADDR(GPIOR2), in/out: "I" \_SFR_IO_ADDR(GPIOR0)

## interrupts
- https://billgrundmann.wordpress.com/2009/03/02/the-overhead-of-arduino-interrupts/
- https://forum.arduino.cc/t/how-fast-can-i-interrupt/25884/5
```
This is from the datasheet for the AT90USB82 processor; things in parenthesis are from me...

The interrupt execution response for all the enabled AVR interrupts is five clock cycles minimum (the processor is fixin' to execute the interrupt)
The vector is normally a jump to the interrupt routine, and this jump takes three clock cycles (the processor jumps to the ISR)
SREG must be saved and restored (the processor doesn't do this for us and SREG is important)
A return from an interrupt handling routine takes three clock cycles
When the AVR exits from an interrupt, it will always return to the main program and execute one more instruction before any pending interrupt is served
Those are the things necessary just to get the ISR called. We have not yet added the application stuff (incrementing an unsigned long in BetterSense's case).

Adding those up gives us 5+3+2+3+1 = 14. The absolute maximum number of interrupts per second that can be handled by the AT90USB82 is 16 million instructions per second / 14 instructions per interrupt = 1,142,857 interrupts per second.
```

## avr asm
- http://www.nongnu.org/avr-libc/user-manual/inline_asm.html
- https://ucexperiment.wordpress.com/2016/03/04/arduino-inline-assembly-tutorial-1/
- https://ucexperiment.wordpress.com/2016/03/11/arduino-inline-assembly-tutorial-5-2/

## atomic
- https://groups.google.com/a/arduino.cc/g/developers/c/cmu0Qy32zxY
- http://www.gammon.com.au/forum/?id=11488
- https://home.csulb.edu/~hill/ee346/Lectures/10%20ATmega32U4%20Interrupts.pdf

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
