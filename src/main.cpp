
#include "tape_follow.h"
#include "ir_sensors.h"
#include "motor_drive.h"
#include "field_nav.h"
#include "claw.h"
#include "arm.h"
#include "treasure.h"
#include "Arduino.h"
#include "Wire.h"
#include "math.h"

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



 /*
  Correlate an analog signal to a 10kHz squarewave, returns the amplitude of the match

  Inputs: pin - Analog pin to read from
  numReadings - The number of samples to read
  sampleInterval - the amount of time in between each sample in microseconds 
  *Note read rates below 85 are not possible with analogRead()*/

double takeSquareSignalSample( byte pin, int numReadings, int sampleInterval)
{
  // Record start time of a cycle and the first sample begin time
  static int32_t tStart = micros(); // us; start time
  static int32_t tbegin = tStart;

  double readVal = 0; //Value read from pin
  double sinDot = 0;  //Accumulator for square wvae at 0 degree phase
  double cosDot = 0;  //Accumulator for square wvae at 90 degree phase
  double sinDot2 = 0;  //Accumulator for square wvae at 0 degree phase
  double cosDot2 = 0;  //Accumulator for square wvae at 90 degree phase
  int i = 0; // index of reading 
  int s = 0; // sin sample multiplier (correlation to expected 0 phase signal)
  int c = 1; // cos sample multiplier (correlation to expected 25 phase offset signal)
  int s2 = 0;  // cos sample multiplier (correlation to expected 12 phase offset signal)
  int c2 = 1; // cos sample multiplier (correlation to expected 37 phase offset signal)
  int cyclePosition = 0;  // position of sample in a 100us cycle

  while (i<numReadings){
    // Only take a reading once per sample interval
    int32_t tNow = micros(); // us; time now
    if (tNow - tStart >= sampleInterval)
      {        
        tStart += sampleInterval; // reset start time to take next sample at exactly the correct pd
        readVal = analogRead(pin); // Read value
        cyclePosition = (tNow-tbegin)%100; // Account for the shift on the 10khz wave signal so far
                                          //Take mod 100 to normalize back into standard period domain [0,99]us
        updateReference(&s,&c,cyclePosition); //update the expected references for sin and cos 
                                              //-> (-1,0,1) depending on location in waveform
        updateReference(&s2,&c2,(cyclePosition+12)%100); //update the expected references for second set sin and cos 
                                              //-> (-1,0,1) depending on location in waveform  
                                    
        sinDot += readVal*s; // add to accumulator
        cosDot += readVal*c; // add to accumulator 
        sinDot2 += readVal*s2; // add to accumulator
        cosDot2 += readVal*c2; // add to accumulator             
      }
      i++;
    }

  // Apply normalization
  double normalize = 1.0/(numReadings);
  // Take the amplitude of the two combined    
  return (sqrt(sinDot*sinDot + cosDot*cosDot + sinDot2*sinDot2 + cosDot2*cosDot2))*normalize;
}

// Make a square wave reference signal to compare against for sampling, 
// update multipliers for the 0 and 90 degree phase offsets
void updateReference(int *s, int *c, int cyclePosition){
  
  if (cyclePosition == 0)
  {
    *s = 0;
    *c = -1;
  }
  else if (cyclePosition < 25)
  {
    *s = 1;
    *c = -1;
  }
    else if (cyclePosition == 25)
  {
    *s = 1;
    *c = 0;
  }
    else if (cyclePosition < 50)
  {
    *s = 1;
    *c = 1;
  }
    else if (cyclePosition == 50)
  {
    *s = 0;
    *c = 1;
  }
    else if (cyclePosition < 75)
  {
    *s = -1;
    *c = 1;
  }
    else if (cyclePosition == 75)
  {
    *s = -1;
    *c = 0;
  }
    else if (cyclePosition < 100)
  {
    *s = -1;
    *c = -1;
  }
    else 
  {
    // Error condition
    *s = -999;
    *c = -999;
  }
}
