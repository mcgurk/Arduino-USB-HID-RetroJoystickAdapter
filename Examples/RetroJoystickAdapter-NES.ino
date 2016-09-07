/*
Description:  Interfacing a NES controller with a PC with an Arduino.
Coded by: Prodigity
Date:   1 December 2011
Revision: V0.93 (beta)
Modified by:    Matt Booth (20 December 2014)
Adapted for USB (ATmega32U4, 5V) by Kurg 3.9.2016
*/

#include "Joystick2.h"

uint8_t lastStatusPort1 = 0xff;
uint8_t newStatusPort1 = 0xff;


const int latch = 8;
const int clock = 9;
const int data  = 7;

#define latchlow digitalWrite(latch, LOW)
#define latchhigh digitalWrite(latch, HIGH)
#define clocklow digitalWrite(clock, LOW)
#define clockhigh digitalWrite(clock, HIGH)
#define dataread digitalRead(data)

// http://www.mit.edu/~tarvizo/nes-controller.html
#define wait delayMicroseconds(12)

byte output;

void setup() {

//  Serial.begin(9600);
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, INPUT);

  Joystick[0].begin(false);
}

void loop() {
  output = 0;
  ReadNESjoy();
  //Serial.println(output,BIN);
  newStatusPort1 = output;
  if (lastStatusPort1 != newStatusPort1) {
    Joystick[0].setYAxis(0);
    Joystick[0].setXAxis(0);
    if (!bitRead(newStatusPort1,4)) Joystick[0].setYAxis(-127); //UP
    if (!bitRead(newStatusPort1,5)) Joystick[0].setYAxis(127); //DOWN
    if (!bitRead(newStatusPort1,6)) Joystick[0].setXAxis(-127); //LEFT
    if (!bitRead(newStatusPort1,7)) Joystick[0].setXAxis(127); //RIGHT
    Joystick[0].setButton(0, !bitRead(newStatusPort1,0)); //BUTTON1 (A)
    Joystick[0].setButton(1, !bitRead(newStatusPort1,1)); //BUTTON2 (B)
    Joystick[0].setButton(2, !bitRead(newStatusPort1,2)); //BUTTON3 (Select)
    Joystick[0].setButton(3, !bitRead(newStatusPort1,3)); //BUTTON4 (Start)
    Joystick[0].sendState();
    lastStatusPort1 = newStatusPort1;
  }
  
}


void ReadNESjoy() {
  latchlow;
  clocklow;
  latchhigh;
  wait;
  latchlow;
 
  for (int i = 0; i < 8; i++) {
     output += dataread * (1 << i);
     clockhigh;
     wait;
     clocklow;
     wait;
  }
}
