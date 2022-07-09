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
uint32_t dutycycle();

#define test A0
#define testLED 6
#define PWM_FREQ1  100
#define PWM_FREQ2  100

DuePWM pwm( PWM_FREQ1, PWM_FREQ2 );
uint32_t dutycycle();
void dutyLoop();

void setup() 
{
    Serial.begin(SERIAL_RATE);
    Serial.print("Hello");

    pwm.setFreq1( PWM_FREQ1);
    pwm.setFreq2( PWM_FREQ2 );

    // Setup PWM Once (Up to two unique frequencies allowed
    //-----------------------------------------------------    
    pwm.pinFreq1( 6 );  // Pin 6 freq set to "pwm_freq1" on clock A
    pwm.pinFreq2( 7 );  // Pin 7 freq set to "pwm_freq2" on clock B
    pwm.pinFreq2( 8 );  // Pin 8 freq set to "pwm_freq2" on clock B
    pwm.pinFreq2( 9 );  // Pin 9 freq set to "pwm_freq2" on clock B
}

void loop() 
{ 
  int testRead = analogRead(test);
  // blueLoop(testRead, analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4));
  
  Serial.print("Analog reading is: ");
  Serial. println();
  Serial.print(testRead);

  uint32_t pwm_duty = dutycycle();
  Serial.print("Duty Cycle is: ");
  Serial.print(pwm_duty);
 Serial. println();

  pwm.pinDuty( 6, pwm_duty );
  delay(1000);
}

uint32_t dutycycle(){
  Serial.print("Started duty cycle");
  Serial. println();

  if (  analogRead(test) < 510){
    //Reverse
    double scaledVal = (analogRead(test) - 510.0)*(-255.0/509.0);
    Serial.print("Less than 510, Scaled Val is: ");
    Serial. println();
    Serial.print(scaledVal);
    return  (uint32_t) scaledVal;

  } else if(  analogRead(test) < 514){
    Serial.print("0 Value");
    return 0;
  } else{
    //Forward
    double scaledVal = (analogRead(test) - 514.0)*(255.0/509.0);
    Serial.print("More than 514, Scaled Val is: ");
    Serial. println();
    Serial.print(scaledVal);
    return (uint32_t) scaledVal;
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