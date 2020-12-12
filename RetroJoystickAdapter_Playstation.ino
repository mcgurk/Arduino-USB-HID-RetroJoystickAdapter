
// 5V (red)
// GND (black)
#define DATA1 2 // (brown)
#define CMD1 3 // (orange)
#define ATT1 4 // (yellow)
#define CLK1 5 // (blue)

/*#define DATA2 6
#define CMD2 7
#define ATT2 8
#define CLK2 9 */


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

#define JOYSTICK_STATE_SIZE 6

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
    /* 16 Buttons */ \
    0x05, 0x09,               /*   USAGE_PAGE (Button) */ \
    0x19, 0x01,               /*   USAGE_MINIMUM (Button 1) */ \
    0x29, 0x10,               /*   USAGE_MAXIMUM (Button 16) */ \
    0x15, 0x00,               /*   LOGICAL_MINIMUM (0) */ \
    0x25, 0x01,               /*   LOGICAL_MAXIMUM (1) */ \
    0x75, 0x01,               /*   REPORT_SIZE (1) */ \
    0x95, 0x10,               /*   REPORT_COUNT (16) */ \
    0x55, 0x00,               /*   UNIT_EXPONENT (0) */ \
    0x65, 0x00,               /*   UNIT (None) */ \
    0x81, 0x02,               /*   INPUT (Data,Var,Abs) */ \
    /* X and Y Axis */ \
    0x05, 0x01,               /*   USAGE_PAGE (Generic Desktop) */ \
    0x09, 0x01,               /*   USAGE (Pointer) */ \
    0xA1, 0x00,               /*   COLLECTION (Physical) */ \
    0x09, 0x32,               /*     USAGE (Z) */ \
    0x09, 0x35,               /*     USAGE (Rz) */ \
    0x09, 0x30,               /*     USAGE (x) */ \
    0x09, 0x31,               /*     USAGE (y) */ \
    0x15, 0x00,               /*     LOGICAL_MINIMUM (0) */ \
    0x26, 0xff, 0x00,         /*     LOGICAL_MAXIMUM (255) */ \
    0x75, 0x08,               /*     REPORT_SIZE (8) */ \
    0x95, 0x04,               /*     REPORT_COUNT (4) */ \
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
  uint8_t olddata[JOYSTICK_STATE_SIZE];
  uint8_t flag;

public:
  uint8_t type;
  uint8_t data[JOYSTICK_STATE_SIZE];

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
    data[2] = 127;
    data[3] = 127;
    data[4] = 127;
    data[5] = 127;
    memcpy(olddata, data, JOYSTICK_STATE_SIZE);
    sendState(1);
  }

  void updateState() {
    if (type != 0x73 && type != 0x53) {
      data[2] = 127;
      data[3] = 127;
      data[4] = 127;
      data[5] = 127;
    }
    if (type == 0x41 || type == 0x73 || type == 0x53) {
      if (memcmp(olddata, data, JOYSTICK_STATE_SIZE)) {    
        memcpy(olddata, data, JOYSTICK_STATE_SIZE);
        flag = 1;
      }
    }
    //sendState();
  }

  void sendState(uint8_t force = 0) {
    if (flag || force) {
      // HID().SendReport(Report number, array of values in same order as HID descriptor, length)
      HID().SendReport(reportId, data, JOYSTICK_STATE_SIZE);
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





uint8_t shift(uint8_t _dataOut) // Does the actual shifting, both in and out simultaneously
{
  uint8_t _temp = 0;
  uint8_t _dataIn = 0;
  uint8_t _delay = 6; //2 unstable; //clock 250kHz

  delayMicroseconds(100); //max acknowledge waiting time 100us
  for (uint8_t _i = 0; _i <= 7; _i++) {
    
    if ( _dataOut & (1 << _i) ) // write bit
      digitalWrite(CMD1, HIGH);
    else 
      digitalWrite(CMD1, LOW);
    
    digitalWrite(CLK1, LOW); // read bit
    delayMicroseconds(_delay);
    _temp = digitalRead(DATA1);
    if (_temp) {
      _dataIn = _dataIn | (B00000001 << _i);
    }

    digitalWrite(CLK1, HIGH);
    delayMicroseconds(_delay);
  }
  return _dataIn;
}

void setup() {
  pinMode(DATA1, INPUT_PULLUP);
  pinMode(CMD1, OUTPUT);
  pinMode(ATT1, OUTPUT);
  pinMode(CLK1, OUTPUT);

  /*pinMode(DATA2, INPUT_PULLUP);
  pinMode(CMD2, OUTPUT);
  pinMode(ATT2, OUTPUT);
  pinMode(CLK2, OUTPUT);*/
  
  #ifdef DEBUG
  Serial.begin(115200);
  #endif

}

void loop() {
  // http://problemkaputt.de/psx-spx.htm#controllerandmemorycardsignals
  uint8_t head, padding, multitap;
  #ifdef DEBUG
  uint8_t data[100];
  #endif

  // first: read gamepad normally
  digitalWrite(ATT1, LOW);
  //digitalWrite(ATT2, LOW);
  head = shift(0x01);
  Joystick[0].type = shift(0x42);
  padding = shift(0x01); //read multitap in next command
  Joystick[0].data[0] = ~shift(0x00); //buttons
  Joystick[0].data[1] = ~shift(0x00); //buttons
  Joystick[0].data[2] = shift(0x00); //right analog
  Joystick[0].data[3] = shift(0x00); //right analog
  Joystick[0].data[4] = shift(0x00); //left analog
  Joystick[0].data[5] = shift(0x00); //left analog
  digitalWrite(ATT1, HIGH);
  //digitalWrite(ATT2, HIGH);

  //delay(100);

  // second: check and read multitap
  digitalWrite(ATT1, LOW);
  head = shift(0x01);
  multitap = shift(0x42);
  padding = shift(0x00); //next time normal read
  if (multitap == 0x80) {
    for (uint8_t i = 0; i < 4; i++) {
      Joystick[i].type = shift(0x00);
      padding = shift(0x00);
      Joystick[i].data[0] = ~shift(0x00); //buttons
      Joystick[i].data[1] = ~shift(0x00); //buttons
      Joystick[i].data[2] = shift(0x00); //right analog
      Joystick[i].data[3] = shift(0x00); //right analog
      Joystick[i].data[4] = shift(0x00); //left analog
      Joystick[i].data[5] = shift(0x00); //left analog
    }
  }
  digitalWrite(ATT1, HIGH);

  #ifdef DEBUG
  for (uint8_t i = 0; i < 4; i++) {
    Serial.print(" multitap: "); Serial.println(multitap, HEX);
    Serial.print(" type: 0x"); Serial.print(Joystick[i].type, HEX);
    Serial.print(" data: 0x"); Serial.print(Joystick[i].data[0], HEX);
    Serial.print(" 0x"); Serial.print(Joystick[i].data[1], HEX);
    Serial.print(" 0x"); Serial.print(Joystick[i].data[2], HEX);
    Serial.print(" 0x"); Serial.print(Joystick[i].data[3], HEX);
    Serial.print(" 0x"); Serial.print(Joystick[i].data[4], HEX);
    Serial.print(" 0x"); Serial.print(Joystick[i].data[5], HEX);
    Serial.println();
  }
  /*Serial.print(" type: 0x"); Serial.print(Joystick[0].type, HEX);
  Serial.print(" data: 0x"); Serial.print(Joystick[0].data[0], HEX);
  Serial.print(" 0x"); Serial.print(Joystick[0].data[1], HEX);
  Serial.print(" 0x"); Serial.print(Joystick[0].data[2], HEX);
  Serial.print(" 0x"); Serial.print(Joystick[0].data[3], HEX);
  Serial.print(" 0x"); Serial.print(Joystick[0].data[4], HEX);
  Serial.print(" 0x"); Serial.print(Joystick[0].data[5], HEX);
  Serial.println();*/
  Serial.flush();
  #endif

  Joystick[0].updateState();
  Joystick[1].updateState();
  Joystick[2].updateState();
  Joystick[3].updateState();
  Joystick[0].sendState();
  Joystick[1].sendState();
  Joystick[2].sendState();
  Joystick[3].sendState();  
  delayMicroseconds(1000);


}
