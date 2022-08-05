#include <Arduino.h>
#include "hardware_def.h"
#include  "Servo.h"
#include "Bridge.h"

Bridge::Bridge(byte pin)
{
    latch.attach(pin);
}

void Bridge::setup(){
    latch.write(BRIDGE_CLOSE);
}

void Bridge::deploy(){
    int delayFactor = 5;

    for (int pos = BRIDGE_CLOSE; pos >= 5; pos -= 1) { // goes from closed configuration to dploy
     latch.write(pos);
     delay(delayFactor);
  }
}

