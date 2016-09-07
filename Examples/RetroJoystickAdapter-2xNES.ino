/*
Description:  Interfacing a 2-port NES controller with a PC with an Arduino.
Coded by: Prodigity
Date:   1 December 2011
Revision: V0.93 (beta)
Modified by:    Matt Booth (20 December 2014)
Adapted for USB (ATmega32U4, 5V) by Kurg 3.9.2016
*/

#include "Joystick2.h"

#define EVENTS_TOTAL 4+2+2 //4 directions, 2 fire-buttons and Start + Select

uint8_t lastStatusPort1[EVENTS_TOTAL];
uint8_t newStatusPort1[EVENTS_TOTAL];
uint8_t lastStatusPort2[EVENTS_TOTAL];
uint8_t newStatusPort2[EVENTS_TOTAL];


#define CLOCK1 7
#define LATCH1 8
#define DATA1 9

#define CLOCK2 2
#define LATCH2 3
#define DATA2 4

// http://www.mit.edu/~tarvizo/nes-controller.html


void setup() {

  //clear statusarrays (1=OFF, 0=ON)
  for (uint8_t i = 0; i < EVENTS_TOTAL; i++) {
    lastStatusPort1[i] = 1;
    newStatusPort1[i] = 1;
    lastStatusPort2[i] = 1;
    newStatusPort2[i] = 1;
  }
  
  pinMode(LATCH1, OUTPUT);
  pinMode(CLOCK1, OUTPUT);
  pinMode(DATA1, INPUT);
  pinMode(LATCH2, OUTPUT);
  pinMode(CLOCK2, OUTPUT);
  pinMode(DATA2, INPUT);

  Joystick[0].begin(false);
  Joystick[1].begin(false);
}


uint8_t flag1 = 0;
uint8_t flag2 = 0;


void loop() {

  ReadNESjoy();

  //check for changes - do not raise a flag if nothing changes
  for (uint8_t i=0; i < EVENTS_TOTAL; i++) {
    if (newStatusPort1[i] != lastStatusPort1[i]) {
      lastStatusPort1[i] = newStatusPort1[i]; 
      flag1 = 1;
    }
    if (newStatusPort2[i] != lastStatusPort2[i]) {
      lastStatusPort2[i] = newStatusPort2[i]; 
      flag2 = 1;
    }
  }
  
  if (flag1) {
    Joystick[0].setYAxis(0);
    Joystick[0].setXAxis(0);
    if (!newStatusPort1[4]) Joystick[0].setYAxis(-127); //UP
    if (!newStatusPort1[5]) Joystick[0].setYAxis(127); //DOWN
    if (!newStatusPort1[6]) Joystick[0].setXAxis(-127); //LEFT
    if (!newStatusPort1[7]) Joystick[0].setXAxis(127); //RIGHT
    Joystick[0].setButton(0, !newStatusPort1[0]); //BUTTON1 (A)
    Joystick[0].setButton(1, !newStatusPort1[1]); //BUTTON2 (B)
    Joystick[0].setButton(2, !newStatusPort1[2]); //BUTTON3 (Select)
    Joystick[0].setButton(3, !newStatusPort1[3]); //BUTTON4 (Start)
  }

  if (flag2) {
    Joystick[1].setYAxis(0);
    Joystick[1].setXAxis(0);
    if (!newStatusPort2[4]) Joystick[1].setYAxis(-127); //UP
    if (!newStatusPort2[5]) Joystick[1].setYAxis(127); //DOWN
    if (!newStatusPort2[6]) Joystick[1].setXAxis(-127); //LEFT
    if (!newStatusPort2[7]) Joystick[1].setXAxis(127); //RIGHT
    Joystick[1].setButton(0, !newStatusPort2[0]); //BUTTON1 (A)
    Joystick[1].setButton(1, !newStatusPort2[1]); //BUTTON2 (B)
    Joystick[1].setButton(2, !newStatusPort2[2]); //BUTTON3 (Select)
    Joystick[1].setButton(3, !newStatusPort2[3]); //BUTTON4 (Start)
  }
  
  if (flag1) Joystick[0].sendState();
  if (flag2) Joystick[1].sendState();
  flag1 = 0;
  flag2 = 0;

}

#define latchlow digitalWrite(LATCH1, LOW); digitalWrite(LATCH2, LOW);
#define latchhigh digitalWrite(LATCH1, HIGH); digitalWrite(LATCH2, HIGH)
#define clocklow digitalWrite(CLOCK1, LOW); digitalWrite(CLOCK2, LOW)
#define clockhigh digitalWrite(CLOCK1, HIGH); digitalWrite(CLOCK2, HIGH)
#define wait delayMicroseconds(12)

void ReadNESjoy() {
  latchlow;
  clocklow;
  latchhigh;
  wait;
  latchlow;
 
  for (int i = 0; i < 8; i++) {
     newStatusPort1[i] = digitalRead(DATA1);
     newStatusPort2[i] = digitalRead(DATA2);
     clockhigh;
     wait;
     clocklow;
     wait;
  }
}
