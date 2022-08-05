// Claw Functions, reacting to sensors, placement of the claw, speed of the claw, location of the claw
#ifndef CLAW_H
#define CLAW_H

#include <Arduino.h>
#include <Servo.h>

//Enter in function definitions for claw movement, with the code located in claw.cpp

void claw_loop();

void test_claw_loop();

void stopBackwardPancakeMotor();

void stopForwardPancakeMotor();

void claw_test_value();

void arm_servo_pos(int position);

#endif
