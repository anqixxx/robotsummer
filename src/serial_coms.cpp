#include "serial_coms.h"
#include "hardware_def.h"

// Setup the serial port indicated in hardware defs SRIAL_OUT
void setupSerialPort(){
  SERIAL_OUT.begin(SERIAL_RATE);
  SERIAL_OUT.print("Serial port opened");
}

/*
 *  This is our primary method for sending out telemetry.  We can define all of the robot states and variables
 *  we wish to monitor and have them sent over the Serial Monitor as comma separated values.  Telemetry reader
 * on the computer is able to interpret them through a GUI
 */
void outputCSV(int reading1, int reading2,  int reading3,  int reading4,  int reading5){
    char telemtery[40];
    
    sprintf(telemtery, "%d, %d, %d, %d, %d", reading1, reading2, reading3, reading4, reading5);
    SERIAL_OUT.println(telemtery);
}


// Setup the SSDD_1306 display for coms
void setupOLED(){
  
}