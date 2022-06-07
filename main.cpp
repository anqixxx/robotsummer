
#include <Wire.h>
#include <math.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
#define INTERRUPTPIN PB9
#define REFLECTANCEPIN PA7
#define SAMPLE_TIME_MS 65
#define SAMPLE_SIZE_PER_MS 11
#define PI 3.14159265359


Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

volatile int buttonState=0;
int loopcount = 0;
int reflectanceVal = 0;

uint16_t numReadings;
double amplitude = 0;
double ref1kzSin[SAMPLE_SIZE_PER_MS];
double ref1kzCos[SAMPLE_SIZE_PER_MS];

void handle_interrupt();
void handle_interrupt_schmitt();
void readIR();
void setupFrequencyReference();
void displayArray(double vals[]);
bool takeAnalogReadings(uint16_t* p_numReadings = nullptr, double** p_analogVals = nullptr, byte pin = PA7);
double correlateTo1kz(double[]);
double takeSignalSample(double** p_analogVals = nullptr, byte pin = PA7);

void setup() {
  display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(INTERRUPTPIN, INPUT_PULLUP);
  pinMode(REFLECTANCEPIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INTERRUPTPIN),handle_interrupt,RISING);
 
  // Displays Adafruit logo by default. call clearDisplay immediately if you don't want this.
  display_handler.display();
  delay(2000);

  setupFrequencyReference();

  display_handler.setTextSize(1);
  display_handler.setTextColor(SSD1306_WHITE);
}

void loop() {


display_handler.clearDisplay();
display_handler.setCursor(0,0);
display_handler.println("Loop counter: ");
display_handler.println(loopcount++);

  amplitude = takeSignalSample(nullptr ,REFLECTANCEPIN);  
  display_handler.setCursor(0,30);
  display_handler.println(amplitude); 
display_handler.display();

delay(50);
}

void handle_interrupt(){
  buttonState++;
}

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
