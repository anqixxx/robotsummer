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

void manualMode();

void setup() 
{
    Serial.begin(SERIAL_RATE);
    Serial.print("Hello");
    blueStart();
    pwm_setup();
    setupRadio();
}

void loop() 
{ 

  rcloop();
  //pwm_loop(); // Can this be romoved or archived for if we need DuePWM on pins 6789
  if (isManual()){
     Serial.println("in manual mode");
     manualMode();
  } else {
     Serial.println("in auto mode");
     // lineFollow(potentiometerData()); // poten is from 0 to 255
  }

}

// Manual control of robot
void manualMode(){
  int mainLeftMotor = leftMotorData();
  int mainRightMotor = rightMotorData();
  drive(10  , 10);
  blueLoop(mainLeftMotor, mainRightMotor, 0,  0,  0);
}
