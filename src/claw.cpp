// Claw Functions, reacting to sensors, placement of the claw, speed of the claw, location of the claw
#include "claw.h"
#include "hardware_def.h"
#include <Servo.h>
#include "ClawClass.h"
#include "Arm.h"
#include "serial_coms.h"
#include "treasure.h"


#define CLAW_REF_THRES 440
#define CLAW_MAG_THRES 700

#define TREASURE_FOUND 1
#define TREASURE_NOT_FOUND 0
#define TREASURE_BOMB -1

ClawClass claw = ClawClass(CLAW_SERVO);

Arm robotArm(PANCAKE_FOR, PANCAKE_BACK,
             ARM_LIMIT_START, ARM_LIMIT_END,
             ARM_SERVO_TOP, ARM_SERVO_BOTTOM);
int treasure = 0;

void stopForwardPancakeMotor();
void stopBackwardPancakeMotor();

// Assumes claw is at the corect range
// Sees if it is detected on Right or Left
void treasureDetected(int start, int end, int treasure)
{

  moveStepper(STEPPER_TREASURE_POS);
  robotArm.moveClawOut();

  int res = sweep(start, end);
  if (res == TREASURE_FOUND){

    moveStepper(getCurrentStepperPos() - 200);
    if (claw.getHall() > CLAW_MAG_THRES){
      claw.reposition(CLAW_CLOSE);
      robotArm.setAngle(90);
      retrieveTreasure();
    }
  }

  outputCSV(claw.getHall(), claw.getReflectance(), 0, 0, 0);
  delay(1000);
    claw.reposition(CLAW_OPEN);
  delay(1000);
}

/**
    Function
    ------
    Loop for picking up treasures
**/

void treasureSequence(int start, int end)
{
  moveStepper(STEPPER_TREASURE_POS);
  robotArm.moveClawOut();
  int res = sweep(start, end);
  SERIAL_OUT.println("Hall, Ref, Results, 0 ,0" );
  outputCSV(claw.getHall(), claw.getReflectance(), res, 0, 0);

  if (res == TREASURE_FOUND){
    moveStepper(getCurrentStepperPos() - 200);
    if (claw.getHall() > CLAW_MAG_THRES){
      claw.reposition(CLAW_CLOSE);
      retrieveTreasure();
      treasure++;  // Treasure found increment counter by one
    }
  }

    claw.reposition(CLAW_OPEN);


 }

 void retrieveTreasure(){
    int angle = 0;

    claw.reposition(CLAW_CLOSE);
    moveStepper(STEPPER_HIGH_POS);
    switch (treasure)
  {
  case 0:
    angle = 10;
    break;
  case 1:
    // From the chicken wire backup to the first treasure
   angle = 50;
    break;
  case 2:
    angle = 110;
    break;
  case 3:
    angle = 150;
  }
  robotArm.setAngle(angle);
      robotArm.moveClawIn();
    moveStepper(2000);   // Fix this number to a constant value *************************** TODO
    claw.reposition(CLAW_OPEN);
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

int sweep(int start, int end){
  int pos;
// Make sure to add angles for claw that is in the tough sector, i.e. power needed means the delay causes it to read
// and move
// Also for one way, set angle to be +/- 5 to account for the side where the ref sensor is
if (start > end){
  for (pos = start; pos >= end; pos -= 1) { // goes from 180 degrees to 0 degrees
   robotArm.setAngle(pos);              // tell servo to go to position in variable 'pos'
    delay(35);                       // waits 15ms for the servo to reach the position
    outputCSV(claw.getReflectance(),0,0,0,0);

    if (claw.getReflectance()< CLAW_REF_THRES){
      return TREASURE_FOUND;
    }
  }

} else{
    for (pos = start; pos <= end; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    robotArm.setAngle(pos);              // tell servo to go to position in variable 'pos'
    delay(35);                       // waits 15ms for the servo to reach the position
    outputCSV(claw.getReflectance(),0,0,0,0);
    if (claw.getReflectance()< CLAW_REF_THRES){
      return TREASURE_FOUND;
    }
  }
}

return TREASURE_NOT_FOUND;


}
