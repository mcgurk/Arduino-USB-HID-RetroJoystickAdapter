#include "Joystick2.h"

/*
#define ATARI
//DB9 (8=GND):                      1   2   3   4   5   6   7   8   9
const uint8_t inputPinsPort1[] =  {10, 16, 14, 15, A1,  0,  0,  0,  3};
const uint8_t inputPinsPort2[] =  { 5,  6,  7,  8, A2,  0,  0,  0,  4};
*/

/*
//#define NES
#define SNES
//Connector (Connect also GND and 5V):  CUP, OUT0,     D1
const uint8_t inputPinsPort1[] =       {  7,     8,     9 };
const uint8_t inputPinsPort2[] =       {  2,     3,     4 };
*/

/*
#define GENESIS_3
//#define GENESIS_6
//DB9 (8=GND, 5=VCC):               1   2   3   4   5   6   7   8   9
const uint8_t inputPinsPort1[] =  {10, 16, 14, 15, A0,  3, A3,  0, A1};
const uint8_t inputPinsPort2[] =  { 5,  6,  7,  8,  9,  4,  2,  0, A2};
*/

#define PSX
//PSX:                             DATA  CMD  ATT  CLK
const uint8_t inputPinsPort1[] =  {   2,   3,   4,   5 };
const uint8_t inputPinsPort2[] =  {   6,   7,   8,   9 };



//#define KONAMI


uint8_t lastStatusPort1[20]; //4 directions + 16 buttons
uint8_t newStatusPort1[20];
uint8_t lastStatusPort2[20];
uint8_t newStatusPort2[20];
void KonamiCode(uint8_t j, uint8_t swap_ab = 0);

//-----PSX/PLAYSTATION (DualShock and Analog not supported)-----
//http://playground.arduino.cc/Main/PSXLibrary
#ifdef PSX

#define DATA1 inputPinsPort1[0]
#define CMD1 inputPinsPort1[1]
#define ATT1 inputPinsPort1[2]
#define CLK1 inputPinsPort1[3]

#define DATA2 inputPinsPort2[0]
#define CMD2 inputPinsPort2[1]
#define ATT2 inputPinsPort2[2]
#define CLK2 inputPinsPort2[3]

//#define EVENTS_TOTAL 4+4+2+4 //4 directions, 2 fire-buttons, select+start, 4 shoulder buttons
#define BITS 16
#define EVENTS_TOTAL BITS

uint8_t p;

byte shift(byte _dataOut, uint8_t readmode = 0) { // Does the actual shifting, both in and out
  boolean _temp = 0;
  byte _dataIn = 0;
  byte _delay = 10;
  for (uint8_t i = 0; i <= 7; i++) {
    if ( _dataOut & (1 << i) ) {
      digitalWrite(CMD1, HIGH); // Writes out the _dataOut bits
      digitalWrite(CMD2, HIGH);
    } else {
      digitalWrite(CMD1, LOW);
      digitalWrite(CMD2, LOW);
    }
    digitalWrite(CLK1, LOW);
    digitalWrite(CLK2, LOW);
    delayMicroseconds(_delay);
    if (readmode) {
      newStatusPort1[p] = digitalRead(DATA1);          // Reads the data pin
      newStatusPort2[p] = digitalRead(DATA2);
      p++;
    }
    digitalWrite(CLK1, HIGH);
    digitalWrite(CLK2, HIGH);
    delayMicroseconds(_delay);
  }
}

void setupJoysticks() {
  pinMode(DATA1, INPUT_PULLUP);
  pinMode(CMD1, OUTPUT);
  pinMode(ATT1, OUTPUT);
  pinMode(CLK1, OUTPUT);

  pinMode(DATA2, INPUT_PULLUP);
  pinMode(CMD2, OUTPUT);
  pinMode(ATT2, OUTPUT);
  pinMode(CLK2, OUTPUT);
}

void readJoysticks() {
  p = 0;
  
  digitalWrite(ATT1, LOW);
  digitalWrite(ATT2, LOW);
  shift(0x01);
  shift(0x42);
  shift(0xFF);

  shift(0xFF, 1); //read
  shift(0xFF, 1);

  digitalWrite(ATT1, HIGH);
  digitalWrite(ATT2, HIGH);

  delayMicroseconds(1000);
}

void interpretJoystickState(uint8_t j, uint8_t *status) {
  Joystick[j].setYAxis(0);
  Joystick[j].setXAxis(0);
  if (!status[4]) Joystick[j].setYAxis(-127); //UP
  if (!status[6]) Joystick[j].setYAxis(127); //DOWN
  if (!status[7]) Joystick[j].setXAxis(-127); //LEFT
  if (!status[5]) Joystick[j].setXAxis(127); //RIGHT
  Joystick[j].setButton(0, !status[3]); //BUTTON1 (Start)
  Joystick[j].setButton(1, !status[0]); //BUTTON2 (Select)
  Joystick[j].setButton(2, !status[14]); //BUTTON3 (A)
  Joystick[j].setButton(3, !status[13]); //BUTTON4 (B)
  Joystick[j].setButton(4, !status[15]); //BUTTON5 (X)
  Joystick[j].setButton(5, !status[12]); //BUTTON6 (Y)
  Joystick[j].setButton(6, !status[8]); //BUTTON7 (LB)  
  Joystick[j].setButton(7, !status[9]); //BUTTON8 (RB)
  Joystick[j].setButton(8, !status[10]); //BUTTON9 (LT)
  Joystick[j].setButton(9, !status[11]); //BUTTON10 (RT)
}
#endif


//--------ATARI/SMS--------
#ifdef ATARI

#define EVENTS_TOTAL 4+2 //4 directions, 2 fire-buttons

void setupJoysticks() {
  for (int i=0; i < 9; i++) {
    pinMode(inputPinsPort1[i], INPUT_PULLUP);
    pinMode(inputPinsPort2[i], INPUT_PULLUP);
  }
}

void readJoysticks() {
  for (uint8_t i=0; i < 4; i++) {
    newStatusPort1[i] = digitalRead(inputPinsPort1[i]); //AXES1
    newStatusPort2[i] = digitalRead(inputPinsPort2[i]); //AXES2
  }
  newStatusPort1[4] = digitalRead(inputPinsPort1[8]); //A1
  newStatusPort1[5] = digitalRead(inputPinsPort1[4]); //B1
  newStatusPort2[4] = digitalRead(inputPinsPort2[8]); //A2
  newStatusPort2[5] = digitalRead(inputPinsPort2[4]); //B2
  delayMicroseconds(1000);
}

void interpretJoystickState(uint8_t j, uint8_t *status) {
  Joystick[j].setYAxis(0);
  Joystick[j].setXAxis(0);
  if (!status[0]) Joystick[j].setYAxis(-127); //UP
  if (!status[1]) Joystick[j].setYAxis(127); //DOWN
  if (!status[2]) Joystick[j].setXAxis(-127); //LEFT
  if (!status[3]) Joystick[j].setXAxis(127); //RIGHT
  Joystick[j].setButton(2, !status[4]); //BUTTON3 (A)
  Joystick[j].setButton(3, !status[5]); //BUTTON4 (B)
}
#endif


//--------Megadrive/Genesis (6-button)--------
//https://www.cs.cmu.edu/~chuck/infopg/segasix.txt
#if defined(GENESIS_3) || defined(GENESIS_6)

#define MODE_SELECT_PORT1 inputPinsPort1[6]
#define MODE_SELECT_PORT2 inputPinsPort2[6]
#define VCC_PORT1 inputPinsPort1[4]
#define VCC_PORT2 inputPinsPort2[4]

#ifdef GENESIS_3
#define EVENTS_TOTAL 4+3+1 //4 directions, 3 fire-buttons and Start
#else
#define EVENTS_TOTAL 4+6+1 //4 directions, 6 fire-buttons and Start
#endif

void modeSelect(uint8_t m) {
  digitalWrite(MODE_SELECT_PORT1, m);
  digitalWrite(MODE_SELECT_PORT2, m);
  delayMicroseconds(20);
}

void setupJoysticks() {
  if (VCC_PORT1 != 0) {
    pinMode(VCC_PORT1, OUTPUT);
    digitalWrite(VCC_PORT1, HIGH);
  }
  if (VCC_PORT2 != 0) {
    pinMode(VCC_PORT2, OUTPUT);
    digitalWrite(VCC_PORT2, HIGH);
  }

  const uint8_t inputlist[] = {0,1,2,3,5,8};
  for (int i=0; i < 6; i++) {
    pinMode(inputPinsPort1[inputlist[i]], INPUT);
    pinMode(inputPinsPort2[inputlist[i]], INPUT);
  }
  
  pinMode(MODE_SELECT_PORT1, OUTPUT);
  pinMode(MODE_SELECT_PORT2, OUTPUT);
  modeSelect(HIGH);
}

void readJoysticks() {
  modeSelect(LOW);
  
  newStatusPort1[4] = digitalRead(inputPinsPort1[5]); //A1
  newStatusPort1[5] = digitalRead(inputPinsPort1[8]); //Start1
  newStatusPort2[4] = digitalRead(inputPinsPort2[5]); //A2
  newStatusPort2[5] = digitalRead(inputPinsPort2[8]); //Start2  

  modeSelect(HIGH);

  for (uint8_t i=0; i < 4; i++) {
    newStatusPort1[i] = digitalRead(inputPinsPort1[i]); //AXES1
    newStatusPort2[i] = digitalRead(inputPinsPort2[i]); //AXES2
  }
  newStatusPort1[6] = digitalRead(inputPinsPort1[5]); //B1
  newStatusPort1[7] = digitalRead(inputPinsPort1[8]); //C1
  newStatusPort2[6] = digitalRead(inputPinsPort2[5]); //B2
  newStatusPort2[7] = digitalRead(inputPinsPort2[8]); //C2

  #ifdef GENESIS_6
  //read X,Y,Z
  modeSelect(LOW);
  modeSelect(HIGH);
  modeSelect(LOW);
  modeSelect(HIGH);
  newStatusPort1[8] = digitalRead(inputPinsPort1[2]); //X1
  newStatusPort1[9] = digitalRead(inputPinsPort1[1]); //Y1
  newStatusPort1[10] = digitalRead(inputPinsPort1[0]); //Z1
  newStatusPort2[8] = digitalRead(inputPinsPort2[2]); //X2
  newStatusPort2[9] = digitalRead(inputPinsPort2[1]); //Y2
  newStatusPort2[10] = digitalRead(inputPinsPort2[0]); //Z2
  #endif
  
  delayMicroseconds(1000);
}

void interpretJoystickState(uint8_t j, uint8_t *status) {

  #ifdef KONAMI
  if (!status[8] && !status[9] && !status[10] && !status[0]) { //X+Y+Z+UP
    KonamiCode(0,0);
    return;
  }
  if (!status[8] && !status[9] && !status[10] && !status[1]) { //X+Y+Z+DOWN
    KonamiCode(0,1);
    return;
  }
  #endif

  Joystick[j].setYAxis(0);
  Joystick[j].setXAxis(0);
  if (!status[0]) Joystick[j].setYAxis(-127); //UP
  if (!status[1]) Joystick[j].setYAxis(127); //DOWN
  if (!status[2]) Joystick[j].setXAxis(-127); //LEFT
  if (!status[3]) Joystick[j].setXAxis(127); //RIGHT
  Joystick[j].setButton(0, !status[5]); //BUTTON1 (START)
  Joystick[j].setButton(1, !status[7]); //BUTTON2 (C) ("Select")
  Joystick[j].setButton(2, !status[4]); //BUTTON3 (A)
  Joystick[j].setButton(3, !status[6]); //BUTTON4 (B)
  #ifdef GENESIS_6
  Joystick[j].setButton(4, !status[8]); //BUTTON5 (X)
  Joystick[j].setButton(5, !status[9]); //BUTTON6 (Y)
  Joystick[j].setButton(6, !status[10]); //BUTTON7 (Z) ("LB")
  #endif
}

#endif


//--------NES--------
// http://www.mit.edu/~tarvizo/nes-controller.html
#if defined(NES) || defined(SNES)

#define CLOCK1 inputPinsPort1[0]
#define LATCH1 inputPinsPort1[1]
#define DATA1 inputPinsPort1[2]

#define CLOCK2 inputPinsPort2[0]
#define LATCH2 inputPinsPort2[1]
#define DATA2 inputPinsPort2[2]

#ifdef NES
#define BITS 8
#else
#define BITS 16
#endif

#define EVENTS_TOTAL BITS

void setupJoysticks() {
  pinMode(LATCH1, OUTPUT);
  pinMode(CLOCK1, OUTPUT);
  pinMode(DATA1, INPUT);
  pinMode(LATCH2, OUTPUT);
  pinMode(CLOCK2, OUTPUT);
  pinMode(DATA2, INPUT);
}

#define latchlow digitalWrite(LATCH1, LOW); digitalWrite(LATCH2, LOW);
#define latchhigh digitalWrite(LATCH1, HIGH); digitalWrite(LATCH2, HIGH)
#define clocklow digitalWrite(CLOCK1, LOW); digitalWrite(CLOCK2, LOW)
#define clockhigh digitalWrite(CLOCK1, HIGH); digitalWrite(CLOCK2, HIGH)
#define wait delayMicroseconds(12)


void readJoysticks() {
  latchlow;
  clocklow;
  latchhigh;
  wait;
  latchlow;

  for (int i = 0; i < BITS; i++) {
    newStatusPort1[i] = digitalRead(DATA1);
    newStatusPort2[i] = digitalRead(DATA2);
    clockhigh;
    wait;
    clocklow;
    wait;
  }
}

void interpretJoystickState(uint8_t j, uint8_t *status) {

  #ifdef KONAMI
  if (!status[1] && !status[9] && !status[11] && !status[4]) { //X+Y+Z+UP
    KonamiCode(0,0);
    return;
  }
  if (!status[1] && !status[9] && !status[11] && !status[5]) { //X+Y+Z+DOWN
    KonamiCode(0,1);
    return;
  }
  #endif
  
  Joystick[j].setYAxis(0);
  Joystick[j].setXAxis(0);
  if (!status[4]) Joystick[j].setYAxis(-127); //UP
  if (!status[5]) Joystick[j].setYAxis(127); //DOWN
  if (!status[6]) Joystick[j].setXAxis(-127); //LEFT
  if (!status[7]) Joystick[j].setXAxis(127); //RIGHT
  #ifdef NES
  Joystick[j].setButton(0, !status[3]); //BUTTON1 (Start)  
  Joystick[j].setButton(1, !status[2]); //BUTTON2 (Select)
  Joystick[j].setButton(2, !status[0]); //BUTTON3 (A)
  Joystick[j].setButton(3, !status[1]); //BUTTON4 (B)
  #else
  Joystick[j].setButton(0, !status[3]); //BUTTON1 (Start)  
  Joystick[j].setButton(1, !status[2]); //BUTTON2 (Select)
  Joystick[j].setButton(2, !status[8]); //BUTTON3 (A)
  Joystick[j].setButton(3, !status[0]); //BUTTON4 (B)
  Joystick[j].setButton(4, !status[9]); //BUTTON5 (X)
  Joystick[j].setButton(5, !status[1]); //BUTTON6 (Y)
  Joystick[j].setButton(6, !status[10]); //BUTTON7 (L1) ("LB")
  Joystick[j].setButton(7, !status[11]); //BUTTON8 (R1) ("RB")
  #endif
}

#endif



//--------Konami code---------

void releaseAll(uint8_t j) {
  delay(50);
  Joystick[j].setButton(0, 0); //BUTTON1 (Start)
  Joystick[j].setButton(1, 0); //BUTTON2 (Select)
  Joystick[j].setButton(2, 0); //BUTTON3 (A)
  Joystick[j].setButton(3, 0); //BUTTON4 (B)
  Joystick[j].setButton(4, 0); //BUTTON5 (X)
  Joystick[j].setButton(5, 0); //BUTTON6 (Y)
  Joystick[j].setButton(6, 0); //BUTTON7 (LB)
  Joystick[j].setButton(7, 0); //BUTTON6 (RB)
  Joystick[j].setButton(8, 0); //BUTTON7 (LT)
  Joystick[j].setButton(9, 0); //BUTTON6 (RT)
  Joystick[j].setButton(10, 0); //BUTTON7 (L-thumb)
  Joystick[j].setButton(11, 0); //BUTTON7 (R-thumb)
  Joystick[j].setYAxis(0);
  Joystick[j].setXAxis(0);
  Joystick[j].sendState();
  delay(50);
}

void KonamiCode(uint8_t j, uint8_t swap_ab = 0) {
  //https://en.wikipedia.org/wiki/Konami_Code
  //https://en.wikipedia.org/wiki/List_of_Konami_code_games
  //UP, UP, DOWN, DOWN, LEFT, RIGHT, LEFT, RIGHT, B, A
  releaseAll(j);
  delay(200);
  Joystick[j].setYAxis(-127); Joystick[j].sendState(); //UP
  releaseAll(j);
  Joystick[j].setYAxis(-127); Joystick[j].sendState(); //UP
  releaseAll(j);
  Joystick[j].setYAxis(127); Joystick[j].sendState(); //DOWN
  releaseAll(j);
  Joystick[j].setYAxis(127); Joystick[j].sendState(); //DOWN
  releaseAll(j);
  Joystick[j].setXAxis(-127); Joystick[j].sendState(); //LEFT
  releaseAll(j);
  Joystick[j].setXAxis(127); Joystick[j].sendState(); //RIGHT
  releaseAll(j);
  Joystick[j].setXAxis(-127); Joystick[j].sendState(); //LEFT
  releaseAll(j);
  Joystick[j].setXAxis(127); Joystick[j].sendState(); //RIGHT
  releaseAll(j);
  if (swap_ab) {
    Joystick[j].setButton(2, 1); Joystick[j].sendState(); //BUTTON1 (A)
    releaseAll(j);
    Joystick[j].setButton(3, 1); Joystick[j].sendState(); //BUTTON2 (B)
    releaseAll(j);
  } else {
    Joystick[j].setButton(3, 1); Joystick[j].sendState(); //BUTTON2 (B)
    releaseAll(j);
    Joystick[j].setButton(2, 1); Joystick[j].sendState(); //BUTTON1 (A)
    releaseAll(j);
  }
  delay(200);
}





void setup() {
  //clear statusarrays (1=OFF, 0=ON)
  for (uint8_t i = 0; i < EVENTS_TOTAL; i++) {
    lastStatusPort1[i] = 1;
    newStatusPort1[i] = 1;
    lastStatusPort2[i] = 1;
    newStatusPort2[i] = 1;
  }
  
  setupJoysticks();

  Joystick[0].begin(false);
  Joystick[1].begin(false);
}


uint8_t flag1 = 0;
uint8_t flag2 = 0;


void loop() {

  readJoysticks();

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
  
  if (flag1) interpretJoystickState(0, newStatusPort1);
  if (flag2) interpretJoystickState(1, newStatusPort2);

  //minimize port1 time advantage - try to send both states as simultaneous as possible
  if (flag1) Joystick[0].sendState();
  if (flag2) Joystick[1].sendState();
  flag1 = 0;
  flag2 = 0;

}

