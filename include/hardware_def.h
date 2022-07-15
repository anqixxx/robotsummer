#ifndef HARDWARE_DEF_H
#define HARDWARE_DEF_H

#include <Arduino.h>

// Serial 3 is the TX RX port #3 (connect to BT)
#define HC06 Serial3

// Serial definitions for bluetooth
#define SERIAL_RATE 9600
#define HC06_RATE 9600

// Pins for RC coms
#define CE 53
#define CNS 52

// Motor frequencies and pins
#define PWM_FREQ1  100
#define PWM_FREQ2  100
#define L_MOTOR_FORWARD 2
#define L_MOTOR_REV 3
#define R_MOTOR_FORWARD 4
#define R_MOTOR_REV 5

// Motor and PWM Values
#define motorInput A0
#define Motor1F 6
#define Motor1B 7
#define Motor2F 8
#define Motor2B 9
#define PWM_FREQ1  100
#define PWM_FREQ2  100
#define FAST 200 // Variables for speed, test to find the one best for us
#define SLOW 150



// Tape Following Values
#define TAPE_L A1
#define TAPE_R A2

#endif