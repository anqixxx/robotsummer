#include "ir_sensors.h"
#include "hardware_def.h"

// Correction factor to account for different sensitivities in the phototransistor responses
double sensitivityCorrection[IR_ARRAY_SIZE] = {1.04, 1.06, 1.05, 1, 0.87, 1.14, 1, 1.02};


int getQuickSignal(int led){
int s1 = 0;
int s2 = 0;
int s3 = 0;

  // Set the MUX to enabled with the ENable pin to low
  digitalWrite(IR_EN, 0);

   setSelectors(&s1, &s2, &s3, led);
    digitalWrite(IR_S1, s1);
    digitalWrite(IR_S2, s2);
    digitalWrite(IR_S3, s3);
   int signal = takeSquareSignalSample(IR_BEACON, TEN_KHZ, 100, SAMPLE_PERIOD, STANDARD_OFFSETS);
   return signal;

}

// Setup the logic pins for the IR Array
void setupIRArray()
{
  pinMode(IR_EN, OUTPUT);
  pinMode(IR_S1, OUTPUT);
  pinMode(IR_S2, OUTPUT);
  pinMode(IR_S3, OUTPUT);
}

double getHeadingToBeacon(int FREQ_PERIOD, int NUM_READINGS, int SAMPLE_INTERVAL, int NUM_OFFSETS)
{
  int SIG[IR_ARRAY_SIZE];
  getIRArrayValues(SIG, FREQ_PERIOD, NUM_READINGS, SAMPLE_INTERVAL, NUM_OFFSETS);
  return convertToHeading(SIG);
}

/*
 * Get the filtered values from the IR Array and write them to the array passed to function
 * IMPORTANT: accepts an array of minimum size IR_ARRAY_SIZE, fills with the current filtered values for 8 IRs
 * Array index corresponds to the IR Phototransistor selected.
 */
void getIRArrayValues(int SIG[], int FREQ_PERIOD, int NUM_READINGS, int SAMPLE_INTERVAL, int NUM_OFFSETS)
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
    SIG[i] = takeSquareSignalSample(IR_BEACON, FREQ_PERIOD, NUM_READINGS, SAMPLE_INTERVAL, NUM_OFFSETS) * sensitivityCorrection[i];
  }
}

/*
 * Get the unfiltered value for one of the IR phototransistors, select the IR phototransistor 
 * Then read the value
 */
int getUnfilteredIRArrayValue(int sel)
{
  // Selector Pins Settings
  int s1 = 0;
  int s2 = 0;
  int s3 = 0;

  // Set the MUX to enabled with the ENable pin to low
  digitalWrite(IR_EN, 0);

    setSelectors(&s1, &s2, &s3, sel);
    digitalWrite(IR_S1, s1);
    digitalWrite(IR_S2, s2);
    digitalWrite(IR_S3, s3);
    return analogRead(IR_BEACON);
  
}

// Converts the filtered array values to a heading, heading is from -7 to 7 with 0 being straight ahead
// returns indicator value as a result if it encounters more than two pins that are above the threshold, indicating error
double convertToHeading(int SIG[])
{
  int threshold =  getThreshold(SIG);
  double heading = 0;
  // Track the condition of there being too many IR hits indicating error
  int pinCount = 0;
  int netPing = 0;


  for (int i = 0; i < IR_ARRAY_SIZE; i++)
  for (int i = 0; i < IR_ARRAY_SIZE; i++)
  {
    if (SIG[i] > threshold)
    {
      heading += i*SIG[i];
      netPing+= SIG[i];
      pinCount++;
    }
  }
  if (pinCount == 0)
  {
    return NO_BEACON_FOUND;
  }


  heading = heading/netPing;
  // The result is the single pin, or the average pin heading (divide by number of pins included)
  return ((2 * heading) - 7);
}

// Define a lower threshold at which the IR is considered on or off the beacon
// threshold is a dynamic value and needs to be recalculated for each reading
int getThreshold(int SIG[])
{
  int threshold = 0;
int COPY[IR_ARRAY_SIZE];

for (int i = 0; i < IR_ARRAY_SIZE; i++)
{
  COPY[i] = SIG[i];
}

  // Sort the array low to high
  int i, j, min, temp;
  for (i = 0; i < IR_ARRAY_SIZE - 1; i++)
  {
    min = i;
    for (j = i + 1; j < IR_ARRAY_SIZE; j++)
      if (COPY[j] < COPY[min])
        min = j;
    temp = COPY[i];
    COPY[i] = COPY[min];
    COPY[min] = temp;
  }

  // Find average of four minimum values
  for (i = 0; i < IR_ARRAY_SIZE/2; i++){
    threshold += COPY[i];
  }
  threshold = threshold/(IR_ARRAY_SIZE/2); // Average
threshold = ((threshold + 2)*3)/2;
  // The threshold is three times the average minimum value of the array
  return threshold ;
}

// Set the selector pins to match a giv= channel integer (0-7) --
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
 FREQ_PERIOD - the period in us of the desired frequency to filter
 NUM_READINGS - The number of samples to read
 SAMPLE_INTERVAL - the amount of time in between each sample in microseconds
 NUM_OFFSETS - the total number of phase offsets to take the root mean squared
 *Note read rates below 5-6 are not possible with analogRead()*/
double takeSquareSignalSample(byte pin, int FREQ_PERIOD, int NUM_READINGS, int SAMPLE_INTERVAL, int NUM_OFFSETS)
{
  // Record start time of a cycle and the first sample begin time
  static int32_t tStart = micros(); // us; start time
  static int32_t tbegin = tStart;

  double readVal = 0;    // Value read from pin
  int i = 0;             // index of reading
  int cyclePosition = 0; // position of sample in a 100us cycle (corresponding to 10kHz)

  double dotProd[NUM_OFFSETS];   // accumulators to store dot product
  int sampleMult[NUM_OFFSETS];   // The corelations corresponding to reference for each stream
  int phaseOffsets[NUM_OFFSETS]; // The amount of phase offset from 0

  // Calculate the phase offset values to spread the total evenly within the first half of sample period
  // of the desired frequency
  for (int j = 0; j < NUM_OFFSETS; j++)
  {
    dotProd[j] = 0;                                          // Initialize all dot product accumulators to 0
    phaseOffsets[j] = (FREQ_PERIOD * j) / (2 * NUM_OFFSETS); // Evenly space the phase offsets over the first half of the signal
  }

  // For 4 offsets for example, the phase offset and multipliers are as follows for a 100us period signal:
  /*
  // 1st sample multiplier (correlation to expected 0  phase signal) -> 0
  // 2nd sample multiplier (correlation to expected 12 phase offset signal)
  // 3rd sample multiplier (correlation to expected 25 phase offset signal)
  // 4th sample multiplier (correlation to expected 37 phase offset signal)
  */

  // Loop to perform concurrent sampling and correlation / dot product
  // This is the repeated part of the sequence that samples at fixed intervals while
  // computing the dot product of parallel streams
  while (i < NUM_READINGS)
  {
    // Only take a reading once per sample interval
    int32_t tNow = micros(); // us; time now
    if (tNow - tStart >= SAMPLE_INTERVAL)
    {
      tStart += SAMPLE_INTERVAL;                     // reset start time to take next sample at exactly the correct pd
      readVal = analogRead(pin);                     // Read value
      cyclePosition = (tNow - tbegin) % FREQ_PERIOD; // Account for the shift on the 10khz wave signal so far
                                                     // Take mod 100 to normalize back into standard period domain [0,99]us

      // Update the correlation to the reference signal based on the cycle position for all phase offsets
      updateReferences(sampleMult, phaseOffsets, FREQ_PERIOD, NUM_OFFSETS, cyclePosition);

      // Perform the next step of the concurrent dot products (one for each offset amount)
      for (int j = 0; j < NUM_OFFSETS; j++)
      {
        dotProd[j] += readVal * sampleMult[j];
      }
    }
    // Index is complete, move to next one
    i++;
  }

  // Combine the dot products using the root mean squared
  double rms = 0;
  for (int j = 0; j < NUM_OFFSETS; j++)
  {
    rms += dotProd[j] * dotProd[j];
  }
  // normalize and return value
  return sqrt(rms) * 2.0 / (NUM_READINGS);
}

void updateReferences(int sampleMult[], int phaseOffsets[], int FREQ_PERIOD, int NUM_OFFSETS, int cyclePosition)
{
  // This is the square correlation, if the sample including offset falls within the first half of a wave period, then it is positive
  for (int i = 0; i < NUM_OFFSETS; i++)
  {
    if (((cyclePosition + phaseOffsets[i]) % FREQ_PERIOD) < (FREQ_PERIOD / 2))
    {
      sampleMult[i] = 1;
    }
    else
    {
      sampleMult[i] = -1;
    }
  }
}