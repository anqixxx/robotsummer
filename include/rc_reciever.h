#ifndef RC_RECIEVER_H
#define RC_RECIEVER_H

#include <Arduino.h>
void rcloop();

void setupRadio();

void resetData();

int potentiometerData();

int leftMotorData();

int rightMotorData();

bool isManual();

#endif