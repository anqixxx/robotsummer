#ifndef SONAR_H
#define SONAR_H

#include <Arduino.h>

void setupSonar();
double readSonar(int sonarID);
bool treasure_detect(int sonarID, int threshold);

#endif