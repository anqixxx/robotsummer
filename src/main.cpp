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

double sigAmp = 0;
int count1 = 0;
int count2 = 0;

void setup() {
  Serial.begin(SERIAL_RATE);
  HC06.begin(HC06_RATE);  
  HC06.write("\nTest Start\n");  
  HC06.println("\nTest Start\n"); 
}

void loop() {
char telemtery[40];

count1++;
count2 = count2 + 2;

sigAmp = takeSquareSignalSample( A0, 1000, 14);
Serial.print("Amplitude is:");
Serial.println(sigAmp);

sprintf(telemtery, "%d, %d, %d, %d, %d", (int) sigAmp, analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4));
HC06.println(telemtery);
delay(200);

}


