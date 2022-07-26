#ifndef SERIAL_COMS_H
#define SERIAL_COMS_H

#include <Arduino.h>

// Initialize serial port defined in Hardware Definitions header
// SERIAL_OUT
void setupSerialPort();

// Send a list of comma seperated integer values over the serial port defined in Hardware Definitions header
// SERIAL_OUT
void outputCSV(int reading1, int reading2,  int reading3,  int reading4,  int reading5);

// Setup the OLED display
void setupOLED();

#endif