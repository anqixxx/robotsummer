// Claw Functions, reacting to sensors, placement of the claw, speed of the claw, location of the claw
#ifndef CLAW_H
#define CLAW_H

#include <Arduino.h>
#include <Servo.h>

//Enter in function definitions for claw movement, with the code located in claw.cpp

void servo_setup();

void servo_loop();

void servo_position(int position);

void claw_loop();


#endif
