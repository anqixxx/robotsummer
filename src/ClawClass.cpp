#include <Arduino.h>
#include "hardware_def.h"
#include "Servo.h"
#include "ClawClass.h"



ClawClass::ClawClass(byte pinServo)
{
    servo.attach(pinServo);
}

void ClawClass::reposition(int pos){
    if (pos < CLAW_MIN){
        pos = CLAW_MIN;
    }
    if (pos > CLAW_MAX){
        pos = CLAW_MAX;
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

