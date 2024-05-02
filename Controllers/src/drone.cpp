// Include libraries
#include <Arduino.h>
// #include "RTK_drone.hpp"
#include "Position.hpp"
#include "RadioManager.hpp"
#include "NMEA.hpp"

#define DEBUG true

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
  maxBrightness = (float) analogRead(PHOTO_RESISTOR);
  // Move forward at start of program.
  // position.setMotorSpeed(200, 200, 200, 200);
}

void loop() {
  // Check for new GPS coordinates
  if(gps.read()){
    // Serial.println((String)gps.valid + ", " + (String)gps.latitude + ", " + (String)gps.longitude + ", " + (String)gps.UTCtime);
    responseSent = false;
  }
  
  bool baseRead = radio.available();
  if (baseRead) {
    // get deviation information from base station.
    radio.getDeviation();
    //Serial.println((String)deviation.latitudeDeviation + ", " + (String)deviation.longitudeDeviation + ", " + (String)deviation.UTCtime);
  }

  if(gps.UTCtime == radio.deviation.UTCtime && !responseSent){
    // Set response flag to prevent repeat messages.
    responseSent = true;

    pointInfo.latitude = gps.latitude - radio.deviation.latitudeDeviation;
    pointInfo.longitude = gps.longitude - radio.deviation.longitudeDeviation;
    pointInfo.lightLevel = (float)analogRead(PHOTO_RESISTOR);

    int ret = radio.sendPointInfo(pointInfo.latitude, pointInfo.longitude, pointInfo.lightLevel);
  }

  // float currentBrightness = (float) analogRead(PHOTO_RESISTOR);
  // if(currentBrightness > maxBrightness){
  //   maxBrightness = currentBrightness;
  // } else if(currentBrightness < 0.95*maxBrightness){
  //   position.setMotorSpeed(0, 0, 0, 0);
  // }

  // Serial.println(currentBrightness);
  // delay(500);
  
  // Move in octagon
  // position.setMotorSpeed(256, 256, 256, 256); // Forward
  // delay(500);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // position.setMotorSpeed(0, 256, 256, 0); // Left Diagonal Forward
  // delay(500);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // position.setMotorSpeed(-256, 256, 256, -256); // Left
  // delay(500);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // position.setMotorSpeed(-256, 0, 0, -256); // Left Diagonal Reverse
  // delay(500);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // position.setMotorSpeed(-256, -256, -256, -256); // Reverse
  // delay(500);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // position.setMotorSpeed(0, -256, -256, 0); // Right Diagonal Reverse
  // delay(500);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // position.setMotorSpeed(256, -256, -256, 256); // Right
  // delay(500);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // position.setMotorSpeed(256, 0, 0, 256); // Right Diagonal Forward
  // delay(500);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);

  // Move Forward and Backwards
  // position.setMotorSpeed(256, 256, 256, 256);
  // delay(500);
  // position.setMotorSpeed(170, 170, 170, 170);
  // delay(500);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(500);
  // position.setMotorSpeed(-170, -170, -170, -170);
  // delay(500);
  // position.setMotorSpeed(-256, -256, -256, -256);
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
