// Field navigation, using reflectance, ultrasonic, etc to detect objects (other than treasure)
#include "field_nav.h"
#include "hardware_def.h"
#include "motor_drive.h"
#include "serial_coms.h"
#include "tape_follow.h"
#include "ir_sensors.h"
#include "Encoders.h"

#define OFFSET 6


// Executes a search for the IR beacon until one of the sensors gets a signal
// Set a time limit to avoid an infite loop
void searchForBeacon(bool dir){
    int tStart = millis(); // Start time of search
    int tNow = 0; // Current time
    int heading = NO_BEACON_FOUND;
    do{
        if (dir == RIGHT){
            drive(MEDIUM, -MEDIUM); // Turn right
        } else {
            drive(-MEDIUM, MEDIUM); // Turn left
    }
    // get heading to 10khz
    heading = getHeadingToBeacon(TEN_KHZ, TEN_KHZ_READINGS, SAMPLE_PERIOD, STANDARD_OFFSETS);
    tNow = millis();
    }
    while (heading == NO_BEACON_FOUND && (tNow-tStart) <  2000); // Stop trying after one second

    // Now that beacon is found, home in a bit more on it at slower rotation
    tStart = millis(); // Start time of search
    do{
        if (dir == RIGHT){
            drive(SLOW, -SLOW); // Turn right
        } else {
            drive(-SLOW, SLOW); // Turn left
    }
    tNow = millis();
    }
    while ((tNow-tStart) <  40); // Only rotate another 40 ms to not overshoot

    // The robot should be locked onto the beacon now and ready for following

}

// Left is positive angle
// Encoder Rotation
void rotate(int angle){

    int target = getEncoderPositionRight() + (angle*25)/90;

    int start = millis();
    int speed = SLOW +30;
    if(angle > 0){
    while (getEncoderPositionRight() < target && millis()-start < 2000){
        
        drive (-speed, speed);
    }
    } else {
            while (getEncoderPositionRight() > target && millis()-start < 2000){
        
        drive (speed, -speed);
    }
    }




    drive(0,0);
}

void backupToTreasure(double dist){
    if (dist > 28){
        dist = 28;
    }
    double conversionFactor = 4/3;
    double offset = OFFSET;
    resetEncoders();
    int start = millis();
    dist = (dist - offset) * conversionFactor;
    while(getEncoderPositionLeft() > -dist && millis()-start < 1500){
    drive(-MEDIUM,-MEDIUM);
    }
    drive(0,0);    

}

void forwardFromTreasure(double dist){
    if (dist > 28){
        dist = 28;
    }

    double conversionFactor = 4/3;
    double offset = OFFSET;
    resetEncoders();
    int start = millis();
    dist = (dist - offset) * conversionFactor;
    while(getEncoderPositionLeft() < dist && millis()-start < 1500){
        drive(MEDIUM,MEDIUM);
    }
    drive(0,0);    
}

void crossChickenwire()
{
  while (onChickenWire()){
      drive(125, 180);
  }

  drive(0, 0);
  drive(250,250);
  delay(400);
  drive(0,0);

  while (offTape())
  {
    drive(0, 150);
  }
}

bool onChickenWire(){
    int accum = 0;

    if ((analogRead(TAPE_FAR_L) > REF_THRES)){
        accum += 1;
    }
    if ((analogRead(TAPE_FAR_R) > REF_THRES)){
        accum += 1;
    }
    if ((analogRead(TAPE_L) >  REF_THRES)){
        accum += 1;
    }
    if ((analogRead(TAPE_R) > REF_THRES)){
        accum += 1;
    }


    // Diff cases:
    // Both outer are on, or one outer and both inner
    if ( accum > 2)
    {
        return(true);
    } 
    else
    {
        return(false);
    }
}
