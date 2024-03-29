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
#define L_MOTOR_FORWARD 8// PMW
#define L_MOTOR_REV 9// PMW
#define R_MOTOR_FORWARD 7 // PMW
#define R_MOTOR_REV 6 // PMW

// Drive Speeds
#define FULL 255
#define FAST  180 // Variables for speed, test to find the one best for us, used to be 200
#define MEDIUM 100 
#define SLOW 75 // used to be 80

// Turning directions
#define RIGHT 1
#define LEFT 0
#define CW 0
#define CCW 1

// Stepper Motor Pins
#define STEPPER_DIR 26  // GREEN WIRE
#define STEPPER_STEP 28
#define STEPPER_SLEEP 30
#define STEPPER_LIMIT 37 

#define STEPPER_LOW_POS 0
#define STEPPER_HIGH_POS 3400 // Figure this one out with hardware testing
#define STEPPER_TREASURE_POS 2000

// Encoder Pins
#define ENCODER_LEFT_A 49
#define ENCODER_LEFT_B 51
#define ENCODER_RIGHT_A 47
#define ENCODER_RIGHT_B 45

// Tape Following Values
#define TAPE_FAR_L A8
#define TAPE_L A9
#define TAPE_R A10
#define TAPE_FAR_R A11
#define REF_THRES 300

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
#define PID_P_TUNING 21
#define PID_I_TUNING 9

// Robot Arm

#define ARM_SERVO_TOP 3 // PMW Pin
#define ARM_SERVO_BOTTOM 2 // PMW Pin
#define ARM_LIMIT_END 39 //Digital
#define ARM_LIMIT_START 41 // Digital       
#define PANCAKE_FOR 12 // PWM 
#define PANCAKE_BACK 11 // PWM 

#define ARMSTART 40
#define ARMEND 140


// Claw Values
#define CLAW_SERVO 4 // PMW Pin
#define CLAW_REF A2
#define CLAW_MAG A3
#define CLAW_OPEN 140 
#define CLAW_CLOSE 50
#define IR_BOOST 38 // pin 38


// Sonars
#define RIGHT_TRIG_PIN 52 // Digital
#define RIGHT_ECHO_PIN 50 // Digital
#define LEFT_TRIG_PIN 36 // Digital
#define LEFT_ECHO_PIN 34 // Digital

//Bridge Deployment
#define BRIDGE_PIN 42     // Pin for the bridge servo
#define BRIDGE_CLOSE 140 // servo setting for a closed latch

#endif      