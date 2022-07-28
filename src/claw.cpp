// Claw Functions, reacting to sensors, placement of the claw, speed of the claw, location of the claw
#include "claw.h"
#include "hardware_def.h"
#include <Servo.h>

#define CLOSE 0
#define OPEN 180
#define CLAW_REF_THRES 40
#define CLAW_MAG_THRES 1020


// Now define the main code for the functions listed in the header file
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void stopForwardPancakeMotor();
void stopBackwardPancakeMotor();

void claw_setup() {
  myservo.attach(CLAW_SERVO);  // attaches the servo on PWM pin 9 to the servo object
  pinMode(CLAW_END, INPUT_PULLUP);
  pinMode(CLAW_START, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLAW_END), stopForwardPancakeMotor, RISING);
  attachInterrupt(digitalPinToInterrupt(CLAW_START), stopBackwardPancakeMotor, RISING);
}

void servo_loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 10ms for the servo to reach the position
  }
  delay(100);                       // waits 10ms for the servo to reach the position
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
}

void servo_position(int position){
  myservo.write(position);
    //Reflectance
}

void claw_loop(){
  //initally opens claw, assume start position is at CLAW_START
  servo_position(OPEN);

  if (analogRead(CLAW_REF) < CLAW_REF_THRES){
    if(analogRead(CLAW_MAG) > CLAW_MAG_THRES){
        servo_position(CLOSE);
        delay(6000);
        // Add boolean to detect magnetic, automatically stop loop if so
    }
  }
  servo_position(OPEN);
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
  Serial.print("Stopped Forward");
  Serial.println();

}

void stopBackwardPancakeMotor(){
  digitalWrite(PANCAKE_BACK, LOW);
  Serial.print("Stopped Backward");
  Serial.println();
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