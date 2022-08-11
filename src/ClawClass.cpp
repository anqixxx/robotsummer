#include <Arduino.h>
#include "hardware_def.h"
#include "Servo.h"
#include "ClawClass.h"

ClawClass::ClawClass(byte pinServo)
{
    servo.attach(pinServo);
    servo.write(CLAW_OPEN);
    pinMode(IR_BOOST, OUTPUT);
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

void ClawClass::irBoostOn(){
    digitalWrite(IR_BOOST, HIGH);
    delayMicroseconds(20);
}

void ClawClass::irBoostOff(){
    digitalWrite(IR_BOOST, LOW);
}

