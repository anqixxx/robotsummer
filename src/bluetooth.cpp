#include "bluetooth.h"
#include "hardware_def.h"

int blueCount1 = 0;
int blueCount2 = 0;

void blueStart(){
  Serial.begin(SERIAL_RATE);
  HC06.begin(HC06_RATE);  
  HC06.write("\nTest Start\n");  
  HC06.println("\nTest Start\n"); 
}

/**
 * Takes as parameters 5 values to be plotted on the bluetooth serial monitor
 * Returns nothing
 * */
void blueLoop(int reading1, int reading2,  int reading3,  int reading4,  int reading5){
    char telemtery[40];

    blueCount1++;
    blueCount2 = blueCount2 + 2;
    
    sprintf(telemtery, "%d, %d, %d, %d, %d", reading1, reading2, reading3, reading4, reading5);
    HC06.println(telemtery);
    delay(200);
}