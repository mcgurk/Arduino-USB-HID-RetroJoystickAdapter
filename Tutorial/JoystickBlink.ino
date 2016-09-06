#include "Joystick2.h"

void setup() {
  Joystick[0].begin(true);
}

void loop() {
  Joystick[0].setButton(0, 1);
  delay(1000);
  Joystick[0].setButton(0, 0);
  delay(1000);
}
