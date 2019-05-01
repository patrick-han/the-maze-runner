// Send joystick data wirelessly via RFM69 module
// Library by Felix Rusu - felix@lowpowerlab.com
// Get the RFM69 library at: https://github.com/LowPowerLab/

#include <RFM69.h>
#include <SPI.h>

const int joyLR = A0;        // L/R Parallax Thumbstick
const int joyUD = A3;        // U/D Parallax Thumbstick

int LR_value;
int UD_value;

// RADIO DEFINES
#define NETWORKID     0  //the same on all nodes that talk to each other
#define NODEID        1    //unique for each node on same network
#define TONODEID       2    // ID of the controller node
#define FREQUENCY   RF69_433MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define SERIAL_BAUD   115200

RFM69 radio;

void setup() {
  // Open a serial port so we can send keystrokes to the module:
  
  Serial.begin(SERIAL_BAUD);
  Serial.print("Node ");
  Serial.print(NODEID,DEC);
  Serial.println(" ready");  

  
  radio.initialize(FREQUENCY,NODEID,NETWORKID); // Initialize radio
  radio.setHighPower(); // Always use this for RFM69HCW
  radio.encrypt(ENCRYPTKEY);  // Use encryption

  
}

void loop() {

    // Set up a "buffer" for characters that we'll send:
  
  static byte sendbuffer[2];  // We will send 2 bytes of data
  static int sendlength = 2;

  LR_value = map(analogRead(joyLR), 0, 1023, 0, 180); // Map each joystick reading to appropriate servo angle
  UD_value = map(analogRead(joyUD), 0, 1023, 0, 180);

  sendbuffer[0] = LR_value; // Store data in buffer for later transmission
  sendbuffer[1] = UD_value;
  
 
  Serial.print("sending to node ");
  Serial.print(TONODEID, DEC);
  Serial.println(":");
  Serial.print("L/R:");
  Serial.println(sendbuffer[0]);
  Serial.print("U/D:");
  Serial.println(sendbuffer[1]);
      
  // No need to use ACK     
  radio.send(TONODEID, sendbuffer, sendlength); // Send buffer wirelessly to node 2 (the maze game board)
 
}


