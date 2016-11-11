//Uno: SDA = A4, SCL = A5
//Pro Micro: SDA = D2, SCL = D3
//ESP8266: SDA = D2, SCL = D1


#define DEBUG

#include <Wire.h>

#define JOYSTICK_DATA_SIZE 6
uint8_t data[JOYSTICK_DATA_SIZE];

#define ADDRESS 0x52

uint16_t type;

const uint8_t ident_nunchuck[] = { 0x00, 0x00, 0xA4, 0x20, 0x00, 0x00 };
const uint8_t ident_classic_controller[] = { 0x00, 0x00, 0xA4, 0x20, 0x01, 0x01 };
const uint8_t ident_classic_controller_pro[] = { 0x01, 0x00, 0xA4, 0x20, 0x00, 0x00 };

#define NUNCHUCK 1
#define CLASSIC_CONTROLLER 2
#define CLASSIC_CONTROLLER_PRO 3


void sendByte(uint8_t data, uint8_t location)  {  
  Wire.beginTransmission(ADDRESS);
  Wire.write(location);
  Wire.write(data);  
  Wire.endTransmission();
}

void sendByte(uint8_t data)  {  
  Wire.beginTransmission(ADDRESS);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t initExtension() {
  uint8_t buf[6];
  uint8_t type = CLASSIC_CONTROLLER; // default type
  sendByte(0x55, 0xF0);
  sendByte(0x00, 0xFB);
  sendByte(0xFA);
  delayMicroseconds(200);
  Wire.requestFrom(ADDRESS, 6);
  uint8_t i = 0;
  while(Wire.available()) {
    buf[i] = Wire.read();
    i++;
    if (i >= 6) break;
  }
  if (memcmp(buf, ident_nunchuck, 6) == 0) type = NUNCHUCK;
  if (memcmp(buf, ident_classic_controller, 6) == 0) type = CLASSIC_CONTROLLER;
  if (memcmp(buf, ident_classic_controller_pro, 6) == 0) type = CLASSIC_CONTROLLER_PRO;
  return type;

}

void setup() {
  Wire.begin();
  //Wire.setClock(400000L);
  //Wire.setClock(300000L);
  //Wire.setClock(200000L);
  //Wire.setClock(100000L);
  type = initExtension();

  Serial.begin(115200);
  Serial.println("start");
  Serial.println(type);
  Serial.flush();
}


void loop() {

  #ifdef DEBUG
  unsigned long t = micros();
  #endif

  sendByte(0x00);
  delayMicroseconds(200);
  Wire.requestFrom(ADDRESS, JOYSTICK_DATA_SIZE);
  uint8_t i = 0;
  while(Wire.available()) {
    data[i] = Wire.read();
    i++;
    if (i >= JOYSTICK_DATA_SIZE) break;
  }

  //detect if init is needed
  if (i < JOYSTICK_DATA_SIZE) {
    delay(10);
    type = initExtension();
    #ifdef DEBUG
    Serial.println("Init!");
    #endif
    delay(10);
  }

 
  // Nunchuck
  uint8_t SX = data[0];
  uint8_t SY = ~data[1];
  uint16_t AX = (data[2] << 2) | (data[5] >> 2 & B11);
  uint16_t AY = (data[3] << 2) | (data[5] >> 4 & B11);
  uint16_t AZ = (data[4] << 2) | (data[5] >> 6 & B11);
  uint8_t BC = ~data[5] >> 1 & 1;
  uint8_t BZ = ~data[5] & 1;
  //http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck

  // Classic Controller
  uint8_t LX = (data[0] & 0x3f);
  uint8_t LY = (data[1] & 0x3f);
  uint8_t RX = ((data[0] & 0xc0) >> 3) + ((data[1] & 0xc0) >> 5) +  ((data[2] & 0x80) >> 7);
  uint8_t RY = (data[2] & 0x1f);
  uint8_t BDU = ~data[5] & 1;
  uint8_t BDD = ~data[4] >> 6 & 1;
  uint8_t BDL = ~data[5] >> 1 & 1;
  uint8_t BDR = ~data[4] >> 7 & 1;
  uint8_t Bselect = ~data[4] >> 4 & 1;
  uint8_t BH = ~data[4] >> 3 & 1;
  uint8_t Bstart = ~data[4] >> 2 & 1;
  uint8_t BA = ~data[5] >> 4 & 1;
  uint8_t BB = ~data[5] >> 6 & 1;
  uint8_t BX = ~data[5] >> 3 & 1;
  uint8_t BY = ~data[5] >> 5 & 1;
  uint8_t BLT = ~data[4] >> 5 & 1;
  uint8_t BRT = ~data[4] >> 1 & 1;
  uint8_t BZL = ~data[5] >> 7 & 1;
  uint8_t BZR = ~data[5] >> 2 & 1;
  uint8_t LT = ((data[2] & 0x60) >> 2) + ((data[3] & 0xe0) >> 5);
  uint8_t RT = (data[3] & 0x1f);
  //http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Classic_Controller
  

  #ifdef DEBUG
  unsigned long d = micros()-t;
  Serial.print(d);
  #endif

  
  #ifdef DEBUG
  /*for (uint8_t i = 0; i < JOYSTICK_DATA_SIZE; i++) {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();*/
  Serial.print(" type:"); Serial.print(type, HEX);

  if (type == NUNCHUCK) {
    // Nunchuck
    Serial.print(" SX:"); Serial.print(SX);
    Serial.print(" SY:"); Serial.print(SY);
    Serial.print(" AX:"); Serial.print(AX);
    Serial.print(" AY:"); Serial.print(AY);
    Serial.print(" AZ:"); Serial.print(AZ);
    Serial.print(" BC:"); Serial.print(BC);
    Serial.print(" BZ:"); Serial.print(BZ);
  }

  if (type == CLASSIC_CONTROLLER || type == CLASSIC_CONTROLLER_PRO) {
    // Classic Controller
    Serial.print(" LX:"); Serial.print(LX);
    Serial.print(" LY:"); Serial.print(LY);
    Serial.print(" RX:"); Serial.print(RX);
    Serial.print(" RY:"); Serial.print(RY);
    Serial.print(" BDU:"); Serial.print(BDU);
    Serial.print(" BDD:"); Serial.print(BDD);
    Serial.print(" BDL:"); Serial.print(BDL);
    Serial.print(" BDR:"); Serial.print(BDR);
    Serial.print(" Bselect:"); Serial.print(Bselect);
    Serial.print(" BH:"); Serial.print(BH);
    Serial.print(" Bstart:"); Serial.print(Bstart);
    Serial.print(" BA:"); Serial.print(BA);
    Serial.print(" BB:"); Serial.print(BB);
    Serial.print(" BX:"); Serial.print(BX);
    Serial.print(" BY:"); Serial.print(BY);
    Serial.print(" BLT:"); Serial.print(BLT);
    Serial.print(" BRT:"); Serial.print(BRT);
    Serial.print(" BZL:"); Serial.print(BZL);
    Serial.print(" BZR:"); Serial.print(BZR);
    Serial.print(" LT:"); Serial.print(LT);
    Serial.print(" RT:"); Serial.print(RT);
  }
  
  Serial.println();
  #endif

  #ifdef DEBUG
  delay(100);
  #endif
  delayMicroseconds(1000);
}
