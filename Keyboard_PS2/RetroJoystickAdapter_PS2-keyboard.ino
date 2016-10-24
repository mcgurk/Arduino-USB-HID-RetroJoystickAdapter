#include "HID.h"

#if ARDUINO < 10606
#error The Joystick2 library requires Arduino IDE 1.6.6 or greater. Please update your IDE.
#endif

#if !defined(USBCON)
#error The Joystick2 library can only be used with a USB MCU (e.g. Arduino Leonardo, Arduino Micro, etc.).
#endif

#if !defined(_USING_HID)
#error "legacy HID core (non pluggable)"
#endif

#define JOYSTICK_REPORT_ID  0x03
#define JOYSTICK2_REPORT_ID 0x04
#define JOYSTICK3_REPORT_ID 0x05
#define JOYSTICK4_REPORT_ID 0x06

#define JOYSTICK_DATA_SIZE 2
#define JOYSTICK_STATE_SIZE 3


#include <PS2Keyboard.h>

const int DataPin = 3; // PS/2 pin 1
const int IRQpin =  2; // PS/2 pin 5

PS2Keyboard keyboard;

//#define DEBUG

//================================================================================
//================================================================================
//  Joystick (Gamepad)


#define HIDDESC_MACRO(REPORT_ID) \
    /* Joystick # */ \
    0x05, 0x01,               /* USAGE_PAGE (Generic Desktop) */ \
    0x09, 0x04,               /* USAGE (Joystick) */ \
    0xa1, 0x01,               /* COLLECTION (Application) */ \
    0x85, REPORT_ID,          /* REPORT_ID */ \
    /* 8 Buttons */ \
    0x05, 0x09,               /*   USAGE_PAGE (Button) */ \
    0x19, 0x01,               /*   USAGE_MINIMUM (Button 1) */ \
    0x29, 0x08,               /*   USAGE_MAXIMUM (Button 8) */ \
    0x15, 0x00,               /*   LOGICAL_MINIMUM (0) */ \
    0x25, 0x01,               /*   LOGICAL_MAXIMUM (1) */ \
    0x75, 0x01,               /*   REPORT_SIZE (1) */ \
    0x95, 0x08,               /*   REPORT_COUNT (8) */ \
    0x55, 0x00,               /*   UNIT_EXPONENT (0) */ \
    0x65, 0x00,               /*   UNIT (None) */ \
    0x81, 0x02,               /*   INPUT (Data,Var,Abs) */ \
    /* X and Y Axis */ \
    0x05, 0x01,               /*   USAGE_PAGE (Generic Desktop) */ \
    0x09, 0x01,               /*   USAGE (Pointer) */ \
    0xA1, 0x00,               /*   COLLECTION (Physical) */ \
    0x09, 0x30,               /*     USAGE (x) */ \
    0x09, 0x31,               /*     USAGE (y) */ \
    0x15, 0x00,               /*     LOGICAL_MINIMUM (0) */ \
    0x26, 0xff, 0x00,         /*     LOGICAL_MAXIMUM (255) */ \
    0x75, 0x08,               /*     REPORT_SIZE (8) */ \
    0x95, 0x02,               /*     REPORT_COUNT (2) */ \
    0x81, 0x02,               /*     INPUT (Data,Var,Abs) */ \
    0xc0,                     /*   END_COLLECTION */ \
    0xc0                      /* END_COLLECTION */




static const uint8_t hidReportDescriptor[] PROGMEM = {
  HIDDESC_MACRO(JOYSTICK_REPORT_ID),
  HIDDESC_MACRO(JOYSTICK2_REPORT_ID),
  HIDDESC_MACRO(JOYSTICK3_REPORT_ID),
  HIDDESC_MACRO(JOYSTICK4_REPORT_ID)
};


class Joystick_ {

private:
  uint8_t joystickId;
  uint8_t reportId;
  uint8_t state[JOYSTICK_STATE_SIZE];
  uint8_t flag;

public:
  uint8_t type;
  uint8_t data[JOYSTICK_DATA_SIZE];

  Joystick_(uint8_t initJoystickId, uint8_t initReportId) {
    // Setup HID report structure
    static bool usbSetup = false;
  
    if (!usbSetup) {
      static HIDSubDescriptor node(hidReportDescriptor, sizeof(hidReportDescriptor));
      HID().AppendDescriptor(&node);
      usbSetup = true;
    }
    
    // Initalize State
    joystickId = initJoystickId;
    reportId = initReportId;
  
    data[0] = 0;
    data[1] = 0;
    updateState();
    sendState(1);
  }

  void updateState() {
    state[0] = data[0];
    state[1] = 127; 
    state[2] = 127;
    if (bitRead(data[1], 0)) state[2] = 0; /* up */
    if (bitRead(data[1], 1)) state[2] = 255; /* down */
    if (bitRead(data[1], 2)) state[1] = 0; /* left */
    if (bitRead(data[1], 3)) state[1] = 255; /* right */
  }

  void sendState(uint8_t force = 0) {
    if (flag || force) {
      // HID().SendReport(Report number, array of values in same order as HID descriptor, length)
      HID().SendReport(reportId, state, JOYSTICK_STATE_SIZE);
      flag = 0;
    }
  }

};


Joystick_ Joystick[4] =
{
    Joystick_(0, JOYSTICK_REPORT_ID),
    Joystick_(1, JOYSTICK2_REPORT_ID),
    Joystick_(2, JOYSTICK3_REPORT_ID),
    Joystick_(3, JOYSTICK4_REPORT_ID)
};

//================================================================================
//================================================================================

   

void setup() {
  delay(1000);
  keyboard.begin(DataPin, IRQpin);
  #ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Start");
  #endif
}


void set(uint32_t code, uint8_t j, uint8_t d, uint8_t b) {
  uint8_t is_break = 0;
  if ( (code >> 8 & 0xff) == 0xF0 ) is_break = 1;
  if (is_break) bitClear(Joystick[j].data[d], b);
  if (!is_break) bitSet(Joystick[j].data[d], b);
  Joystick[j].updateState();
  Joystick[j].sendState(1);
}

void loop() {
  if (keyboard.available()) {
    
    // read whole code at once
    int32_t code = keyboard.read();
    uint8_t c = code & 0xff;
    uint8_t e;
    //if ( (code & 0xff) == 0xE0 || (code >> 8 & 0xff) == 0xE0 || (code >> 16 & 0xff) == 0xE0 || (code >> 24 & 0xff) == 0xE0 ) e = 1;
    if ( (code >> 8 & 0xff) == 0xE0 || (code >> 16 & 0xff) == 0xE0 ) e = 1; else e = 0;
    //if (code^0xE000 || code^0xE00000) e = 1;
    
    #ifdef DEBUG
    Serial.print("e:");Serial.print(e);Serial.print(" ");
    Serial.println(code, HEX);
    #endif

    //http://www.computer-engineering.org/ps2keyboard/scancodes2.html
    //set-function parameters: code, joystick (0-3), data (0 or 1), bit (0-7)
    // if e is 1, it means codes with E0 prefix

    //Joystick 0
    if (e == 1 && c == 0x75) set(code, 0, 1, 0); // up
    if (e == 1 && c == 0x72) set(code, 0, 1, 1); // down
    if (e == 1 && c == 0x6B) set(code, 0, 1, 2); // left
    if (e == 1 && c == 0x74) set(code, 0, 1, 3); // right
    if (e == 1 && c == 0x14) set(code, 0, 0, 0); // R-ctrl = a
    if (e == 1 && c == 0x11) set(code, 0, 0, 1); // alt gr = b
    if (e == 0 && c == 0x5A) set(code, 0, 0, 2); // enter = start
    if (e == 0 && c == 0x59) set(code, 0, 0, 3); // R-shift = select
    //if (e == x && c == 0xXX) set(code, 0, 0, 4); //  = x
    //if (e == x && c == 0xXX) set(code, 0, 0, 5); //  = y
    //if (e == x && c == 0xXX) set(code, 0, 0, 6); //  = left shoulder
    //if (e == x && c == 0xXX) set(code, 0, 0, 7); //  = right shoulder

    //Joystick 1
    if (e == 0 && c == 0x1D) set(code, 1, 1, 0); // w = up
    if (e == 0 && c == 0x1B) set(code, 1, 1, 1); // s = down
    if (e == 0 && c == 0x1C) set(code, 1, 1, 2); // a = left
    if (e == 0 && c == 0x23) set(code, 1, 1, 3); // d = right
    if (e == 0 && c == 0x14) set(code, 1, 0, 0); // L-ctrl = a
    if (e == 0 && c == 0x11) set(code, 1, 0, 1); // L-alt = b
    if (e == 0 && c == 0x0D) set(code, 1, 0, 2); // tab = start
    if (e == 0 && c == 0x12) set(code, 1, 0, 3); // L-shift = select
    //if (e == x && c == 0xXX) set(code, 1, 0, 4); //  = x
    //if (e == x && c == 0xXX) set(code, 1, 0, 5); //  = y
    //if (e == x && c == 0xXX) set(code, 1, 0, 6); //  = left shoulder
    //if (e == x && c == 0xXX) set(code, 1, 0, 7); //  = right shoulder

    //Joystick 2
    if (e == 0 && c == 0x43) set(code, 2, 1, 0); // i = up
    if (e == 0 && c == 0x42) set(code, 2, 1, 1); // k = down
    if (e == 0 && c == 0x3B) set(code, 2, 1, 2); // j = left
    if (e == 0 && c == 0x4B) set(code, 2, 1, 3); // l = right
    if (e == 0 && c == 0x3A) set(code, 2, 0, 0); // m = a
    if (e == 0 && c == 0x31) set(code, 2, 0, 1); // n = b
    if (e == 0 && c == 0x44) set(code, 2, 0, 2); // o = start
    if (e == 0 && c == 0x3C) set(code, 2, 0, 3); // u = select
    //if (e == x && c == 0xXX) set(code, 2, 0, 4); //  = x
    //if (e == x && c == 0xXX) set(code, 2, 0, 5); //  = y
    //if (e == x && c == 0xXX) set(code, 2, 0, 6); //  = left shoulder
    //if (e == x && c == 0xXX) set(code, 2, 0, 7); //  = right shoulder

    //Joystick 3
    if (e == 0 && c == 0x75) set(code, 3, 1, 0); // keypad 8
    if (e == 0 && c == 0x73) set(code, 3, 1, 1); // keypad 5
    if (e == 0 && c == 0x6B) set(code, 3, 1, 2); // keypad 4
    if (e == 0 && c == 0x74) set(code, 3, 1, 3); // keypad 6
    if (e == 0 && c == 0x70) set(code, 3, 0, 0); // keypad 0 = a
    if (e == 0 && c == 0x71) set(code, 3, 0, 1); // keypad , = b
    if (e == 1 && c == 0x5A) set(code, 3, 0, 2); // keypad enter = start
    if (e == 0 && c == 0x79) set(code, 3, 0, 3); // keypad + = select
    //if (e == x && c == 0xXX) set(code, 3, 0, 4); //  = x
    //if (e == x && c == 0xXX) set(code, 3, 0, 5); //  = y
    //if (e == x && c == 0xXX) set(code, 3, 0, 6); //  = left shoulder
    //if (e == x && c == 0xXX) set(code, 3, 0, 7); //  = right shoulder
      
  }
  
}
