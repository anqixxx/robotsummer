#ifndef HARDWARE_DEF_H
#define HARDWARE_DEF_H

#include <Arduino.h>

// Serial 3 is the TX RX port #3 (connect to BT)
#define HC06 Serial3

// Serial definitions for bluetooth
#define SERIAL_RATE 9600
#define HC06_RATE 9600

// Motor and PWM Values
#define motorInput A0
#define Motor1F 6
#define Motor1B 7
#define Motor2F 8
#define Motor2B 9
#define PWM_FREQ1  100
#define PWM_FREQ2  100

#endif