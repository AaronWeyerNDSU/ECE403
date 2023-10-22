// Include libraries
#include <Arduino.h>
#include "NMEA.hpp"

// Define constants
#define GPS Serial1 // GPS module is connected to serial port 1 on Teensy.

NMEA gps(&GPS);
int heartBeat;

void setup() {
  // Initialize serial communication to USB port and GPS module.
  Serial.begin(9600);
  GPS.begin(9600);
  heartBeat = 0;
}

void loop() {
  if (gps.read()) {
    Serial.println(gps.valid);
    Serial.println(gps.latitude,5);
    Serial.println(gps.longitude,5);
    Serial.println(gps.UTCtime,3);
    heartBeat = 0;
  }

  if (++heartBeat%50000 == 0) {
    heartBeat = 0;
    Serial.println(".");
  }
}
