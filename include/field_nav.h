// Tape following functions, using reflectance sensors to reorient ourselves

#ifndef FIELD_NAV_H
#define FIELD_NAV_H

#include <Arduino.h>

// Search for an IR beacon by spinning in a direction, input RIGHT for right and LEFT for left
void searchForBeacon(bool dir);
void rotate(int angle);
void backupToTreasure(double dist);
void forwardFromTreasure(double dist);
void crossChickenwire();
bool onChickenWire();


#endif
