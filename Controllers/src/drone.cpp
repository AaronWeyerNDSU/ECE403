// Include libraries
#include <Arduino.h>
// #include "RTK_drone.hpp"
#include "Position.hpp"
#include "RadioManager.hpp"
#include "NMEA.hpp"

#define DEBUG false

// Define constants
#define GPS Serial1 // GPS module is connected to serial port 1 on Teensy.
#define SPI_chip_enable 14
#define SPI_chip_select 10
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
#define PHOTO_RESISTOR A9

NMEA gps(&GPS);
RadioManager radio(SPI_chip_enable, SPI_chip_select);
Position position(ENCODER_FL, ENCODER_FR, ENCODER_BL, ENCODER_BR, SPEED_FL, SPEED_FR, SPEED_BL, SPEED_BR, DIRECTION_FL, DIRECTION_FR, DIRECTION_BL, DIRECTION_BR);

positionInformation pointInfo;

int heartBeat = 0;
float maxBrightness = 0;

bool responseSent = false;

// Demo Variables
float fakePos[9][2] = {
  {1,1},
  {1,2},
  {1,3},
  {2,1},
  {2,2},
  {2,3},
  {3,1},
  {3,2},
  {3,3}
};
int motSpeed = 200;
int driveTime = 500;
int posIndex = 0;

void setup() {
  // Set motor speed to zero to stop motors from spinning at startup.
  position.setMotorSpeed(0, 0, 0, 0);

  // Initialize debugging serial communication.
  Serial.begin(115200);

  // Initialize GPS serial communication.
  GPS.begin(9600);

  // Begin Radio communication
  Serial.println("Radio starting.");
  radio.startRadio("00002", "00001");
  Serial.println("Radio started.");

  // Set initial brightness of starting position.
  // maxBrightness = (float) analogRead(PHOTO_RESISTOR);
  // Move forward at start of program.
  position.setMotorSpeed(200, 200, 200, 200);
}

void loop() {
  // FakeGPS demo
  // for(int i = 0; i < 9; i++){
  //   radio.sendPointInfo(fakePos[i][0],fakePos[i][1], (float) analogRead(PHOTO_RESISTOR));
  //   delay(1000);
  // }

  // Check for new GPS coordinates
  // if(gps.read()){
  //   // Serial.println((String)gps.valid + ", " + (String)gps.latitude + ", " + (String)gps.longitude + ", " + (String)gps.UTCtime);
  //   responseSent = false;
  // }
  
  // bool baseRead = radio.available();
  // if (baseRead) {
  //   // get deviation information from base station.
  //   radio.getDeviation();
  //   //Serial.println((String)deviation.latitudeDeviation + ", " + (String)deviation.longitudeDeviation + ", " + (String)deviation.UTCtime);
  // }

  // if(gps.UTCtime == radio.deviation.UTCtime && !responseSent){
  //   // Set response flag to prevent repeat messages.
  //   responseSent = true;

  //   pointInfo.latitude = gps.latitude - radio.deviation.latitudeDeviation;
  //   pointInfo.longitude = gps.longitude - radio.deviation.longitudeDeviation;
  //   pointInfo.lightLevel = (float)analogRead(PHOTO_RESISTOR);

  //   int ret = radio.sendPointInfo(pointInfo.latitude, pointInfo.longitude, pointInfo.lightLevel);
  // }

  float currentBrightness = (float) analogRead(PHOTO_RESISTOR);
  if(currentBrightness > maxBrightness){
    maxBrightness = currentBrightness;
  } else if(currentBrightness < 0.95*maxBrightness){
    position.setMotorSpeed(0, 0, 0, 0);
  }

  Serial.println(currentBrightness);
  delay(500);
  
  // Move in octagon
  // position.setMotorSpeed(motSpeed, motSpeed, motSpeed, motSpeed); // Forward
  // delay(driveTime);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // posIndex = 0;
  // radio.sendPointInfo(fakePos[posIndex][0],fakePos[posIndex][1], (float) analogRead(PHOTO_RESISTOR));
  // position.setMotorSpeed(0, motSpeed, motSpeed, 0); // Left Diagonal Forward
  // delay(driveTime);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // posIndex = 1;
  // radio.sendPointInfo(fakePos[posIndex][0],fakePos[posIndex][1], (float) analogRead(PHOTO_RESISTOR));
  // position.setMotorSpeed(-motSpeed, motSpeed, motSpeed, -motSpeed); // Left
  // delay(driveTime);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // posIndex = 2;
  // radio.sendPointInfo(fakePos[posIndex][0],fakePos[posIndex][1], (float) analogRead(PHOTO_RESISTOR));
  // position.setMotorSpeed(-motSpeed, 0, 0, -motSpeed); // Left Diagonal Reverse
  // delay(driveTime);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // posIndex = 3;
  // radio.sendPointInfo(fakePos[posIndex][0],fakePos[posIndex][1], (float) analogRead(PHOTO_RESISTOR));
  // position.setMotorSpeed(-motSpeed, -motSpeed, -motSpeed, -motSpeed); // Reverse
  // delay(driveTime);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // posIndex = 4;
  // radio.sendPointInfo(fakePos[posIndex][0],fakePos[posIndex][1], (float) analogRead(PHOTO_RESISTOR));
  // position.setMotorSpeed(0, -motSpeed, -motSpeed, 0); // Right Diagonal Reverse
  // delay(driveTime);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // posIndex = 5;
  // radio.sendPointInfo(fakePos[posIndex][0],fakePos[posIndex][1], (float) analogRead(PHOTO_RESISTOR));
  // position.setMotorSpeed(motSpeed, -motSpeed, -motSpeed, motSpeed); // Right
  // delay(driveTime);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // posIndex = 6;
  // radio.sendPointInfo(fakePos[posIndex][0],fakePos[posIndex][1], (float) analogRead(PHOTO_RESISTOR));
  // position.setMotorSpeed(motSpeed, 0, 0, motSpeed); // Right Diagonal Forward
  // delay(driveTime);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // posIndex = 8;
  // radio.sendPointInfo(fakePos[posIndex][0],fakePos[posIndex][1], (float) analogRead(PHOTO_RESISTOR));


  // Move Forward and Backwards
  // position.setMotorSpeed(motSpeed, motSpeed, motSpeed, motSpeed);
  // delay(500);
  // position.setMotorSpeed(170, 170, 170, 170);
  // delay(500);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // position.setMotorSpeed(-170, -170, -170, -170);
  // delay(500);
  // position.setMotorSpeed(-motSpeed, -motSpeed, -motSpeed, -motSpeed);
  // delay(500);
  // position.setMotorSpeed(-170, -170, -170, -170);
  // delay(500);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // position.setMotorSpeed(170, 170, 170, 170);
  // delay(500);

  
  // if (gps.read(true)) {
  //   Serial.print(gps.UTCtime,3);
  //   Serial.print(',');
  //   Serial.print(gps.latitude,5);
  //   Serial.print(',');
  //   Serial.println(gps.longitude,5);
  // }

  if (++heartBeat%1000 == 0 && DEBUG) {
    heartBeat = 0;
    // Print long debug status string.
    printf("GPS info:\n\tvalid: %s\n\tlat: %f\n\tlon: %f\n\tUTC: %f\n", gps.valid ? "Valid" : "Invalid", gps.latitude, gps.longitude, gps.UTCtime);
    printf("Base Station info:\n\tlat: %f\n\tlon: %f\n\tUTC: %f\n", radio.deviation.latitudeDeviation, radio.deviation.longitudeDeviation, radio.deviation.UTCtime);
    printf("Light Level: %f\n", pointInfo.lightLevel);
    printf("Sent to Base: %s\n", responseSent ? "Message sent" : "Message not sent");
    printf("-----------------------------\n");
  }
}
