 
#include <PS2KeyRaw.h>

#define DATAPIN 4
#define IRQPIN  3

//#define DEBUG
//#define COMMANDO
//#define DECATHLON

#define UP1 0x75
#define DOWN1 0x72
#define LEFT1 0x6B
#define RIGHT1 0x74
#define START1 0x16
#define SELECT1 0x26
#define A1 0x12
#define B1 0x1A
#define C1 0x22
#define X1 0x14
#define Y1 0x11
#define Z1 0x29
#define L1 0x21
#define R1 0x2E

#define UP2 0x2D
#define DOWN2 0x2B
#define LEFT2 0x23
#define RIGHT2 0x34
#define START2 0x1E
#define SELECT2 0x25
#define A2 0x1D
#define B2 0x24
#define C2 0x54
#define X2 0x1C
#define Y2 0x1B
#define Z2 0x15
#define L2 0x5B
#define R2 0x36

inline void translateState(uint8_t *data, uint8_t *state) {
  state[0] = data[0];
  state[1] = data[1];
  state[2] = 127; 
  state[3] = 127;
  if (bitRead(data[2], 2)) state[2] = 0; /* up */
  if (bitRead(data[2], 3)) state[2] = 255; /* down */
  if (bitRead(data[2], 0)) state[3] = 0; /* left */
  if (bitRead(data[2], 1)) state[3] = 255; /* right */
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

#define JOYSTICK_DATA_SIZE 3
#define JOYSTICK_STATE_SIZE 4


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
    0x29, 0x10,               /*   USAGE_MAXIMUM (Button 16) */ \
    0x15, 0x00,               /*   LOGICAL_MINIMUM (0) */ \
    0x25, 0x01,               /*   LOGICAL_MAXIMUM (1) */ \
    0x75, 0x01,               /*   REPORT_SIZE (1) */ \
    0x95, 0x10,               /*   REPORT_COUNT (16) */ \
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
  uint8_t olddata[JOYSTICK_DATA_SIZE];
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
    data[2] = 0;
    memcpy(olddata, data, JOYSTICK_DATA_SIZE);
    translateState(data, state);
    sendState(1);
  }

  void updateState() {
    if (memcmp(olddata, data, JOYSTICK_DATA_SIZE)) {    
      memcpy(olddata, data, JOYSTICK_DATA_SIZE);
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

  void updateAndSendState() {
    translateState(data, state);
    HID().SendReport(reportId, state, JOYSTICK_STATE_SIZE);
  }

};


Joystick_ Joystick[2] =
{
    Joystick_(0, JOYSTICK_REPORT_ID),
    Joystick_(1, JOYSTICK2_REPORT_ID)
};

//================================================================================
//================================================================================


PS2KeyRaw keyboard;

void setup() {
  keyboard.begin(DATAPIN, IRQPIN);
  Serial.begin(115200);
  Serial.println( "PS2 Raw Test of PS2 Keyboard codes" );
  Joystick[0].data[0] = 0x00;
  Joystick[0].data[1] = 0x00;
  Joystick[0].data[2] = 0x00;
  Joystick[1].data[0] = 0x00;
  Joystick[1].data[1] = 0x00;
  Joystick[1].data[2] = 0x00;
}

void loop() {
  
  if (keyboard.available()) {
    // read the next key
    int c = keyboard.read();
    uint8_t j;

    if (c == 0xF0) {
      #ifdef DEBUG
      Serial.print("0x"); Serial.print(c, HEX); Serial.print(" ");
      #endif
      while (!keyboard.available()) {}
      c = keyboard.read();
      #ifdef DEBUG
      Serial.print("0x"); Serial.println(c, HEX); 
      #endif
      j = clearData(c);
    } else {
      #ifdef DEBUG
      Serial.print("0x"); Serial.println(c, HEX); 
      #endif
      j = setData(c);
    }

    #ifdef DEBUG
    Serial.print(" data0 j1: 0x"); Serial.print(Joystick[0].data[0], HEX);
    Serial.print(" data1 j1: 0x"); Serial.print(Joystick[0].data[1], HEX);
    Serial.print(" data2 j1: 0x"); Serial.print(Joystick[0].data[2], HEX);
    Serial.print(" data0 j2: 0x"); Serial.print(Joystick[1].data[0], HEX);
    Serial.print(" data1 j2: 0x"); Serial.print(Joystick[1].data[1], HEX);
    Serial.print(" data2 j2: 0x"); Serial.print(Joystick[1].data[2], HEX);
    Serial.println();
    Serial.flush();
    #endif

    //Joystick[j].updateState();
    //Joystick[j].sendState();
    Joystick[j].updateAndSendState();
  }

}


#define SET(j,d,b) bitSet(Joystick[j].data[d], b); return j
#define UNSET(j,d,b) bitClear(Joystick[j].data[d], b); return j

#ifdef COMMANDO
#define L1_SET SET(1, 1, 0)
#define L1_UNSET UNSET(1, 1, 0)
#define R1_SET SET(1, 1, 1)
#define R1_UNSET UNSET(1, 1, 1)
#define L2_SET SET(0, 1, 0)
#define L2_UNSET UNSET(0, 1, 0)
#define R2_SET SET(0, 1, 1)
#define R2_UNSET UNSET(0, 1, 1)
#endif

#ifdef DECATHLON
#define L1_SET SET(0, 2, 2)
#define L1_UNSET UNSET(0, 2, 2)
#define R1_SET SET(0, 2, 3)
#define R1_UNSET UNSET(0, 2, 3)
#define L2_SET SET(1, 2, 2)
#define L2_UNSET UNSET(1, 2, 2)
#define R2_SET SET(1, 2, 3)
#define R2_UNSET UNSET(1, 2, 3)
#endif


inline uint8_t setData(uint8_t c) {
  switch (c) {
    case A1:
      SET(0, 0, 0);
    case B1:
      SET(0, 0, 1);
    case C1:
      SET(0, 0, 2);
    case X1:
      SET(0, 0, 3);
    case Y1:
      SET(0, 0, 4);
    case Z1:
      SET(0, 0, 5);
    case START1:
      SET(0, 0, 6);
    case SELECT1:
      SET(0, 0, 7);
    case L1:
      #ifdef L1_SET
      L1_SET;
      #else
      SET(0, 1, 0);
      #endif
    case R1:
      #ifdef R1_SET
      R1_SET;
      #else
      SET(0, 1, 1);
      #endif
    case UP1:
      SET(0, 2, 0);
    case DOWN1:
      SET(0, 2, 1);
    case LEFT1:
      SET(0, 2, 2);
    case RIGHT1:
      SET(0, 2, 3);
    case A2:
      SET(1, 0, 0);
    case B2:
      SET(1, 0, 1);
    case C2:
      SET(1, 0, 2);
    case X2:
      SET(1, 0, 3);
    case Y2:
      SET(1, 0, 4);
    case Z2:
      SET(1, 0, 5);
    case START2:
      SET(1, 0, 6);
    case SELECT2:
      SET(1, 0, 7);
    case L2:
      #ifdef L2_SET
      L2_SET;
      #else
      SET(1, 1, 0);
      #endif
    case R2:
      #ifdef R2_SET
      R2_SET;
      #else
      SET(1, 1, 1);
      #endif
    case UP2:
      SET(1, 2, 0);
    case DOWN2:
      SET(1, 2, 1);
    case LEFT2:
      SET(1, 2, 2);
    case RIGHT2:
      SET(1, 2, 3);
  }
}



inline uint8_t clearData(uint8_t c) {
  switch (c) {
    case A1:
      UNSET(0, 0, 0);
    case B1:
      UNSET(0, 0, 1);
    case C1:
      UNSET(0, 0, 2);
    case X1:
      UNSET(0, 0, 3);
    case Y1:
      UNSET(0, 0, 4);
    case Z1:
      UNSET(0, 0, 5);
    case START1:
      UNSET(0, 0, 6);
    case SELECT1:
      UNSET(0, 0, 7);
    case L1:
      #ifdef L1_UNSET
      L1_UNSET;
      #else
      UNSET(0, 1, 0);
      #endif
    case R1:
      #ifdef R1_UNSET
      R1_UNSET;
      #else
      UNSET(0, 1, 1);
      #endif
    case UP1:
      UNSET(0, 2, 0);
    case DOWN1:
      UNSET(0, 2, 1);
    case LEFT1:
      UNSET(0, 2, 2);
    case RIGHT1:
      UNSET(0, 2, 3);
    case A2:
      UNSET(1, 0, 0);
    case B2:
      UNSET(1, 0, 1);
    case C2:
      UNSET(1, 0, 2);
    case X2:
      UNSET(1, 0, 3);
    case Y2:
      UNSET(1, 0, 4);
    case Z2:
      UNSET(1, 0, 5);
    case START2:
      UNSET(1, 0, 6);
    case SELECT2:
      UNSET(1, 0, 7);
    case L2:
      #ifdef L2_UNSET
      L2_UNSET;
      #else
      UNSET(1, 1, 0);
      #endif
    case R2:
      #ifdef R2_UNSET
      R2_UNSET;
      #else
      UNSET(1, 1, 1);
      #endif
    case UP2:
      UNSET(1, 2, 0);
    case DOWN2:
      UNSET(1, 2, 1);
    case LEFT2:
      UNSET(1, 2, 2);
    case RIGHT2:
      UNSET(1, 2, 3);
  }
}
