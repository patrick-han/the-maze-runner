// Send joystick data wirelessly via RFM69 module
// Library for RFM69 by Felix Rusu - felix@lowpowerlab.com
// Get the RFM69 library at: https://github.com/LowPowerLab/

#include <RFM69.h>
#include <SPI.h>
#include <Servo.h>

const int servo1 = 12;       // first servo
const int servo2 = 19;       // second servo

Servo UD_servo;
Servo LR_servo;

int LR_value;   // LR value from joystick
int UD_value;   // UD value from joystick


// RADIO DEFINES
#define NETWORKID     0  //the same on all nodes that talk to each other
#define NODEID        2    //unique for each node on same network
#define TONODEID       1  // ID of receiver node on the maze game board
#define FREQUENCY   RF69_433MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define SERIAL_BAUD   115200

RFM69 radio


void setup() {
  // Open a serial port so we can send keystrokes to the module:
  
  Serial.begin(SERIAL_BAUD);
  Serial.print("Node ");
  Serial.print(NODEID,DEC);
  Serial.println(" ready");  

  
  radio.initialize(FREQUENCY,NODEID,NETWORKID); // Initialize radio
  radio.setHighPower(); // Always use this for RFM69HCW
  radio.encrypt(ENCRYPTKEY);

  UD_servo.attach(servo1);  // Initialize all servos
  LR_servo.attach(servo2);
  UD_servo.write(90);     // Set both servos to the middle of their range
  LR_servo.write(90);

}

void loop() {

  //check for any received packets
  if (radio.receiveDone())
  {
    LR_value = map(radio.DATA[0], 0, 180, 2250, 750);   // Map received value to pulse width
    UD_value = map(radio.DATA[1], 0, 180, 750, 2250);

    LR_servo.writeMicroseconds(LR_value);   // Write appropriate pulse width to each servo
    UD_servo.writeMicroseconds(UD_value);

    
    Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.println("] ");
    Serial.print("L/R:");
    Serial.println(radio.DATA[0]);
    Serial.print("U/D:");
    Serial.println(radio.DATA[1]);
    Serial.print("   [RX_RSSI:");Serial.print(radio.RSSI);Serial.print("]");
    Serial.println();

    delay(15);  // wait for servos to move to correct position
  }
 
}


