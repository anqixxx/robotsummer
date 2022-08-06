#include <Arduino.h>
#include "hardware_def.h"
#include "Servo.h"
#include "ClawClass.h"

ClawClass::ClawClass(byte pinServo)
{
    servo.attach(pinServo);
    servo.write(CLAW_OPEN);
}

void ClawClass::reposition(int pos){
    if (pos < CLAW_CLOSE){
        pos = CLAW_CLOSE;
    }
    if (pos > CLAW_OPEN){
        pos = CLAW_OPEN;
    }
    servo.write(pos);
}

int ClawClass::getReflectance(){
    reflectance = analogRead(CLAW_REF);
    return reflectance;
}

int ClawClass::getHall(){
    hall = analogRead(CLAW_MAG);
    return hall;
}

