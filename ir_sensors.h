#ifndef IR_SENSORS_H
#define IR_SENSORS_H

#include <Arduino.h>

//Enter in function definitions for ir sensing, with the code located in ir_sensors.cpp
void readIR();

void setupFrequencyReference();

void displayArray(double vals[]);

bool takeAnalogReadings(uint16_t* p_numReadings = nullptr, double** p_analogVals = nullptr, byte pin = PA7);

double correlateTo1kz(double[]);

double takeSignalSample(double** p_analogVals = nullptr, byte pin = PA7);

#endif
