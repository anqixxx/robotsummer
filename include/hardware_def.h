#ifndef HARDWARE_DEF_H
#define HARDWARE_DEF_H

#include <Arduino.h>

// Serial 3 is the TX RX port #3 (connect to BT)
#define HC06 Serial3

#define SERIAL_RATE 9600
#define HC06_RATE 9600

#define motorInput A0
#define Motor1F 6
#define Motor1B 7
#define PWM_FREQ1  100
#define PWM_FREQ2  100

#endif