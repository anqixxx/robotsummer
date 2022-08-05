#include <Arduino.h>
#include "hardware_def.h"
#include "Servo.h"
#include "Arm.h"
#include "Stepper.h"
#include "ClawClass.h"

Arm::Arm(byte pinPancakeF, byte pinPancakeB,
         byte pinLimitStart, byte pinLimitEnd,
         byte pinShoulderTop, byte pinShoulderBottom)
{
    top.attach(pinShoulderTop);
    bottom.attach(pinShoulderBottom);
    horzLeadForward = pinPancakeF;
    horzLeadBackward = pinPancakeB;
    limitStart = pinLimitStart;
    limitEnd = pinLimitEnd;
    setup();
}

void Arm::setup()
{
    pinMode(horzLeadBackward, OUTPUT);
    pinMode(horzLeadForward, OUTPUT);
    pinMode(limitEnd, INPUT_PULLUP);
    pinMode(limitStart, INPUT_PULLUP);
    pinMode(horzLeadBackward, OUTPUT);
    pinMode(horzLeadForward, OUTPUT);

    moveClawOut();   // Move the claw out for stepper calibration
    setupStepper();    // Initiate the stepper with calibration
    moveStepper(ARM_HEIGHT_HIGH); // Set the arm height to high
}

// Move the claw back in, stops the attempt after 2 seconds if not successful
void Arm::moveClawIn(){
    int startTime = millis();
    while (isOut && (millis()-startTime < HORZ_WAIT_TIME)){
        digitalWrite(horzLeadBackward, HIGH);
        digitalWrite(horzLeadForward, LOW);
        isOut = (digitalRead(limitStart) == HIGH ); // Read the limit switch
    }
    digitalWrite(horzLeadBackward, LOW);
    digitalWrite(horzLeadForward, LOW);
    isOut = false;
}

// Move the claw out, stops the attempt after 2 seconds if not successful
void Arm::moveClawOut(){
    int startTime = millis();
    while (!isOut && (millis()-startTime < HORZ_WAIT_TIME)){
        digitalWrite(horzLeadBackward, LOW);
        digitalWrite(horzLeadForward, HIGH);
        isOut = (digitalRead(limitEnd) == LOW ); // Read the limit switch
    }
    digitalWrite(horzLeadBackward, LOW);
    digitalWrite(horzLeadForward, LOW);
    isOut = true;
}


// Move the arm to a set position
void Arm::setAngle(int pos){
    angle = pos;
    bottom.write(pos);              
    top.write(180-pos);
}