// Motor Drive Functions, rotary encoder and wheel motor directions

#ifndef MOTOR_DRIVE_H
#define MOTOR_DRIVE_H

#include <Arduino.h>

//Enter in function definitions for motor drive functions, with the code located in motor_drive.cpp
int dutycycle();
void dutyLoop();
void pwm_setup();
void pwm_loop();
void drive(int speedLeft, int speedRight);

#endif
