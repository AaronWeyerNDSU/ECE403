// Include libraries
#include <Arduino.h>
#include "RTK_drone.hpp"
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

// RTK_drone gps(&GPS);
Position position(ENCODER1, ENCODER2, ENCODER3, ENCODER4);
NMEA gps(&GPS);

RF24 radio(7, 8); // CE, CSN

int heartBeat;
float oldPos[2];

void setup() {
  // Initialize serial communication to USB port and GPS module.
  Serial.begin(9600);
  GPS.begin(9600);
  
  position.setMotorSpeed(1,1,1,1);

  // oldPos[2] = {0};

  heartBeat = 0;


  Serial.println("finishing Setup.");

  
  const byte address[6] = "00001";
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  // Serial.println("Readin this shit.");
  if (gps.read()) {
    Serial.println("GPS:");
    Serial.println(gps.valid);
    Serial.println(gps.latitude,5);
    Serial.println(gps.longitude,5);
    Serial.println(gps.UTCtime,3);

    // Serial.println("Position:");
    // Serial.println(position.getCurrentState());
    // Serial.println(position.getMotorSpeed());
    // Serial.println(position.X);
    // Serial.println(position.Y);
    // position.update();
  }
  // if (radio.available()) {
  //   char text[32] = "";
  //   radio.read(&text, sizeof(text));
  //   Serial.println(text);
  // }
  if (radio.available()) {
    float latitudeDeviation, longitudeDeviation, remoteUTCtime, remoteLat, remoteLong, remoteLatExact, remoteLongExact;

    // Read the byte array
    byte byteArray[sizeof(float) * 3];
    radio.read(byteArray, sizeof(byteArray));

    // Convert byte array to floats
    memcpy(&latitudeDeviation, byteArray, sizeof(float));
    memcpy(&longitudeDeviation, byteArray + sizeof(float), sizeof(float));
    memcpy(&remoteUTCtime, byteArray + 2 * sizeof(float), sizeof(float));
    memcpy(&remoteLat, byteArray + 3 * sizeof(float), sizeof(float));
    memcpy(&remoteLong, byteArray + 4 * sizeof(float), sizeof(float));
    memcpy(&remoteLatExact, byteArray + 5 * sizeof(float), sizeof(float));
    memcpy(&remoteLongExact, byteArray + 6 * sizeof(float), sizeof(float));


    // Print the received floats
    Serial.print("lat deviation: ");
    Serial.println(latitudeDeviation,6);
    Serial.print("long deviation: ");
    Serial.println(longitudeDeviation,6);
    Serial.print("UTC Time: ");
    Serial.println(remoteUTCtime,3);
    Serial.print("remote lat: ");
    Serial.println(remoteLat,6);
    Serial.print("remote Long: ");
    Serial.println(remoteLong,6);
    Serial.print("remote Lat Exact: ");
    Serial.println(remoteLatExact,6);
    Serial.print("Remote long Exact: ");
    Serial.println(remoteLongExact,6);
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
