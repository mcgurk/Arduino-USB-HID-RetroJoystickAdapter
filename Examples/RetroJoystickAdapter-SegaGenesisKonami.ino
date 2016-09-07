//Sega Megadrive/Genesis 6-button controller adapter by Kurg 2.9.2016
//Tested with Arduino Pro Micro (ATmega32U4, 5V) from Ebay ($4) 
//Selected board from Arduino IDE: Arduino Leonardo
//Joystick2 library from https://github.com/MHeironimus/ArduinoJoystickLibrary
//https://www.cs.cmu.edu/~chuck/infopg/segasix.txt

#include "Joystick2.h"

//DB9 pin (9-pin D-connector)
//Select low  Select high   3rd pulse
//1=Up        Up            Z
//2=Down      Down          Y
//3=Ground    Left          X
//4=Ground    Right
//5=+5V
//6=A         B
//7=Control("Select")
//8=Ground
//9=Start     C

#define EVENTS_TOTAL 4+6+1 //4 directions, 6 fire-buttons and Start
#define INPUT_PINS_TOTAL 6

//DB9 (port1):                      1   2   3   4   6   9
const uint8_t inputPinsPort1[] =  {10, 16, 14, 15,  3, A1};
#define VCC_PORT1 A0 //DB9 (port1) pin 5 //comment out if not connected to IO-pin
#define MODE_SELECT_PORT1 A3 //DB9 (port1) pin 7
//DB9 (port 2) pin 8 = GND

//DB9 (port2):                      1   2   3   4   6   9
const uint8_t inputPinsPort2[] =  { 5,  6,  7,  8,  4, A2};
#define VCC_PORT2 9 //DB9 (port 2) pin 5 //comment out if not connected to IO-pin
#define MODE_SELECT_PORT2 2 //DB9 (port2) pin 7
//DB9 (port 2) pin 8 = GND

uint8_t lastStatusPort1[EVENTS_TOTAL];
uint8_t newStatusPort1[EVENTS_TOTAL];
uint8_t lastStatusPort2[EVENTS_TOTAL];
uint8_t newStatusPort2[EVENTS_TOTAL];


void modeSelect(uint8_t m) {
  digitalWrite(MODE_SELECT_PORT1, m);
  digitalWrite(MODE_SELECT_PORT2, m);
  delayMicroseconds(20);
}

void releaseAll(uint8_t j) {
  delay(20);
  Joystick[j].setButton(0, 0); //BUTTON1
  Joystick[j].setButton(1, 0); //BUTTON2
  Joystick[j].setButton(2, 0); //BUTTON3
  Joystick[j].setButton(3, 0); //BUTTON4
  Joystick[j].setButton(4, 0); //BUTTON5
  Joystick[j].setButton(5, 0); //BUTTON6
  Joystick[j].setButton(6, 0); //BUTTON7
  Joystick[j].setYAxis(0);
  Joystick[j].setXAxis(0);
  Joystick[j].sendState();
  delay(20);
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
    Joystick[j].setButton(0, 1); Joystick[j].sendState(); //BUTTON1 (A)
    releaseAll(j);
    Joystick[j].setButton(1, 1); Joystick[j].sendState(); //BUTTON2 (B)
    releaseAll(j);
  } else {
    Joystick[j].setButton(1, 1); Joystick[j].sendState(); //BUTTON2 (B)
    releaseAll(j);
    Joystick[j].setButton(0, 1); Joystick[j].sendState(); //BUTTON1 (A)
    releaseAll(j);
  }
  delay(100);
}


void setup() {

  //clear statusarrays (1=OFF, 0=ON)
  for (uint8_t i = 0; i < EVENTS_TOTAL; i++) {
    lastStatusPort1[i] = 1;
    newStatusPort1[i] = 1;
    lastStatusPort2[i] = 1;
    newStatusPort2[i] = 1;
  }

  #if defined(VCC_PORT1) && defined(VCC_PORT2)
  pinMode(VCC_PORT1, OUTPUT);
  pinMode(VCC_PORT2, OUTPUT);
  digitalWrite(VCC_PORT1, HIGH);
  digitalWrite(VCC_PORT2, HIGH);
  #endif

  for (int i=0; i < INPUT_PINS_TOTAL; i++) {
    pinMode(inputPinsPort1[i], INPUT_PULLUP);
    pinMode(inputPinsPort2[i], INPUT_PULLUP);
  }
  
  pinMode(MODE_SELECT_PORT1, OUTPUT);
  pinMode(MODE_SELECT_PORT2, OUTPUT);
  modeSelect(HIGH);

  Joystick[0].begin(false);
  Joystick[1].begin(false);

}


void read3buttons() {
  
  modeSelect(LOW);
  
  newStatusPort1[4] = digitalRead(inputPinsPort1[4]); //A1
  newStatusPort1[7] = digitalRead(inputPinsPort1[5]); //Start1
  newStatusPort2[4] = digitalRead(inputPinsPort2[4]); //A2
  newStatusPort2[7] = digitalRead(inputPinsPort2[5]); //Start2  

  modeSelect(HIGH);

  for (uint8_t i=0; i < 4; i++) {
    newStatusPort1[i] = digitalRead(inputPinsPort1[i]); //AXES1
    newStatusPort2[i] = digitalRead(inputPinsPort2[i]); //AXES2
  }
  newStatusPort1[5] = digitalRead(inputPinsPort1[4]); //B1
  newStatusPort1[6] = digitalRead(inputPinsPort1[5]); //C1
  newStatusPort2[5] = digitalRead(inputPinsPort2[4]); //B2
  newStatusPort2[6] = digitalRead(inputPinsPort2[5]); //C2
  
}


uint8_t flag1 = 0;
uint8_t flag2 = 0;

void loop() {

  read3buttons();

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
  delayMicroseconds(1000);

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
    if (!newStatusPort1[0]) Joystick[0].setYAxis(-127); //UP
    if (!newStatusPort1[1]) Joystick[0].setYAxis(127); //DOWN
    if (!newStatusPort1[2]) Joystick[0].setXAxis(-127); //LEFT
    if (!newStatusPort1[3]) Joystick[0].setXAxis(127); //RIGHT
    Joystick[0].setButton(0, !newStatusPort1[4]); //BUTTON1
    Joystick[0].setButton(1, !newStatusPort1[5]); //BUTTON2
    Joystick[0].setButton(2, !newStatusPort1[6]); //BUTTON3
    Joystick[0].setButton(3, !newStatusPort1[7]); //BUTTON4
    Joystick[0].setButton(4, !newStatusPort1[8]); //BUTTON5
    Joystick[0].setButton(5, !newStatusPort1[9]); //BUTTON6
    Joystick[0].setButton(6, !newStatusPort1[10]); //BUTTON7
    Joystick[0].sendState();
    flag1 = 0;
    if (!newStatusPort1[4] && !newStatusPort1[5] && !newStatusPort1[6] && !newStatusPort1[0]) KonamiCode(0,0);
    if (!newStatusPort1[4] && !newStatusPort1[5] && !newStatusPort1[6] && !newStatusPort1[1]) KonamiCode(0,1);
  }
    
  if (flag2) {
    Joystick[1].setYAxis(0);
    Joystick[1].setXAxis(0);
    if (!newStatusPort2[0]) Joystick[1].setYAxis(-127); //UP
    if (!newStatusPort2[1]) Joystick[1].setYAxis(127); //DOWN
    if (!newStatusPort2[2]) Joystick[1].setXAxis(-127); //LEFT
    if (!newStatusPort2[3]) Joystick[1].setXAxis(127); //RIGHT
    Joystick[1].setButton(0, !newStatusPort2[4]); //BUTTON1
    Joystick[1].setButton(1, !newStatusPort2[5]); //BUTTON2
    Joystick[1].setButton(2, !newStatusPort2[6]); //BUTTON3
    Joystick[1].setButton(3, !newStatusPort2[7]); //BUTTON4
    Joystick[1].setButton(4, !newStatusPort2[8]); //BUTTON5
    Joystick[1].setButton(5, !newStatusPort2[9]); //BUTTON6
    Joystick[1].setButton(6, !newStatusPort2[10]); //BUTTON7
    Joystick[1].sendState();
    flag2 = 0;
    if (!newStatusPort2[4] && !newStatusPort2[5] && !newStatusPort2[6] && !newStatusPort2[0]) KonamiCode(0,0);
    if (!newStatusPort2[4] && !newStatusPort2[5] && !newStatusPort2[6] && !newStatusPort2[1]) KonamiCode(0,1);
  }

 
}
