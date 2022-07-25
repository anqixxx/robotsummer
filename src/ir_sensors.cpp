#include "ir_sensors.h"
#include "hardware_def.h"

// Correction factor to account for different sensitivities in the phototransistor responses
double sensitivityCorrection[IR_ARRAY_SIZE] = {1.04, 1.06, 1.05, 1, 0.87, 1.14, 1, 1.02};

// Setup the logic pins for the IR Array
void setupIRArray()
{
  pinMode(IR_EN, OUTPUT);
  pinMode(IR_S1, OUTPUT);
  pinMode(IR_S2, OUTPUT);
  pinMode(IR_S3, OUTPUT);
}

int getHeadingToBeacon()
{
  int SIG[IR_ARRAY_SIZE];
  getIRArrayValues(SIG);
  return convertToHeading(SIG, BEACON_THRESHHOLD);
}

/*
 * Get the filtered values from the IR Array and write them to the array passed to function
 * IMPORTANT: accepts an array of minimum size IR_ARRAY_SIZE, fills with the current filtered values for 8 IRs
 * Array index corresponds to the IR Phototransistor selected.
 */
void getIRArrayValues(int SIG[])
{
  // Selector Pins Settings
  int s1 = 0;
  int s2 = 0;
  int s3 = 0;

  // Set the MUX to enabled with the ENable pin to low
  digitalWrite(IR_EN, 0);

  for (int i = 0; i < IR_ARRAY_SIZE; i++)
  {
    setSelectors(&s1, &s2, &s3, i);
    digitalWrite(IR_S1, s1);
    digitalWrite(IR_S2, s2);
    digitalWrite(IR_S3, s3);
    SIG[i] = takeSquareSignalSample(IR_BEACON, 200, 6) * sensitivityCorrection[i];
  }
}

// Converts the filtered array values to a heading, heading is from -7 to 7 with 0 being straight ahead
// returns indicator value as a result if it encounters more than two pins that are above the threshold, indicating error
int convertToHeading(int SIG[], int threshold)
{
  int heading = 0;
  // Track the condition of there being too many IR hits indicating error
  int pinCount = 0;

  for (int i = 0; i < IR_ARRAY_SIZE; i++)
  {
    if (SIG[i] > threshold)
    {
      heading += i;
      pinCount++;
    }
  }
  if (pinCount == 0)
  {
    return NO_BEACON_FOUND;
  }
  if (pinCount > 2)
  {
    return TOO_MANY_SIGNALS; // Too many pins active, indicate an error
  }
  // The result is the single pin, or the average pin heading (divide by number of pins included)
  return (((2 * heading) / pinCount) - 7);
}

// Set the selector pins to match a given channel integer (0-7) --
// Input: - pointers to the three select pin states s1,s2,s3 where s3 is most significant bit
//        - sel, the channel to be selected, an integer from 0 to 7
void setSelectors(int *s1, int *s2, int *s3, int sel)
{
  *s1 = sel % 2;
  sel = sel / 2;
  *s2 = sel % 2;
  sel = sel / 2;
  *s3 = sel % 2;
}

// Now define the main code for the functions listed in the header file

/*
 Correlate an analog signal to a 10kHz squarewave, returns the amplitude of the match

 Inputs: pin - Analog pin to read from
 numReadings - The number of samples to read
 sampleInterval - the amount of time in between each sample in microseconds
 *Note read rates below 5-6 are not possible with analogRead()*/
double takeSquareSignalSample(byte pin, int numReadings, int sampleInterval)
{
  // Record start time of a cycle and the first sample begin time
  static int32_t tStart = micros(); // us; start time
  static int32_t tbegin = tStart;

  double readVal = 0;    // Value read from pin
  double sinDot = 0;     // Accumulator for square wave at 0 degree phase
  double cosDot = 0;     // Accumulator for square wave at 90 degree phase
  double sinDot2 = 0;    // Accumulator for square wave at 0 degree phase
  double cosDot2 = 0;    // Accumulator for square wave at 90 degree phase
  int i = 0;             // index of reading
  int s = 0;             // sin sample multiplier (correlation to expected 0 degree phase signal)
  int c = 1;             // cos sample multiplier (correlation to expected 25 degree phase offset signal)
  int s2 = 0;            // cos sample multiplier (correlation to expected 12 degree phase offset signal)
  int c2 = 1;            // cos sample multiplier (correlation to expected 37 degree phase offset signal)
  int cyclePosition = 0; // position of sample in a 100us cycle (corresponding to 10kHz)

  while (i < numReadings)
  {
    // Only take a reading once per sample interval
    int32_t tNow = micros(); // us; time now
    if (tNow - tStart >= sampleInterval)
    {
      tStart += sampleInterval;               // reset start time to take next sample at exactly the correct pd
      readVal = analogRead(pin);              // Read value
      cyclePosition = (tNow - tbegin) % 100;  // Account for the shift on the 10khz wave signal so far
                                              // Take mod 100 to normalize back into standard period domain [0,99]us
      updateReference(&s, &c, cyclePosition); // update the expected references for sin and cos
                                              //-> (-1,0,1) depending on location in waveform
      updateReference(&s2, &c2, (cyclePosition + 12) % 100); // update the expected references for second set sin and cos
                                                             //-> (-1,0,1) depending on location in waveform

      sinDot += readVal * s;   // add to accumulator
      cosDot += readVal * c;   // add to accumulator
      sinDot2 += readVal * s2; // add to accumulator
      cosDot2 += readVal * c2; // add to accumulator
    }
    i++;
  }
  // Take the amplitude of the two combined
  return ((sqrt(sinDot * sinDot + cosDot * cosDot + sinDot2 * sinDot2 + cosDot2 * cosDot2)) * 2.0) / (numReadings);
}

// Make a square wave reference signal to compare against for sampling,
// update multipliers for the 0 and 90 degree phase offsets
void updateReference(int *s, int *c, int cyclePosition)
{

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
    SERIAL_OUT.println("Error encountered in IR Sensing updateReference()");
  }
}