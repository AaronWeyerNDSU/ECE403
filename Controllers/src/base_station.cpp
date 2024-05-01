#include <Arduino.h>
#include "RTK_base.hpp"
#include <SoftwareSerial.h>
#include "RadioManager.hpp"

#define SPI_chip_enable 7
#define SPI_chip_select 8
#define GPS_RX 3
#define GPS_TX 2

// Define base station resources.
SoftwareSerial GPS(GPS_RX, GPS_TX);
RTK_base gps(&GPS);
RadioManager radio(SPI_chip_enable, SPI_chip_select);


void setup() {
  // Open base station comms
  Serial.begin(9600);

  // Start communication with the GPS module.
  Serial.println("GPS starting.");
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  GPS.begin(9600);
  Serial.println("GPS started.");

  // Start Radio communication.
  Serial.println("Radio starting.");
  radio.startRadio("00001", "00002");
  Serial.println("Radio started.");

  // Calibrate GPS to get current location.
  Serial.println("GPS calibrating");
  gps.calibrate(1);
  Serial.println("GPS calibrated");
}

void loop() {
  // Check if new GPS deviation is available.
  if (gps.getDeviation()){
    // Send the deviation information to the robot.
    int ret = radio.sendDeviation(gps.latitudeDeviation, gps.longitudeDeviation, gps.UTCtime);
    // Print return code from radio.
    Serial.println(ret);
  }

  // If there is new data available on the radio.
  if(radio.available()){
    // Read in the new point information.
    radio.getPointInfo();
    // Print point information to terminal.
    Serial.println((String)radio.pointInfo.latitude + ", " + (String)radio.pointInfo.longitude + ", " + (String)radio.pointInfo.UTCtime + ", " + (String)radio.pointInfo.lightLevel);
  }
}