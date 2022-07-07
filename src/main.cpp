#include "Arduino.h"
#include "Wire.h"

#include "hardware_def.h"
#include "tape_follow.h"
#include "ir_sensors.h"
#include "motor_drive.h"
#include "field_nav.h"
#include "claw.h"
#include "arm.h"
#include "treasure.h"

#define SERIAL_RATE 115200
#define HC06_RATE 9600

// Serial 3 is the TX RX port #3 (connect to BT)
#define HC06 Serial3

double sigAmp = 0;
int count1 = 0;
int count2 = 0;

void updateReference(int *s, int *c, int cyclePosition);
double takeSquareSignalSample( byte pin, int numReadings, int sampleInterval);


void setup() {
  Serial.begin(SERIAL_RATE);
  HC06.begin(HC06_RATE);  
  HC06.write("\nTest Start\n");  
  HC06.println("\nTest Start\n"); 
}

void loop() { 
  printf("test");
}