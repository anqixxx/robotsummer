// Claw Functions, reacting to sensors, placement of the claw, speed of the claw, location of the claw
#ifndef CLAW_H
#define CLAW_H

#include <Arduino.h>
#include <Servo.h>

//Enter in function definitions for claw movement, with the code located in claw.cpp


int sweep(int start, int end);

void retrieveTreasure();

void treasureDetected(int start, int end, int treasure);

void treasureSequence(int start, int end);

void stopBackwardPancakeMotor();

void stopForwardPancakeMotor();

void arm_servo_pos(int position);

void claw_test_value();

void irOn();

#endif
