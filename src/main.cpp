#include "Arduino.h"
#include "Wire.h"
#include <stdio.h>
#include <stdbool.h>

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
#include "rc_reciever.h"
#include "RF24.h"
#include "RF24_config.h"


double sigAmp = 0;

int pot1;
int pot2;
int tapeLeft;
int tapeRight;

void manualMode();



void setup() 
{
    Serial.begin(SERIAL_RATE);
    Serial.print("Hello");
    blueStart();
   pwm_setup();
  //  setupRadio();  Radio out of service
}

void loop() 
{ 
  // RADIO OUT OF SERVICE
  // rcloop();
  // if (isManual()){
  //    Serial.println("in manual mode");
  //    manualMode();
  // } else {
  //    Serial.println("in auto mode");
  //     drive(0  , 0);
  //    // lineFollow(potentiometerData()); // poten is from 0 to 255
  // }
  pot1 = analogRead(POT1);
  pot1 = analogRead(POT2);
  tapeLeft = analogRead(TAPE_L);
  tapeRight = analogRead(TAPE_R);

    blueLoop(pot1, pot2, tapeLeft, tapeRight ,0);
      drive( analogRead(POT1)/10 , analogRead(POT2)/10);
      

}

// Manual control of robot
void manualMode(){
  int mainLeftMotor = leftMotorData();
  int mainRightMotor = rightMotorData();
  drive(mainLeftMotor  , mainRightMotor);
  blueLoop(mainLeftMotor, mainRightMotor, 0,  0,  0);
}
