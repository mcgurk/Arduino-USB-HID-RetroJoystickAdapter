/*
  Suzo Prof 9000 De Luxe

  http://qqtrading.com.my/image/catalog/Products/Arduino/pro-micro/pro_micro_pinout_v1_0_blue.jpg
  https://docs.arduino.cc/retired/hacking/software/PortManipulation/
  https://github.com/dmadison/ArduinoXInput_AVR
  https://github.com/dmadison/ArduinoXInput

  https://www.msx.org/wiki/Joystick_control
  https://allpinouts.org/pinouts/connectors/input_device/joystick-msx-9-pin/
  https://repairbas.file-hunter.com/new-doc/nederlands_BAS%2304_Arcade%20joysticks%20repareren.pdf
  https://generation-msx.nl/hardware/suzo/suzo-prof-competition-9000-deluxe/1185
*/

#include <XInput.h>

#define PIN_UP    A3      // DB9(1), (21), PF4
#define PIN_DOWN  A2      // DB9(2), (20), PF5
#define PIN_LEFT  A1      // DB9(3), (19), PF6
#define PIN_RIGHT A0      // DB9(4), (18), PF7
#define PIN_5V    7 //15      // DB9(5), PB1
#define PIN_BTN1  3 //14      // DB9(6), PB3 // autofire
#define PIN_BTN2  4 //16      // DB9(7), PB2
#define PIN_OUTPUT 6 //8      // DB9(8), PB4
#define PIN_GND    5 //9      // DB9(9), PB5
#define PIN_OPT1   0      // jumper 0<->GND, PD2 -> separate fires
#define PIN_OPT2   2      // jumper 2<->GND, PD1 -> separate fires, swapped buttons
//#define PIN_LED   30  // TX led (invert), PD5
//#define PIN_LED   17      // RX led (invert), PB0

#define STATE_UP !(*portInputRegister(digitalPinToPort(PIN_UP)) & digitalPinToBitMask(PIN_UP)) // (((PINF >> 4)&1)^1)
#define STATE_DOWN !(*portInputRegister(digitalPinToPort(PIN_DOWN)) & digitalPinToBitMask(PIN_DOWN)) // (((PINF >> 5)&1)^1)
#define STATE_LEFT !(*portInputRegister(digitalPinToPort(PIN_LEFT)) & digitalPinToBitMask(PIN_LEFT)) // (((PINF >> 6)&1)^1)
#define STATE_RIGHT !(*portInputRegister(digitalPinToPort(PIN_RIGHT)) & digitalPinToBitMask(PIN_RIGHT)) // (((PINF >> 7)&1)^1)
#define STATE_BTN1 !(*portInputRegister(digitalPinToPort(PIN_BTN1)) & digitalPinToBitMask(PIN_BTN1)) // (((PINB >> 3)&1)^1)
#define STATE_BTN2 !(*portInputRegister(digitalPinToPort(PIN_BTN2)) & digitalPinToBitMask(PIN_BTN2)) // (((PINB >> 2)&1)^1)
#define STATE_OPT1 !(*portInputRegister(digitalPinToPort(PIN_OPT1)) & digitalPinToBitMask(PIN_OPT1)) // (((PIND >> 2)&1)^1)
#define STATE_OPT2 !(*portInputRegister(digitalPinToPort(PIN_OPT2)) & digitalPinToBitMask(PIN_OPT2)) // (((PIND >> 1)&1)^1)

// Output wiring, PORT B, OUTPUTS
#define BIT_LED digitalPinToBitMask(17)       // PB0 (RX led (inverted))

// Output wiring
#define BIT_UP digitalPinToBitMask(15)        // PB1, DB9: sininen 1
#define BIT_DOWN digitalPinToBitMask(14)      // PB3, DB9: vihreä 2
#define BIT_LEFT digitalPinToBitMask(16)      // PB2, DB9: harmaa 3
#define BIT_RIGHT digitalPinToBitMask(10)     // PB6, DB9: purppura 4
#define BIT_FIRE1 digitalPinToBitMask(9)      // PB5, DB9: keltanen 6
#define BIT_FIRE2 digitalPinToBitMask(8)      // PB4, DB9: ruskea 9 (huom: kytkee 5V:hen!)
                                              // 5V,  DB9: punainen 7
                                              // GND, DB9: musta 8

void setup() {
  //Serial.begin(115200);
  XInput.setAutoSend(false);  // Wait for all controls before sending
  XInput.begin(); // -32768..32767

  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_5V, OUTPUT);
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  pinMode(PIN_OUTPUT, OUTPUT);
  pinMode(PIN_GND, OUTPUT);
  pinMode(PIN_OPT1, INPUT_PULLUP);
  pinMode(PIN_OPT2, INPUT_PULLUP);

  digitalWrite(PIN_5V, HIGH);
  digitalWrite(PIN_OUTPUT, LOW);
  digitalWrite(PIN_GND, LOW);

  //pinMode(PIN_LED, OUTPUT);
  //digitalWrite(PIN_LED, HIGH);
  DDRB = BIT_LED | BIT_FIRE2;  // 0 = input/highz, 1 = output, clears other bits
  PORTB = BIT_LED; // 0 = low/no pullup, 1 = high/with pullup, clears other bits
}


void loop() {
  /*int16_t X = 0;
  int16_t Y = 0;
  if (STATE_UP) Y = 32767;
  	else if (STATE_DOWN) Y = -32768;
  		else Y = 0;
  if (STATE_LEFT) X = -32768;
  	else if (STATE_RIGHT) X = 32767;
  		else X = 0;
  XInput.setJoystick(JOY_LEFT,  X, Y);*/

  XInput.setDpad(STATE_UP, STATE_DOWN, STATE_LEFT, STATE_RIGHT);

  if (!STATE_OPT1 && !STATE_OPT2) {
      XInput.setButton(BUTTON_A, STATE_BTN1 | STATE_BTN2);
  }
  if (STATE_OPT1) {
      XInput.setButton(BUTTON_A, STATE_BTN1); // autofire
      XInput.setButton(BUTTON_B, STATE_BTN2);
  }
  if (STATE_OPT2) {
      XInput.setButton(BUTTON_B, STATE_BTN1); // autofire
      XInput.setButton(BUTTON_A, STATE_BTN2);
  }

  XInput.send();

  if (STATE_BTN1) // for autofire speed indication led
    //digitalWrite(PIN_LED, LOW); 
    PORTB &= ~BIT_LED;
  else
    //digitalWrite(PIN_LED, HIGH);
    PORTB |= BIT_LED;

  uint8_t shadow_DDRB = BIT_LED | BIT_FIRE2; // keep BIT_LED and BIT_FIRE2 as output
  if (STATE_UP) shadow_DDRB |= BIT_UP;
  if (STATE_DOWN) shadow_DDRB |= BIT_DOWN;
  if (STATE_LEFT) shadow_DDRB |= BIT_LEFT;
  if (STATE_RIGHT) shadow_DDRB |= BIT_RIGHT;
  if (STATE_BTN1) shadow_DDRB |= BIT_FIRE1;
  DDRB = shadow_DDRB;

  //delay(10);
  //Serial.print(PINF, BIN); Serial.print(", "); Serial.println(PINB, BIN);
}
