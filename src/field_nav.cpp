// Field navigation, using reflectance, ultrasonic, etc to detect objects (other than treasure)
#include "field_nav.h"
#include "hardware_def.h"
#include "motor_drive.h"
#include "serial_coms.h"
#include "tape_follow.h"
#include "ir_sensors.h"



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
    while (heading == NO_BEACON_FOUND && (tNow-tStart) <  1000); // Stop trying after one second

    if (heading == NO_BEACON_FOUND){
        // Drive forward for 100 ms and then search again
        tStart = millis();
        while (tNow - tStart < 100){
        drive(FAST,FAST);
        tNow = millis();
        }
        searchForBeacon(dir);
    }

    // Now that beacon is found, home in a bit more on it at slower rotation
    tStart = millis(); // Start time of search
    do{
        if (dir == RIGHT){
            drive(SLOW, -SLOW); // Turn right
        } else {
            drive(-SLOW, SLOW); // Turn left
    }
    heading = getHeadingToBeacon(TEN_KHZ, TEN_KHZ_READINGS, SAMPLE_PERIOD, STANDARD_OFFSETS);
    tNow = millis();
    }
    while ((tNow-tStart) <  40); // Only rotate another 40 ms to not overshoot

    // The robot should be locked onto the beacon now and ready for following

}


