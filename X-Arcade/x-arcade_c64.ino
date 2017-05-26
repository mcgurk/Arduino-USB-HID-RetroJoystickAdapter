 
#include <PS2KeyRaw.h>

#define DATAPIN 4
#define IRQPIN  3

//#define DEBUG

#define oUP1 5
#define oDOWN1 6
#define oLEFT1 7
#define oRIGHT1 8
#define oFIRE1 9

#define oUP2 A0
#define oDOWN2 15
#define oLEFT2 14
#define oRIGHT2 16
#define oFIRE2 10

#define UP1 0x75
#define DOWN1 0x72
#define LEFT1 0x6B
#define RIGHT1 0x74
#define START1 0x16
#define SELECT1 0x26
#define A1 0x12
#define B1 0x1A
#define C1 0x22
#define X1 0x14
#define Y1 0x11
#define Z1 0x29
#define L1 0x21
#define R1 0x2E

#define UP2 0x2D
#define DOWN2 0x2B
#define LEFT2 0x23
#define RIGHT2 0x34
#define START2 0x1E
#define SELECT2 0x25
#define A2 0x1D
#define B2 0x24
#define C2 0x54
#define X2 0x1C
#define Y2 0x1B
#define Z2 0x15
#define L2 0x5B
#define R2 0x36

uint8_t s1 = 0;
uint8_t s2 = 0;
uint8_t st1 = 0;
uint8_t st2 = 0;
uint8_t commando = 0;

PS2KeyRaw keyboard;

void setup() {
  keyboard.begin(DATAPIN, IRQPIN);
  Serial.begin(115200);
  Serial.println( "X-Arcade -> Atari -adapter" );

  pinMode(oUP1, INPUT);
  pinMode(oDOWN1, INPUT);
  pinMode(oLEFT1, INPUT);
  pinMode(oRIGHT1, INPUT);
  pinMode(oFIRE1, INPUT);

  pinMode(oUP2, INPUT);
  pinMode(oDOWN2, INPUT);
  pinMode(oLEFT2, INPUT);
  pinMode(oRIGHT2, INPUT);
  pinMode(oFIRE2, INPUT);

}

void loop() {
  
  if (keyboard.available()) {
    // read the next key
    int c = keyboard.read();
    uint8_t j;

    if (c == 0xF0) {
      #ifdef DEBUG
      Serial.print("0x"); Serial.print(c, HEX); Serial.print(" ");
      #endif
      while (!keyboard.available()) {}
      c = keyboard.read();
      #ifdef DEBUG
      Serial.print("0x"); Serial.println(c, HEX); 
      #endif
      clearDataC64(c);
    } else {
      #ifdef DEBUG
      Serial.print("0x"); Serial.println(c, HEX); 
      #endif
      setDataC64(c);
    }

    if (s1 && s2) commando = 1;
    if (st1 && st2) commando = 0;

  }

}


#define SET64(p) pinMode(p, OUTPUT); break;
#define UNSET64(p) pinMode(p, INPUT); break;

inline void setDataC64(uint8_t c) {
  switch (c) {
    case SELECT1:
      s1 = 1;
      break;
    case START1:
      st1 = 1;
      break;
    case A1:
    case B1:
      SET64(oFIRE1);
    case X1:
    case Y1:
      if (commando) {
        SET64(oFIRE2);
      } else {
        SET64(oFIRE1);
      }
    case C1:
      SET64(oDOWN1);
    case Z1:
      SET64(oUP1);
    case L1:
      SET64(oLEFT1);
    case R1:
      SET64(oRIGHT1);
    case UP1:
      SET64(oUP1);
    case DOWN1:
      SET64(oDOWN1);
    case LEFT1:
      SET64(oLEFT1);
    case RIGHT1:
      SET64(oRIGHT1);
    case SELECT2:
      s2 = 1;
      break;
    case START2:
      st2 = 1;
      break;
    case A2:
    case B2:
      SET64(oFIRE2);
    case X2:
    case Y2:
      if (commando) {
        SET64(oFIRE1);
      } else {
        SET64(oFIRE2);
      }
    case C2:
      SET64(oDOWN2);
    case Z2:
      SET64(oUP2);
    case L2:
      SET64(oLEFT2);
    case R2:
      SET64(oRIGHT2);
    case UP2:
      SET64(oUP2);
    case DOWN2:
      SET64(oDOWN2);
    case LEFT2:
      SET64(oLEFT2);
    case RIGHT2:
      SET64(oRIGHT2);
  }
}



inline void clearDataC64(uint8_t c) {
  switch (c) {
    case SELECT1:
      s1 = 0;
      break;
    case START1:
      st1 = 0;
      break;
    case A1:
    case B1:
      UNSET64(oFIRE1);
    case X1:
    case Y1:
      if (commando) {
        UNSET64(oFIRE2);
      } else{
        UNSET64(oFIRE1);
      }
    case C1:
      UNSET64(oDOWN1);
    case Z1:
      UNSET64(oUP1);
    case L1:
      UNSET64(oLEFT1);
    case R1:
      UNSET64(oRIGHT1);
    case UP1:
      UNSET64(oUP1);
    case DOWN1:
      UNSET64(oDOWN1);
    case LEFT1:
      UNSET64(oLEFT1);
    case RIGHT1:
      UNSET64(oRIGHT1);
    case SELECT2:
      s2 = 0;
      break;
    case START2:
      st2 = 0;
      break;
    case A2:
    case B2:
      UNSET64(oFIRE2);
    case X2:
    case Y2:
      if (commando) {
        UNSET64(oFIRE1);
      } else {
        UNSET64(oFIRE2);
      }
    case C2:
      UNSET64(oDOWN2);
    case Z2:
      UNSET64(oUP2);
    case L2:
      UNSET64(oLEFT2);
    case R2:
      UNSET64(oRIGHT2);
    case UP2:
      UNSET64(oUP2);
    case DOWN2:
      UNSET64(oDOWN2);
    case LEFT2:
      UNSET64(oLEFT2);
    case RIGHT2:
      UNSET64(oRIGHT2);
  }
}

