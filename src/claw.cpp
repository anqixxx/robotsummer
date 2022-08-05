// Claw Functions, reacting to sensors, placement of the claw, speed of the claw, location of the claw
#include "claw.h"
#include "hardware_def.h"
#include <Servo.h>
#include "ClawClass.h"
#include "Arm.h"

#define CLOSE 120
#define OPEN 180
#define CLAW_REF_THRES 80
#define CLAW_DET_THRES 300
#define CLAW_MAG_THRES 1020
#define ARMSTART 0
#define ARMEND 180

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
void claw_loop(){
  //initally opens claw, assume start position is at CLAW_START
  claw.reposition(OPEN);

  robotArm.moveClawOut();

  // Sets initial angle as 40, to allow for sweep
  // Will exit loop if the threshold is not broached

  robotArm.setAngle(ARMSTART);
  delay(100);
  for (int angle = ARMSTART; angle < ARMEND && claw.getReflectance() > CLAW_REF_THRES; angle++)
  {
    robotArm.setAngle(angle);
    delay(15);
  }

  if(claw.getHall() < CLAW_MAG_THRES){
    robotArm.moveClawIn();
  } else{
      //stepper motor move down

    if (claw.getReflectance() < CLAW_REF_THRES){
      if(claw.getHall() > CLAW_MAG_THRES){
        claw.reposition(CLOSE);
    }

    robotArm.moveClawIn();
    claw.reposition(OPEN);
  }

  }
}

void test_claw_loop(){
  //initally opens claw, assume start position is at CLAW_START
  claw.reposition(OPEN);

  if (claw.getReflectance() < CLAW_REF_THRES){
    if(claw.getHall() > CLAW_MAG_THRES){
        claw.reposition(CLOSE);
        delay(6000);
        // Add boolean to detect magnetic, automatically stop loop if so
    } else{
        Serial.println("Magnetic detected");
    }
  }
  claw.reposition(OPEN);
  delay(1000);
}

void claw_test_value(){
  claw.reposition(OPEN);
  claw.getReflectance();
  claw.getHall();
  delay(1000);
}

void stopForwardPancakeMotor(){
  digitalWrite(PANCAKE_FOR, LOW);
}

void stopBackwardPancakeMotor(){
  digitalWrite(PANCAKE_BACK, LOW);
}

//rementants for logic
void claw_limitswitch(){
  if(!digitalRead(ARM_LIMIT_END)){
    Serial.print("At End");
    Serial.println();
  } else if(!digitalRead(ARM_LIMIT_START)){
    Serial.print("At Start");
    Serial.println();    
  } else{
    Serial.print("Middle");
    Serial.println();   
  }
}