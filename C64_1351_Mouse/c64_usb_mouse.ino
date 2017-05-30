#include <hidboot.h>
#include <usbhub.h>

/*#define SENSEPORT   PORTD       ///< SID sense port
#define SENSEDDR    DDRD        ///< SID sense data direction
#define SENSEPIN    PIND        ///< SID sense input*/
#define POTSENSE    3           ///< INT1 attached to PORTD.3,

/*#define POTPORT     PORTB       ///< POT-controlling outputs X and Y
#define POTDDR      DDRB        ///< POT outputs data direction
#define POTPIN      PINB        ///< POT outputs input ;)*/
#define POTX        9           ///< X-line, also OC1A
#define POTY        10          ///< Y-line, also OC1B
#define LBTN        4           ///< Joystick FIRE switch
#define RBTN        5           ///< Joystick UP switch

/*#define JOYPORT     PORTC       ///< Joystick pins
#define JOYDDR      DDRC        ///< Joystick pins data direction (out = switch closed to gnd)
#define JOYPIN      PINC        ///< Joystick in*/
/*#define JOYUP       5           ///< Joystick UP switch
#define JOYDOWN     6           ///< Joystick DOWN switch
#define JOYLEFT     7           ///< Joystick LEFT switch
#define JOYRIGHT    8           ///< Joystick RIGHT switch
#define JOYFIRE     9           ///< Joystick FIRE switch*/

//#define DEBUG

int16_t dx=0;
int16_t dy=0;
uint8_t buttons=0;

uint8_t update = 0;
#define FIX_START 90
int16_t fix = FIX_START;

class MouseRptParser : public MouseReportParser {
protected:
	void OnMouseMove(MOUSEINFO *mi);
	void OnLeftButtonUp(MOUSEINFO *mi);
	void OnLeftButtonDown(MOUSEINFO *mi);
	void OnRightButtonUp(MOUSEINFO *mi);
	void OnRightButtonDown(MOUSEINFO *mi);
	void OnMiddleButtonUp(MOUSEINFO *mi);
	void OnMiddleButtonDown(MOUSEINFO *mi);
};
void MouseRptParser::OnMouseMove(MOUSEINFO *mi) {
  #ifdef DEBUG
  Serial.print("dx=");
  Serial.print(mi->dX, DEC);
  Serial.print(" dy=");
  Serial.println(mi->dY, DEC);
  #endif
  dx=mi->dX;
  dy=mi->dY;
  update = 1;
};
void MouseRptParser::OnLeftButtonUp(MOUSEINFO *mi) {
  #ifdef DEBUG
  Serial.println("L Butt Up");
  #endif
  buttons &= ~1;
  update = 1;
};
void MouseRptParser::OnLeftButtonDown(MOUSEINFO *mi) {
  #ifdef DEBUG
  Serial.println("L Butt Dn");
  #endif
  buttons |= 1;
  update = 1;
  fix++;
};
void MouseRptParser::OnRightButtonUp(MOUSEINFO *mi) {
  #ifdef DEBUG
  Serial.println("R Butt Up");
  #endif
  buttons &= ~2;
  update = 1;
};
void MouseRptParser::OnRightButtonDown(MOUSEINFO *mi) {
  #ifdef DEBUG
  Serial.println("R Butt Dn");
  #endif
  buttons |= 2;
  update = 1;
  fix--;
};
void MouseRptParser::OnMiddleButtonUp(MOUSEINFO *mi) {
  #ifdef DEBUG
  Serial.println("M Butt Up");
  #endif
};
void MouseRptParser::OnMiddleButtonDown(MOUSEINFO *mi) {
  #ifdef DEBUG
  Serial.println("M Butt Dn");
  #endif
  fix = FIX_START;
};

USB Usb;
USBHub Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);

MouseRptParser Prs;


enum _potmode {
  POTMOUSE_C1351 = 0,             //<! proportional mode
  POTMOUSE_JOYSTICK               //<! joystick mode
};


static uint8_t potmouse_xcounter;           ///< x axis counter
static uint8_t potmouse_ycounter;           ///< y axis counter

static volatile uint16_t ocr1a_load;        ///< precalculated OCR1A value (YPOT)
static volatile uint16_t ocr1b_load;        ///< precalculated OCR1B value (XPOT)

#define OCR_ZERO 320          ///< zero point (320us)

//static volatile uint8_t mode;               ///< mouse mode
uint8_t mode;               ///< mouse mode

void setup() {
  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Start");
  Serial.flush();
  delay(200);
  #endif

  if (Usb.Init() == -1) {
    #ifdef DEBUG
    Serial.println("OSC did not start.");
    #endif
  }
  delay(200);
  HidMouse.SetReportParser(0, &Prs);

  #ifdef DEBUG
  Serial.flush();
  delay(200);
  #endif

  mode = POTMOUSE_C1351;
  pinMode(LBTN, INPUT); pinMode(RBTN, INPUT);
  
  pinMode(POTX, OUTPUT); pinMode(POTY, OUTPUT);
  digitalWrite(POTX, HIGH); digitalWrite(POTY, HIGH);
  pinMode(POTSENSE, INPUT); // pullup off, hi-biased by OC1A
  attachInterrupt(digitalPinToInterrupt(POTSENSE), potSenseInt, FALLING);

  potmouse_movt(0,0,0);

  // prepare INT1
  //GICR &= ~_BV(INT1);                     // disable INT1
  //MCUCR &= ~(_BV(ISC11)|_BV(ISC10));  
  //MCUCR |= _BV(ISC11);                    // ISC11:ISC10 == 10, @negedge   
}

void loop() {
  Usb.Task();
  if (update) {
    potmouse_movt(dx, dy, buttons);
    update = 0;
  }
  delayMicroseconds(200);
  //potmouse_movt(dx, dy, buttons);
  /*noInterrupts();
  if(digitalRead(POTSENSE
  interrupts();*/
}


void potmouse_movt(int16_t dx, int16_t dy, uint8_t button) {
  uint16_t a, b;
    
  switch (mode) {
    case POTMOUSE_C1351:
      /*potmouse_xcounter = (potmouse_xcounter + dx) & 077; // modulo 64
      //potmouse_ycounter = (potmouse_ycounter + dy) & 077;
      potmouse_ycounter = (potmouse_ycounter - dy) & 077;*/

      //potmouse_xcounter = (potmouse_xcounter + (dx/2)) & 077; // modulo 64
      //potmouse_ycounter = (potmouse_ycounter - (dy/2)) & 077;
      potmouse_xcounter = (potmouse_xcounter + (dx/4)) & 077; // modulo 64
      potmouse_ycounter = (potmouse_ycounter - (dy/4)) & 077;

      //potmouse_xcounter = (millis()>>6) & 077; // modulo 64
      //potmouse_ycounter = (millis()>>6) & 077;
      /*(button & 001) ? (JOYDDR |= _BV(JOYFIRE)) : (JOYDDR &= ~_BV(JOYFIRE));
      (button & 002) ? (JOYDDR |= _BV(JOYUP))   : (JOYDDR &= ~_BV(JOYUP));
      (button & 004) ? (JOYDDR |= _BV(JOYDOWN)) : (JOYDDR &= ~_BV(JOYDOWN));*/

      (button & 001) ? pinMode(LBTN, OUTPUT) : pinMode(LBTN, INPUT);
      (button & 002) ? pinMode(RBTN, OUTPUT) : pinMode(RBTN, INPUT);
      
            
      // scale should be 2x here, but for this particular chip, 66 counts work better where
      // 64 counds should be. so 66/64=100/96 and times two
      //a = (OCR_ZERO + potmouse_xcounter*200/96)*2; // "*2" because 16MHz, not 8MHz //yli
      //b = (OCR_ZERO + potmouse_ycounter*200/96)*2; // "*2" because 16MHz, not 8MHz
      //a = OCR_ZERO + potmouse_xcounter*2; // "*2" because 16MHz, not 8MHz
      //b = OCR_ZERO + potmouse_ycounter*2; // "*2" because 16MHz, not 8MHz
      //a = (OCR_ZERO + potmouse_xcounter*200/90)*2; // "*2" because 16MHz, not 8MHz
      //b = (OCR_ZERO + potmouse_ycounter*200/90)*2; // "*2" because 16MHz, not 8MHz
      //a = OCR_ZERO*2 + potmouse_xcounter*400/85; // "*2" because 16MHz, not 8MHz
      //b = OCR_ZERO*2 + potmouse_ycounter*400/85; // "*2" because 16MHz, not 8MHz
      //a = OCR_ZERO*2 + potmouse_xcounter*400/80; // "*2" because 16MHz, not 8MHz //ali
      //b = OCR_ZERO*2 + potmouse_ycounter*400/80; // "*2" because 16MHz, not 8MHz
      //a = OCR_ZERO*2 + potmouse_xcounter*400/93; // "*2" because 16MHz, not 8MHz //vois olla lähellä
      //b = OCR_ZERO*2 + potmouse_ycounter*400/93; // "*2" because 16MHz, not 8MHz
      //a = OCR_ZERO*2 + potmouse_xcounter*400/91; // "*2" because 16MHz, not 8MHz //vois olla lähellä
      //b = OCR_ZERO*2 + potmouse_ycounter*400/91; // "*2" because 16MHz, not 8MHz
      //a = OCR_ZERO*2-1 + potmouse_xcounter*400/94; // "*2" because 16MHz, not 8MHz
      //b = OCR_ZERO*2-1 + potmouse_ycounter*400/94; // "*2" because 16MHz, not 8MHz
      //a = OCR_ZERO*2-2 + potmouse_xcounter*400/92; // "*2" because 16MHz, not 8MHz
      //b = OCR_ZERO*2-2 + potmouse_ycounter*400/92; // "*2" because 16MHz, not 8MHz
      //a = OCR_ZERO*2+2 + potmouse_xcounter*400/92; // "*2" because 16MHz, not 8MHz //valuu vasemmalle alas
      //b = OCR_ZERO*2+2 + potmouse_ycounter*400/92; // "*2" because 16MHz, not 8MHz

      //a = OCR_ZERO*2 + potmouse_xcounter*400/fix; // "*2" because 16MHz, not 8MHz
      //b = OCR_ZERO*2 + potmouse_ycounter*400/fix; // "*2" because 16MHz, not 8MHz

      //a = OCR_ZERO*2-20 + potmouse_xcounter*400/fix; // "*2" because 16MHz, not 8MHz
      //b = OCR_ZERO*2-20 + potmouse_ycounter*400/fix; // "*2" because 16MHz, not 8MHz

      a = OCR_ZERO*2 + potmouse_xcounter*400/92; // "*2" because 16MHz, not 8MHz
      b = OCR_ZERO*2 + potmouse_ycounter*400/92; // "*2" because 16MHz, not 8MHz

      ocr1a_load = a;
      ocr1b_load = b;
      break;
    case POTMOUSE_JOYSTICK:
      /*JOYDDR  &= ~(_BV(JOYFIRE) | _BV(JOYUP) | _BV(JOYDOWN) | _BV(JOYLEFT) | _BV(JOYRIGHT));

      (dx < 0) ? (JOYDDR |= _BV(JOYLEFT)) : (JOYDDR &= ~_BV(JOYLEFT));
      (dx > 0) ? (JOYDDR |= _BV(JOYRIGHT)): (JOYDDR &= ~_BV(JOYRIGHT));
      (dy < 0) ? (JOYDDR |= _BV(JOYDOWN)) : (JOYDDR &= ~_BV(JOYDOWN));
      (dy > 0) ? (JOYDDR |= _BV(JOYUP))   : (JOYDDR &= ~_BV(JOYUP));
      (button & 001) ? (JOYDDR |= _BV(JOYFIRE)) : (JOYDDR &= ~_BV(JOYFIRE));
      (button & 002) ? (POTDDR |= _BV(POTX)) : (POTDDR &= ~_BV(POTX));
  
      TCNT1 = 65535-256;
      TCCR1A = 0;
      TCCR1B = _BV(CS12)|_BV(CS10);
      TIFR |= _BV(TOV1);
      TIMSK |= _BV(TOIE1);*/
      break;
  }
}


/// SID measuring cycle detected.
///
/// 1. SID pulls POTX low\n
/// 2. SID waits 256 cycles us\n
/// 3. SID releases POTX\n
/// 4. 0 to 255 cycles until the cap is charged\n
///
/// This handler stops the Timer1, clears OC1A/OC1B outputs,
/// loads the timer with values precalculated in potmouse_movt()
/// and starts the timer. 
///
/// OC1A/OC1B (YPOT/XPOT) lines will go up by hardware. 
/// Normal SID cycle is 512us. Timer will overflow not before 65535us.
/// Next cycle will begin before that so there's no need to stop the timer.
/// Output compare match interrupts are thus not used.

void potSenseInt() {
  #ifdef DEBUG
  Serial.println("PotSenseInt"); Serial.flush();
  #endif
  // SID started to measure the pots, uuu
  
  // disable INT1 until the measurement cycle is complete
  // stop the timer
  TCCR1B = 0;
    
  // clear OC1A/OC1B:
  // 1. set output compare to clear OC1A/OC1B ("10" in table 37 on page 97)
  TCCR1A = _BV(COM1A1) | _BV(COM1B1);
  // 2. force output compare to make it happen
  TCCR1C |= _BV(FOC1A) | _BV(FOC1B);

  // Set OC1A/OC1B on Compare Match (Set output to high level) 
  // WGM13:0 = 00, normal mode: count from BOTTOM to MAX
  TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(COM1B0);

  // load the timer 
  TCNT1 = 0;
    
  // init the output compare values 
  OCR1A = ocr1a_load;
  OCR1B = ocr1b_load;
    
  // start timer with prescaler clk/8 (1 count = 1us) // (with 16MHz, 1 count = 0.5us)
  TCCR1B = _BV(CS11);

  // TEST!!!
  //TCCR1B = _BV(CS12) | _BV(CS10); // Prescaler clock/1024

  //TEST!!!:
  /*OCR1A = 15625*2; // 16 000 000 / 1024 / 15625 = 1s
  OCR1B = 15625/2;
  TCCR1B = _BV(CS12) | _BV(CS10); // Prescaler clock/1024*/
  
}

/*
/// TIMER1 Overflow vector
///
/// Ends joystick emulator pulse.
ISR(TIMER1_OVF_vect) {
    //JOYDDR  &= ~(_BV(JOYFIRE) | _BV(JOYUP) | _BV(JOYDOWN) | _BV(JOYLEFT) | _BV(JOYRIGHT));
    //POTDDR  &= ~_BV(POTX);
    TIMSK1 &= ~_BV(TOIE1);
}*/

