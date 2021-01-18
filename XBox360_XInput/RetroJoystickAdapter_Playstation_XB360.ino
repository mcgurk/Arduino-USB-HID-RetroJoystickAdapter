#define XINPUT

#ifdef XINPUT
#include <XInput.h>
#endif

#ifndef XINPUT
#define DEBUG
#endif
//#define DEBUG //doesn't work with XInput

// 5V (red)
// GND (black)
#define DATA1 2 // (brown)
#define CMD1 3 // (orange)
#define ATT1 4 // (yellow)
#define CLK1 5 // (blue)

#define JOYSTICK_STATE_SIZE 6

//data[0]
//bit0 = select
//bit1 = L3
//bit2 = R3
//bit3 = start
//bit4 = up
//bit5 = right
//bit6 = down
//bit7 = left
//data[1}
//bit0 = L2
//bit1 = R2
//bit2 = L1
//bit3 = R1
//bit4 = T
//bit5 = O
//bit6 = X
//bit7 = S

#define PS_select (data[0] & ( 1 << 0 ))
#define PS_L3 (data[0] & ( 1 << 1 ))
#define PS_R3 (data[0] & ( 1 << 2 ))
#define PS_start (data[0] & ( 1 << 3 ))
#define PS_up (data[0] & ( 1 << 4 ))
#define PS_right (data[0] & ( 1 << 5 ))
#define PS_down (data[0] & ( 1 << 6 ))
#define PS_left (data[0] & ( 1 << 7 ))

#define PS_L2 (data[1] & ( 1 << 0 ))
#define PS_R2 (data[1] & ( 1 << 1 ))
#define PS_L1 (data[1] & ( 1 << 2 ))
#define PS_R1 (data[1] & ( 1 << 3 ))
#define PS_T (data[1] & ( 1 << 4 ))
#define PS_O (data[1] & ( 1 << 5 ))
#define PS_X (data[1] & ( 1 << 6 ))
#define PS_S (data[1] & ( 1 << 7 ))

#define PS_LX (((uint16_t)data[4]*257)-32768) // 0..255 -> -32768..32767
#define PS_LY (((255-(uint16_t)data[5])*257)-32768) // 0..255 -> 32767..-32768
#define PS_RX (((uint16_t)data[2]*257)-32768) // 0..255 -> -32768..32767
#define PS_RY (((255-(uint16_t)data[3])*257)-32768) // 0..255 -> 32767..-32768

uint8_t head;
uint8_t type;
uint8_t padding;
uint8_t data[JOYSTICK_STATE_SIZE];
uint8_t olddata[JOYSTICK_STATE_SIZE];
uint8_t flag;

/*
void setup() {
  XInput.begin();
}

void loop() {
  XInput.press(BUTTON_A);
  delay(1000);
  
  XInput.release(BUTTON_A);
  delay(1000);
}*/




/*
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
};*/





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
  
  #ifdef DEBUG
  Serial.begin(115200);
  while(!Serial);
  #endif

  #ifdef XINPUT
  XInput.begin();
  #endif

  olddata[0] = 0xff; 
  olddata[1] = 0xff;

}

void loop() {
  // http://problemkaputt.de/psx-spx.htm#controllerandmemorycardsignals
  // first: read gamepad normally
  digitalWrite(ATT1, LOW);
  //digitalWrite(ATT2, LOW);
  head = shift(0x01);
  type = shift(0x42);
  padding = shift(0x01); //read multitap in next command
  data[0] = ~shift(0x00); //buttons
  data[1] = ~shift(0x00); //buttons
  data[2] = shift(0x00); //right analog
  data[3] = shift(0x00); //right analog
  data[4] = shift(0x00); //left analog
  data[5] = shift(0x00); //left analog
  digitalWrite(ATT1, HIGH);

  #ifdef DEBUG
  Serial.print(" type: 0x"); Serial.print(type, HEX);
  Serial.print(" data: b"); Serial.print(data[0], BIN);
  Serial.print(" b"); Serial.print(data[1], BIN);
  Serial.print(" 0x"); Serial.print(data[2], HEX);
  Serial.print(" 0x"); Serial.print(data[3], HEX);
  Serial.print(" 0x"); Serial.print(data[4], HEX);
  Serial.print(" 0x"); Serial.print(data[5], HEX);
  Serial.println();
  Serial.flush();
  delay(500);
  #endif

  #ifdef XINPUT
  if (data[0] != olddata[0]) {
    if (PS_select) XInput.press(BUTTON_BACK); else XInput.release(BUTTON_BACK); //7
    if (PS_start) XInput.press(BUTTON_START); else XInput.release(BUTTON_START); //8
    if (PS_L3) XInput.press(BUTTON_L3); else XInput.release(BUTTON_L3); //9
    if (PS_R3) XInput.press(BUTTON_R3); else XInput.release(BUTTON_R3); //10
    XInput.setDpad(PS_up, PS_down, PS_left, PS_right);
    olddata[0] = data[0];
  }
  if (data[1] != olddata[1]) {
    if (PS_X) XInput.press(BUTTON_A); else XInput.release(BUTTON_A); //1
    if (PS_O) XInput.press(BUTTON_B); else XInput.release(BUTTON_B); //2
    if (PS_S) XInput.press(BUTTON_X); else XInput.release(BUTTON_X); //3
    if (PS_T) XInput.press(BUTTON_Y); else XInput.release(BUTTON_Y); //4
    if (PS_L1) XInput.press(BUTTON_LB); else XInput.release(BUTTON_LB); //5
    if (PS_R1) XInput.press(BUTTON_RB); else XInput.release(BUTTON_RB); //6
    if (PS_L2) XInput.setTrigger(TRIGGER_LEFT, 255); else XInput.setTrigger(TRIGGER_LEFT, 0);
    if (PS_R2) XInput.setTrigger(TRIGGER_RIGHT, 255); else XInput.setTrigger(TRIGGER_RIGHT, 0);
    olddata[1] = data[1];
  }
  XInput.setJoystick(JOY_LEFT, PS_LX, PS_LY); //left-right: -32768..32767, down-up: 32767..-32768
  XInput.setJoystick(JOY_RIGHT, PS_RX, PS_RY); //left-right: -32768..32767, down-up: 32767..-32768
  #endif


  delayMicroseconds(1000);


}
