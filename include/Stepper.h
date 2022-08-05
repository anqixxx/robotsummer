
#ifndef Stepper_H
#define Stepper_H

#include <Arduino.h>
#include "AccelStepper.h"

/*
* Stepper Functions
*/
void setupStepper();

// ISR to prevent stepper failure
void resetStepper();

// Stepper calibration to set position to correct 0 value
void calibrateStepper();

// Move stepper to a preset position 0-3400
void moveStepper(int position);

// Returns the current position of the stepper
int getCurrentStepperPos();

#endif