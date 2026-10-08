/*
  Suzo Prof 9000 De Luxe

  https://docs.arduino.cc/retired/hacking/software/PortManipulation/
  https://github.com/mheironimus/arduinojoysticklibrary
  https://github.com/MHeironimus/ArduinoJoystickLibrary/archive/master.zip

  https://www.msx.org/wiki/Joystick_control
  https://allpinouts.org/pinouts/connectors/input_device/joystick-msx-9-pin/
  https://repairbas.file-hunter.com/new-doc/nederlands_BAS%2304_Arcade%20joysticks%20repareren.pdf
  https://generation-msx.nl/hardware/suzo/suzo-prof-competition-9000-deluxe/1185
*/

#include <Joystick.h>

//Joystick_ Joystick;
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_GAMEPAD,
  2,     // button count
  1,     // hat switch count
  true, true, // X and Y axes
  false, false, false, // Z, Rx, Ry
  false, false, // Rz, rudder
  false, false, false); // throttle, accelerator, brake, steering

#define PIN_UP    21  // DB9(1), PF4
#define PIN_DOWN  20  // DB9(2), PF5
#define PIN_LEFT  19  // DB9(3), PF6
#define PIN_RIGHT 18  // DB9(4), PF7
#define PIN_5V    15  // DB9(5), PB1
#define PIN_B1    14  // DB9(6), PB3 // autofire
#define PIN_B2    16  // DB9(7), PB2
#define PIN_OUTPUT 8  // DB9(8), PB4
#define PIN_GND    9  // DB9(9), PB5

void setup() {

  //Serial.begin(115200);
  Joystick.begin();

  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_5V, OUTPUT);
  pinMode(PIN_B1, INPUT_PULLUP);
  pinMode(PIN_B2, INPUT_PULLUP);
  pinMode(PIN_OUTPUT, OUTPUT);
  pinMode(PIN_GND, OUTPUT);

  digitalWrite(PIN_5V, HIGH);
  digitalWrite(PIN_OUTPUT, LOW);
  digitalWrite(PIN_GND, LOW);

}

void loop() {
  uint8_t I1 = PINF;
  uint8_t I2 = PINB;

  if (((I1 >> 4)&1)^1) Joystick.setYAxis(0);
  else if (((I1 >> 5)&1)^1) Joystick.setYAxis(1023);
  else Joystick.setYAxis(512);
  if (((I1 >> 6)&1)^1) Joystick.setXAxis(0);
  else if (((I1 >> 7)&1)^1) Joystick.setXAxis(1023);
  else Joystick.setXAxis(512);

  Joystick.setButton(0, (((I2 >> 3)&1)^1));
  Joystick.setButton(1, (((I2 >> 2)&1)^1));

  delay(10);

  //Serial.print(PINF, BIN);
  //Serial.print(", ");
  //Serial.println(PINB, BIN);

}
