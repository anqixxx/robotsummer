#include "Arduino.h"
#include "Wire.h"
#include <stdio.h>
#include <stdbool.h>
#include <SPI.h>
#include <RF24.h>
#include <Adafruit_SSD1306.h>
#include <AccelStepper.h>

#include "hardware_def.h"
#include "tape_follow.h"
#include "ir_sensors.h"
#include "motor_drive.h"
#include "field_nav.h"
#include "claw.h"
#include "treasure.h"
#include "serial_coms.h"
#include "DuePWM.h"
#include "Encoders.h"
#include "Bridge.h"
#include "Stepper.h"
#include "Arm.h"
#include "ClawClass.h"
#include "sonar.h"

#include "PID_v1.h"

/*
Radio Variables and Data Structure
*/
RF24 radio(CE, CNS); // nRF24L01 (CE, CSN)
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

struct Data_Package
{
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  byte j2PotX;
  byte j2PotY;
  byte j2Button;
  byte pot1;
  byte pot2;
  byte tSwitch1;
  byte tSwitch2;
  byte button1;
  byte button2;
  byte button3;
  byte button4;
};

Data_Package data;

/*
IR PID Controller and variables
*/
// Global variables for the setpoint, input and output
double pidSetpoint, pidInput, pidOutput;
// Specify the links and initial tuning parameters
PID myPID(&pidInput, &pidOutput, &pidSetpoint, 2, 0, 0, DIRECT);

// OLED handler
Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Bridge mechanism
Bridge myBridge(BRIDGE_PIN);

int treasures = 0; // Total number of treasures in storage
int timer = 0;
int sonarTimeout = 0;


// RC Functions
void rcloop();
void setupRadio();
void resetRadioData();

// Stepper motor
void setupStepper();
void resetStepper(); // ISR for limit switch
void calibrateStepper();

// Robot arm pinModes
void setArmPins();

// Robot Mode Selection
void selectRobotMode();
void dispMode();

// Mode operations
void manualMode();
void moveToTreasure1(double dist);
void crossChickenwire();
void captureBeacon();
void followBeacon(int);
void moveToTreasure4();
void moveToTreasure2();
void sonarDetect(int threshold);

// Testing Functions (TO BE REMOVED EVENTUALLY!)
void IRReadingMode();
void UltrasonicTesting();

/************************************************************************************
 **************************** S E T U P -- A N D -- L O O P *************************
 ************************************************************************************/

/*
Robot mode - Select which stage of operation the robot is in
*/
int MODE = -1; // Start the robot in its initial operating state from the start line   <=================== SELECT START MODE ===============

void setup()
{
  setupSerialPort();
  setupRadio();   // Open the RC radio communications
  setupIRArray(); // Setup the logic pins for the IR Array
  pinMode(PANCAKE_FOR, OUTPUT);
  pinMode(PANCAKE_BACK, OUTPUT);
  setupSonar();

  ultra_setup();
  // Sets up sonars
  // claw_setup();                                               //
  myPID.SetOutputLimits(-PID_OUTPUT_LIMIT, PID_OUTPUT_LIMIT); // Set the limits for the PID output values
  myPID.SetSampleTime(20);                                    // Set PID sample rate (value in ms)
  setupPWM();                                                 // Adjust pwm to correct frequency for the drive motors
  myBridge.setup();
  setupEncoders();
  setupStepper();
  moveStepper(STEPPER_TREASURE_POS); // Set the arm height to high

  display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Turn on OLED
  display_handler.display();                         // Display logo
  delay(1000);                                       // Allow logo to flash before
  dispMode();                                        // Display the operation mode of robot on OLED
  setArmPins();
}

void loop()
{
  // while(1){
  // //   outputCSV(analogRead(TAPE_FAR_L), analogRead(TAPE_L), analogRead(TAPE_R), analogRead(TAPE_FAR_R), 0);
  // claw_test_value();
  // }


  // Check RC input
  rcloop();
  // Right side toggle switch in up position is indicator for manual mode

  if (data.tSwitch2 == 0)
  {
    manualMode();
  }
  else
  {

    selectRobotMode();
  }
}

// Robot is operated through a sequence of states, current state is MODE variable
// select the set of functions matchingb the current operation mode
void selectRobotMode()
{
  switch (MODE)
  {
  case -1:
    timer = millis();
    MODE++;
    dispMode();
    sonarTimeout = millis();


  case 0:
    // Starting mode, line follow until first treasure is detected
    while ((millis() - sonarTimeout) < 3500)
    {
      lineFollow();
      
    }
    
    sonarDetect(35);
    break;

  case 1:
    // From sonar signals, back up to treasure, pick up
    double dist;
    delay(60); // change to 60 later? Leave as is for now as this is working 
    dist = readSonar(RIGHT);

    moveToTreasure1(dist);
    
    treasureSequence(160, 10);

    forwardFromTreasure(dist);

    while(offTape()){
      drive(+FAST, -FAST);
    }    

    lineFollow();
    dispMode();
    MODE++;

  case 2:
    if (onChickenWire()){
      drive(0,0);      
    MODE++;
    dispMode();
    }
    else{lineFollow();}

    break;

  case 3:
    // Crosses Chicken Wire
    
    crossChickenwire();
    sonarTimeout = millis();

    MODE++;
    dispMode();
    break;

  case 4:
    while ((millis() - sonarTimeout) < 500)
    {
      lineFollow();
    }    
    
    sonarDetect(30);
    // Debug protocol
    outputCSV(analogRead(TAPE_L), analogRead(TAPE_R), analogRead(TAPE_FAR_L), analogRead(TAPE_FAR_R), 0);
    break;

  case 5:
    // From sonar signals, back up to treasure, pick up
    moveToTreasure2();
    
    treasureSequence(160, 10);

    drive(0, 0);

    break;

  case 6:
    // Use IR to determine when in line for the Arch
    if (getQuickSignal(4) > 14 && getQuickSignal(5) > 14)
    {
      drive(0, 0);
      MODE++;
      dispMode();
    }
    else
    {
      // Debug protocol
      outputCSV(analogRead(TAPE_L), analogRead(TAPE_R), data.pot1, data.pot2, 0);
      // Rotate until signal is found
      drive(-FAST,FAST);
    }
    break;
  case 7:
  // Drives forward until we reach treasure 3 
  
    if ((millis() - timer) > 60)
    {
      timer = millis();
      int sonarReading = readSonar(LEFT);
      SERIAL_OUT.print("LEFT SIDE SONAR: ");
      SERIAL_OUT.println(sonarReading);


      if (sonarReading < 40)
      {
        timer = millis();

        while (millis()-timer < 60){
          lineFollow(.7);
        }

        sonarReading = readSonar(LEFT);

        if (sonarReading < 40){
        drive(0, 0);
        SERIAL_OUT.println(readSonar(LEFT));
        // Increment mode to reach next one
        MODE++;
        // Update display with new mode
        dispMode();
        }       
      }
    }
    else
    {
      //IR CODE
    }
    MODE++;
    dispMode();
    break;
  case 8:
    delay(60); 
    dist = readSonar(LEFT);

    rotate(90);
    backupToTreasure(dist);
    treasureSequence(180, 10);
    // MODE++;
    // dispMode();
    break;
  case 9:
  // Rotate until we find IR heading again
    MODE++;
    break;
  case 10:
  // Find Treasure 4
    // Drives forward until we reach treasure 3 
    if ((millis() - timer) > 60)
    {
      timer = millis();
      int sonarReading = readSonar(RIGHT);
      SERIAL_OUT.print("LEFT SIDE SONAR: ");
      SERIAL_OUT.println(sonarReading);


      if (sonarReading < 40)
      {
        timer = millis();

        while (millis()-timer < 60){
          lineFollow(.7);
        }

        sonarReading = readSonar(RIGHT);

        if (sonarReading < 40){
        drive(0, 0);
        SERIAL_OUT.println(readSonar(RIGHT));
        // Increment mode to reach next one
        MODE++;
        // Update display with new mode
        dispMode();
        }       
      }
    }
    else
    {
      //IR CODE
    }
    break;
  case 11:
    // outputCSV(getHeadingToBeacon(TEN_KHZ, TEN_KHZ_READINGS, SAMPLE_PERIOD, STANDARD_OFFSETS),getHeadingToBeacon(ONE_KHZ, ONE_KHZ_READINGS, SAMPLE_PERIOD, STANDARD_OFFSETS),0,0,0);
    IRReadingMode(); // debug mode
    break;
  case 12:
    // Check for a ping on the left IR beacon LED to determine when in position
    if (getQuickSignal(1) > 10)
    {
      drive(0, 0);
      MODE++;
      dispMode();
    }
    else
    {
      // Debug protocol
      outputCSV(analogRead(TAPE_L), analogRead(TAPE_R), data.pot1, data.pot2, 0);
      // Follow line
      lineFollow();
    }
    break;
  case 13:
    SERIAL_OUT.println("End of automation sequence");
    break;
  case 14:

    break;

  case -99:
    SERIAL_OUT.println("Error");
    break;
  }
}

// Manual control of robot, allows drive control and MODE select
void manualMode()
{
  // Normalize drive values to -255 to 255 range
  int leftMotor = (data.j1PotY + 3 - 128) * 1.9;
  int rightMotor = (data.j2PotY + 1 - 128) * 1.9;

  if (leftMotor < 10 && leftMotor > -10)
  {
    leftMotor = 0;
  }
  if (rightMotor < 10 && rightMotor > -10)
  {
    rightMotor = 0;
  }

  drive(leftMotor, rightMotor);
  outputCSV(leftMotor, rightMotor, data.tSwitch2, data.pot2 * 15, MODE);

  moveStepper(data.pot2 * 15);

  if (data.j2PotX > 220)
  {
    digitalWrite(PANCAKE_FOR, HIGH);
    digitalWrite(PANCAKE_BACK, LOW);
  }
  else if (data.j2PotX < 20)
  {
    digitalWrite(PANCAKE_FOR, LOW);
    digitalWrite(PANCAKE_BACK, HIGH);
  }
  else
  {
    digitalWrite(PANCAKE_FOR, LOW);
    digitalWrite(PANCAKE_BACK, LOW);
  }

  // Use button 1 to increment MODE to start robot in desired state
  // When automatic drive is toggled
  if (data.button2 == 0)
  {
    MODE++;
    resetRadioData();
    delay(100);
    setupRadio();
    dispMode();
  }
  else if (data.button1 == 0)
  {
    MODE--;
    resetRadioData();
    delay(100);
    setupRadio();
    dispMode();
  }

  // if (data.button3 == 0)
  // {
  //   while (digitalRead(CLAW_START))
  //   {
  //     claw_backward();
  //   }
  //   resetRadioData();
  //   delay(100);
  //   setupRadio();
  // }
  // else if (data.button4 == 0)
  // {
  //   while (digitalRead(CLAW_END))
  //   {
  //     claw_forward();
  //   }
  //   resetRadioData();
  //   delay(100);
  //   setupRadio();
  // }
}

// Move from main course to treasure 1, can we hardcode this?  seems straightforward
void moveToTreasure1(double dist)
{
  rotate(90);
  backupToTreasure(dist);
}

void moveToTreasure2(){
  backupToTreasure(17);
  rotate(270);
  backupToTreasure(15);
}

// capture the IR beacon and move to next mode
void captureBeacon()
{

  int heading = getHeadingToBeacon(TEN_KHZ, TEN_KHZ_READINGS, SAMPLE_PERIOD, STANDARD_OFFSETS);

  if (heading == TOO_MANY_SIGNALS)
  {
    // React to having pins overloaded
  }
  else if (heading == NO_BEACON_FOUND)
  {
    // React to no beacon found
    searchForBeacon(RIGHT);
  }
}

// Move from near treasure 3 to treasure 4 by following the beacon
void moveToTreasure4()
{
  if (0)
  {
    // Check for the end condition to indicate that treasure 4 is reached
    myPID.SetMode(MANUAL); // Turn off PID
    MODE++;
  }
  else
  {
    // Check that PID is active, turn on if it is off (manual)
    if (myPID.GetMode() == MANUAL)
    {
      myPID.SetMode(AUTOMATIC); // Turn on PID
    }

    followBeacon(0); // Follow the beacon with a heading of 0;
  }
}

// Follows a set heading (probably stable in the -7 to 7 range) using PID control.
// Is this supposed to be 0 to 7
void followBeacon(int heading)
{
  int averageSpeed = 100;
  pidSetpoint = heading;                                                                     // 0 is the heading towards the beacon
  pidInput = getHeadingToBeacon(TEN_KHZ, TEN_KHZ_READINGS, SAMPLE_PERIOD, STANDARD_OFFSETS); // Use the heading offset from beacon as the input

  myPID.SetTunings(PID_P_TUNING, PID_I_TUNING, 0); // Set the P and I using the two potentiometers for tuning
  myPID.Compute();                                 // This will update the pidOutput variable that is linked to myPID

  // outputCSV(pot1, pot2, pidInput, (int)pidSetpoint, (int)pidOutput); // Debugging information
  drive(averageSpeed - pidOutput, averageSpeed + pidOutput); // Use the PID output as a wheel speed differential
}

// DEBUGGING and TESTING MODE for IR Array and Heading indicators
void IRReadingMode()
{
  int IRArrayValues[IR_ARRAY_SIZE];

  // Direction from robot to beacon -7 to 7
  // int heading = 0;
  // getIRArrayValues(IRArrayValues, TEN_KHZ, 100, SAMPLE_PERIOD, STANDARD_OFFSETS);
  // heading = convertToHeading(IRArrayValues);

  for (int i = 0; i < IR_ARRAY_SIZE; i++)
  {
    IRArrayValues[i] = getQuickSignal(i);
  }

  char telemtery[60];
  sprintf(telemtery, "%d, %d, %d, %d, %d, %d, %d, %d",
          IRArrayValues[0], IRArrayValues[1], IRArrayValues[2], IRArrayValues[3],
          IRArrayValues[4], IRArrayValues[5], IRArrayValues[6], IRArrayValues[7]);
  SERIAL_OUT.println(telemtery);
}

void UltrasonicTesting()
{
  ultra_loop();
}

/*
 * Display OLED Functions
 */

// Increment the MODE variable to enter into the next mode and update the OLED
void dispMode()
{
  display_handler.clearDisplay();
  display_handler.setTextSize(2);
  display_handler.setTextColor(SSD1306_WHITE);
  display_handler.setCursor(0, 0);
  display_handler.println("MODE:");
  display_handler.setTextSize(5);
  display_handler.println(MODE);
  display_handler.display();
}

// Arm pin setup
void setArmPins()
{
  pinMode(ARM_LIMIT_END, INPUT_PULLUP);
  pinMode(ARM_LIMIT_START, INPUT_PULLUP);
}

/*
Radio Functions
*/
void rcloop()
{
  // Check whether there is data to be received
  if (radio.available())
  {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis();              // At this moment we have received the data
  }
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if (currentTime - lastReceiveTime > 1000)
  {                   // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetRadioData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  }
}
void setupRadio()
{
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetRadioData();
}
void resetRadioData()
{
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotX = 127;
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.j1Button = 1;
  data.j2Button = 1;
  data.pot1 = 1;
  data.pot2 = 1;
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.button1 = 1;
  data.button2 = 1;
  data.button3 = 1;
  data.button4 = 1;
}

// Detects treasures while line following
void sonarDetect(int threshold){
  
    if ((millis() - timer) > 60)
    {
      timer = millis();
      int sonarReading = readSonar(RIGHT);
      SERIAL_OUT.print("RIGHT SIDE SONAR: ");
      SERIAL_OUT.println(sonarReading);


      if (sonarReading < threshold)
      {
        timer = millis();

        while (millis()-timer < 60){
          lineFollow(.7);
        }

        sonarReading = readSonar(RIGHT);

        if (sonarReading < threshold){
        drive(0, 0);
        SERIAL_OUT.println(readSonar(RIGHT));
        // Increment mode to reach next one
        MODE++;
        // Update display with new mode
        dispMode();
        }       
      }
    }
    else
    {
      // Follow line
      lineFollow();
    }
}