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
#define L_MOTOR_FORWARD 6 // PMW
#define L_MOTOR_REV 7 // PMW
#define R_MOTOR_FORWARD 8 // PMW
#define R_MOTOR_REV 9 // PMW

// Motor and PWM Values
#define motorInput A0
#define Motor1F 6 // PMW
#define Motor1B 7 // PMW
#define Motor2F 8 // PMW
#define Motor2B 9 // PMW
#define PWM_FREQ1  100
#define PWM_FREQ2  100

#define POT1 A0
#define POT2 A1



// Tape Following Values
#define TAPE_L A2
#define TAPE_R A3

// Claw Values
#define CLAW_SERVO 4 // PMW Pin
#define CLAW_REF A4
#define CLAW_MAG A5
#endif