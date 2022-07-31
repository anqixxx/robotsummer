#include <Arduino.h>

#include "Encoders.h"
#include "hardware_def.h"

volatile byte aFlagLeft = 0; //pinA signal that the encoder has arrived at a detent
volatile byte bFlagLeft = 0; //pinB signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile int encoderPosLeft = 0; //current value of encoder position.
volatile byte readingLeft = 0; //values read from interrupt pins before checking to see if we have moved a whole detent

volatile byte aFlagRight = 0; //pinA signal that the encoder has arrived at a detent
volatile byte bFlagRight = 0; //pinB signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile int encoderPosRight = 0; //current value of encoder position.
volatile byte readingRight = 0; //values read from interrupt pins before checking to see if we have moved a whole detent

void setupEncoders(){
pinMode(ENCODER_LEFT_A, INPUT);
  pinMode(ENCODER_LEFT_B, INPUT);
  pinMode(ENCODER_RIGHT_A, INPUT);
  pinMode(ENCODER_RIGHT_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_A), EncoderLAISR, RISING); 
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_B), EncoderLBISR, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_A), EncoderRAISR, RISING); 
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_B), EncoderRBISR, RISING);

}

void EncoderLAISR(){

noInterrupts(); //stop interrupts happening before we read pin values
  bool stateENCA = digitalRead(ENCODER_LEFT_A);
  bool stateENCB = digitalRead(ENCODER_LEFT_B);
  if(stateENCA && stateENCB && aFlagLeft) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPosLeft --; //decrement the encoder's position count
    bFlagLeft = 0; //reset flags for the next turn
    aFlagLeft = 0; //reset flags for the next turn
  }
  else if (stateENCA && !stateENCB) bFlagLeft = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  interrupts(); //restart interrupts
}

void EncoderLBISR(){
noInterrupts(); //stop interrupts happening before we read pin values
  bool stateENCA = digitalRead(ENCODER_LEFT_A);
  bool stateENCB = digitalRead(ENCODER_LEFT_B);
  if(stateENCA && stateENCB && bFlagLeft) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPosLeft ++; //decrement the encoder's position count
    bFlagLeft = 0; //reset flags for the next turn
    aFlagLeft = 0; //reset flags for the next turn
  }
  else if (stateENCB && !stateENCA) aFlagLeft = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  interrupts(); //restart interrupts

}

void EncoderRAISR(){
  bool stateENCA = digitalRead(ENCODER_RIGHT_A);
  bool stateENCB = digitalRead(ENCODER_RIGHT_B);
  if(stateENCA && stateENCB && aFlagRight) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPosRight --; //decrement the encoder's position count
    bFlagRight = 0; //reset flags for the next turn
    aFlagRight = 0; //reset flags for the next turn
  }
  else if (stateENCA && !stateENCB) bFlagRight = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  interrupts(); //restart interrupts
}

void EncoderRBISR(){
  bool stateENCA = digitalRead(ENCODER_RIGHT_A);
  bool stateENCB = digitalRead(ENCODER_RIGHT_B);
  if(stateENCA && stateENCB && bFlagRight) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPosRight ++; //decrement the encoder's position count
    bFlagRight = 0; //reset flags for the next turn
    aFlagRight = 0; //reset flags for the next turn
  }
  else if (stateENCB && !stateENCA) aFlagRight = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  interrupts(); //restart interrupts
}

int getEncoderPositionLeft(){
  return encoderPosLeft;
}

int getEncoderPositionRight(){
  return encoderPosRight;
}

void resetEncoders(){
  encoderPosLeft = 0;
  encoderPosRight = 0;
}








