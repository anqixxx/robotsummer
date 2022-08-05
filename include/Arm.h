#ifndef arm_h
#define arm_h

#include <Arduino.h>
#include "hardware_def.h"
#include  "Servo.h"
#include "ClawClass.h"
#include "Stepper.h"

#define HORZ_WAIT_TIME 2000 // The time to wait before stopping a horizontal
                            // Motion that didn't trigger limit switch

#define ARM_HEIGHT_HIGH 3500 

class Arm
{
    public:
    Arm(byte pinPancakeF, byte pinPancakeB, 
    byte pinLimitStart, byte pinLimitEnd, 
    byte pinShoulderTop, byte pinShoulderBottom);

    void setup();
    void moveClawIn();
    void moveClawOut();
    void setAngle(int pos);


    int angle;
    boolean isOut = true;


    private:
    Servo top;
    Servo bottom;
    byte horzLeadForward;
    byte horzLeadBackward;
    byte limitStart;
    byte limitEnd;



};

#endif