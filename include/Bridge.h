#ifndef Bridge_h
#define Bridge_h

#include <Arduino.h>
#include "hardware_def.h"
#include  "Servo.h"

class Bridge
{
    public:
    Bridge(byte pin);

    void setup();

    void deploy();


    private:
    Servo latch;


};

#endif