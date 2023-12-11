#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "RTK_base.hpp"

#define GPS Serial // GPS module is connected to serial port.
#define SPI_chip_enable 7
#define SPI_chip_select 8

RTK_base gps(&GPS);
RF24 radio(SPI_chip_enable, SPI_chip_select); // CE, CSN
const byte address[6] = "00001";

void setup() {
  // Start communication with the GPS module.
  GPS.begin(9600);
  gps.calibrate();

  // Start communication with wireless radio.
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // Check if new GPS deviation is available.
  if (gps.getDeviation()){
    // Convert deviation information to byte array
    byte byteArray[sizeof(float) * 3];
    memcpy(byteArray, &gps.latitudeDeviation, sizeof(float));
    memcpy(byteArray + sizeof(float), &gps.longitudeDeviation, sizeof(float));
    memcpy(byteArray + 2 * sizeof(float), &gps.UTCtime, sizeof(float));

    // Send the data over the radio.
    radio.write(byteArray, sizeof(byteArray));
  } 
}