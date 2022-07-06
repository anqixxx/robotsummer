# robotsummer
A repository for code created for robot summer in ENPH 259!

| Header File   | Source File   | Description   |
| ------------- | ------------- | ------------- |
| N/A  | Content Cell  | Content Cell  |
| Content Cell  | Content Cell  | Content Cell  |
| Content Cell  | Content Cell  | Content Cell  |
| Content Cell  | Content Cell  | Content Cell  |
| Content Cell  | Content Cell  | Content Cell  |

N/A
Main
Allows for the actual operation of the robot, potentially split up into different modes
tape_follow.h
tape_follow.cpp
Tape following functions, using reflectance sensors to reorient ourselves
ir_sensors.h
ir_sensors.cpp
IR sensor functions, to filter and determine the robot’s location
motor_drive.h
motor_drive.cpp
Motor Drive Functions, rotary encoder and wheel motor directions
field_nav.h
field_nav.cpp
Field navigation, using reflectance, ultrasonic, etc to detect objects (other than treasure)
claw.h
claw.cpp
Claw Functions, reacting to sensors, placement of the claw, speed of the claw, location of the claw
arm.h
arm.cpp
Arm Functions, height and angle for the claw
treasure.h
treasure.cpp
Treasure detecting functions, from ultrasonic sensors, hall effect sensors, and reflectance sensors

