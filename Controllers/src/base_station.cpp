#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "RTK_base.hpp"
#include "SoftwareSerial.h"

// #define GPS SoftwareSerial // GPS module is connected to serial port.
#define SPI_chip_enable 7
#define SPI_chip_select 8
#define Serial Serial

SoftwareSerial GPS = SoftwareSerial(3, 2);
RTK_base gps(&GPS);
RF24 radio(SPI_chip_enable, SPI_chip_select); // CE, CSN
const byte address[6] = "00001";
unsigned long start_Timer;
unsigned long end_Timer;
int lat;
int lon;
int utime;
int lightLevel;

void setup() {
  // Start communication with the GPS module.
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  GPS.begin(9600);
  Serial.begin(9600);
  //gps.calibrate();


  // Start communication with wireless radio.
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  //Serial.println("Setup complete");
  delay(500);
}

void loop() {
  // // Check if new GPS deviation is available.
  // if (gps.getDeviation()){
  //   // Convert deviation information to byte array
  //   memcpy(byteArray, &gps.latitudeDeviation, sizeof(float));
  //   memcpy(byteArray + sizeof(float), &gps.longitudeDeviation, sizeof(float));
  //   memcpy(byteArray + 2 * sizeof(float), &gps.UTCtime, sizeof(float));

  //   // Serial.print(gps.latitudeDeviation, 6);
  //   // Serial.print(", ");
  //   // Serial.print(gps.latitudeDeviation, 6);
  //   // Serial.print(", ");
  //   // Serial.println(gps.UTCtime);

  //   // Send the data over the radio.
  //   //radio.write(byteArray, sizeof(byteArray));
  // }

  if (radio.available()) {
    end_Timer = micros();
    //float time = ((float)end_Timer - (float)start_Timer)/1000000;
    byte byteArray[8];
    radio.read(&byteArray, sizeof(byteArray));

    memcpy(&lat, byteArray, sizeof(float));
    memcpy(&lon, byteArray + 4, sizeof(float));
    memcpy(&utime, byteArray + 8, sizeof(float));
    memcpy(&lightLevel, byteArray + 12, sizeof(int));

    //Serial.println(sizeof(float));

    Serial.println((String)lat);
    Serial.println(" " + (String)lon);
    Serial.println(" " + (String)utime);
    Serial.println(" " + (String)lightLevel);

    // Serial.println();
    // Serial.println("Time since last hit:" + (String)(time));
    // start_Timer = end_Timer;
  }

  lat = 1;
  lon = 2;
  utime = 3;
  lightLevel = 4;

  Serial.print((String)lat + " ");
  Serial.print((String)lon + " ");
  Serial.print((String)utime + " ");
  Serial.println((String)lightLevel);
}