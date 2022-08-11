#include "hardware_def.h"
#include "sonar.h"


void setupSonar()
{
    pinMode(RIGHT_TRIG_PIN, OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(RIGHT_ECHO_PIN, INPUT);  // Sets the echoPin as an INPUT
    pinMode(LEFT_TRIG_PIN, OUTPUT);  // Sets the trigPin as an OUTPUT
    pinMode(LEFT_ECHO_PIN, INPUT);   // Sets the echoPin as an INPUT
}

double readSonar(int sonarID)
{
    byte trigPin;
    byte echoPin;
    long duration;
    double distance;

    switch (sonarID)
    {
    case RIGHT:
        trigPin = RIGHT_TRIG_PIN;
        echoPin = RIGHT_ECHO_PIN;
        break;
    case LEFT:
        trigPin = LEFT_TRIG_PIN;
        echoPin = LEFT_ECHO_PIN;
        break;
    }

    // Clears the trigPin condition
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * (0.034 / 2); // Speed of sound wave divided by 2 (go and back)

    return distance;
}

// NOTE: If using often in a row, must wait at least 60ms before next measurement
// i.e: delay(60);
bool treasure_detect(int sonarID, int threshold){
    byte trigPin;
    byte echoPin;
    long duration;
    double distance;

    switch (sonarID)
    {
    case RIGHT:
        trigPin = RIGHT_TRIG_PIN;
        echoPin = RIGHT_ECHO_PIN;
        break;
    case LEFT:
        trigPin = LEFT_TRIG_PIN;
        echoPin = LEFT_ECHO_PIN;
        break;
    }

  if (readSonar(sonarID) <= threshold){
    Serial.println("Treasure in Range");
    return true;
  } else{
    return false;
  }
}
