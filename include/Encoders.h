#ifndef ENCODERS_h
#define ENCODERS_h

#include <Arduino.h>
#include "hardware_def.h"

void setupEncoders();

void EncoderLAISR();
void EncoderLBISR();
void EncoderRAISR();
void EncoderRBISR();


// Get the encoder positions
int getEncoderPositionLeft();
int getEncoderPositionRight();

// Reset both encoders
void resetEncoders();

#endif
