#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "RTK_base.hpp"

#define GPS Serial // GPS module is connected to serial port.
#define SPI_chip_enable 7
#define SPI_chip_select 8

RTK_base gps(&GPS);
//NMEA gps(&GPS);
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

  // Indicator light for if GPS data is valid.
  //pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // if (Serial.available()) {
  //   String data = Serial.readStringUntil(13);
  //   data.trim(); // Trim leading blankspaces.
  //   if (data.startsWith("$GPRMC")) { 
  //     int strLength = data.length() + 1;
  //     char msgChar[strLength];
  //     data.toCharArray(msgChar, strLength);
  //     Serial.println(msgChar);
  //     //const char text[] = "Bello Borld";
  //     radio.write(&msgChar, sizeof(msgChar));
  //     //delay(1000);
  //   }
  // }

  // Serial.println("gamer");
  // if (gps.read()) {
  //   Serial.println("GPS:");
  //   Serial.println(gps.valid);
  //   Serial.println(gps.latitude,5);
  //   Serial.println(gps.longitude,5);
  //   Serial.println(gps.UTCtime,3);
  // }

  // Check if new GPS deviation is available.
  if (gps.getDeviation()){
    // Convert floats to byte array
    byte byteArray[sizeof(float) * 3];
    memcpy(byteArray, &gps.latitudeDeviation, sizeof(float));
    memcpy(byteArray + sizeof(float), &gps.longitudeDeviation, sizeof(float));
    memcpy(byteArray + 2 * sizeof(float), &gps.UTCtime, sizeof(float));

    memcpy(byteArray + 3 * sizeof(float), &gps.latitude, sizeof(float));
    memcpy(byteArray + 4 * sizeof(float), &gps.longitude, sizeof(float));
    memcpy(byteArray + 5 * sizeof(float), &gps.latitudeExact, sizeof(float));
    memcpy(byteArray + 6 * sizeof(float), &gps.longitudeExact, sizeof(float));

    // Send the byte array
    radio.write(byteArray, sizeof(byteArray));
  }

  // Visiual Indication if data is valid.
  // if (gps.valid){
  //   digitalWrite(LED_BUILTIN, HIGH);
  // } else {
  //   digitalWrite(LED_BUILTIN, LOW);
  // }  
}