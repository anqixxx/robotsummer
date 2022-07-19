//Motor Drive Functions, rotary encoder and wheel motor directions
#include "motor_drive.h"
#include "hardware_def.h"
#include "DuePWM.h"


DuePWM pwm( PWM_FREQ1, PWM_FREQ2 );

/**
 * Sets up PWM library and functions 
 * Insert into main setup to run
 * */
void pwm_setup(){
    pwm.setFreq1( PWM_FREQ1);
    pwm.setFreq2( PWM_FREQ2 );

    // Setup PWM Once (Up to two unique frequencies allowed
    pwm.pinFreq1( 6 );  // Pin 6 freq set to "pwm_freq1" on clock A
    pwm.pinFreq1( 7 );  // Pin 7 freq set to "pwm_freq1" on clock B
    pwm.pinFreq2( 8 );  // Pin 8 freq set to "pwm_freq2" on clock B
    pwm.pinFreq2( 9 );  // Pin 9 freq set to "pwm_freq2" on clock B
}

// Drive the robot by applying a speed to each motor
// Inputs: left is a speed between -255 and 255 indicating forward or reverse
//        right is a speed between -255 and 255 indicating forward or reverse
void drive(int left, int right) {

    //Take speedLeft as Motor 1, speedRight as Motor 2
    if (left >= 0) {
        pwm.pinDuty(L_MOTOR_FORWARD, left);
        pwm.pinDuty(L_MOTOR_REV, 0);
    } else {
        pwm.pinDuty(L_MOTOR_FORWARD, 0);
        pwm.pinDuty(L_MOTOR_REV, -1*left);
    }
    
    if (right >= 0) {
        pwm.pinDuty(R_MOTOR_FORWARD, right);
        pwm.pinDuty(R_MOTOR_REV, 0);   
    } else {
        pwm.pinDuty(R_MOTOR_FORWARD, 0);
        pwm.pinDuty(R_MOTOR_REV, -1*right);
    }
}




