#define USBHOST

#ifdef USBHOST
#include <hidboot.h>
#include <usbhub.h>
#endif

#define POTSENSE    4           //ICP1

#define POTX        9           ///< X-line, also OC1A
#define POTY        10          ///< Y-line, also OC1B
#define LBTN        5           ///< Joystick FIRE switch
#define RBTN        6           ///< Joystick UP switch

//#define DEBUG

int16_t dx=0;
int16_t dy=0;
uint8_t buttons=0;

uint8_t update = 0;
#define FIX_START 100
int16_t fix = FIX_START;

#ifdef USBHOST
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
#endif

static uint8_t potmouse_xcounter;           ///< x axis counter
static uint8_t potmouse_ycounter;           ///< y axis counter

static volatile uint16_t ocr1a_load;        ///< precalculated OCR1A value (YPOT)
static volatile uint16_t ocr1b_load;        ///< precalculated OCR1B value (XPOT)

void setup() {
  #ifdef DEBUG
  Serial.begin(115200);
  delay(200);
  Serial.println("Start");
  Serial.flush();
  #endif

  #ifdef USBHOST
  if (Usb.Init() == -1) {
    #ifdef DEBUG
    Serial.println("OSC did not start.");
    #endif
  }
  delay(200);
  HidMouse.SetReportParser(0, &Prs);
  #endif

  #ifdef DEBUG
  Serial.flush();
  delay(200);
  #endif

  pinMode(LBTN, INPUT); pinMode(RBTN, INPUT);
  
  pinMode(POTX, OUTPUT); pinMode(POTY, OUTPUT);
  digitalWrite(POTX, HIGH); digitalWrite(POTY, HIGH);
  pinMode(POTSENSE, INPUT); // pullup off, hi-biased by OC1A

  potmouse_movt(0,0,0);

  startTimers();

  #ifndef USBHOST
  TIMSK0 = 0;
  #endif
  
}

void loop() {
  #ifdef USBHOST
  Usb.Task();
  if (update) {
    potmouse_movt(dx, dy, buttons);
    update = 0;
  }
  delayMicroseconds(200);
  #endif

  #ifndef USBHOST
  potmouse_movt(0, 0, buttons);
  #endif

}

volatile uint8_t counter = 0;
volatile uint8_t upd = 0;

void potmouse_movt(int16_t dx, int16_t dy, uint8_t button) {
  uint16_t a, b;

  #ifndef USBHOST
  if (upd) {
    potmouse_xcounter++;
    potmouse_ycounter++;
    upd = 0;
  }
  #endif

  potmouse_xcounter = (potmouse_xcounter + (dx/2)) & 0177; // modulo 128
  potmouse_ycounter = (potmouse_ycounter - (dy/2)) & 0177;
  
  //potmouse_xcounter = (millis()>>6) & 077; // modulo 64
  //potmouse_ycounter = (millis()>>6) & 077;

  (button & 001) ? pinMode(LBTN, OUTPUT) : pinMode(LBTN, INPUT);
  (button & 002) ? pinMode(RBTN, OUTPUT) : pinMode(RBTN, INPUT);
            
  // scale should be 2x here, but for this particular chip, 66 counts work better where
  // 64 counds should be. so 66/64=100/96 and times two
  //a = 320*2 + potmouse_xcounter*2*100/fix;
  //b = 320*2 + potmouse_ycounter*2*100/fix;
  //a = 320*2*100/fix + potmouse_xcounter*2;
  //b = 320*2*100/fix + potmouse_ycounter*2;
  a = 320*2 + potmouse_xcounter*2;
  b = 320*2 + potmouse_ycounter*2;

  ocr1a_load = a;
  ocr1b_load = b;
}


// Start timer1, Input Capture setup
// ICNC1: Input Capture Noise Canceller (Bit 7 of register TCCR1B) 
// ICES1: Input Capture Edge Select (Bit 6 of register TCCR1B) 0 = FALLING, 1 = RISING
// CS12, CS11, CS10: Set prescaler (CS11 TIMER1: F_CPU/8)
//#define START_TIMER1 TCCR1B = _BV(ICNC1) | _BV(CS11)
#define START_TIMER1 TCCR1B = _BV(CS11)
// CS32, CS31, CS30: Set prescaler (CS31 TIMER3: F_CPU/8)
#define START_TIMER2 TCCR3B = _BV(CS31)

inline void startTimers() {
  #ifdef DEBUG
  Serial.println("startTimers"); Serial.flush();
  #endif
  cli();

  // Prepare TIMER1
  //TCCR1A = 0;

  // ICIE1: Timer/Counter Input Capture Interrupt Enable, ISR(TIMER1_CAPT_vect)
  // TOIE1: Timer/Counter Overflow Interrupt Enable
  TIMSK1 = _BV(ICIE1); // ICIE1: Timer/Counter1, Input Capture Interrupt Enable

  // Start timer1, Input Capture setup
  // ICNC1: Input Capture Noise Canceller (Bit 7 of register TCCR1B) 
  // ICES1: Input Capture Edge Select (Bit 6 of register TCCR1B) 0 = FALLING, 1 = RISING
  // CS12, CS11, CS10: Set prescaler (CS11 TIMER1: F_CPU/8)
  //TCCR1B = _BV(ICNC1) | _BV(CS11)
  TCCR1B = _BV(CS11)

  TIFR1 = 0xff; // Clear all pending TIMER1 interrupt flags

  sei();  
}



ISR(TIMER1_CAPT_vect) {
  // Now we little after start of SID reading process
  // Start moment is in ICR1L and time now is in TCNT2
  #ifdef DEBUG
  Serial.println("TIMER1_CAPT_vect:");
  #endif

  #ifndef USBHOST
  counter++;
  counter &= 63;
  if (counter == 0) upd = 1;
  #endif
  
  // clear OC1A/OC1B (9 and 10 to LOW):
  // 1. set output compare to clear OC1A/OC1B ("10" in table 37 on page 97)
  TCCR1A = _BV(COM1A1) | _BV(COM1B1); // Clear OC1A / OC1B on Compare Match (Set output to low level).
  // 2. force output compare to make it happen (doesn't raise interrupts)
  TCCR1C |= _BV(FOC1A) | _BV(FOC1B); // FOC1A / FOC1B Force Output Compare A and B (that are in register TCCR1C)
  
  // OCIE1A: Timer/Counter Output Compare Match Interrupt Enable A, ISR(TIMER1_COMPA_vect)
  TIMSK1 = _BV(OCIE1A);
  
  // init the output compare values 
  //OCR1A = 15625*2; // 16 000 000 / 1024 / 15625 = 1s
  //OCR1B = 15625/2;
  OCR1A = ocr1a_load + ICR1;
  OCR1B = ocr1b_load + ICR1;
  //TEST!
  //OCR1A = ocr1a_load;
  //OCR1B = ocr1b_load;

  // Set OC1A/OC1B on Compare Match (Set output to high level) 
  // WGM13:0 = 00, normal mode: count from BOTTOM to MAX
  TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(COM1B0); // Set OC1A / OC1B on Compare Match (Set output to high level).

  #ifdef DEBUG
  Serial.print(c); Serial.print(" "); Serial.print(a); Serial.print(" "); Serial.println(b);
  Serial.flush();
  #endif
  
  TIFR1 = 0xff; //clear all timer1 interrupt flags
}

ISR(TIMER1_COMPA_vect) {
  // now potx are sent. we don't know if poty is still in progress.
  // POTX is HIGH from OC1A TIMER1 compare match
  
  #ifdef DEBUG
  Serial.println("TIMER1_COMPA_vect"); Serial.flush();
  #endif

  TIMSK1 = _BV(ICIE1); // ICIE1: Timer/Counter1, Input Capture Interrupt Enable // disable TIMER1 interrupts (Compare Match Interrupt A)
  TIFR1 = 0xff; //clear all timer1 interrupt flags
}

