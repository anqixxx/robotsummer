#ifndef HARDWARE_DEF_H
#define HARDWARE_DEF_H

#include <Arduino.h>

// Serial 3 is the TX RX port #3 (connect to BT)
// Serial is the USB programming port
#define SERIAL_OUT Serial3
#define SERIAL_RATE 9600

// OLED Display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible

// Pins for RC coms
#define CE 23
#define CNS 22
 
// Motor frequencies and pins
#define PWM_FREQ1  100
#define PWM_FREQ2  100
#define L_MOTOR_FORWARD 6 // PMW
#define L_MOTOR_REV 7 // PMW
#define R_MOTOR_FORWARD 8 // PMW
#define R_MOTOR_REV 9 // PMW


// Drive Speeds
#define FULL 255
#define FAST  200 // Variables for speed, test to find the one best for us
#define MEDIUM 100
#define SLOW 80

// Turning directions
#define RIGHT 1
#define LEFT 0

#define POT1 A0
#define POT2 A1

// Tape Following Values
#define TAPE_FAR_L A8
#define TAPE_L A9
#define TAPE_R A10
#define TAPE_FAR_R A11

// IR Beacon Readings
#define TEN_KHZ 100  // period of 10khz in us
#define ONE_KHZ 1000  // period of 1khz in us
#define TEN_KHZ_READINGS 500
#define ONE_KHZ_READINGS 1000
#define SAMPLE_PERIOD 8       // 7 us sample period by default
#define STANDARD_OFFSETS 4    // number of parallel dot products

#define IR_BEACON A6
#define IR_EN 27
#define IR_S3 29
#define IR_S2 31
#define IR_S1 33
#define IR_ARRAY_SIZE 8
#define BEACON_THRESHHOLD 15 // Threshold value between noise and signal for beacon following

#define NO_BEACON_FOUND -8 // Flag for heading indicating that no beacon is found
#define TOO_MANY_SIGNALS 8 // Flag for too many IR sensors being pinged by beacon

#define PID_OUTPUT_LIMIT 100 // Sets the limits for positive or negative on the PID output



// Claw Values
#define CLAW_SERVO 4 // PMW Pin
#define CLAW_REF A2
#define CLAW_MAG A3
#define CLAW_END 53 //Digital
#define CLAW_START 51 //Digital
#define PANCAKE_FOR 49 //Digital
#define PANCAKE_BACK 47 //Digital
#define ARM_SERVO 5 // PMW Pin

#endif