// Treasure detecting functions, from ultrasonic sensors, hall effect sensors, and reflectance sensors
#include "treasure.h"
#include "hardware_def.h"

// Now define the main code for the functions listed in the header file
/**
   HC-SR04 Demo
   Demonstration of the HC-SR04 Ultrasonic Sensor
   Date: August 3, 2016
   Description:
    Connect the ultrasonic sensor to the Arduino as per the
    hardware connections below. Run the sketch and open a serial
    monitor. The distance read from the sensor will be displayed
    in centimeters and inches.
   Hardware Connections:
    Arduino | HC-SR04
    -------------------
      5V    |   VCC
      7     |   Trig
      8     |   Echo
      GND   |   GND
   License:
    Public Domain
*/

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;

void ultra_setup() {

  // The Trigger pin will tell the sensor to range find
  pinMode(RIGHT_TRIG_PIN, OUTPUT);
  digitalWrite(RIGHT_TRIG_PIN, LOW);
  pinMode(LEFT_TRIG_PIN, OUTPUT);
  digitalWrite(LEFT_TRIG_PIN, LOW);

  //Set Echo pin as input to measure the duration of 
  //pulses coming back from the distance sensor
  pinMode(RIGHT_ECHO_PIN, INPUT);
  pinMode(LEFT_ECHO_PIN, INPUT);


  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
}

//Testing Setup
void test_ultra_setup(int TRIG_PIN, int ECHO_PIN) {

  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  //Set Echo pin as input to measure the duration of 
  //pulses coming back from the distance sensor
  pinMode(ECHO_PIN, INPUT);

  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
}

//Testing Function
void test_ultra_loop(int TRIG_PIN, int ECHO_PIN) {

  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float inches;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;
  inches = pulse_width / 148.0;

  // Print out results
  if ( pulse_width > MAX_DIST ) {
    Serial.println("Out of range");
  } else {
    Serial.print(cm);
    Serial.print(" cm \t");
    Serial.print(inches);
    Serial.println(" in");
  }

  // Wait at least 60ms before next measurement
  delay(60);
}

int sonar_cm(int TRIGGER, int ECHO) {
  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance
  cm = pulse_width / 58.0;

  // Print out results
  if ( pulse_width > MAX_DIST ) {
    Serial.println("Out of range");
    return -1;
  } else {
    return cm;
  }

  // Wait at least 60ms before next measurement
  delay(60);
}

int sonar_in(int TRIGGER, int ECHO) {
  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float inches;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance
  inches = pulse_width / 148.0;

  // Print out results
  if ( pulse_width > MAX_DIST ) {
    Serial.println("Out of range");
    return -1;
    //shows that it is out of range
  } else {
    return inches;
  }

  // Wait at least 60ms before next measurement
  delay(60);
}