// Claw Functions, reacting to sensors, placement of the claw, speed of the claw, location of the claw
#include "claw.h"
#include "hardware_def.h"
#include <Servo.h>

#define CLOSE 0
#define OPEN 180
#define CLAW_REF_THRES 40
#define CLAW_MAG_THRES 1020


// Now define the main code for the functions listed in the header file
Servo clawservo;  // create servo object to control claw
Servo armservo;  // create servo object to control arm
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void stopForwardPancakeMotor();
void stopBackwardPancakeMotor();

void claw_setup() {
  clawservo.attach(CLAW_SERVO);  // attaches the claw servo on PWM pin to the servo object
  armservo.attach(ARM_SERVO);  // attaches the arm servo on PWM pin to the servo object
  pinMode(CLAW_END, INPUT_PULLUP);
  pinMode(CLAW_START, INPUT_PULLUP);
  pinMode(PANCAKE_BACK, OUTPUT);
  pinMode(PANCAKE_FOR, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CLAW_END), stopForwardPancakeMotor, RISING);
  attachInterrupt(digitalPinToInterrupt(CLAW_START), stopBackwardPancakeMotor, RISING);
  claw_servo_pos(OPEN);
  
  while(digitalRead(CLAW_START)){
    claw_backward();
  }  

}

/**
    Parameters
    ----------
    Void
   
    Function
    ------
    Servoloop for testing purposes

    Returns
    -------
    Void
**/
void servo_loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    clawservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 10ms for the servo to reach the position
  }
  delay(100);                       // waits 10ms for the servo to reach the position
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    clawservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
}

/**
    Parameters
    ----------
    position : int
        Position for the servo to go to
   
    Raises
    ------
    Sends the servo at myservo to a certain position

    Returns
    -------
    Void
**/
void claw_servo_pos(int position){
  clawservo.write(position);
}

/**
    Function
    ------
    Loop for picking up treasures
**/
void claw_loop(){
  //initally opens claw, assume start position is at CLAW_START
  claw_servo_pos(OPEN);
  while(digitalRead(CLAW_END)){
    claw_forward();
  }

  // Sets initial angle as 40, to allow for sweep
  // Will exit loop if the threshold is not broached
  int final_angle = 40;
  for (int angle = 40; angle < 140 && (analogRead(CLAW_REF) > CLAW_REF_THRES); angle++)
  {
    arm_servo_pos(angle);
    delay(100);
    final_angle = angle;
  }
  delay(15);

  if (analogRead(CLAW_REF) < CLAW_REF_THRES){
    if(analogRead(CLAW_MAG) > CLAW_MAG_THRES){
        claw_servo_pos(CLOSE);

        if (final_angle < 90){
          arm_servo_pos(40);
        } else {
          arm_servo_pos(140);
        }
        delay(1000);
        arm_servo_pos(final_angle);
        delay(200);
    }
  }

  // while(digitalRead(CLAW_START)){
  //   claw_backward();
  // }
  // claw_servo_pos(OPEN);
  // delay(1000);
}

void test_claw_loop(){
  //initally opens claw, assume start position is at CLAW_START
  claw_servo_pos(OPEN);

  if (analogRead(CLAW_REF) < CLAW_REF_THRES){
    if(analogRead(CLAW_MAG) > CLAW_MAG_THRES){
        claw_servo_pos(CLOSE);
        delay(6000);
        // Add boolean to detect magnetic, automatically stop loop if so
    }
  }
  claw_servo_pos(OPEN);
  delay(1000);
}

void claw_forward(){
  //If we aren't at the end already, then go forwards
  if (digitalRead(CLAW_END)){
    digitalWrite(PANCAKE_BACK, LOW);
    digitalWrite(PANCAKE_FOR, HIGH);
    Serial.print("Going Forward");
    Serial.println();
  }
}

void claw_backward(){
//If we aren't currently at the start, then go backwards
if (digitalRead(CLAW_START)){
  digitalWrite(PANCAKE_FOR, LOW);
  digitalWrite(PANCAKE_BACK, HIGH);
  Serial.print("Going Backwards");
  Serial.println();
  }
}

void stopForwardPancakeMotor(){
  digitalWrite(PANCAKE_FOR, LOW);
}

void stopBackwardPancakeMotor(){
  digitalWrite(PANCAKE_BACK, LOW);
}

/**
    Parameters
    ----------
    position : int
        Position for the servo to go to
   
    Raises
    ------
    Sends the arm servo to a certain position

    Returns
    -------
    Void
**/
void arm_servo_pos(int position){
  armservo.write(position);
}

void claw_limitswitch(){
  if(!digitalRead(CLAW_END)){
    Serial.print("At End");
    Serial.println();
  } else if(!digitalRead(CLAW_START)){
    Serial.print("At Start");
    Serial.println();    
  } else{
    Serial.print("Middle");
    Serial.println();   
  }
}