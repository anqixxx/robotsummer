#ifndef IR_SENSORS_H
#define IR_SENSORS_H

#include <Arduino.h>

// Initialize the selection pins to digital write for the MUX
void setupIRArray();

// Returns the heading to the beacon, heading is a value from -7 to 7 with 
// values -8 and 8 reserved for no signal or too many signals
// Negative is to the left, positive is to the right
int getHeadingToBeacon();

// Returns the filtered amplitude for an array of 8 IR phototransistors
void getIRArrayValues(int[]);

// Convert an array of IR readings and a threshold to a heading indication.  
// Heading is from -7 to 7 indicating direction of beacon, negative is left, positive right
int convertToHeading(int[], int);

// Converts an integer 'sel' to three binary values 's1,s2,s3' for MUX selection
void setSelectors(int *s1, int *s2, int*s3, int sel);

// Update the expected reference value for correlation of signal
void updateReference(int *s, int *c, int cyclePosition);

// Return the 10khz correlation for a signal, this is the doftware filteing algorithm
double takeSquareSignalSample( byte pin, int numReadings, int sampleInterval);








#endif
