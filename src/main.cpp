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


double sigAmp = 0;
int count1 = 0;
int count2 = 0;

void setup() {
  blueStart(); 
}

void loop() {
  blueLoop();
}


