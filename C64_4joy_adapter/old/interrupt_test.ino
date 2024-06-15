#define LED_PIN 5  // digital pin #13 (portb) (pin #9 with Arduino Pro Micro)
#define LED_ON() PORTB |= _BV(LED_PIN)
#define LED_OFF() PORTB &= ~_BV(LED_PIN)

volatile uint8_t output1;
volatile uint8_t output2;

ISR(INT0_vect, ISR_NAKED)
{
    asm volatile(
    "    push r0                \n"  // save register r0
    "    lds r0, output1        \n"
    "    out %[pin], r0         \n"
    "    pop r0                 \n"  // restore previous r0
    "    rjmp INT0_vect_part_2  \n"  // go to part 2 for required prologue and epilogue
    :: [pin] "I" (_SFR_IO_ADDR(PORTB)));
}

ISR(INT0_vect_part_2) {
}

ISR(INT1_vect, ISR_NAKED)
{
    asm volatile(
    "    push r0                \n"  // save register r0
    "    lds r0, output2        \n"
    "    out %[pin], r0         \n"
    "    pop r0                 \n"  // restore previous r0
    "    rjmp INT1_vect_part_2  \n"  // go to part 2 for required prologue and epilogue
    :: [pin] "I" (_SFR_IO_ADDR(PORTB)));
}

ISR(INT1_vect_part_2) {
}

void setup() {
  //pinMode(13, OUTPUT); // Arduino Uno
  pinMode(9, OUTPUT); // Arduino Pro Micro
  TIMSK0 = 0; // disable timer0 interrupts (Arduino Uno/Pro Micro millis() update ISR)
  EICRA = B1011;  // INT0 – rising edge on 2 (B11), INT1 - falling edge on 3 (B10xx)
  EIMSK = B11;  // enable int0 (Bx1) and int1 (B1x)
}

void loop() {
  LED_ON();
  //LED_OFF();
  output1 = 0;
  delayMicroseconds(50);
}
