// Motor Drive Functions, rotary encoder and wheel motor directions

#ifndef MOTOR_DRIVE_H
#define MOTOR_DRIVE_H

#include <Arduino.h>

// Setup the two drive motors
void setupPWM();
// Drive functions from -255 to 255
void drive(int speedLeft, int speedRight);

#endif
