#ifndef RC_RECIEVER_H
#define RC_RECIEVER_H

#include <Arduino.h>
void rcloop();

void setupRadio();

void resetData();

void updateMotorPWM(int leftMotor, int rightMotor);

void autoRadioLoop();

int potentiometerData();

int leftMotorData();

int rightMotorData();

bool manualOrAuto();

#endif