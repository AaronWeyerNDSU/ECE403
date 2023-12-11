// Include libraries
#include <Arduino.h>
#include "RTK_drone.hpp"
#include "Position.hpp"

// Define constants
#define GPS Serial1 // GPS module is connected to serial port 1 on Teensy.
#define ENCODER1 2
#define ENCODER2 3
#define ENCODER3 4
#define ENCODER4 5

RTK_drone gps(&GPS);
Position position(ENCODER1, ENCODER2, ENCODER3, ENCODER4);

int heartBeat;
float oldPos[2];

void setup() {
  // Initialize serial communication to USB port and GPS module.
  Serial.begin(9600);
  GPS.begin(9600);
  
  position.setMotorSpeed(1,1,1,1);

  heartBeat = 0;

  gps.startRadio();
}

void loop() {
  if (gps.read(true)) {
    Serial.print(gps.UTCtime,3);
    Serial.print(',');
    Serial.print(gps.latitude,5);
    Serial.print(',');
    Serial.println(gps.longitude,5);
  }

  // if (++heartBeat%50000 == 0) {
  //   heartBeat = 0;
  //   Serial.println(".");
  // }
}
