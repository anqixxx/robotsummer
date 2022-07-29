#include "Arduino.h"
#include "Wire.h"
#include <stdio.h>
#include <stdbool.h>
#include <SPI.h>
#include <RF24.h>
#include <Adafruit_SSD1306.h>

#include "hardware_def.h"
#include "tape_follow.h"
#include "ir_sensors.h"
#include "motor_drive.h"
#include "field_nav.h"
#include "claw.h"
#include "treasure.h"
#include "serial_coms.h"
#include "DuePWM.h"

#include "PID_v1.h"

/*
Radio Variables and Data Structure
*/
RF24 radio(CE, CNS); // nRF24L01 (CE, CSN)
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
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

/*
Tape Follow Calibration
*/
int pot1;
int pot2;

// RC Functions
void rcloop();
void setupRadio();
void resetRadioData();

// Robot Mode Selection
void selectRobotMode();
void dispMode();

// Mode operations
void manualMode();
void moveToTreasure1();
void captureBeacon();
void followBeacon(int);
void moveToTreasure4();

// Testing Functions (TO BE REMOVED EVENTUALLY!)
void IRReadingMode();
void UltrasonicTesting();

/************************************************************************************
 **************************** S E T U P -- A N D -- L O O P *************************
 ************************************************************************************/

/*
Robot mode - Select which stage of operation the robot is in
*/
int MODE = 1; // Start the robot in its initial operating state from the start line   <=================== SELECT START MODE ===============

void setup()
{
  MODE = 3; // Start the robot in its initial operating state from the start line   <=================== SELECT START MODE ===============
  setupSerialPort();
  setupRadio();                                                 // Open the RC radio communications
  setupIRArray();                                          // Setup the logic pins for the IR Array
  ultra_setup();                                              // Sets up sonars
  claw_setup();                                              //
  myPID.SetOutputLimits(-PID_OUTPUT_LIMIT, PID_OUTPUT_LIMIT); // Set the limits for the PID output values
  myPID.SetSampleTime(20);                                    // Set PID sample rate (value in ms)
  pwm_setup();                                                // Adjust pwm to correct frequency for the drive motors

  display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Turn on OLED
  display_handler.display();                         // Display logo

  
  delay(2000);                                        // Allow logo to flash before
  dispMode();                                        // Display the operation mode of robot on OLED
}

void loop()
{
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
  case 0:
    // Starting mode, line follow until reaching the state of 4 reflectance sensors turned off
    // if all four are turned off or some other trigger
    if (0)
    {
      // Increment mode to reach next one
      MODE++;
      // Update display with new mode
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
  case 1:
    // From the chicken wire backup to the first treasure
    moveToTreasure1();
    break;
  case 2:
  for (int angle = 40; angle < 140; angle++)
  {
    arm_servo_pos(angle);
    delay(100);
  }
  arm_servo_pos(0);
  delay(300);


      break;
  case 3:
  claw_loop();
  break;
  case 4:

    break;
  case 5:

    break;
  case 6:
    captureBeacon();
    MODE++;
    break;
  case 7:
    moveToTreasure4();
    break;
  case 8:
    // outputCSV(getHeadingToBeacon(TEN_KHZ, TEN_KHZ_READINGS, SAMPLE_PERIOD, STANDARD_OFFSETS),getHeadingToBeacon(ONE_KHZ, ONE_KHZ_READINGS, SAMPLE_PERIOD, STANDARD_OFFSETS),0,0,0);
    IRReadingMode(); // debug mode
    break;
  case 9:
    SERIAL_OUT.println("End of automation sequence");
    break;

  case -1:
    SERIAL_OUT.println("Error");
    break;
  }
}

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

// Manual control of robot, allows drive control and MODE select
void manualMode()
{
  // Normalize drive values to -255 to 255 range
  int leftMotor = (data.j1PotY - 128) * 1.9;
  int rightMotor = (data.j2PotY - 128) * 1.9;
  drive(leftMotor, rightMotor);
  outputCSV(leftMotor, rightMotor, data.tSwitch2, 0, MODE);

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
}

// Move from main course to treasure 1, can we hardcode this?  seems straightforward
void moveToTreasure1()
{
  SERIAL_OUT.println("Moving to treasure 1");
  // Hardcoded sequence here

  // Move to next mode after (grab treasure)
  MODE++;
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

// Follows a set heading (probably stable in the -3 to 3 range) using PID control.
// The left potentiometer tunes P value
// The right potentiometer tunes I value

void followBeacon(int heading)
{
  pidSetpoint = heading;                                                                     // 0 is the heading towards the beacon
  pidInput = getHeadingToBeacon(TEN_KHZ, TEN_KHZ_READINGS, SAMPLE_PERIOD, STANDARD_OFFSETS); // Use the heading offset from beacon as the input
  pot1 = data.pot1 / 2;
  pot2 = data.pot2 / 2;

  myPID.SetTunings(pot1, pot2, 0); // Set the P and I using the two potentiometers for tuning
  myPID.Compute();                 // This will update the pidOutput variable that is linked to myPID

  outputCSV(pot1, pot2, pidInput, (int)pidSetpoint, (int)pidOutput); // Debugging information

  drive(MEDIUM - pidOutput, MEDIUM + pidOutput); // Use the PID output as a wheel speed differential
}

// DEBUGGING and TESTING MODE for IR Array and Heading indicators
void IRReadingMode()
{

  int IRArrayValues[IR_ARRAY_SIZE];

  // Direction from robot to beacon -7 to 7
  int heading;
  getIRArrayValues(IRArrayValues, TEN_KHZ, TEN_KHZ_READINGS, SAMPLE_PERIOD, STANDARD_OFFSETS);
   //heading = convertToHeading(IRArrayValues);

  char telemtery[60];
  sprintf(telemtery, "%d, %d, %d, %d, %d, %d, %d, %d, %d",
          IRArrayValues[0], IRArrayValues[1], IRArrayValues[2], IRArrayValues[3],
          IRArrayValues[4], IRArrayValues[5], IRArrayValues[6], IRArrayValues[7], heading);
  SERIAL_OUT.println(telemtery);
}

void UltrasonicTesting()
{
  ultra_loop();
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
