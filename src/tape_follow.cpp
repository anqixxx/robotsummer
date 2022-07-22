// Tape following functions, using reflectance sensors to reorient ourselves
#include "tape_follow.h"
#include "hardware_def.h"
#include "motor_drive.h" // until I can figure out how to compartmentalize this
#define REF_THRES 300
#define FAST  180 // Variables for speed, test to find the one best for us
#define SLOW 65

// Global variables for tape following
bool onTapeL = true, onTapeR = true, lastL = true, lastR  = true;


// Now define the main code for the functions listed in the header file
void lineFollow(){
    // Checks to see if the reflectance sensor is on tape or not
    onTapeL = (analogRead(TAPE_L) > REF_THRES);
    onTapeR = (analogRead(TAPE_R) > REF_THRES);

    if (onTapeL || onTapeR) {
        // Either sensor on tape
        if (onTapeL && onTapeR) {
            drive(FAST, FAST); 
        } else if (!onTapeL && onTapeR)
        {
            drive(FAST, SLOW);
        } else if (onTapeL && !onTapeR)
        {
            drive(SLOW, FAST);
        }

        lastL = onTapeL;
        lastR = onTapeR;
    }
    else {
        // Both sensors off tape, without updating last values until one sensor is on tape again
        if (lastL) {
            // Pushes back against left drift, allows for course correction
            drive(-SLOW, FAST);
        } else if (lastR)
        {
            // Pushes back against right drift, allows for course correction
            drive(FAST, -SLOW);
        } else
        {
            drive(-SLOW, -SLOW); // no way to ever get to this line
        }  
    }
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//TODO Move tape following back into the tape_follow.cpp file and test that it is functioning in order