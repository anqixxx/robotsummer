#ifndef IR_SENSORS_H
#define IR_SENSORS_H

#include <Arduino.h>

// Initialize the selection pins to digital write for the MUX
void setupIRArray();

// Returns the heading to the beacon, heading is a value from -7 to 7 with 
// values -8 and 8 reserved for no signal or too many signals
// Negative is to the left, positive is to the right
double getHeadingToBeacon(int FREQ_PERIOD, int NUM_READINGS, int SAMPLE_INTERVAL, int NUM_OFFSETS);

// Returns the filtered amplitude for an array of 8 IR phototransistors
void getIRArrayValues(int SIG[], int FREQ_PERIOD, int NUM_READINGS, int SAMPLE_INTERVAL, int NUM_OFFSETS);

// Returns the unfiltered amplitudes for any IR signal coming in
int getUnfilteredIRArrayValue(int sel);

// Convert an array of IR readings and a threshold to a heading indication.  
// Heading is from -7 to 7 indicating direction of beacon, negative is left, positive right
double convertToHeading(int[]);

// Get the threshold for on off of an array
int getThreshold(int SIG[]);

// Converts an integer 'sel' to three binary values 's1,s2,s3' for MUX selection
void setSelectors(int *s1, int *s2, int*s3, int sel);

// Update the expected reference value for correlation of signal
void updateReferences(int sampleMult[], int phaseOffsets[], int FREQ_PERIOD, int NUM_OFFSETS, int cyclePosition);

//  Inputs: pin - Analog pin to read from
//  FREQ_PERIOD - the period in us of the desired frequency to filter
//  NUM_READINGS - The number of samples to read
//  SAMPLE_INTERVAL - the amount of time in between each sample in microseconds
//  NUM_OFFSETS - the total number of phase offsets to take the root mean squared
//  *Note read rates below 5-6 are not possible with analogRead()
double takeSquareSignalSample( byte pin, int FREQ_PERIOD, int NUM_READINGS, int SAMPLE_INTERVAL, int NUM_OFFSETS);








#endif
