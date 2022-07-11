#include "Arduino.h"
#include "Wire.h"

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



double sigAmp = 0;
int count1 = 0;
int count2 = 0;

void setup() 
{
    Serial.begin(SERIAL_RATE);
    Serial.print("Hello");

    pwm_setup();
}

void loop() 
{ 
  int testRead = analogRead(motorInput);
  // blueLoop(testRead, analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4));
  
  // Serial.print("Analog reading is: ");
  // Serial. println();
  // Serial.print(testRead);

  pwm_loop();
}