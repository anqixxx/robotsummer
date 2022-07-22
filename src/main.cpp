#include "Arduino.h"
#include "Wire.h"
#include <stdio.h>
#include <stdbool.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "hardware_def.h"
#include "tape_follow.h"
#include "ir_sensors.h"
#include "motor_drive.h"
#include "field_nav.h"
#include "claw.h"
#include "arm.h"
#include "treasure.h"
#include "bluetooth.h"
#include "DuePWM.h"
#include "nRF24L01.h"
#include "RF24.h"
#include "RF24_config.h"


/*
Radio Variables and Data Structure
*/
RF24 radio(CE, CNS);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  byte j2PotX;
  byte j2PotY;
  byte j2Button;
  byte pot1;
  byte pot2;
  byte tSwitch1;
  byte tSwitch2;
  byte button1;
  byte button2;
  byte button3;
  byte button4;
};

Data_Package data;

/*
Tape Follow Calibration
*/
int pot1;
int pot2;
int tapeLeft;
int tapeRight;

/*
IR detection
*/
double sigAmp = 0;

// RC Functions
void rcloop();
void setupRadio();
void resetRadioData();

// Robot Modes
void manualMode();
void lineFollow(int );

void setup() 
{
  Serial.begin(SERIAL_RATE);
  Serial.print("Hello");
  blueStart(); // Configure Bluetooth
  pwm_setup(); // Adjust pwm to correct frequency for the drive motors
  setupRadio(); // Open the RC radio communications through 
}

void loop() 
{ 
  // Check RC input
   rcloop();
   // Right side toggle switch in up position is indicator for manual mode
   if (data.tSwitch2 == 0){
      manualMode();
   } else {
    // Output sensor readings to bluetooth for debuggin
        // Read the relevant sensors for tape following procedure
  pot1 = analogRead(POT1);
  pot2 = analogRead(POT2);
  tapeLeft = analogRead(TAPE_L);
  tapeRight = analogRead(TAPE_R);
    blueLoop(pot1, pot2, tapeLeft, tapeRight ,0);




    // Follow the line
   lineFollow(); 
   }    

}


// Manual control of robot
void manualMode(){
  //Normalize drive values to -255 to 255 range
  int leftMotor = (data.j1PotY-128)*1.9;
  int rightMotor = (data.j2PotY-128)*1.9;
  drive(leftMotor  , rightMotor);
  blueLoop(leftMotor, rightMotor, data.tSwitch2,  0,  0);
}

/*
Radio Functions
*/
void rcloop() {
  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetRadioData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  }
}
void setupRadio(){
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetRadioData();
  }
void resetRadioData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotX = 127;
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.j1Button = 1;
  data.j2Button = 1;
  data.pot1 = 1;
  data.pot2 = 1;
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.button1 = 1;
  data.button2 = 1;
  data.button3 = 1;
  data.button4 = 1;
}
