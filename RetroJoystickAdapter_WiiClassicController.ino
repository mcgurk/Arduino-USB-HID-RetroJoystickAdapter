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
/*#define JOYSTICK2_REPORT_ID 0x04
#define JOYSTICK3_REPORT_ID 0x05
#define JOYSTICK4_REPORT_ID 0x06*/


#define JOYSTICK_STATE_SIZE 6

//#define DEBUG

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
  HIDDESC_MACRO(JOYSTICK_REPORT_ID)/*,
  HIDDESC_MACRO(JOYSTICK2_REPORT_ID),
  HIDDESC_MACRO(JOYSTICK3_REPORT_ID),
  HIDDESC_MACRO(JOYSTICK4_REPORT_ID)*/
};

class Joystick_ {

private:
  uint8_t joystickId;
  uint8_t reportId;
  uint8_t olddata[JOYSTICK_STATE_SIZE];
  uint8_t state[JOYSTICK_STATE_SIZE];
  uint8_t flag;
  
public:
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
    memcpy(olddata, data, JOYSTICK_STATE_SIZE);
    state[0] = 0;
    state[1] = 0;
    state[2] = 127;
    state[3] = 127;
    state[4] = 127;
    state[5] = 127;
    sendState(1);
  }

  void updateState() {
    if (memcmp(olddata, data, JOYSTICK_STATE_SIZE)) {    
      memcpy(olddata, data, JOYSTICK_STATE_SIZE);
      flag = 1;
    }
    //uint8_t rightShoulderPressure = (Joystick[0].data[3] & (unsigned char)0x1f); //rightmost 5 bits
    //uint8_t leftShoulderPressure = ((Joystick[0].data[2] & (unsigned char)0x60) >> 2) + ((data[3] & (unsigned char)0xe0) >> 5); //'leftish' bits
    uint8_t leftStickX = (data[0] & (unsigned char)0x3f); //rightmost 6 bits
    uint8_t leftStickY = (data[1] & (unsigned char)0x3f); //rightmost 6 bits      
    uint8_t rightStickX = ((data[0] & (unsigned char)0xc0) >> 3) + ((data[1] & (unsigned char)0xc0) >> 5) +  ((data[2] & (unsigned char)0x80) >> 7); //bits. They're there.
    uint8_t rightStickY = (data[2] & (unsigned char)0x1f);    //rightmost 5 bits
    state[0] = ~data[4];
    state[1] = ~data[5];
    state[2] = rightStickX << 3;
    state[3] = rightStickY << 3; //is this right or should it be mirror?
    state[4] = leftStickX << 2;
    state[5] = 255 - (leftStickY << 2);
    /*state[2] = rightStickX;
    state[3] = rightStickY; //is this right or should it be mirror?
    state[4] = leftStickX;
    state[5] = 63 - leftStickY;*/
    //sendState();
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
    Joystick_(0, JOYSTICK_REPORT_ID)/*,
    Joystick_(1, JOYSTICK2_REPORT_ID),
    Joystick_(2, JOYSTICK3_REPORT_ID),
    Joystick_(3, JOYSTICK4_REPORT_ID)*/
};

//================================================================================
//================================================================================

/*
  Andrew Mascolo 7/13/2014
  Updated WiiController library
  
  !!!DOES NOT WORK WITH NUNCHUCK!!!
  
  If you are using the adapter in this link,http://www.ebay.com/itm/Wii-WiiChuck-Nunchuck-Adapter-Module-Board-for-Arduino-/351052353286?pt=LH_DefaultDomain_0&hash=item51bc59cf06 
  then you most likely have it plug into the arduino across pins A2 - A5. Please know that the controller needs 3.3V to work properly and NOT 5V. You may cause possible
  to the controller if it is powered by 5 volts. What I have done here is allowed the user to use pin A2 as ground, by setting the pin to LOW, and making pin A3 an input
  so the user can have a jumper to the 3.3V pin without harming the Arduino.
  
  **.begin(Vcc, Gnd)**
  Vcc: Set to ON if you are using the adapter AND the Arduino board you are using outputs 3.3V. (Like a Pro Micro or DUE)
  Vcc: Set to NEUTRAL if you are using your own wires and are not using pin A3 for power. (pin will be set to INPUT)
  
  Gnd: Set to OFF if you have the adapter in the pin A2, this will automatically set pin A2 to LOW. Otherwise leave empty, default is OFF
*/ 

//#include<WiiClassicControl.h>
#include<Wire.h>

//WiiClassicControl Wii;

#define ADDRESS 0x52

void sendByte(int data, int location)  {  
  Wire.beginTransmission(ADDRESS);
  Wire.write(location);
  Wire.write(data);  
  Wire.endTransmission();
  delay(10);
}

void setup() {
  //Wii.begin(NEUTRAL, OFF); // SEE ABOVE
  #ifdef DEBUG
  Serial.begin(115200);
  //pinMode(9, INPUT);
  #endif
  
  Wire.begin();
   // transmit to device (byte)0x52
  sendByte(0x55, 0xF0);
  sendByte(0x00, 0xFB); 
  Wire.endTransmission ();
  
}

void loop() {

  Wire.requestFrom(ADDRESS, 6); //request data from wii classic
  /*Serial.print(Wire.read(), HEX);Serial.print(" ");
  Serial.print(Wire.read(), HEX);Serial.print(" ");
  Serial.print(Wire.read(), HEX);Serial.print(" ");
  Serial.print(Wire.read(), HEX);Serial.print(" ");
  Serial.print(Wire.read(), HEX);Serial.print(" ");
  Serial.print(Wire.read(), HEX);Serial.print(" ");
  sendByte(0x00, 0x00); 
  Serial.println();*/
  uint8_t i = 0;
  while(Wire.available()) {
    Joystick[0].data[i] = Wire.read();
    i++;
    if (i > 5) break;
  }

  //detect if init is needed
  if (i < 5) {
    delay(100);
    sendByte(0x55, 0xF0);
    sendByte(0x00, 0xFB);
    #ifdef DEBUG
    Serial.println("Init!");
    #endif
    delay(100);
  } else sendByte(0x00, 0x00);


  /*for (uint8_t i = 0; i < 6; i++) {
    Joystick[0].data[i] = Wire.read();
  }
  sendByte(0x00, 0x00); */
  /*while (Wire.available ()) {
    // read byte as an integer
    if (cnt < 4) {
      status[cnt] = Wire.read();
    } else {
      lastButtons[cnt-4] = buttons[cnt-4];
      buttons[cnt-4] = Wire.read();
    }
    cnt++;
    }
    if (cnt > 5) {
      WiiClassy::_sendByte(0x00, 0x00);
      cnt = 0;                   
    }*/
    //Reference for this found here: http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Classic_Controller
      
  
  
  // the lowest value is 0, and the highest value is 248.
  /*Serial.print("LSX: "); Serial.print(Wii.leftStickX());
  Serial.print("| LSY: "); Serial.print(Wii.leftStickY());
  Serial.print("| RSX: "); Serial.print(Wii.rightStickX());
  Serial.print("| RSY: "); Serial.println(Wii.rightStickY());

  Joystick[0].data[4] = Wii.leftStickX();
  Joystick[0].data[5] = Wii.leftStickY();
  Joystick[0].data[2] = Wii.rightStickX();
  Joystick[0].data[3] = Wii.rightStickY();
  Joystick[0].sendState();*/
  uint8_t rightShoulderPressure = (Joystick[0].data[3] & (unsigned char)0x1f); //rightmost 5 bits
  uint8_t leftShoulderPressure = ((Joystick[0].data[2] & (unsigned char)0x60) >> 2) + ((Joystick[0].data[3] & (unsigned char)0xe0) >> 5); //'leftish' bits
  uint8_t leftStickX = (Joystick[0].data[0] & (unsigned char)0x3f); //rightmost 6 bits
  uint8_t leftStickY = (Joystick[0].data[1] & (unsigned char)0x3f); //rightmost 6 bits      
  uint8_t rightStickX = ((Joystick[0].data[0] & (unsigned char)0xc0) >> 3) + ((Joystick[0].data[1] & (unsigned char)0xc0) >> 5) +  ((Joystick[0].data[2] & (unsigned char)0x80) >> 7); //bits. They're there.
  uint8_t rightStickY = (Joystick[0].data[2] & (unsigned char)0x1f);    //rightmost 5 bits  
    
  #ifdef DEBUG
  for (uint8_t i = 0; i < 6; i++) {
    Serial.print(Joystick[0].data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" - ");
  Serial.print(rightShoulderPressure, HEX); Serial.print(" ");
  Serial.print(leftShoulderPressure, HEX); Serial.print(" ");
  Serial.print(leftStickX, HEX); Serial.print(" ");
  Serial.print(leftStickY, HEX); Serial.print(" ");
  Serial.print(rightStickX, HEX); Serial.print(" ");
  Serial.print(rightStickY , HEX); Serial.print(" ");
  Serial.println();
  //Serial.print(digitalRead(9)); Serial.println();
  #endif

  Joystick[0].updateState();
  Joystick[0].sendState();
  //delay(100); // used to see changes in the serial monitor easier.
  delayMicroseconds(1000);
}
