#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>

//Enter in function definitions for claw movement, with the code located in claw.cpp
void blueStart();
void blueLoop(int reading1, int reading2,  int reading3,  int reading4,  int reading5);

#endif