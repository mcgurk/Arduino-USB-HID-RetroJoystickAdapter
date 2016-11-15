
//https://www.cs.cmu.edu/~chuck/infopg/segasix.txt

//DB9 (8=GND, 5=VCC):               1   2   3   4   5   6   7   8   9
const uint8_t inputPinsPort1[] =  { 2,  3,  4,  5,  6,  7,  8,  0,  9};
const uint8_t inputPinsPort2[] =  {10, 16, 14, 15, A0, A1, A2,  0, A3};

// if you use two DB9 connectors solded back to back on your ATmega32u4, you should use this inputs
//const uint8_t inputPinsPort1[] =  { 2,  3,  4,  5,  6,  7,  8,  0,  9};
//const uint8_t inputPinsPort2[] =  {15, A0, A1, A2, A3, 14, 16,  0, 10};

// yet another version (Images/sega_genesis_adapter.jpg)
//const uint8_t inputPinsPort1[] =  { 5,  6,  7,  8,  9,  4,  2,  0, A2};
//const uint8_t inputPinsPort2[] =  {10, 16, 14, 15, A0,  3, A3,  0, A1};


//#define DEBUG

inline void translateState(uint8_t *data, uint8_t *state) {
  state[0] = ~data[0];
  state[1] = 127; 
  state[2] = 127;
  if (!bitRead(data[1], 0)) state[2] = 0; /* up */
  if (!bitRead(data[1], 1)) state[2] = 255; /* down */
  if (!bitRead(data[1], 2)) state[1] = 0; /* left */
  if (!bitRead(data[1], 3)) state[1] = 255; /* right */
}

uint8_t J1BTN6 = 0;
uint8_t J2BTN6 = 0;

uint8_t plugged1 = 0;
uint8_t plugged2 = 0;

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

#define JOYSTICK_DATA_SIZE 2
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
    /* 8 Buttons */ \
    0x05, 0x09,               /*   USAGE_PAGE (Button) */ \
    0x19, 0x01,               /*   USAGE_MINIMUM (Button 1) */ \
    0x29, 0x08,               /*   USAGE_MAXIMUM (Button 8) */ \
    0x15, 0x00,               /*   LOGICAL_MINIMUM (0) */ \
    0x25, 0x01,               /*   LOGICAL_MAXIMUM (1) */ \
    0x75, 0x01,               /*   REPORT_SIZE (1) */ \
    0x95, 0x08,               /*   REPORT_COUNT (8) */ \
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

};


Joystick_ Joystick[2] =
{
    Joystick_(0, JOYSTICK_REPORT_ID),
    Joystick_(1, JOYSTICK2_REPORT_ID)
};

//================================================================================
//================================================================================

#define MODE_SELECT_PORT1 inputPinsPort1[6]
#define MODE_SELECT_PORT2 inputPinsPort2[6]
#define VCC_PORT1 inputPinsPort1[4]
#define VCC_PORT2 inputPinsPort2[4]

void modeSelect(uint8_t m) {
  digitalWrite(MODE_SELECT_PORT1, m);
  digitalWrite(MODE_SELECT_PORT2, m);
  delayMicroseconds(20);
}


void setup() {
  for (uint8_t i = 0; i < 9; i++) {
    if (inputPinsPort1[i] != 0 && i != 4 && i != 6)
      pinMode(inputPinsPort1[i], INPUT_PULLUP);
    if (inputPinsPort2[i] != 0 && i != 4 && i != 6)
      pinMode(inputPinsPort2[i], INPUT_PULLUP);
  } //without PULLUP every button are read as pressed down if controller is not connected.
  
  pinMode(VCC_PORT1, OUTPUT);
  pinMode(VCC_PORT2, OUTPUT);
  digitalWrite(VCC_PORT1, HIGH);
  digitalWrite(VCC_PORT2, HIGH);

  pinMode(MODE_SELECT_PORT1, OUTPUT);
  pinMode(MODE_SELECT_PORT2, OUTPUT);
  modeSelect(HIGH);
  
  #ifdef DEBUG
  Serial.begin(9600);
  #endif

}



void loop() {

  Joystick[0].data[0] = 0xff;
  Joystick[1].data[0] = 0xff;
  Joystick[0].data[1] = 0xff;
  Joystick[1].data[1] = 0xff;
  
  modeSelect(LOW);

  bitWrite(Joystick[0].data[1], 6, digitalRead(inputPinsPort1[2])); //detect1 j1
  bitWrite(Joystick[0].data[1], 7, digitalRead(inputPinsPort1[3])); //detect2 j1
  bitWrite(Joystick[1].data[1], 6, digitalRead(inputPinsPort2[2])); //detect1 j2
  bitWrite(Joystick[1].data[1], 7, digitalRead(inputPinsPort2[3])); //detect2 j2

  bitWrite(Joystick[0].data[0], 0, digitalRead(inputPinsPort1[5])); //A1
  bitWrite(Joystick[0].data[0], 3, digitalRead(inputPinsPort1[8])); //Start1
  bitWrite(Joystick[1].data[0], 0, digitalRead(inputPinsPort2[5])); //A2
  bitWrite(Joystick[1].data[0], 3, digitalRead(inputPinsPort2[8])); //Start2

  modeSelect(HIGH);
  
  for (uint8_t i = 0; i < 4; i++) {
    bitWrite(Joystick[0].data[1], i, digitalRead(inputPinsPort1[i])); //AXES1
    bitWrite(Joystick[1].data[1], i, digitalRead(inputPinsPort2[i])); //AXES2
  }

  bitWrite(Joystick[0].data[0], 1, digitalRead(inputPinsPort1[5])); //B1
  bitWrite(Joystick[0].data[0], 2, digitalRead(inputPinsPort1[8])); //C1
  bitWrite(Joystick[1].data[0], 1, digitalRead(inputPinsPort2[5])); //B2
  bitWrite(Joystick[1].data[0], 2, digitalRead(inputPinsPort2[8])); //C2


  //read X,Y,Z,mode
  modeSelect(LOW);
  modeSelect(HIGH);
  modeSelect(LOW);
  modeSelect(HIGH);
  if (J1BTN6) {
    bitWrite(Joystick[0].data[0], 4, digitalRead(inputPinsPort1[2])); //X1
    bitWrite(Joystick[0].data[0], 5, digitalRead(inputPinsPort1[1])); //Y1
    bitWrite(Joystick[0].data[0], 6, digitalRead(inputPinsPort1[0])); //Z1
    bitWrite(Joystick[0].data[0], 7, digitalRead(inputPinsPort1[3])); //mode
  }
  if (J2BTN6) {
    bitWrite(Joystick[1].data[0], 4, digitalRead(inputPinsPort2[2])); //X1
    bitWrite(Joystick[1].data[0], 5, digitalRead(inputPinsPort2[1])); //Y1
    bitWrite(Joystick[1].data[0], 6, digitalRead(inputPinsPort2[0])); //Z1
    bitWrite(Joystick[1].data[0], 7, digitalRead(inputPinsPort2[3])); //mode
  }


  //detect button mode and detect if controller is unplugged
  uint8_t detect1 = !(Joystick[0].data[1] & B11000000);
  if (!plugged1 && detect1) {
    plugged1 = 1;
    digitalWrite(VCC_PORT1, LOW);
    delay(100);
    digitalWrite(VCC_PORT1, HIGH);
    if (!digitalRead(inputPinsPort1[0]) && !digitalRead(inputPinsPort1[1])) J1BTN6 = 1;
  }
  if (!detect1) {
    plugged1 = 0;
    J1BTN6 = 0;
  }

  //detect button mode and detect if controller is unplugged
  uint8_t detect2 = !(Joystick[1].data[1] & B11000000);
  if (!plugged2 && detect2) {
    plugged2 = 1;
    digitalWrite(VCC_PORT2, LOW);
    delay(100);
    digitalWrite(VCC_PORT2, HIGH);
    if (!digitalRead(inputPinsPort2[0]) && !digitalRead(inputPinsPort2[1])) J2BTN6 = 1;
  }
  if (!detect2) {
    plugged2 = 0;
    J2BTN6 = 0;
  }

  #ifdef DEBUG
  Serial.print(" data0 j1: 0x"); Serial.print(Joystick[0].data[0], HEX);
  Serial.print(" data0 j2: 0x"); Serial.print(Joystick[1].data[0], HEX);
  Serial.print(" data1 j1: 0x"); Serial.print(Joystick[0].data[1], HEX);
  Serial.print(" data1 j2: 0x"); Serial.print(Joystick[1].data[1], HEX);
  Serial.print(" 6btn j1: 0x"); Serial.print(J1BTN6, HEX);
  Serial.print(" 6btn j2: 0x"); Serial.print(J2BTN6, HEX);
  Serial.println();
  delay(50);
  Serial.flush();
  #endif

  Joystick[0].updateState();
  Joystick[1].updateState();
  Joystick[0].sendState();
  Joystick[1].sendState();
  delayMicroseconds(1000);


}
