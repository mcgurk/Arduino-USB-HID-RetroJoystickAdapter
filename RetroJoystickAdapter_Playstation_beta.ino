#include "HID.h"

#if ARDUINO < 10606
#error The Joystick2 library requires Arduino IDE 1.6.6 or greater. Please update your IDE.
#endif

#if !defined(USBCON)
#error The Joystick2 library can only be used with a USB MCU (e.g. Arduino Leonardo, Arduino Micro, etc.).
#endif

#define JOYSTICK_REPORT_ID  0x03
#define JOYSTICK2_REPORT_ID 0x04
#define JOYSTICK3_REPORT_ID 0x05
#define JOYSTICK4_REPORT_ID 0x06

//================================================================================
//================================================================================
//  Joystick (Gamepad)

class Joystick_ {

private:
  uint8_t joystickId;
  uint8_t reportId;

public:
  Joystick_(uint8_t initJoystickId, uint8_t initReportId);

  void sendState();

  int8_t xAxis;
  int8_t yAxis;
  uint16_t buttons;

  //void begin(bool initAutoSendState = true);
  //void end();

  //void setXAxis(int8_t value);
  //void setYAxis(int8_t value);

  //void setButton(uint8_t button, uint8_t value);
  //void pressButton(uint8_t button);
  //void releaseButton(uint8_t button);

};


#define JOYSTICK_STATE_SIZE 4

#define HIDDESC_MACRO(REPORT_ID) \
    /* Joystick # */ \
    0x05, 0x01,               /* USAGE_PAGE (Generic Desktop) */ \
    0x09, 0x05,               /* USAGE (Joystick) */ \
    0xA1, 0x02,               /* COLLECTION (Logical) */ \
    0xA1, 0x01,               /*   COLLECTION (Application) */ \
    0x85, REPORT_ID,          /*     REPORT_ID */ \
    /* X and Y Axis */ \
    0x09, 0x30,               /*     USAGE (x) */ \
    0x09, 0x31,               /*     USAGE (y) */ \
    0x15, 0x00,               /*     LOGICAL_MINIMUM (0) */ \
    0x26, 0xff, 0x00,         /*     LOGICAL_MAXIMUM (255) */ \
    0x75, 0x08,               /*     REPORT_SIZE (8) */ \
    0x95, 0x02,               /*     REPORT_COUNT (2) */ \
    0x81, 0x02,               /*     INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position) */ \
    /* 16 Buttons */ \
    0x05, 0x09,               /*     USAGE_PAGE (Button) */ \
    0x19, 0x01,               /*     USAGE_MINIMUM (Button 1) */ \
    0x29, 0x10,               /*     USAGE_MAXIMUM (Button 16) */ \
    0x15, 0x00,               /*     LOGICAL_MINIMUM (0) */ \
    0x25, 0x01,               /*     LOGICAL_MAXIMUM (1) */ \
    0x75, 0x01,               /*     REPORT_SIZE (1) */ \
    0x95, 0x10,               /*     REPORT_COUNT (16) */ \
    0x81, 0x02,               /*     INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position) */ \
    0xC0,                     /*   END_COLLECTION */ \
    0xC0                      /* END_COLLECTION */


static const uint8_t hidReportDescriptor[] PROGMEM = {
  HIDDESC_MACRO(JOYSTICK_REPORT_ID),
  HIDDESC_MACRO(JOYSTICK2_REPORT_ID),
  HIDDESC_MACRO(JOYSTICK3_REPORT_ID),
  HIDDESC_MACRO(JOYSTICK4_REPORT_ID)
};


Joystick_::Joystick_(uint8_t initJoystickId, uint8_t initReportId)
{
  // Setup HID report structure
  static bool usbSetup = false;
  
  if (!usbSetup)
  {
    static HIDSubDescriptor node(hidReportDescriptor, sizeof(hidReportDescriptor));
    HID().AppendDescriptor(&node);
    usbSetup = true;
  }
    
  // Initalize State
  joystickId = initJoystickId;
  reportId = initReportId;
  xAxis = 0;
  yAxis = 0;
  buttons = 0;
  
  sendState();
}

void Joystick_::sendState()
{
    int8_t data[JOYSTICK_STATE_SIZE];
    uint16_t buttonTmp = buttons;

    // Split 16 bit button-state into 2 bytes
    data[2] = buttonTmp & 0xFF;        
    buttonTmp >>= 8;
    data[3] = buttonTmp & 0xFF;

    data[0] = xAxis;
    data[1] = yAxis;

    // HID().SendReport(Report number, array of values in same order as HID descriptor, length)
    HID().SendReport(reportId, data, JOYSTICK_STATE_SIZE);
}

Joystick_ Joystick[4] =
{
    Joystick_(0, JOYSTICK_REPORT_ID),
    Joystick_(1, JOYSTICK2_REPORT_ID),
    Joystick_(2, JOYSTICK3_REPORT_ID),
    Joystick_(3, JOYSTICK4_REPORT_ID)
};

//================================================================================
//================================================================================



#define DATA1 2
#define CMD1 3
#define ATT1 4
#define CLK1 5

/*#define DATA2 6
#define CMD2 7
#define ATT2 8
#define CLK2 9 */

uint8_t shift(uint8_t _dataOut) // Does the actual shifting, both in and out simultaneously
{
  uint8_t _temp = 0;
  uint8_t _dataIn = 0;
  uint8_t _delay = 25;//25;//20; //10; //4; //10; //with 15 unstable. meni epävakaaks 20:lläkin... //testaa kunnon kaapelilla uudestaan
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
  Serial.begin(115200);

}

void loop() {
  // http://problemkaputt.de/psx-spx.htm#controllerandmemorycardsignals
  uint8_t head, type, padding, multitap, data1, data2, data3, data4, data5, data6;
  uint8_t data[100];

  delayMicroseconds(20);
  digitalWrite(ATT1, LOW);
  //digitalWrite(ATT2, LOW);
  delayMicroseconds(50);
  head = shift(0x01);
  type = shift(0x42);
  padding = shift(0x01); //read multitap in next read
  data1 = shift(0x00); //read
  data2 = shift(0x00);
  data3 = shift(0x00);
  data4 = shift(0x00);
  data5 = shift(0x00);
  data6 = shift(0x00);
  digitalWrite(ATT1, HIGH);
  //digitalWrite(ATT2, HIGH);
  Joystick[0].buttons = ~( (data1) | (data2 << 8) );
  //delayMicroseconds(160);
  //delay(16);

  /*Serial.print("head: 0x"); Serial.print(head, HEX); Serial.print(" type: 0x"); Serial.print(type, HEX); Serial.print(" padding: 0x"); Serial.print(padding, HEX); 
  Serial.print(" data1: 0x"); Serial.print(data1, HEX); Serial.print(" "), Serial.print(data1, BIN);
  Serial.print(" data2: 0x"); Serial.print(data2, HEX); Serial.print(" "), Serial.print(data2, BIN);
  Serial.print(" rest: "); Serial.print(data3, DEC); Serial.print(" "); Serial.print(data4, DEC); Serial.print(" "); Serial.print(data5, DEC); Serial.print(" "); Serial.print(data6, DEC);
  Serial.println();*/

  // check and read multitap
  delayMicroseconds(20);
  digitalWrite(ATT1, LOW);
  delayMicroseconds(50);
  head = shift(0x01);
  multitap = shift(0x42);
  padding = shift(0x00); //next time normal read
  //padding = shift(0x01);
  //padding = shift(0x01);
  if (multitap == 0x80) {
    for (uint8_t i = 0; i < 4; i++) {
      type = shift(0x00); //controller ID
      padding = shift(0x00); //controller ID
      //type = shift(0x00); //controller ID
      //padding = shift(0x00); //controller ID
      data1 = shift(0x00); //buttons
      data2 = shift(0x00); //buttons
      data3 = shift(0x00); //left analog
      data4 = shift(0x00); //left analog
      data5 = shift(0x00); //right analog
      data6 = shift(0x00); //right analog
      if (type == 0x73) {
        Joystick[i].xAxis = data5;
        Joystick[i].yAxis = data6;
      } else {
        Joystick[i].xAxis = 128;
        Joystick[i].yAxis = 128;
        if (!bitRead(data1,4)) Joystick[i].yAxis = 0;
        if (!bitRead(data1,5)) Joystick[i].xAxis = 255;
        if (!bitRead(data1,6)) Joystick[i].yAxis = 255;
        if (!bitRead(data1,7)) Joystick[i].xAxis = 0;
        data1 = data1 | B11110000;
      }
      Joystick[i].buttons = ~( (data1) | (data2 << 8) );
    }
  }
  /*for (uint8_t i = 0; i < 8*4; i++) {
    data[i] = shift(0x00);
  }*/
  digitalWrite(ATT1, HIGH);

  /*Serial.print(" head/type/padding: 0x"); Serial.print(head, HEX);
  Serial.print(" 0x"); Serial.print(type, HEX);
  Serial.print(" 0x"); Serial.print(padding, HEX);
  Serial.print(" -  ");
  for (uint8_t i = 0; i < 8*4; i++) {
    Serial.print(data[i], HEX); Serial.print(" ");
  }*/
  Serial.print(" data: 0x"); Serial.print(data1, HEX);
  Serial.print(" 0x"); Serial.print(data2, HEX);
  Serial.print(" 0x"); Serial.print(data3, HEX);
  Serial.print(" 0x"); Serial.print(data4, HEX);
  Serial.print(" 0x"); Serial.print(data5, HEX);
  Serial.print(" 0x"); Serial.print(data6, HEX);
  //Serial.print(" 0x"); Serial.print(data7, HEX);
  //Serial.print(" 0x"); Serial.print(data8, HEX);
  /*Serial.print("--head: 0x"); Serial.print(head, HEX); Serial.print(" type: 0x"); Serial.print(type, HEX); Serial.print(" padding: 0x"); Serial.print(padding, HEX); 
  Serial.print(" data1: 0x"); Serial.print(data1, HEX); Serial.print(" "), Serial.print(data1, BIN);
  Serial.print(" data2: 0x"); Serial.print(data2, HEX); Serial.print(" "), Serial.print(data2, BIN);
  Serial.print(" rest: "); Serial.print(data3, DEC); Serial.print(" "); Serial.print(data4, DEC); Serial.print(" "); Serial.print(data5, DEC); Serial.print(" "); Serial.print(data6, DEC);*/
  Serial.println();
  Serial.flush();
  
  //Serial.print(" type2: "); Serial.print(type, HEX);
  //Serial.println();

  //delayMicroseconds(50);
  Joystick[0].sendState();
  Joystick[1].sendState();
  Joystick[2].sendState();
  Joystick[3].sendState();
  delay(50);
}
