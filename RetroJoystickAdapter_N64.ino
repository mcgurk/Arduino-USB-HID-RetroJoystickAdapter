/**
 * Gamecube controller to Nintendo 64 adapter
 * by Andrew Brown
 * Rewritten for N64 to HID by Peter Den Hartog
 * Modified for Atmega32u4 USB-HID by Jarno Lehtinen
 */

/**
 * To use, hook up the following to the Arduino Duemilanove:
 * Digital I/O 2: N64 serial line
 * All appropriate grounding and power lines
 */

#include "Joystick.h"

//#define DEBUG

//USE PIN 4 FOR DATA PIN!
//USE ONLY 3.3V FOR VCC AND DATA!



#include "pins_arduino.h"

#define N64_PIN 4 //2
#define N64_PIN_DIR DDRD
// these two macros set arduino pin 2 to input or output, which with an
// external 1K pull-up resistor to the 3.3V rail, is like pulling it high or
// low.  These operations translate to 1 op code, which takes 2 cycles
#define N64_HIGH DDRD &= ~0x16 //0x04
#define N64_LOW DDRD |= 0x16 //0x04
#define N64_QUERY (PIND & 0x16)//0x04)

// 8 bytes of data that we get from the controller
struct {
    // bits: 0, 0, 0, start, y, x, b, a
    unsigned char data1;
    // bits: 1, L, R, Z, Dup, Ddown, Dright, Dleft
    unsigned char data2;
    char stick_x;
    char stick_y;
} N64_status;
char N64_raw_dump[33]; // 1 received bit per byte


void N64_send(unsigned char *buffer, char length);
void N64_get();
void print_N64_status();
void translate_raw_data();


/**
 * This CRC table for repeating bytes is take from
 * the cube64 project
 *  http://cia.vc/stats/project/navi-misc/cube64
 */
unsigned char crc_repeating_table[] = {
    0xFF, 0x14, 0xAC, 0x47, 0x59, 0xB2, 0x0A, 0xE1, 0x36,
    0xDD, 0x65, 0x8E, 0x90, 0x7B, 0xC3, 0x28, 0xE8, 0x03,
    0xBB, 0x50, 0x4E, 0xA5, 0x1D, 0xF6, 0x21, 0xCA, 0x72,
    0x99, 0x87, 0x6C, 0xD4, 0x3F, 0xD1, 0x3A, 0x82, 0x69,
    0x77, 0x9C, 0x24, 0xCF, 0x18, 0xF3, 0x4B, 0xA0, 0xBE,
    0x55, 0xED, 0x06, 0xC6, 0x2D, 0x95, 0x7E, 0x60, 0x8B,
    0x33, 0xD8, 0x0F, 0xE4, 0x5C, 0xB7, 0xA9, 0x42, 0xFA,
    0x11, 0xA3, 0x48, 0xF0, 0x1B, 0x05, 0xEE, 0x56, 0xBD,
    0x6A, 0x81, 0x39, 0xD2, 0xCC, 0x27, 0x9F, 0x74, 0xB4,
    0x5F, 0xE7, 0x0C, 0x12, 0xF9, 0x41, 0xAA, 0x7D, 0x96,
    0x2E, 0xC5, 0xDB, 0x30, 0x88, 0x63, 0x8D, 0x66, 0xDE,
    0x35, 0x2B, 0xC0, 0x78, 0x93, 0x44, 0xAF, 0x17, 0xFC,
    0xE2, 0x09, 0xB1, 0x5A, 0x9A, 0x71, 0xC9, 0x22, 0x3C,
    0xD7, 0x6F, 0x84, 0x53, 0xB8, 0x00, 0xEB, 0xF5, 0x1E,
    0xA6, 0x4D, 0x47, 0xAC, 0x14, 0xFF, 0xE1, 0x0A, 0xB2,
    0x59, 0x8E, 0x65, 0xDD, 0x36, 0x28, 0xC3, 0x7B, 0x90,
    0x50, 0xBB, 0x03, 0xE8, 0xF6, 0x1D, 0xA5, 0x4E, 0x99,
    0x72, 0xCA, 0x21, 0x3F, 0xD4, 0x6C, 0x87, 0x69, 0x82,
    0x3A, 0xD1, 0xCF, 0x24, 0x9C, 0x77, 0xA0, 0x4B, 0xF3,
    0x18, 0x06, 0xED, 0x55, 0xBE, 0x7E, 0x95, 0x2D, 0xC6,
    0xD8, 0x33, 0x8B, 0x60, 0xB7, 0x5C, 0xE4, 0x0F, 0x11,
    0xFA, 0x42, 0xA9, 0x1B, 0xF0, 0x48, 0xA3, 0xBD, 0x56,
    0xEE, 0x05, 0xD2, 0x39, 0x81, 0x6A, 0x74, 0x9F, 0x27,
    0xCC, 0x0C, 0xE7, 0x5F, 0xB4, 0xAA, 0x41, 0xF9, 0x12,
    0xC5, 0x2E, 0x96, 0x7D, 0x63, 0x88, 0x30, 0xDB, 0x35,
    0xDE, 0x66, 0x8D, 0x93, 0x78, 0xC0, 0x2B, 0xFC, 0x17,
    0xAF, 0x44, 0x5A, 0xB1, 0x09, 0xE2, 0x22, 0xC9, 0x71,
    0x9A, 0x84, 0x6F, 0xD7, 0x3C, 0xEB, 0x00, 0xB8, 0x53,
    0x4D, 0xA6, 0x1E, 0xF5 //0xFF
};

void setupJoysticks() {

  // Communication with gamecube controller on this pin
  // Don't remove these lines, we don't want to push +5V to the controller
  digitalWrite(N64_PIN, LOW);  
  pinMode(N64_PIN, INPUT);


  // Initialize the gamecube controller by sending it a null byte.
  // This is unnecessary for a standard controller, but is required for the
  // Wavebird.
  unsigned char initialize = 0x00;
  noInterrupts();
  N64_send(&initialize, 1);

  // Stupid routine to wait for the gamecube controller to stop
  // sending its response. We don't care what it is, but we
  // can't start asking for status if it's still responding
  int x;
  for (x=0; x<64; x++) {
      // make sure the line is idle for 64 iterations, should
      // be plenty.
      if (!N64_QUERY)
          x = 0;
  }

  // Query for the gamecube controller's status. We do this
  // to get the 0 point for the control stick.
  unsigned char command[] = {0x01};
  N64_send(command, 1);
  // read in data and dump it to N64_raw_dump
  N64_get();
  interrupts();
  translate_raw_data();  
}


void translate_raw_data()
{
    // The get_N64_status function sloppily dumps its data 1 bit per byte
    // into the get_status_extended char array. It's our job to go through
    // that and put each piece neatly into the struct N64_status
    int i;
    memset(&N64_status, 0, sizeof(N64_status));
    // line 1
    // bits: A, B, Z, Start, Dup, Ddown, Dleft, Dright
    for (i=0; i<8; i++) {
        N64_status.data1 |= N64_raw_dump[i] ? (0x80 >> i) : 0;
    }
    // line 2
    // bits: 0, 0, L, R, Cup, Cdown, Cleft, Cright
    for (i=0; i<8; i++) {
        N64_status.data2 |= N64_raw_dump[8+i] ? (0x80 >> i) : 0;
    }
    // line 3
    // bits: joystick x value
    // These are 8 bit values centered at 0x80 (128)
    for (i=0; i<8; i++) {
        N64_status.stick_x |= N64_raw_dump[16+i] ? (0x80 >> i) : 0;
    }
    for (i=0; i<8; i++) {
        N64_status.stick_y |= N64_raw_dump[24+i] ? (0x80 >> i) : 0;
    }
}


/**
 * This sends the given byte sequence to the controller
 * length must be at least 1
 * Oh, it destroys the buffer passed in as it writes it
 */
void N64_send(unsigned char *buffer, char length)
{
    // Send these bytes
    char bits;
    
    bool bit;

    // This routine is very carefully timed by examining the assembly output.
    // Do not change any statements, it could throw the timings off
    //
    // We get 16 cycles per microsecond, which should be plenty, but we need to
    // be conservative. Most assembly ops take 1 cycle, but a few take 2
    //
    // I use manually constructed for-loops out of gotos so I have more control
    // over the outputted assembly. I can insert nops where it was impossible
    // with a for loop
    
    asm volatile (";Starting outer for loop");
outer_loop:
    {
        asm volatile (";Starting inner for loop");
        bits=8;
inner_loop:
        {
            // Starting a bit, set the line low
            asm volatile (";Setting line to low");
            N64_LOW; // 1 op, 2 cycles

            asm volatile (";branching");
            if (*buffer >> 7) {
                asm volatile (";Bit is a 1");
                // 1 bit
                // remain low for 1us, then go high for 3us
                // nop block 1
                asm volatile ("nop\nnop\nnop\nnop\nnop\n");
                
                asm volatile (";Setting line to high");
                N64_HIGH;

                // nop block 2
                // we'll wait only 2us to sync up with both conditions
                // at the bottom of the if statement
                asm volatile ("nop\nnop\nnop\nnop\nnop\n"  
                              "nop\nnop\nnop\nnop\nnop\n"  
                              "nop\nnop\nnop\nnop\nnop\n"  
                              "nop\nnop\nnop\nnop\nnop\n"  
                              "nop\nnop\nnop\nnop\nnop\n"  
                              "nop\nnop\nnop\nnop\nnop\n"  
                              );

            } else {
                asm volatile (";Bit is a 0");
                // 0 bit
                // remain low for 3us, then go high for 1us
                // nop block 3
                asm volatile ("nop\nnop\nnop\nnop\nnop\n"  
                              "nop\nnop\nnop\nnop\nnop\n"  
                              "nop\nnop\nnop\nnop\nnop\n"  
                              "nop\nnop\nnop\nnop\nnop\n"  
                              "nop\nnop\nnop\nnop\nnop\n"  
                              "nop\nnop\nnop\nnop\nnop\n"  
                              "nop\nnop\nnop\nnop\nnop\n"  
                              "nop\n");

                asm volatile (";Setting line to high");
                N64_HIGH;

                // wait for 1us
                asm volatile ("; end of conditional branch, need to wait 1us more before next bit");
                
            }
            // end of the if, the line is high and needs to remain
            // high for exactly 16 more cycles, regardless of the previous
            // branch path

            asm volatile (";finishing inner loop body");
            --bits;
            if (bits != 0) {
                // nop block 4
                // this block is why a for loop was impossible
                asm volatile ("nop\nnop\nnop\nnop\nnop\n"  
                              "nop\nnop\nnop\nnop\n");
                // rotate bits
                asm volatile (";rotating out bits");
                *buffer <<= 1;

                goto inner_loop;
            } // fall out of inner loop
        }
        asm volatile (";continuing outer loop");
        // In this case: the inner loop exits and the outer loop iterates,
        // there are /exactly/ 16 cycles taken up by the necessary operations.
        // So no nops are needed here (that was lucky!)
        --length;
        if (length != 0) {
            ++buffer;
            goto outer_loop;
        } // fall out of outer loop
    }

    // send a single stop (1) bit
    // nop block 5
    asm volatile ("nop\nnop\nnop\nnop\n");
    N64_LOW;
    // wait 1 us, 16 cycles, then raise the line 
    // 16-2=14
    // nop block 6
    asm volatile ("nop\nnop\nnop\nnop\nnop\n"
                  "nop\nnop\nnop\nnop\nnop\n"  
                  "nop\nnop\nnop\nnop\n");
    N64_HIGH;

}

void N64_get()
{
    // listen for the expected 8 bytes of data back from the controller and
    // blast it out to the N64_raw_dump array, one bit per byte for extra speed.
    // Afterwards, call translate_raw_data() to interpret the raw data and pack
    // it into the N64_status struct.
    asm volatile (";Starting to listen");
    unsigned char timeout;
    char bitcount = 32;
    char *bitbin = N64_raw_dump;

    // Again, using gotos here to make the assembly more predictable and
    // optimization easier (please don't kill me)
read_loop:
    timeout = 0x3f;
    // wait for line to go low
    while (N64_QUERY) {
        if (!--timeout)
            return;
    }
    // wait approx 2us and poll the line
    asm volatile (
                  "nop\nnop\nnop\nnop\nnop\n"  
                  "nop\nnop\nnop\nnop\nnop\n"  
                  "nop\nnop\nnop\nnop\nnop\n"  
                  "nop\nnop\nnop\nnop\nnop\n"  
                  "nop\nnop\nnop\nnop\nnop\n"  
                  "nop\nnop\nnop\nnop\nnop\n"  
            );
    *bitbin = N64_QUERY;
    ++bitbin;
    --bitcount;
    if (bitcount == 0)
        return;

    // wait for line to go high again
    // it may already be high, so this should just drop through
    timeout = 0x3f;
    while (!N64_QUERY) {
        if (!--timeout)
            return;
    }
    goto read_loop;

}

void debug()
{
    int i;
    // bits: A, B, Z, Start, Dup, Ddown, Dleft, Dright
    // bits: 0, 0, L, R, Cup, Cdown, Cleft, Cright
    Serial.println();
    Serial.print("Start: ");
    Serial.println(N64_status.data1 & 16 ? 1:0);

    Serial.print("Z:     ");
    Serial.println(N64_status.data1 & 32 ? 1:0);

    Serial.print("B:     ");
    Serial.println(N64_status.data1 & 64 ? 1:0);

    Serial.print("A:     ");
    Serial.println(N64_status.data1 & 128 ? 1:0);

    Serial.print("L:     ");
    Serial.println(N64_status.data2 & 32 ? 1:0);
    Serial.print("R:     ");
    Serial.println(N64_status.data2 & 16 ? 1:0);

    Serial.print("Cup:   ");
    Serial.println(N64_status.data2 & 0x08 ? 1:0);
    Serial.print("Cdown: ");
    Serial.println(N64_status.data2 & 0x04 ? 1:0);
    Serial.print("Cright:");
    Serial.println(N64_status.data2 & 0x01 ? 1:0);
    Serial.print("Cleft: ");
    Serial.println(N64_status.data2 & 0x02 ? 1:0);
    
    Serial.print("Dup:   ");
    Serial.println(N64_status.data1 & 0x08 ? 1:0);
    Serial.print("Ddown: ");
    Serial.println(N64_status.data1 & 0x04 ? 1:0);
    Serial.print("Dright:");
    Serial.println(N64_status.data1 & 0x01 ? 1:0);
    Serial.print("Dleft: ");
    Serial.println(N64_status.data1 & 0x02 ? 1:0);

    Serial.print("Stick X:");
    Serial.println(N64_status.stick_x, DEC);
    Serial.print("Stick Y:");
    Serial.println(N64_status.stick_y, DEC);
}


void readJoysticks() {
  int i;
  unsigned char data, addr;

  // Command to send to the gamecube
  // The last bit is rumble, flip it to rumble
  // yes this does need to be inside the loop, the
  // array gets mutilated when it goes through N64_send
  unsigned char command[] = {0x01};

  // don't want interrupts getting in the way
  noInterrupts();
  // send those 3 bytes
  N64_send(command, 1);
  // read in data and dump it to N64_raw_dump
  N64_get();
  // end of time sensitive code
  interrupts();

  // translate the data in N64_raw_dump to something useful
  translate_raw_data();
}




void interpretJoystickState() {
  Joystick.setXAxis(N64_status.stick_x);
  Joystick.setYAxis(-N64_status.stick_y);
    
  Joystick.setButton(0, (N64_status.data1 & 16) ); //BUTTON1 (Start)
  Joystick.setButton(1, (N64_status.data1 & 32) ); //BUTTON2 (Select) (Z)
  Joystick.setButton(2, (N64_status.data1 & 128) ); //BUTTON3 (A)
  Joystick.setButton(3, (N64_status.data1 & 64) ); //BUTTON4 (B)
  Joystick.setButton(4, (N64_status.data2 & 0x04) ); //BUTTON5 (X) (Cdown)
  Joystick.setButton(5, (N64_status.data2 & 0x02) ); //BUTTON6 (Y) (Cleft)
  Joystick.setButton(6, (N64_status.data2 & 32) ); //BUTTON7 (LB) (L)
  Joystick.setButton(7, (N64_status.data2 & 16) ); //BUTTON8 (RB) (R)
  Joystick.setButton(8, (N64_status.data2 & 0x08) ); //BUTTON9 (LT) (Cup)
  Joystick.setButton(9, (N64_status.data2 & 0x01) ); //BUTTON10 (RT) (Cright)

  /*Joystick.setXAxisRotation(180);
  Joystick.setYAxisRotation(180);
  Joystick.setHatSwitch(0,-1);
  Joystick.setButton(10, 0);
  Joystick.setButton(11, 0);*/

  Joystick.setHatSwitch(0,-1);
  if ( (N64_status.data1 & 0x08) ) Joystick.setHatSwitch(0,0); //UP
  if ( (N64_status.data1 & 0x04) ) Joystick.setHatSwitch(0,180); //DOWN
  if ( (N64_status.data1 & 0x02) ) Joystick.setHatSwitch(0,270); //LEFT
  if ( (N64_status.data1 & 0x01) ) Joystick.setHatSwitch(0,90); //RIGHT

  if ( (N64_status.data1 & 0x08) && (N64_status.data1 & 0x01) ) Joystick.setHatSwitch(0,45); //UP-RIGHT
  if ( (N64_status.data1 & 0x01) && (N64_status.data1 & 0x04) ) Joystick.setHatSwitch(0,135); //RIGHT-DOWN
  if ( (N64_status.data1 & 0x04) && (N64_status.data1 & 0x02) ) Joystick.setHatSwitch(0,225); //DOWN-LEFT
  if ( (N64_status.data1 & 0x02) && (N64_status.data1 & 0x08) ) Joystick.setHatSwitch(0,315); //LEFT-UP

}


void setup()
{
  #ifdef DEBUG
  Serial.begin(9600);
  #endif

  setupJoysticks();
  
  Joystick.begin(false);

}


uint8_t oldData1 = 0xff;
uint8_t oldData2 = 0xff;
int8_t oldStick_x = 0;
int8_t oldStick_y = 0;

uint8_t flag = 0;

void loop() {

  readJoysticks();

  if (N64_status.data1 != oldData1) {
    oldData1 = N64_status.data1;
    flag = 1;
  }
  if (N64_status.data2 != oldData2) {
    oldData2 = N64_status.data2;
    flag = 1;
  }
  if (N64_status.stick_x != oldStick_x) {
    oldStick_x = N64_status.stick_x;
    flag = 1;
  }
  if (N64_status.stick_y != oldStick_y) {
    oldStick_y = N64_status.stick_y;
    flag = 1;
  }

  if (flag) {
    interpretJoystickState();
    Joystick.sendState();
    flag = 0;
  }
  
  
  delayMicroseconds(1000);
  #ifdef DEBUG
  debug();
  delay(200);
  #endif
}


