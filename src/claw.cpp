// Claw Functions, reacting to sensors, placement of the claw, speed of the claw, location of the claw
#include "claw.h"
#include "hardware_def.h"
#include <Servo.h>
#include "ClawClass.h"
#include "Arm.h"
#include "serial_coms.h"

#define CLAW_REF_THRES 140
#define CLAW_DET_THRES 100
#define CLAW_MAG_THRES 400

#define TREASURE_FOUND 1
#define TREASURE_NOT_FOUND 0
#define TREASURE_BOMB -1

ClawClass claw = ClawClass(CLAW_SERVO);

Arm robotArm(PANCAKE_FOR, PANCAKE_BACK,
             ARM_LIMIT_START, ARM_LIMIT_END,
             ARM_SERVO_TOP, ARM_SERVO_BOTTOM);

void stopForwardPancakeMotor();
void stopBackwardPancakeMotor();



/**
    Function
    ------
    Loop for picking up treasures
**/

void treasureSequence(int treasure)
{

  moveStepper(STEPPER_TREASURE_POS);
  robotArm.moveClawOut();
  int res = sweep(CCW);
  if (res == TREASURE_FOUND){

    moveStepper(getCurrentStepperPos() - 200);
    if (claw.getHall() > CLAW_MAG_THRES){
    claw.reposition(CLAW_CLOSE);
    retrieveTreasure(treasure);
    }
  }
  outputCSV(claw.getHall(), claw.getReflectance(), 0, 0, 0);
  delay(1000);
    claw.reposition(CLAW_OPEN);
  delay(1000);

 }

 void retrieveTreasure(int treasure){

    claw.reposition(CLAW_CLOSE);
    moveStepper(STEPPER_HIGH_POS);
    robotArm.moveClawIn();
    moveStepper(2000);   // Fix this number to a constant value *************************** TODO
    claw.reposition(CLAW_OPEN);
 }

void claw_loop()
{
  int angle;
  // initally opens claw, assume start position is at CLAW_START
  claw.reposition(CLAW_OPEN);
  robotArm.moveClawOut();

  // Sets initial angle as 40, to allow for sweep
  // Will exit loop if the threshold is not broached

  robotArm.setAngle(ARMSTART);
  delay(100);
  for (angle = ARMSTART; angle < ARMEND && claw.getReflectance() > CLAW_DET_THRES; angle++)
  {
    robotArm.setAngle(angle);
    delay(25);
  }

  for (; angle > ARMSTART && claw.getReflectance() > CLAW_DET_THRES; angle--)
  {
    robotArm.setAngle(angle);
    delay(25);
  }

  if (claw.getHall() < CLAW_MAG_THRES)
  {
    // robotArm.moveClawIn();
  }
  else
  {
    // stepper motor move down

    if (claw.getReflectance() < CLAW_REF_THRES)
    {
      if (claw.getHall() > CLAW_MAG_THRES)
      {
        claw.reposition(CLAW_CLOSE);
      }

      // robotArm.moveClawIn();
      claw.reposition(CLAW_OPEN);
    }
  }
}

void test_claw_loop()
{
  // initally opens claw, assume start position is at CLAW_START
  claw.reposition(CLAW_OPEN);

  if (claw.getReflectance() < CLAW_DET_THRES)
  {
    if (claw.getHall() > CLAW_MAG_THRES)
    {
      claw.reposition(CLAW_CLOSE);
      delay(6000);
      // Add boolean to detect magnetic, automatically stop loop if so
    }
    else
    {
      Serial.println("Magnetic detected");
    }
  }
  claw.reposition(CLAW_OPEN);
  delay(1000);
}

void claw_test_value()
{
  claw.reposition(CLAW_OPEN);
  claw.getReflectance();
  claw.getHall();
  delay(1000);
}

void stopForwardPancakeMotor()
{
  digitalWrite(PANCAKE_FOR, LOW);
}

void stopBackwardPancakeMotor()
{
  digitalWrite(PANCAKE_BACK, LOW);
}

// rementants for logic
void claw_limitswitch()
{
  if (!digitalRead(ARM_LIMIT_END))
  {
    Serial.print("At End");
    Serial.println();
  }
  else if (!digitalRead(ARM_LIMIT_START))
  {
    Serial.print("At Start");
    Serial.println();
  }
  else
  {
    Serial.print("Middle");
    Serial.println();
  }
}


int sweep(int dir)
{

  int pos;
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    robotArm.setAngle(pos);              // tell servo to go to position in variable 'pos'
    delay(25);                       // waits 15ms for the servo to reach the position
    outputCSV(claw.getReflectance(),0,0,0,0);
    if (claw.getReflectance()< CLAW_MAG_THRES){
      return TREASURE_FOUND;
    }
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
   robotArm.setAngle(pos);              // tell servo to go to position in variable 'pos'
    delay(25);                       // waits 15ms for the servo to reach the position
        outputCSV(claw.getReflectance(),0,0,0,0);
    if (claw.getReflectance()< CLAW_MAG_THRES){
      return TREASURE_FOUND;
    }
  }
  return TREASURE_NOT_FOUND;
}
