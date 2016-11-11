//Uno: SDA = A4, SCL = A5
//Pro Micro: SDA = D2, SCL = D3

//================================================================================
//================================================================================
//  Joystick (Gamepad)

#include "HID.h"

#if ARDUINO < 10606
#error The Joystick2 library requires Arduino IDE 1.6.6 or greater. Please update your IDE.
#endif

#if !defined(USBCON)
#error The Joystick2 library can only be used with a USB MCU (e.g. Arduino Leonardo, Arduino Micro, etc.).
#endif

#if !defined(_USING_HID)
#error "Using legacy HID core (non pluggable)"
#endif

#define JOYSTICK_REPORT_ID  0x04
#define JOYSTICK_STATE_SIZE 7
#define JOYSTICK_DATA_SIZE 6

#define NUNCHUCK 1
#define CLASSIC_CONTROLLER 2
#define CLASSIC_CONTROLLER_PRO 3

//#define DEBUG

#define HIDDESC_MACRO(REPORT_ID) \
    /* Joystick # */ \
    0x05, 0x01,               /* USAGE_PAGE (Generic Desktop) */ \
    0x09, 0x04,               /* USAGE (Joystick) */ \
    0xa1, 0x01,               /* COLLECTION (Application) */ \
    0x85, REPORT_ID,          /* REPORT_ID */ \
    /* 15 Buttons */ \
    0x05, 0x09,               /*   USAGE_PAGE (Button) */ \
    0x19, 0x01,               /*   USAGE_MINIMUM (Button 1) */ \
    0x29, 0x0F,               /*   USAGE_MAXIMUM (Button 15) */ \
    0x15, 0x00,               /*   LOGICAL_MINIMUM (0) */ \
    0x25, 0x01,               /*   LOGICAL_MAXIMUM (1) */ \
    0x75, 0x01,               /*   REPORT_SIZE (1) */ \
    0x95, 0x0F,               /*   REPORT_COUNT (15) */ \
    0x81, 0x02,               /*   INPUT (Data,Var,Abs) */ \
    0x75, 0x01,               /*   REPORT_SIZE (1) */ \
    0x95, 0x01,               /*   REPORT_COUNT (1) */ \
    0x81, 0x03,               /*   INPUT (Cnst,Var,Abs) */ \
    /* X and Y Axis */ \
    0x05, 0x01,               /*   USAGE_PAGE (Generic Desktop) */ \
    0x09, 0x01,               /*   USAGE (Pointer) */ \
    0xA1, 0x00,               /*   COLLECTION (Physical) */ \
    0x09, 0x30,               /*     USAGE (x) */ \
    0x09, 0x31,               /*     USAGE (y) */ \
    0x09, 0x33,               /*     USAGE (Rx) */ \
    0x09, 0x34,               /*     USAGE (Ry) */ \
    0x09, 0x35,               /*     USAGE (Rz) */ \
    0x15, 0x00,               /*     LOGICAL_MINIMUM (0) */ \
    0x26, 0xff, 0x00,         /*     LOGICAL_MAXIMUM (255) */ \
    0x75, 0x08,               /*     REPORT_SIZE (8) */ \
    0x95, 0x05,               /*     REPORT_COUNT (5) */ \
    0x81, 0x02,               /*     INPUT (Data,Var,Abs) */ \
    0xc0,                     /*   END_COLLECTION */ \
    0xc0                      /* END_COLLECTION */


static const uint8_t hidReportDescriptor[] PROGMEM = {
  HIDDESC_MACRO(JOYSTICK_REPORT_ID)
};

class Joystick_ {

private:
  uint8_t joystickId;
  uint8_t reportId;
  uint8_t olddata[JOYSTICK_DATA_SIZE];
  uint8_t state[JOYSTICK_STATE_SIZE];
  uint8_t flag;
  
public:
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
    memcpy(olddata, data, JOYSTICK_DATA_SIZE);
    state[0] = 0;
    state[1] = 0;
    state[2] = 127;
    state[3] = 127;
    state[4] = 127;
    state[5] = 127;
    state[6] = 127;
    sendState(1);
  }

  void updateState(uint8_t type) {
    if (memcmp(olddata, data, JOYSTICK_DATA_SIZE)) {    
      memcpy(olddata, data, JOYSTICK_DATA_SIZE);
      flag = 1;
    }

    if (type == NUNCHUCK) {
      // http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
      uint8_t SX = data[0];
      uint8_t SY = data[1];
      uint16_t AX = (data[2] << 2) | (data[5] >> 2 & B11);
      uint16_t AY = (data[3] << 2) | (data[5] >> 4 & B11);
      uint16_t AZ = (data[4] << 2) | (data[5] >> 6 & B11);
      uint8_t BC = (~data[5] >> 1 & 1);
      uint8_t BZ = (~data[5] & 1);

      state[0] = (BZ << 1) | BC;
      state[1] = 0;
      state[2] = SX;
      state[3] = ~SY;
      state[4] = AX >> 2;
      state[5] = AY >> 2;
      state[6] = AZ >> 2;
    }
    if (type == CLASSIC_CONTROLLER || type == CLASSIC_CONTROLLER_PRO) {
      // http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Classic_Controller
      uint8_t LX = (data[0] & 0x3f); //6bit
      uint8_t LY = (data[1] & 0x3f); //6bit
      uint8_t RX = ((data[0] & 0xc0) >> 3) + ((data[1] & 0xc0) >> 5) +  ((data[2] & 0x80) >> 7); //5bit
      uint8_t RY = (data[2] & 0x1f); //5bit
      uint8_t BDU = ~data[5] & 1;
      uint8_t BDD = ~data[4] >> 6 & 1;
      uint8_t BDL = ~data[5] >> 1 & 1;
      uint8_t BDR = ~data[4] >> 7 & 1;
      uint8_t Bselect = ~data[4] >> 4 & 1;
      uint8_t BH = ~data[4] >> 3 & 1;
      uint8_t Bstart = ~data[4] >> 2 & 1;
      uint8_t BA = ~data[5] >> 4 & 1;
      uint8_t BB = ~data[5] >> 6 & 1;
      uint8_t BX = ~data[5] >> 3 & 1;
      uint8_t BY = ~data[5] >> 5 & 1;
      uint8_t BLT = ~data[4] >> 5 & 1;
      uint8_t BRT = ~data[4] >> 1 & 1;
      uint8_t BZL = ~data[5] >> 7 & 1;
      uint8_t BZR = ~data[5] >> 2 & 1;
      uint8_t LT = ((data[2] & 0x60) >> 2) + ((data[3] & 0xe0) >> 5);
      uint8_t RT = (data[3] & 0x1f);

      state[0] = (BZR << 7) | (BZL << 6) | (BRT << 5) | (BLT << 4) | (BY << 3) | (BX << 2) | (BB << 1) | BA;
      state[1] = (Bstart << 6) | (BH << 5) | (Bselect << 4) | (BDR << 3) | (BDL << 2) | (BDD << 1) | BDU;
      state[2] = LX << 2;
      state[3] = ~(LY << 2);
      state[4] = RX << 3;
      state[5] = ~(RY << 3);
      state[6] = 127;
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


Joystick_ Joystick[1] =
{
    Joystick_(0, JOYSTICK_REPORT_ID)
};

//================================================================================
//================================================================================

#include <Wire.h>

#define ADDRESS 0x52

uint16_t type;

const uint8_t ident_nunchuck[] = { 0x00, 0x00, 0xA4, 0x20, 0x00, 0x00 };
const uint8_t ident_classic_controller[] = { 0x00, 0x00, 0xA4, 0x20, 0x01, 0x01 };
const uint8_t ident_classic_controller_pro[] = { 0x01, 0x00, 0xA4, 0x20, 0x00, 0x00 };


void sendByte(uint8_t data, uint8_t location)  {  
  Wire.beginTransmission(ADDRESS);
  Wire.write(location);
  Wire.write(data);  
  Wire.endTransmission();
}

void sendByte(uint8_t data)  {  
  Wire.beginTransmission(ADDRESS);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t initExtension() {
  uint8_t buf[6];
  uint8_t type = CLASSIC_CONTROLLER; // default type
  sendByte(0x55, 0xF0);
  sendByte(0x00, 0xFB);
  sendByte(0xFA);
  delayMicroseconds(200);
  Wire.requestFrom(ADDRESS, 6);
  uint8_t i = 0;
  while(Wire.available()) {
    buf[i] = Wire.read();
    i++;
    if (i >= 6) break;
  }
  if (memcmp(buf, ident_nunchuck, 6) == 0) type = NUNCHUCK;
  if (memcmp(buf, ident_classic_controller, 6) == 0) type = CLASSIC_CONTROLLER;
  if (memcmp(buf, ident_classic_controller_pro, 6) == 0) type = CLASSIC_CONTROLLER_PRO;
  return type;
}

void setup() {
  Wire.begin();
  type = initExtension();

  #ifdef DEBUG
  Serial.begin(115200);
  #endif

}

void loop() {

  #ifdef DEBUG
  unsigned long t = micros();
  #endif

  sendByte(0x00);
  delayMicroseconds(200);
  Wire.requestFrom(ADDRESS, 6); //request data from wii nunchuck
  uint8_t i = 0;
  while(Wire.available()) {
    Joystick[0].data[i] = Wire.read();
    i++;
    if (i >= JOYSTICK_DATA_SIZE) break;
  }

  //detect if init is needed
  if (i < JOYSTICK_DATA_SIZE) {
    delay(10);
    type = initExtension();
    #ifdef DEBUG
    Serial.println("Init!");
    #endif
    delay(10);
  }
  
  #ifdef DEBUG
  Serial.println(micros()-t);
  #endif

  #ifdef DEBUG
  for (uint8_t i = 0; i < JOYSTICK_DATA_SIZE; i++) {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  #endif

  Joystick[0].updateState(type);
  Joystick[0].sendState();
  #ifdef DEBUG
  delay(100);
  #endif
  delayMicroseconds(500);
}
