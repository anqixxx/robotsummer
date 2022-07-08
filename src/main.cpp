#include "Arduino.h"
#include "Wire.h"

#include "hardware_def.h"
#include "tape_follow.h"
#include "ir_sensors.h"
#include "motor_drive.h"
#include "field_nav.h"
#include "claw.h"
#include "arm.h"
#include "treasure.h"
#include "bluetooth.h"
#include "DuePWM.h"



double sigAmp = 0;
int count1 = 0;
int count2 = 0;

#define PWM_FREQ1  100
#define PWM_FREQ2  5000

// Initializes DUE
DuePWM pwm( PWM_FREQ1, PWM_FREQ2 );

void setup() 
{
    //uint32_t  pwm_duty = 32767;
    uint32_t pwm_duty = 127; // 50% duty cycle

    pwm.setFreq1( PWM_FREQ1 );
    pwm.setFreq2( PWM_FREQ2 );

    // Setup PWM Once (Up to two unique frequencies allowed
    //-----------------------------------------------------    
    pwm.pinFreq1( 6 );  // Pin 6 freq set to "pwm_freq1" on clock A
    // pwm.pinFreq2( 7 );  // Pin 7 freq set to "pwm_freq2" on clock B
    // pwm.pinFreq2( 8 );  // Pin 8 freq set to "pwm_freq2" on clock B
    // pwm.pinFreq2( 9 );  // Pin 9 freq set to "pwm_freq2" on clock B
      
    // Write PWM Duty Cycle Anytime After PWM Setup
    //-----------------------------------------------------    
    pwm.pinDuty( 6, pwm_duty );  // 50% duty cycle on Pin 6
    // pwm.pinDuty( 7, pwm_duty );  // 50% duty cycle on Pin 7
    // pwm.pinDuty( 8, pwm_duty );  // 50% duty cycle on Pin 8
    // pwm.pinDuty( 9, pwm_duty );  // 50% duty cycle on Pin 9

    delay(30000);  // 30sec Delay; PWM signal will still stream
    
    // Change frequency 1 on PWM clock A to 10kHz
    // Change frequency 2 on PWM clock B to 20kHz
    pwm.setFreq1( 10000 );
    pwm.setFreq2( 20000 );
    
    pwm_duty = 255; // 100% duty cycle
    pwm.pinDuty( 6, pwm_duty );  // 100% duty cycle on Pin 6
    // pwm.pinDuty( 7, pwm_duty );  // 100% duty cycle on Pin 7
    // pwm.pinDuty( 8, pwm_duty );  // 100% duty cycle on Pin 8
    // pwm.pinDuty( 9, pwm_duty );  // 100% duty cycle on Pin 9

    // Pin 6 should now show 10kHz and 7, 8, and 9, 20kHz respectively.
    // Duty cycles remain unchanged.
    
    delay(30000);  // 30sec Delay; PWM signal will stream new freq
    
    // Force PWM Stop On All Pins
    //-----------------------------    
    pwm.stop( 6 );
    // pwm.stop( 7 );
    // pwm.stop( 8 );
    // pwm.stop( 9 );
}

void loop() 
{  

}
