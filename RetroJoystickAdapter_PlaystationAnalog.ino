#include "Joystick.h"
//http://www.gamesx.com/controldata/psxcont/psxcont.htm

#define DATA 2
#define CMD 3
#define ATT 4
#define CLK 5


//#define DEBUG

byte shift(byte dataOut, uint8_t readmode = 0) { // Does the actual shifting, both in and out
  byte dataIn = 0;
  byte delay = 10;
  for (uint8_t i = 0; i <= 7; i++) {
    digitalWrite(CMD, bitRead(dataOut,i));
    digitalWrite(CLK, LOW);
    delayMicroseconds(delay);
    if (digitalRead(DATA)) bitSet(dataIn, i);
    digitalWrite(CLK, HIGH);
    delayMicroseconds(delay);
  }
  return dataIn;
}

uint8_t type;
uint8_t ready;
uint8_t btngrp1;
uint8_t btngrp2;
uint8_t analog1;
uint8_t analog2;
uint8_t analog3;
uint8_t analog4;

void debug() {
  Serial.print("d1: ");
  Serial.print(type,HEX);
  Serial.print(", ");
  Serial.print("d2: ");
  Serial.print(ready,HEX);
  Serial.print(", ");
  Serial.print("d3: ");
  Serial.print(btngrp1,HEX);
  Serial.print(", ");
  Serial.print("d4: ");
  Serial.print(btngrp2,HEX);
  Serial.print(", ");
  Serial.print("d5: ");
  Serial.print(analog1,HEX);
  Serial.print(", ");
  Serial.print("d6: ");
  Serial.print(analog2,HEX);
  Serial.print(", ");
  Serial.print("d7: ");
  Serial.print(analog3,HEX);
  Serial.print(", ");
  Serial.print("d8: ");
  Serial.println(analog4,HEX);
}

void readJoysticks() {
  
  digitalWrite(ATT, LOW);
  shift(0x01);
  type = shift(0x42); //type (0x41 / 0x73)
  ready = shift(0xFF); //ready (0x5a)
  btngrp1 = shift(0xFF);
  btngrp2 = shift(0xFF);
  if (type == 0x73) {
    analog1 = shift(0xFF);
    analog2 = shift(0xFF);
    analog3 = shift(0xFF);
    analog4 = shift(0xFF);
  }
  digitalWrite(ATT, HIGH);

  delayMicroseconds(1000);
  #ifdef DEBUG
  debug();
  delay(100);
  #endif
}

//btngrp1
#define LEFT      0x80
#define DOWN      0x40
#define RIGHT     0x20
#define UP        0x10
#define START     0x08
#define JOYR      0x04
#define JOYL      0x02
#define SELECT    0x01

//btngrp2
#define SQUARE    0x80
#define X         0x40
#define O         0x20
#define TRIANGLE  0x10
#define R1        0x08
#define L1        0x04
#define R2        0x02
#define L2        0x01

void interpretJoystickState_analog() {
  Joystick.setXAxis( ((signed int)analog3)-127 );
  Joystick.setYAxis( ((signed int)analog4)-127 );
  Joystick.setXAxisRotation( (analog1*1.4) );
  Joystick.setYAxisRotation( (255-analog2)*1.4 );
  Joystick.setButton(0, !(btngrp1&START) ); //BUTTON1 (Start)
  Joystick.setButton(1, !(btngrp1&SELECT) ); //BUTTON2 (Select)
  Joystick.setButton(2, !(btngrp2&X) ); //BUTTON3 (A)
  Joystick.setButton(3, !(btngrp2&O) ); //BUTTON4 (B)
  Joystick.setButton(4, !(btngrp2&SQUARE) ); //BUTTON5 (X)
  Joystick.setButton(5, !(btngrp2&TRIANGLE) ); //BUTTON6 (Y)
  Joystick.setButton(6, !(btngrp2&L2) ); //BUTTON7 (LB)  
  Joystick.setButton(7, !(btngrp2&R2) ); //BUTTON8 (RB)
  Joystick.setButton(8, !(btngrp2&L1) ); //BUTTON9 (LT)
  Joystick.setButton(9, !(btngrp2&R1) ); //BUTTON10 (RT)
  Joystick.setButton(10, !(btngrp1&JOYL) ); //BUTTON11 (Left Thumb)
  Joystick.setButton(11, !(btngrp1&JOYR) ); //BUTTON12 (Right Thumb)

  Joystick.setHatSwitch(0,-1);
  if ( !(btngrp1&UP) ) Joystick.setHatSwitch(0,0); //UP
  if ( !(btngrp1&DOWN) ) Joystick.setHatSwitch(0,180); //DOWN
  if ( !(btngrp1&LEFT) ) Joystick.setHatSwitch(0,270); //LEFT
  if ( !(btngrp1&RIGHT) ) Joystick.setHatSwitch(0,90); //RIGHT
}


void interpretJoystickState() {
  Joystick.setYAxis(0);
  Joystick.setXAxis(0);
  if ( !(btngrp1&UP) ) Joystick.setYAxis(-127); //UP
  if ( !(btngrp1&DOWN) ) Joystick.setYAxis(127); //DOWN
  if ( !(btngrp1&LEFT) ) Joystick.setXAxis(-127); //LEFT
  if ( !(btngrp1&RIGHT) ) Joystick.setXAxis(127); //RIGHT
    
  Joystick.setButton(0, !(btngrp1&START) ); //BUTTON1 (Start)
  Joystick.setButton(1, !(btngrp1&SELECT) ); //BUTTON2 (Select)
  Joystick.setButton(2, !(btngrp2&X) ); //BUTTON3 (A)
  Joystick.setButton(3, !(btngrp2&O) ); //BUTTON4 (B)
  Joystick.setButton(4, !(btngrp2&SQUARE) ); //BUTTON5 (X)
  Joystick.setButton(5, !(btngrp2&TRIANGLE) ); //BUTTON6 (Y)
  Joystick.setButton(6, !(btngrp2&L2) ); //BUTTON7 (LB)  
  Joystick.setButton(7, !(btngrp2&R2) ); //BUTTON8 (RB)
  Joystick.setButton(8, !(btngrp2&L1) ); //BUTTON9 (LT)
  Joystick.setButton(9, !(btngrp2&R1) ); //BUTTON10 (RT)

  Joystick.setXAxisRotation(180);
  Joystick.setYAxisRotation(180);
  Joystick.setHatSwitch(0,-1);
  Joystick.setButton(10, 0);
  Joystick.setButton(11, 0);

}


void setup() {
  
  pinMode(DATA, INPUT_PULLUP);
  pinMode(CMD, OUTPUT);
  pinMode(ATT, OUTPUT);
  pinMode(CLK, OUTPUT);

  Joystick.begin(false);
  
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
  
}




void loop() {

  readJoysticks();
  
  if (type == 0x73) {
    interpretJoystickState_analog();
  }
  else {
    interpretJoystickState();
  }
  
  Joystick.sendState();

}
