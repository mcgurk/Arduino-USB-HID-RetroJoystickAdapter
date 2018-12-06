// Arduino Pro Micro
// http://cloud.cbm8bit.com/penfold42/joytester.zip
// https://arduino.stackexchange.com/questions/8758/arduino-interruption-on-pin-change/8926
// http://www.pighixxx.net/wp-content/uploads/2016/07/pro_micro_pinout_v1_0_red.png

// TXD (INT3,PD3) and RXD (INT2,PD2) together and to userport L
// INT2(RXD) is used for rising edge and INT3(TXD) for falling edge 


// Joystick port 3
// GND = GND (8)
#define up1 (~PD & _BV(7)) // 6,PD7 = (1)
#define down1 (~PC & _BV(6)) // 5,PC6 = (2)
#define left1 (~PD & _BV(4)) // 4,PD4 = (3)
#define right1 (~PD & _BV(0)) // 3,PD0 = (4)
#define fire1 (~PD & _BV(1)) // 2,PD1 = (6)

// Joystick port 4
// GND = GND (8)
#define up2 (~PF & _BV(7)) // A0,PF7 = (1)
#define down2 (~PF & _BV(6)) // A1,PF6 = (2)
#define left2 (~PF & _BV(5)) // A2,PF5 = (3)
#define right2 (~PF & _BV(4)) // A3,PF4 = (4)
#define fire2 (~PE & _BV(6)) // 7,PE6 = (6)

// Arduino <-> Userport
// VCC = +5V (2)
// GND = GND (A)
// TXD+RXD = Select = PB7 (L)
#define upC 1 // 15,PB1 = PB0 (C) 
#define downC 3 // 14,PB3 = PB1(D)
#define leftC 2 // 16,PB2 = PB2 (E)
#define rightC 6 // 10,PB6 = PB3 (F)
#define fire1C 4 // 8,PB4 = PB4 (H)
#define fire2C 5 // 9,PB5 = PB5 (J)

volatile uint8_t output1;
volatile uint8_t output2;

ISR(INT2_vect, ISR_NAKED) {
    asm volatile(
    "    push r0                \n"  // save register r0
    "    lds r0, output1        \n"
    "    out %[pin], r0         \n"
    "    pop r0                 \n"  // restore previous r0
    "    rjmp INT2_vect_part_2  \n"  // go to part 2 for required prologue and epilogue
    :: [pin] "I" (_SFR_IO_ADDR(PORTB)));
}

ISR(INT2_vect_part_2) {}

ISR(INT3_vect, ISR_NAKED) {
    asm volatile(
    "    push r0                \n"  // save register r0
    "    lds r0, output2        \n"
    "    out %[pin], r0         \n"
    "    pop r0                 \n"  // restore previous r0
    "    rjmp INT3_vect_part_2  \n"  // go to part 2 for required prologue and epilogue
    :: [pin] "I" (_SFR_IO_ADDR(PORTB)));
}

ISR(INT3_vect_part_2) {}

void setup() {
  pinMode(5, INPUT_PULLUP); // pin5 (PC6) is input
  pinMode(7, INPUT_PULLUP); // pin7 (PE6) is input
  DDRB = 0xff; //all PB-ports are outputs
  DDRF = 0; PORTF = 0xff; // all PF-ports are inputs with pullups
  DDRD = 0; PORTD = B11010011; // all PD-ports are inputs with pullups (PD2,PD3,PD5 without pullup)
  
  TIMSK0 = 0; // disable timer0 interrupts (Arduino Uno/Pro Micro millis() update ISR)
  EICRA = B10110000;  // INT2 – rising edge on RXD (Bxx11xxxx), INT3 - falling edge on TXD (B10xxxxxx)
  EIMSK = B1100;  // enable int2 (Bx1xx) and int3 (B1xxx)

  //Serial.begin(115200);
}

uint8_t joy1, joy2, PB, PF, PD, PC, PE;

void loop() {
  PB = PINB; PF = PINF; PD = PIND; PC = PINC; PE = PINE;
  joy1 = 0; joy2 = 0;
  if (up1) bitSet(joy1,upC);
  if (down1) bitSet(joy1,downC);
  if (left1) bitSet(joy1,leftC);
  if (right1) bitSet(joy1,rightC);
  if (up2) bitSet(joy2,upC);
  if (down2) bitSet(joy2,downC);
  if (left2) bitSet(joy2,leftC);
  if (right2) bitSet(joy2,rightC);
  if (fire1) { bitSet(joy1,fire1C); bitSet(joy2,fire1C); }
  if (fire2) { bitSet(joy1,fire2C); bitSet(joy2,fire2C); }
  output1 = joy1; output2 = joy2;
  //here should be something that updates even if interrupts doesn't trigger
  //Serial.print(joy1, BIN); Serial.print(" "); Serial.println(joy2, BIN); delay(100);
  //delayMicroseconds(50);
}
