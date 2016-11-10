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
#define JOYSTICK_STATE_SIZE 6
#define JOYSTICK_DATA_SIZE 6

//#define DEBUG

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
    0x95, 0x02,               /*   REPORT_COUNT (2) */ \
    0x81, 0x02,               /*   INPUT (Data,Var,Abs) */ \
    0x75, 0x06,               /*   REPORT_SIZE (6) */ \
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
    sendState(1);
  }

  void updateState() {
    if (memcmp(olddata, data, JOYSTICK_DATA_SIZE)) {    
      memcpy(olddata, data, JOYSTICK_DATA_SIZE);
      flag = 1;
    }

    uint8_t SX = data[0];
    uint8_t SY = ~data[1];
    uint16_t AX = (data[2] << 2) | (data[5] >> 2 & B11);
    uint16_t AY = (data[3] << 2) | (data[5] >> 4 & B11);
    uint16_t AZ = (data[4] << 2) | (data[5] >> 6 & B11);
    uint8_t BC = (~data[5] >> 1 & 1);
    uint8_t BZ = (~data[5] & 1);
    // http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck

    state[0] = (BZ << 1) | BC;
    state[1] = SX;
    state[2] = SY;
    state[3] = AX >> 2;
    state[4] = AY >> 2;
    state[5] = AZ >> 2;
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

void sendByte(int data, int location)  {  
  Wire.beginTransmission(ADDRESS);
  Wire.write(location);
  Wire.write(data);  
  Wire.endTransmission();
}

void sendZero()  {  
  Wire.beginTransmission(ADDRESS);
  Wire.write(0x00);  
  Wire.endTransmission();
}

void setup() {
  Wire.begin();
  sendByte(0x55, 0xF0);
  sendByte(0x00, 0xFB);

  #ifdef DEBUG
  Serial.begin(115200);
  #endif

}

void loop() {

  #ifdef DEBUG
  unsigned long t = micros();
  #endif

  sendZero();
  delayMicroseconds(200);
  Wire.requestFrom(ADDRESS, 6); //request data from wii nunchuck
  uint8_t i = 0;
  while(Wire.available()) {
    Joystick[0].data[i] = Wire.read();
    i++;
    if (i > JOYSTICK_DATA_SIZE) break;
  }

  //detect if init is needed
  if (i < 6) {
    delay(100);
    sendByte(0x55, 0xF0);
    sendByte(0x00, 0xFB);
    #ifdef DEBUG
    Serial.println("Init!");
    #endif
    delay(100);
  }
  
  #ifdef DEBUG
  Serial.println(micros()-t);
  #endif

  #ifdef DEBUG
  /*for (uint8_t i = 0; i < JOYSTICK_DATA_SIZE; i++) {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();*/
  uint8_t SX = Joystick[0].data[0];
  uint8_t SY = Joystick[0].data[1];
  uint16_t AX = (Joystick[0].data[2] << 2) | (Joystick[0].data[5] >> 2 & B11);
  uint16_t AY = (Joystick[0].data[3] << 2) | (Joystick[0].data[5] >> 4 & B11);
  uint16_t AZ = (Joystick[0].data[4] << 2) | (Joystick[0].data[5] >> 6 & B11);
  uint8_t BC = Joystick[0].data[5] >> 1 & 1;
  uint8_t BZ = Joystick[0].data[5] & 1;
  Serial.print(" SX:"); Serial.print(SX);
  Serial.print(" SY:"); Serial.print(SY);
  Serial.print(" AX:"); Serial.print(AX);
  Serial.print(" AY:"); Serial.print(AY);
  Serial.print(" AZ:"); Serial.print(AZ);
  Serial.print(" BC:"); Serial.print(BC);
  Serial.print(" BZ:"); Serial.print(BZ);
  Serial.println();
  #endif

  Joystick[0].updateState();
  Joystick[0].sendState();
  #ifdef DEBUG
  delay(100); // used to see changes in the serial monitor easier.
  #endif
  delayMicroseconds(1000);
}
