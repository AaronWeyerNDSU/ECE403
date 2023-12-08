// Include libraries
#include <Arduino.h>
#include "NMEA.hpp"
#include "Position.hpp"

// Define constants
#define GPS Serial1 // GPS module is connected to serial port 1 on Teensy.
#define ENCODER1 2
#define ENCODER2 3
#define ENCODER3 4
#define ENCODER4 5

NMEA gps(&GPS);
// Position position(ENCODER1, ENCODER2, ENCODER3, ENCODER4);
MotorEncoder testenc1(ENCODER1);
MotorEncoder testenc2(ENCODER2);
int heartBeat;
float oldPos[2];

void setup() {
  // Initialize serial communication to USB port and GPS module.
  Serial.begin(9600);
  GPS.begin(9600);
  
  // position.setVelocity(1);
  testenc1.setMotorSpeed(1);
  testenc2.setMotorSpeed(1);

  // oldPos[2] = {0};

  heartBeat = 0;
}

void loop() {
  if (gps.read()) {
    // Serial.println(gps.valid);
    // Serial.println(gps.latitude,5);
    // Serial.println(gps.longitude,5);
    // Serial.println(gps.UTCtime,3);
    // heartBeat = 0;
  }
  

  //position.compareMotionProfiles();
  //position.update();
  // Serial.println(position.getCurrentState());
  // if (oldPos[0] != position.position[0] || oldPos[1] != position.position[1]){
  //   oldPos[0] = position.position[0];
  //   oldPos[1] = position.position[1];
    
  //   Serial.println(position.position[0]);
  //   Serial.println(position.position[1]);
  //   Serial.println(position.getCurrentState());   
  // }

  if (++heartBeat%50000 == 0) {
    heartBeat = 0;
    Serial.println(testenc1.getCount());
    Serial.println(testenc2.getCount());
    Serial.println('.');
    //Serial.println(position.getCurrentState());
  }

  

}
