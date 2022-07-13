//Motor Drive Functions, rotary encoder and wheel motor directions
#include "motor_drive.h"
#include "hardware_def.h"
#include "DuePWM.h"

DuePWM pwm( PWM_FREQ1, PWM_FREQ2 );

// Now define the main code for the functions listed in the header file
int dutycycle(){
  Serial.print("Started duty cycle");
  Serial. println();
  double motorInVal = analogRead(motorInput);

  if (  motorInVal < 510){
    //Backward
    double scaledVal = (motorInVal - 510.0)*(255.0/509.0);
    // Serial.print("Less than 510, Scaled Val is: ");
    // Serial.println();
    // Serial.print(scaledVal);
    return scaledVal; //uint32_t will be negative for backward

  } else if(  motorInVal < 514){
    Serial.print("0 Value");
    return 0;

  } else{
    //Forward
    double scaledVal = (motorInVal - 514.0)*(255.0/509.0);
    // Serial.print("More than 514, Scaled Val is: ");
    // Serial.println();
    // Serial.print(scaledVal);
    return scaledVal; //uint32_t will be negative for forward
  }
}


void dutyLoop(){
    uint32_t pwm_duty = 0; 
    pwm.pinDuty( 6, pwm_duty );
    delay(1000);

   pwm_duty = 100;
   pwm.pinDuty( 6, pwm_duty); 
   delay(1000);
   
   pwm_duty = 255;
   pwm.pinDuty( 6, pwm_duty); 
   delay(1000);
}

/**
 * Sets up PWM library and functions 
 * Insert into main setup to run
 * */
void pwm_setup(){
    pwm.setFreq1( PWM_FREQ1);
    pwm.setFreq2( PWM_FREQ2 );

    // Setup PWM Once (Up to two unique frequencies allowed
    pwm.pinFreq1( 6 );  // Pin 6 freq set to "pwm_freq1" on clock A
    pwm.pinFreq1( 7 );  // Pin 7 freq set to "pwm_freq2" on clock B
    pwm.pinFreq2( 8 );  // Pin 8 freq set to "pwm_freq2" on clock B
    pwm.pinFreq2( 9 );  // Pin 9 freq set to "pwm_freq2" on clock B
}

// Insert into loop to run, used to test lights
void pwm_loop(){
    int pwm_duty_int = dutycycle();
  
  // Serial.print("Duty Cycle is: ");
  // Serial.print(pwm_duty_int);
  // Serial. println();

  if(pwm_duty_int < 0){
    uint32_t pwm_duty = pwm_duty_int*(-1);
    // Serial.println();
    // Serial.print(pwm_duty);

    // Reverse
    pwm.pinDuty( Motor1F, 0 );
    pwm.pinDuty( Motor1B, pwm_duty );
    // delay(1000);
  }
  else{
    // Forward
    uint32_t pwm_duty = pwm_duty_int;
    // Serial.println();
    // Serial.print(pwm_duty);

    pwm.pinDuty( Motor1F, pwm_duty );
    pwm.pinDuty( Motor1B, 0 );
    // delay(1000);
  }
}



// test speed and drive with motor
void drive(int speedLeft, int speedRight) {
    //Take speedLeft as Motor 1, speedRight as Motor 2
    if (speedLeft >= 0) {
        analogWrite(Motor1F, speedLeft);
        analogWrite(Motor1B, 0);
    } else {
        analogWrite(Motor1F, 0);
        analogWrite(Motor1B, -1*speedLeft);
    }
    
    if (speedRight >= 0) {
        analogWrite(Motor2F, speedRight);
        analogWrite(Motor2B, 0);   
    } else {
        analogWrite(Motor2F, 0);
        analogWrite(Motor2B, -1*speedRight);
    }
}
