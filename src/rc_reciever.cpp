/*
    DIY Arduino based RC Transmitter Project
              == Receiver Code ==
  by Dejan Nedelkovski, www.HowToMechatronics.com
  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdbool.h>
#include "hardware_def.h" // Includes all global Robot Specific Variables

RF24 radio(CE, CNS);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001";

// Global Variables
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
int leftMotor = 0;
int rightMotor = 0;

// Function Definitions
void rcloop();
void setupRadio();
void resetData();
void updateMotorPWM(int leftMotor, int rightMotor);

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
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

Data_Package data; //Create a variable with the above structure

void rcloop() {
  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  }

  // Print the data in the Serial Monitor
  Serial.println("j1PotX: ");
  Serial.print(data.j1PotX);
  Serial.print("; j1PotY: ");
  Serial.print(data.j1PotY);
  Serial.print("; button1: ");
  Serial.print(data.button1);
  Serial.print("; j2PotX: ");
  Serial.print(data.j2PotX); 
  Serial.print("; pot1: ");
  Serial.print(data.pot1); 
  Serial.print("; pot2: ");
  Serial.print(data.pot2); 
}

void updateMotorPWM(int leftMotor, int rightMotor){
  int SIZE = 2;
  int motors[SIZE];
  motors[0] = leftMotor;
  motors[1] = rightMotor;
  int neutral = 128;

// Adjust left motor
    if (motors[0] == neutral){
      analogWrite(L_MOTOR_FORWARD, 0); //  analogWrite values from 0 to 255
      analogWrite(L_MOTOR_REV, 0); //  analogWrite values from 0 to 255
    }
    else if (motors[0] < neutral){
// negative value indicates reverse,
      // Adjust 0-127 signal to full PWM span    
      analogWrite(L_MOTOR_FORWARD, 0); //  analogWrite values from 0 to 255
      analogWrite(L_MOTOR_REV, (neutral-motors[0])*2); //  analogWrite values from 0 to 255  
    }
    else if (motors[0] > neutral){
      // Adjust 128-255 signal to full PWM span   
      analogWrite(L_MOTOR_FORWARD,  (motors[0]-neutral)*(2)); //  analogWrite values from 0 to 255
      analogWrite(L_MOTOR_REV, 0); //  analogWrite values from 0 to 255  
    } else {
      analogWrite(L_MOTOR_FORWARD, 0); //  analogWrite values from 0 to 255
      analogWrite(L_MOTOR_REV, 0); //  analogWrite values from 0 to 255
    }

    // Adjust Right motor
    if (motors[1] == neutral){
      analogWrite(R_MOTOR_FORWARD, 0); //  analogWrite values from 0 to 255
      analogWrite(R_MOTOR_REV, 0); //  analogWrite values from 0 to 255
    }
    else if (motors[1] < neutral){
      // negative value indicates reverse,
      // Adjust 0-127 signal to full PWM span    
      analogWrite(R_MOTOR_FORWARD, 0); //  analogWrite values from 0 to 255
      analogWrite(R_MOTOR_REV, (neutral-motors[1])*2); //  analogWrite values from 0 to 255  
    }
    else if (motors[1] > neutral){
      // Adjust 128-255 signal to full PWM span  
      analogWrite(R_MOTOR_FORWARD,  (motors[1]-neutral)*(2)); //  analogWrite values from 0 to 255
      analogWrite(R_MOTOR_REV, 0); //  analogWrite values from 0 to 255  
    } else {
      analogWrite(R_MOTOR_FORWARD, 0); //  analogWrite values from 0 to 255
      analogWrite(R_MOTOR_REV, 0); //  analogWrite values from 0 to 255
    }
}

void setupRadio(){
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();
  }

void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotX = 127;
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.j1Button = 1;
  data.j2Button = 1;
  data.pot1 = 1;
  data.pot2 = 1;
  data.tSwitch1 = 0;
  data.tSwitch2 = 1;
  data.button1 = 1;
  data.button2 = 1;
  data.button3 = 1;
  data.button4 = 1;
}

// Simon's Code for radio looping, not quite sure how to integrate, may have to break apart into many componets
void autoRadioLoop() {
  // Check whether there is data to be received from RC transmission
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  }

  // Read the left and right joysticks to determine the power to the motors
  leftMotor = data.j1PotY;
  rightMotor = data.j2PotY;

  // Adjust motor PWM signal based on current state
  updateMotorPWM(leftMotor, rightMotor);
}

// Allows us to access left motor data from main
int leftMotorData(){
  return( leftMotor);
}

// Allows us to access right motor data from main
int rightMotorData(){
  return( rightMotor );
}

// Allows us to access potentiometer1 data from main, for tuning
int potentiometerData(){
  int scaledPotData = (int) data.pot1*4;
  return( scaledPotData);
}

// Allows for mode switching, Manual if false, Auto if true
bool manualOrAuto(){
  if (radio.available() && (data.tSwitch1==1)){
      return true;
  }
  else{
    return false;
  }
}