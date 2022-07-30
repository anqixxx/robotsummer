#ifndef Encoders_h
#define Encoders_h

#include <Arduino.h>
#include "hardware_def.h"

void setupEncoders();

void EncoderLAISR();
void EncoderLBISR();
void EncoderRAISR();
void EncoderRBISR();

int getEncoderPositionLeft();
int getEncoderPositionRight();

void resetEncoders();

#endif
