/*
 * Simple Atari joystick adapter (keyboard HID) for learning purposes
 * McGurk 6.9.2016
 */

#include <Keyboard.h>;
// List of special keys: https://www.arduino.cc/en/Reference/KeyboardModifiers

// Here we define Arduino pins we use. Along with these we have to connect joystick ground to Arduino GND.
// Because of Arduino internal pullup resistor off state is 1. Moving joystick switches corresponding pin to ground and then it is 0.
#define UP 2 // 9-pin D-connector pin 1
#define DOWN 3 // 9-pin D-connector pin 2
#define LEFT 4 // 9-pin D-connector pin 3
#define RIGHT 5 // 9-pin D-connector pin 4
#define BUTTON 6 // 9-pin D-connector pin 6
// 9-pin D-connector pin 8 to GND


// We keep record how everything was last time, so we can compare if anything have changed. (1 = off, 0 = on)
byte lastUP = 1;
byte lastDOWN = 1;
byte lastLEFT = 1;
byte lastRIGHT = 1;
byte lastBUTTON = 1;

byte newUP = 1;
byte newDOWN = 1;
byte newLEFT = 1;
byte newRIGHT = 1;
byte newBUTTON = 1;


void setup() {
  
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(BUTTON, INPUT_PULLUP);

  Keyboard.begin();

}

// We use this flag to indicate if anything changes.
byte flag = 0;


void loop()
{

  // Read joystick state (1 = off, 0 = on).
  newUP = digitalRead(UP);
  newDOWN = digitalRead(DOWN);
  newLEFT = digitalRead(LEFT);
  newRIGHT = digitalRead(RIGHT);
  newBUTTON = digitalRead(BUTTON);


  // Check if anything changed. This way we don't have to send anything to USB, if nothing changes.
  
  if (newUP != lastUP) {
    lastUP = newUP;
    flag = 1;
  }

  if (newDOWN != lastDOWN) {
    lastDOWN = newDOWN;
    flag = 1;
  }

  if (newLEFT != lastLEFT) {
    lastLEFT = newLEFT;
    flag = 1;
  }

  if (newRIGHT != lastRIGHT) {
    lastRIGHT = newRIGHT;
    flag = 1;
  }

  if (newBUTTON != lastBUTTON) {
    lastBUTTON = newBUTTON;
    flag = 1;
  }

  // If anythings changed, send new state.
  if (flag) {
    
    // Press or release keyboard button according what we read earlier.
    // We need exclamation mark in these if-conditions, because swithes are active low (0 = on, 1 = off).
    if (!newUP) {
      Keyboard.press('w'); 
    } else {
      Keyboard.release('w');
    }
    if (!newDOWN) {
      Keyboard.press('s');
    } else {
      Keyboard.release('s');
    }
    if (!newLEFT) {
      Keyboard.press('a');
    } else {
      Keyboard.release('a');
    }
    if (!newRIGHT) {
      Keyboard.press('d');
    } else {
      Keyboard.release('d');
    }
    if (!newBUTTON) {
      Keyboard.press('e');
    } else {
      Keyboard.release('e');
    }
    
    // Remember to clear flag.
    flag = 0;
    
  }
  
  // Little 1ms breathing break.
  delayMicroseconds(1000);
  
}
