#include "bluetooth.h"
#include "hardware_def.h"
#include "ir_sensors.h"

double blueSigAmp = 0;
int blueCount1 = 0;
int blueCount2 = 0;

void blueStart(){
  Serial.begin(SERIAL_RATE);
  HC06.begin(HC06_RATE);  
  HC06.write("\nTest Start\n");  
  HC06.println("\nTest Start\n"); 
}

void blueLoop(){
    char telemtery[40];

    blueCount1++;
    blueCount2 = blueCount2 + 2;
    blueSigAmp = takeSquareSignalSample( A0, 1000, 14);
    Serial.print("Amplitude is:");
    Serial.println(blueSigAmp);
    
    sprintf(telemtery, "%d, %d, %d, %d, %d", (int) blueSigAmp, analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4));
    HC06.println(telemtery);
    delay(200);
}

