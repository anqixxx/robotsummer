// Claw Functions, reacting to sensors, placement of the claw, speed of the claw, location of the claw
#include "claw.h"
#include "hardware_def.h"
#include <Servo.h>
#include "ClawClass.h"
#include "Arm.h"

#define CLOSE 20
#define OPEN 120
#define CLAW_REF_THRES 40
#define CLAW_MAG_THRES 1020

    // ClawClass claw = ClawClass(CLAW_SERVO);

    // Arm robtoArm(PANCAKE_FOR, PANCAKE_BACK, 
    // ARM_LIMIT_START, ARM_LIMIT_END, 
    // ARM_SERVO_TOP, ARM_SERVO_BOTTOM);


// Now define the main code for the functions listed in the header file
Servo clawservo;  // create servo object to control claw
Servo armservo;  // create servo object to control arm
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void stopForwardPancakeMotor();
void stopBackwardPancakeMotor();

void claw_setup() {
  int timer = 0;

  clawservo.attach(CLAW_SERVO);  // attaches the claw servo on PWM pin to the servo object
  // Arm servos have been doubled now //armservo.attach(ARM_SERVO);  // attaches the arm servo on PWM pin to the servo object
  pinMode(ARM_LIMIT_END, INPUT_PULLUP);
  pinMode(ARM_LIMIT_START, INPUT_PULLUP);
  pinMode(PANCAKE_BACK, OUTPUT);
  pinMode(PANCAKE_FOR, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ARM_LIMIT_END), stopForwardPancakeMotor, FALLING);
  attachInterrupt(digitalPinToInterrupt(ARM_LIMIT_START), stopBackwardPancakeMotor, FALLING);
  claw_servo_pos(OPEN);
  
  timer = millis();
  while(digitalRead(ARM_LIMIT_START) && millis()-timer < 1500){
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
  for (pos = OPEN; pos <= CLOSE; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    claw_servo_pos(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 10ms for the servo to reach the position
  }
  delay(100);                       // waits 10ms for the servo to reach the position
  for (pos = CLOSE; pos >= OPEN; pos -= 1) { // goes from 180 degrees to 0 degrees
    claw_servo_pos(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
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
  int timer;
  //initally opens claw, assume start position is at CLAW_START
  claw_servo_pos(OPEN);

  timer = millis();
 while(digitalRead(ARM_LIMIT_END) && millis()-timer < 1500){
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

  timer = millis();
  while(digitalRead(ARM_LIMIT_START)  && millis()-timer < 1500){
    claw_backward();
  }
  claw_servo_pos(OPEN);
  delay(1000);
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
  if (digitalRead(ARM_LIMIT_END)){
    digitalWrite(PANCAKE_BACK, LOW);
    digitalWrite(PANCAKE_FOR, HIGH);
    Serial.print("Going Forward");
    Serial.println();
  }
}

void claw_backward(){
//If we aren't currently at the start, then go backwards
if (digitalRead(ARM_LIMIT_START)){
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