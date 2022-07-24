#ifndef HARDWARE_DEF_H
#define HARDWARE_DEF_H

#include <Arduino.h>

// Serial 3 is the TX RX port #3 (connect to BT)
// Serial is the USB programming port
#define SERIAL_OUT Serial
#define SERIAL_RATE 9600

// Pins for RC coms
#define CE 23
#define CNS 22
 
// Motor frequencies and pins
#define PWM_FREQ1  100
#define PWM_FREQ2  100
#define L_MOTOR_FORWARD 6
#define L_MOTOR_REV 7
#define R_MOTOR_FORWARD 8
#define R_MOTOR_REV 9

// Motor and PWM Values
#define motorInput A0
#define Motor1F 6
#define Motor1B 7
#define Motor2F 8
#define Motor2B 9
#define PWM_FREQ1  100
#define PWM_FREQ2  100

// Drive Speeds
#define FULL 255
#define FAST  180 // Variables for speed, test to find the one best for us
#define MEDIUM 100
#define SLOW 65

// Turning directions
#define RIGHT 1
#define LEFT 0

#define POT1 A0
#define POT2 A1

// Tape Following Values
#define TAPE_L A2
#define TAPE_R A3
#define TAPE_FAR_L A4
#define TAPE_FAR_R A5

// IR Beacon Readings
#define IR_BEACON A6
#define IR_EN 24
#define IR_S3 26
#define IR_S2 28
#define IR_S1 30
#define IR_ARRAY_SIZE 8
#define BEACON_THRESHHOLD 12 // Threshold value between noise and signal for beacon following

#define NO_BEACON_FOUND -8 // Flag for heading indicating that no beacon is found
#define TOO_MANY_SIGNALS 8 // Flag for too many IR sensors being pinged by beacon

#define BEACON_PID_OUTPUT_LIMIT 100 // Sets the limits for positive or negative on the PID output



#endif