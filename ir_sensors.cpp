#include "ir_sensors.h"

// Now define the main code for the functions listed in the header file
void readIR(){
reflectanceVal = analogRead(REFLECTANCEPIN);
display_handler.println("Sensor Reading: ");
display_handler.println(reflectanceVal);
}

void setupFrequencyReference(){
int k = 0;
double tstep = 2*PI/SAMPLE_SIZE_PER_MS;
  for (k=0; k<SAMPLE_SIZE_PER_MS; k++){
    ref1kzCos[k] = cos(tstep*k);
    ref1kzSin[k] = sin(tstep*k);
  }
}

void displayArray(double vals[]){
    for(int k=0;k<SAMPLE_TIME_MS* SAMPLE_SIZE_PER_MS;k= k+2){
display_handler.clearDisplay();
display_handler.setCursor(0,0);
display_handler.println("Array Vals:");
display_handler.println(vals[k]);
display_handler.setCursor(0,20);
display_handler.println(k);
display_handler.display();
delay(200);
}
}

double takeSignalSample(double** p_analogVals, byte pin)
{

  static const uint16_t NUM_READINGS = SAMPLE_TIME_MS * SAMPLE_SIZE_PER_MS;
  static double analogVals[1];
  const uint32_t SAMPLE_PD = 1000/SAMPLE_SIZE_PER_MS; // us; sample period (how often to take a new sample)
  static uint32_t tStart = micros(); // us; start time

  double readVal = 0;
  double sinDot = 0;
  double cosDot = 0;
  int i = 0; // index of reading 
  int k = 0; // index for reference

  while (i<NUM_READINGS){
    // Only take a reading once per SAMPLE_PD
    uint32_t tNow = micros(); // us; time now
    if (tNow - tStart >= SAMPLE_PD)
      {
        tStart += SAMPLE_PD; // reset start time to take next sample at exactly the correct pd
        readVal = analogRead(pin); 
        sinDot += readVal*ref1kzSin[k];
        cosDot += readVal*ref1kzCos[k];
        
        i++;
        k++;
        if (k>=SAMPLE_SIZE_PER_MS){
          k = 0;
        }
      }
    }

      if (p_analogVals != nullptr)
    {
      *p_analogVals = analogVals;
    }

  // Apply normalization
  double normalize = 2.0/(SAMPLE_TIME_MS*SAMPLE_SIZE_PER_MS);
  // Take the amplitude of the two combined    
  return sqrt((sinDot*sinDot + cosDot*cosDot))*normalize;
}
