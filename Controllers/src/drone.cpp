// Include libraries
#include <Arduino.h>
#include "NMEA.hpp"
#include "Position.hpp"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define constants
#define GPS Serial1 // GPS module is connected to serial port 1 on Teensy.
#define ENCODER1 2
#define ENCODER2 3
#define ENCODER3 4
#define ENCODER4 5

NMEA gps(&GPS);
Position position(ENCODER1, ENCODER2, ENCODER3, ENCODER4);
RF24 radio(7, 8); // CE, CSN

int heartBeat;
float oldPos[2];
const byte address[6] = "00001";

void setup() {
  // Initialize serial communication to USB port and GPS module.
  Serial.begin(9600);
  
  GPS.begin(9600);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
  position.setMotorSpeed(1,1,1,1);

  // oldPos[2] = {0};

  heartBeat = 0;
}

void loop() {
  if (gps.read()) {
    Serial.println("GPS:");
    Serial.println(gps.valid);
    Serial.println(gps.latitude,5);
    Serial.println(gps.longitude,5);
    Serial.println(gps.UTCtime,3);

    Serial.println("Position:");
    Serial.println(position.getCurrentState());
    Serial.println(position.getMotorSpeed());
    Serial.println(position.X);
    Serial.println(position.Y);
    position.update();
  }
  
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
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

  // if (++heartBeat%50000 == 0) {
  //   heartBeat = 0;
    
  // }
}
