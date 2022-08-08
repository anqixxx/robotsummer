// Treasure detecting functions, from ultrasonic sensors, hall effect sensors, and reflectance sensors

#ifndef TREASURE_H
#define TREASURE_H

#include <Arduino.h>

//Enter in function definitions for treasure detection, with the code located in treasure.cpp
void ultra_loop();
void ultra_setup();

void test_ultra_setup(int TRIG_PIN, int ECHO_PIN);
void test_ultra_loop(int TRIG_PIN, int ECHO_PIN);

double sonar_cm(int TRIGGER, int ECHO);
double sonar_in(int TRIGGER, int ECHO);
bool treasure_detect(int TRIGGER, int ECHO, int TREAS_THRES);
int treasureNav();

#endif
