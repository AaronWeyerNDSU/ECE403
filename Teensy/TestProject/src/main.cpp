// Include libraries
#include <Arduino.h>
#include "gps.h"

// Define constants
#define GPS Serial1 // GPS module is connected to serial port 1 on Teensy.

// Define global variables
String GPSmsg;
GPSmessage GPSparsed;

void setup() {
  // Initialize serial communication to USB port and GPS module.
  Serial.begin(9600);
  GPS.begin(9600);
}

void loop() {
  if (GPS.available() > 0) { // Test if read buffer is not empty.
    GPSmsg = GPS.readStringUntil(13); // Read input message
    GPSmsg.trim(); // Trim out any leading spaces.
    if (GPSmsg.startsWith("$GPRMC")) { // Test if image is RMC message
      Serial.println(GPSmsg);
      GPSparsed = parseGPSmessage(GPSmsg);
      Serial.println(GPSparsed.valid);
      Serial.println(GPSparsed.latitude,5);
      Serial.println(GPSparsed.longitude,5);
    }
  } 
}

/* 
Useful links:
  https://www.sparkfun.com/datasheets/GPS/NMEA%20Reference%20Manual-Rev2.1-Dec07.pdf
*/

