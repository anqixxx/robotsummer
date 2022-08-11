#ifndef ClawClass_h
#define ClawClass_h

#include <Arduino.h>
#include "hardware_def.h"
#include  "Servo.h"

class ClawClass
{
    public:
    ClawClass(byte pinServo);

    void reposition(int pos);
    int getReflectance();
    int getHall();
    void irBoostOn();
    void irBoostOff();

    int reflectance;
    int hall;
    int position;

    private:
    Servo servo;


};

#endif