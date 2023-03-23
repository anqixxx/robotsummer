# Team Oscillidope, Robot Summer 2022
A repository for code created for robot summer in ENPH 253!

| Header File   | Source File   | Description   |
| ------------- | ------------- | ------------- |
| N/A  | main.cpp  | Allows for the actual operation of the robot, potentially split up into different modes  |
| hardware_def.h  | N/A  | Defines a pins for repository-wide for the specific uses needed |
| tape_follow.h | tape_follow.cpp | Tape following functions, using reflectance sensors to reorient ourselves |
| ir_sensors.h | ir_sensors.cpp | IR sensor functions, to filter and determine the robot’s location |
| motor_drive.h | motor_drive.cpp | Motor Drive Functions, rotary encoder and wheel motor directions |
| field_nav.h | field_nav.cpp | Field navigation, using reflectance, ultrasonic, etc to detect objects (other than treasure) |
| claw.h | claw.cpp | Claw Functions, reacting to sensors, placement of the claw, speed of the claw, location of the claw, arm rotation |
| treasure.h | treasure.cpp | Treasure detecting functions, from ultrasonic sensors, hall effect sensors, and reflectance sensors |
| serial_coms.h | serial_coms.cpp | Allows for communication between bluetooth and our Robot! (Due to this, it has access to more libraries than other source files) |
| rc_reciever.h*  | rc_reciever.cpp*  | Allows for controlling the robot with an RC Controller, testing mode only  |

Video of robot picking up treasures: https://drive.google.com/file/d/1E-0XAfuZY4gkAFgjbmka8zm1gYZMEI6N/view?usp=sharing 

* Note, will  also utlize nRF24L01.h, RF24_config.h, RF24.h, and RF24.cpp, all taken online from the TMRh20/RF24 library, https://github.com/tmrh20/RF24/

