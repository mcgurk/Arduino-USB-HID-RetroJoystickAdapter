//DB9 (8=GND):                      1   2   3   4   5   6   7   8   9
const uint8_t inputPinsPort1[] =  { 5,  6,  7,  8,  0,  4,  0,  0,  A2};
const uint8_t inputPinsPort2[] =  {10, 16, 14, 15,  0,  3,  0,  0,  A1};
//(1 = up, 2 = down, 3 = left, 4 = right, 6 = btn1, 9 = btn2)

#define DEBUG

inline void translateState(uint8_t data, uint8_t *state) {
  state[0] = !bitRead(data, 4) | !bitRead(data, 5)<<1;
  state[1] = 127; 
  state[2] = 127;
  if (!bitRead(data, 0)) state[2] = 0; /* up */
  if (!bitRead(data, 1)) state[2] = 255; /* down */
  if (!bitRead(data, 2)) state[1] = 0; /* left */
  if (!bitRead(data, 3)) state[1] = 255; /* right */
}


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

#define JOYSTICK_REPORT_ID  0x04
#define JOYSTICK2_REPORT_ID 0x05

//#define JOYSTICK_DATA_SIZE 1
#define JOYSTICK_STATE_SIZE 3


//================================================================================
//================================================================================
//  Joystick (Gamepad)


#define HIDDESC_MACRO(REPORT_ID) \
    /* Joystick # */ \
    0x05, 0x01,               /* USAGE_PAGE (Generic Desktop) */ \
    0x09, 0x04,               /* USAGE (Joystick) */ \
    0xa1, 0x01,               /* COLLECTION (Application) */ \
    0x85, REPORT_ID,          /* REPORT_ID */ \
    /* 2 Buttons */ \
    0x05, 0x09,               /*   USAGE_PAGE (Button) */ \
    0x19, 0x01,               /*   USAGE_MINIMUM (Button 1) */ \
    0x29, 0x02,               /*   USAGE_MAXIMUM (Button 2) */ \
    0x15, 0x00,               /*   LOGICAL_MINIMUM (0) */ \
    0x25, 0x01,               /*   LOGICAL_MAXIMUM (1) */ \
    0x75, 0x01,               /*   REPORT_SIZE (1) */ \
    0x95, 0x08,               /*   REPORT_COUNT (8) (full byte) */ \
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
  HIDDESC_MACRO(JOYSTICK2_REPORT_ID)
};


class Joystick_ {

private:
  uint8_t joystickId;
  uint8_t reportId;
  //uint8_t olddata[JOYSTICK_DATA_SIZE];
  uint8_t olddata;
  uint8_t state[JOYSTICK_STATE_SIZE];
  uint8_t flag;

public:
  uint8_t type;
  //uint8_t data[JOYSTICK_DATA_SIZE];
  uint8_t data;

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
  
    //data[0] = 0;
    //data[1] = 0;
    data = 0;
    //memcpy(olddata, data, JOYSTICK_DATA_SIZE);
    olddata = data;
    translateState(data, state);
    sendState(1);
  }

  void updateState() {
    //if (memcmp(olddata, data, JOYSTICK_DATA_SIZE)) {    
      //memcpy(olddata, data, JOYSTICK_DATA_SIZE);
    if (olddata != data) {
      olddata = data;
      translateState(data, state);
      flag = 1;
    }
  }

  void sendState(uint8_t force = 0) {
    if (flag || force) {
      // HID().SendReport(Report number, array of values in same order as HID descriptor, length)
      HID().SendReport(reportId, state, JOYSTICK_STATE_SIZE);
      flag = 0;
    }
  }

};


Joystick_ Joystick[2] =
{
    Joystick_(0, JOYSTICK_REPORT_ID),
    Joystick_(1, JOYSTICK2_REPORT_ID)
};

//================================================================================
//================================================================================





void setup() {
  for (uint8_t i = 0; i < 9; i++) {
    if (inputPinsPort1[i] != 0)
      pinMode(inputPinsPort1[i], INPUT_PULLUP);
    if (inputPinsPort2[i] != 0)
      pinMode(inputPinsPort2[i], INPUT_PULLUP);
  } //without PULLUP every button are read as pressed down if controller is not connected.
  
  #ifdef DEBUG
  Serial.begin(115200);
  #endif

}



void loop() {

  Joystick[0].data = 0xff;
  Joystick[1].data = 0xff;
  
  for (uint8_t i = 0; i < 4; i++) {
    bitWrite(Joystick[0].data, i, digitalRead(inputPinsPort1[i])); //AXES1
    bitWrite(Joystick[1].data, i, digitalRead(inputPinsPort2[i])); //AXES2
  }

  bitWrite(Joystick[0].data, 4, digitalRead(inputPinsPort1[5]));
  bitWrite(Joystick[0].data, 5, digitalRead(inputPinsPort1[8]));
  bitWrite(Joystick[1].data, 4, digitalRead(inputPinsPort2[5]));
  bitWrite(Joystick[1].data, 5, digitalRead(inputPinsPort2[8]));


  #ifdef DEBUG
  Serial.print(" data0 j1: 0x"); Serial.print(Joystick[0].data, BIN);
  Serial.print(" data0 j2: 0x"); Serial.print(Joystick[1].data, BIN);
  Serial.println();
  delay(50);
  Serial.flush();
  #endif

  Joystick[0].updateState();
  Joystick[1].updateState();
  Joystick[0].sendState();
  Joystick[1].sendState();
  delayMicroseconds(500);


}
