#ifndef IR_SENSORS_H
#define IR_SENSORS_H

#include <Arduino.h>

//Enter in function definitions for ir sensing, with the code located in ir_sensors.cpp
void updateReference(int *s, int *c, int cyclePosition);

double takeSquareSignalSample( byte pin, int numReadings, int sampleInterval);

#endif
