#include <Arduino.h>
#include "hardware_def.h"
#include "Servo.h"
#include "Arm.h"
#include "Stepper.h"
#include "ClawClass.h"
#include "serial_coms.h"

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
    pinMode(horzLeadBackward, OUTPUT);
    pinMode(horzLeadForward, OUTPUT);

    moveClawOut();   // Move the claw out for stepper calibration
    setAngle(110);


}

// Move the claw back in, stops the attempt after 2 seconds if not successful
void Arm::moveClawIn(){
    int limit = digitalRead(ARM_LIMIT_START);
    int startTime = millis();
    while (limit && (millis()-startTime < HORZ_WAIT_TIME)){
        digitalWrite(horzLeadBackward, HIGH);
        digitalWrite(horzLeadForward, LOW);
        limit = (digitalRead(ARM_LIMIT_START)== HIGH);
        outputCSV(limit,0,0,0,0);
    }
    digitalWrite(horzLeadBackward, LOW);
    digitalWrite(horzLeadForward, HIGH);
    delay(50);
    digitalWrite(horzLeadForward, LOW);
    isOut = false;
}

// Move the claw out, stops the attempt after 2 seconds if not successful
void Arm::moveClawOut(){
    int startTime = millis();
    int limit = digitalRead(ARM_LIMIT_END);
    while (limit && (millis()-startTime < HORZ_WAIT_TIME)){
        digitalWrite(horzLeadBackward, LOW);
        digitalWrite(horzLeadForward, HIGH);
        limit = (digitalRead(ARM_LIMIT_END)== HIGH);
                outputCSV(0,1,0,0,0);
    }
    digitalWrite(horzLeadBackward, HIGH);
    digitalWrite(horzLeadForward, LOW);
    delay(50);
    digitalWrite(horzLeadBackward, LOW);
    isOut = true;
}


// Move the arm to a set position
void Arm::setAngle(int pos){
    angle = pos;
    bottom.write(pos);              
    top.write(180-pos);
}