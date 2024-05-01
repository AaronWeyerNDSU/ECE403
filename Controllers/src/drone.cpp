// Include libraries
#include <Arduino.h>
#include "RTK_drone.hpp"
#include "Position.hpp"
// #include "motors.hpp"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define constants
#define GPS Serial1 // GPS module is connected to serial port 1 on Teensy.
#define ENCODER_FL 18
#define ENCODER_FR 17
#define ENCODER_BL 16
#define ENCODER_BR 15
#define SPEED_FL 5
#define SPEED_FR 4
#define SPEED_BL 3
#define SPEED_BR 2
#define DIRECTION_FL 9
#define DIRECTION_FR 8
#define DIRECTION_BL 7
#define DIRECTION_BR 6

// Motors motors;
RTK_drone gps(&GPS);
Position position(ENCODER_FL, ENCODER_FR, ENCODER_BL, ENCODER_BR, SPEED_FL, SPEED_FR, SPEED_BL, SPEED_BR, DIRECTION_FL, DIRECTION_FR, DIRECTION_BL, DIRECTION_BR);

RF24 radio(14,9);
const byte address[6] = "00016";

int heartBeat;
float oldPos[2];

void setup() {
  // Set motor speed to 0.
  position.setMotorSpeed(0, 0, 0, 0);

  // Initialize serial communication to USB port and GPS module.
  Serial.begin(9600);
  GPS.begin(9600);
  
  // position.setMotorSpeed(1,1,1,1);

  heartBeat = 0;

  gps.startRadio();

  // pinMode(6, OUTPUT);
  // pinMode(7, OUTPUT);
  // pinMode(8, OUTPUT);
  // pinMode(9, OUTPUT);
}

void loop() {
  // digitalWrite(6, HIGH);
  // digitalWrite(7, HIGH);
  // digitalWrite(8, HIGH);
  // digitalWrite(9, HIGH);
  // delay(1000);
  // digitalWrite(6, LOW);
  // digitalWrite(7, LOW);
  // digitalWrite(8, LOW);
  // digitalWrite(9, LOW);
  // delay(1000);
  position.setMotorSpeed(256, 256, 256, 256);
  delay(1000);
  position.setMotorSpeed(0, 0, 0, 0);
  delay(1000);
  position.setMotorSpeed(-256, -256, -256, -256);
  delay(1000);
  position.setMotorSpeed(0, 0, 0, 0);
  delay(1000);

  // const char text[] = "hello world!";
  // radio.write(&text, sizeof(text));
  // delay(1000);
  // motorsForward();
  // delay(1000);
  // motorsBackward();
  // delay(1000);

  // delay(1000);
  // motors.BR(0);
  // delay(1000);
  // motors.BR(128);
  // delay(1000);
  // motors.BR(256);  
  
  // if (gps.read(true)) {
  //   Serial.print(gps.UTCtime,3);
  //   Serial.print(',');
  //   Serial.print(gps.latitude,5);
  //   Serial.print(',');
  //   Serial.println(gps.longitude,5);
    // lightVal = analogRead(A9);
    // Serial.println(lightVal);

    // // memcpy(lightVal, &test, sizeof(double));
    // radio.write(&lightVal, sizeof(double));
    
  // }
  // if(gps.radio.available()){
  //   gps.radio.read(lightVal, sizeof(int));
  //   memcpy(&test, lightVal, sizeof(int));
  //   Serial.println(test);

  // }

  // if (++heartBeat%50000 == 0) {
  //   heartBeat = 0;
  //   Serial.println(".");
  // }
}
