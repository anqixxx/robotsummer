
#include "Stepper.h"
#include "hardware_def.h"
#include "AccelStepper.h"

/*
 * Stepper motor definition
 */
AccelStepper stepper(AccelStepper::DRIVER, STEPPER_STEP, STEPPER_DIR);
int stepperPosition = 0;

/*
 * Stepper Functions
 */
void setupStepper()
{
    // Setup the stepper pins and interrupt
    stepper.setMaxSpeed(1500);
    stepper.setAcceleration(1500);
    pinMode(STEPPER_SLEEP, OUTPUT);
    pinMode(STEPPER_LIMIT, INPUT_PULLUP);
    digitalWrite(STEPPER_SLEEP, HIGH); // Set the slepper to awake mode
    stepper.setPinsInverted(true, false, false);
    calibrateStepper();

    attachInterrupt(digitalPinToInterrupt(STEPPER_LIMIT), resetStepper, FALLING);
    digitalWrite(STEPPER_SLEEP, LOW); // Set the stepper to sleep
}

// ISR to prevent stepper failure
void resetStepper()
{
    stepper.stop();
}

// Stepper calibration to set position to correct 0 value
void calibrateStepper()
{
    // If limit switch is active, move off of switch first
    if (digitalRead(STEPPER_LIMIT) == LOW)
    {
        stepper.move(1000);
        stepper.runToPosition();
    }

    // Move to the extreme low to trigger interrupt and position reset
    stepper.moveTo(-8000);

    int time = millis();
    while (digitalRead(STEPPER_LIMIT) == HIGH && millis() - time < 4000)
    {
        stepper.run();
    }
    stepper.stop();
    digitalWrite(STEPPER_SLEEP, LOW);
    delay(50);
    digitalWrite(STEPPER_SLEEP, HIGH);
    stepper.setCurrentPosition(0);
    stepper.move(200);
    stepper.runToPosition();
    stepper.setCurrentPosition(0);
    // Let the limit switch activate and reset to a new 0 position
}

void moveStepper(int position)
{
    if (position > 3400)
    {
        position = 3400;
    }

    if (position < 0)
    {
        position = 0;
    }

    digitalWrite(STEPPER_SLEEP, HIGH);
    stepper.moveTo(position);
    int dist = stepper.distanceToGo();

    if (dist > 10 || dist < -10){
        stepper.runToPosition();

    }
     digitalWrite(STEPPER_SLEEP, LOW);
}

int getCurrentStepperPos(){
    return stepper.currentPosition();
}