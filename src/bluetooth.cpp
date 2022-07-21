#include "bluetooth.h"
#include "hardware_def.h"

/**
 * Sets up Bluetooth module
 * Insert into main setup to run
 * */
void blueStart(){
  HC06.begin(HC06_RATE);  
  HC06.println("\nTest Start\n"); 
}

/**
 * Takes as parameters 5 values to be plotted on the bluetooth serial monitor
 * Insert into loop to run
 * Returns nothing
 * */
void blueLoop(int reading1, int reading2,  int reading3,  int reading4,  int reading5){
    char telemtery[40];
    
    sprintf(telemtery, "%d, %d, %d, %d, %d", reading1, reading2, reading3, reading4, reading5);
    HC06.println(telemtery);
}