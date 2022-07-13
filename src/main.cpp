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
int count1 = 0;
int count2 = 0;

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
  analogRead(TAPE_L);
  analogRead(TAPE_R);

  rcloop();
  pwm_loop();
  blueLoop(analogRead(motorInput), 0, 0,  0,  0);
  if (!manualOrAuto()){
      lineFollow(potentiometerData()); // poten is from 0 to 255
  }

}

void autoMode(){
  autoRadioLoop();
  int mainLeftMotor = leftMotorData();
  int mainRightMotor = rightMotorData();
  updateMotorPWM(mainLeftMotor, mainRightMotor);
  blueLoop(mainLeftMotor, mainRightMotor, 0,  0,  0);
}