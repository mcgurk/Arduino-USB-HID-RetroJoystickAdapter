#include <hidboot.h>
#include <usbhub.h>

#define POTSENSE    4           //ICP1 (Arduino Pro Micro: pin4, Arduino Uno: pin8)

#define POTX        9           // POT AX 9, also OC1A
#define POTY        10          // POT AY 5, also OC1B
#define LBTN        5           // BTN 6, Joystick FIRE switch
#define RBTN        6           // UP 1, Joystick UP switch

int16_t dx = 0;
int16_t dy = 0;
uint8_t buttons = 0;

uint8_t update = 1;

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
  dx=mi->dX;
  dy=mi->dY;
  update = 1;
};
void MouseRptParser::OnLeftButtonUp(MOUSEINFO *mi) {
  buttons &= ~1;
  update = 1;
};
void MouseRptParser::OnLeftButtonDown(MOUSEINFO *mi) {
  buttons |= 1;
  update = 1;
};
void MouseRptParser::OnRightButtonUp(MOUSEINFO *mi) {
  buttons &= ~2;
  update = 1;
};
void MouseRptParser::OnRightButtonDown(MOUSEINFO *mi) {
  buttons |= 2;
  update = 1;
};
void MouseRptParser::OnMiddleButtonUp(MOUSEINFO *mi) {
};
void MouseRptParser::OnMiddleButtonDown(MOUSEINFO *mi) {
};

USB Usb;
USBHub Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);

MouseRptParser Prs;


static uint8_t potmouse_xcounter = 0;           ///< x axis counter
static uint8_t potmouse_ycounter = 0;           ///< y axis counter

static volatile uint16_t x;        ///< precalculated OCR1A value (YPOT)
static volatile uint16_t y;        ///< precalculated OCR1B value (XPOT)

void setup() {
  if (Usb.Init() == -1) {
  }
  delay(200);
  HidMouse.SetReportParser(0, &Prs);

  pinMode(LBTN, INPUT); pinMode(RBTN, INPUT);
  
  pinMode(POTX, OUTPUT); pinMode(POTY, OUTPUT);
  digitalWrite(POTX, HIGH); digitalWrite(POTY, HIGH);
  pinMode(POTSENSE, INPUT); // pullup off, hi-biased by OC1A

  TIMSK1 = _BV(ICIE1); // ICIE1: Timer/Counter1, Input Capture Interrupt Enable
  TCCR1B = _BV(ICNC1) | _BV(CS10); //CS10: No prescaler, ICNC1: Input Capture Noise Canceller
}


void loop() {
  Usb.Task();
  if (update) {
    potmouse_xcounter = (potmouse_xcounter + (dx/2)) & 0177; // modulo 128
    potmouse_ycounter = (potmouse_ycounter - (dy/2)) & 0177;

    (buttons & 001) ? pinMode(LBTN, OUTPUT) : pinMode(LBTN, INPUT);
    (buttons & 002) ? pinMode(RBTN, OUTPUT) : pinMode(RBTN, INPUT);
            
    cli();
    x = (320 + potmouse_xcounter)*16; //16 clock cycles = 1us
    y = (320 + potmouse_ycounter)*16;
    sei();
    update = 0;
  }
  delayMicroseconds(100);
}


ISR(TIMER1_CAPT_vect) { // ICIE1
  // OC1A/OC1B -> LOW
  TCCR1A = _BV(COM1A1) | _BV(COM1B1); // Clear OC1A / OC1B on Compare Match (Set output to low level)
  TCCR1C |= _BV(FOC1A) | _BV(FOC1B); // FOC1A / FOC1B Force Output Compare A and B

  // init the output compare values
  OCR1A = ICR1 + x; //ICR1: Input Capture Register
  OCR1B = ICR1 + y;

  TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(COM1B0); // Set OC1A / OC1B on Compare Match (Set output to high level).
  
  TIMSK1 = _BV(OCIE1A); // OCIE1A: Timer/Counter Output Compare Match Interrupt Enable A // disable other TIMER1 interrupts
  TIFR1 = 0xff; // Clear all pending TIMER1 interrupt flags
}


ISR(TIMER1_COMPA_vect) { // OCIE1A
  TIMSK1 = _BV(ICIE1); // ICIE1: Timer/Counter1, Input Capture Interrupt Enable // disable other TIMER1 interrupts
  TIFR1 = 0xff; // Clear all pending TIMER1 interrupt flags
}
