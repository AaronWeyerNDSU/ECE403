// Include libraries
#include <Arduino.h>
// #include "RTK_drone.hpp"
#include "Position.hpp"
#include "RadioManager.hpp"
#include "NMEA.hpp"

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

NMEA gps(&GPS);
RadioManager radio(SPI_chip_enable, SPI_chip_select);
Position position(ENCODER_FL, ENCODER_FR, ENCODER_BL, ENCODER_BR, SPEED_FL, SPEED_FR, SPEED_BL, SPEED_BR, DIRECTION_FL, DIRECTION_FR, DIRECTION_BL, DIRECTION_BR);

positionInformation pointInfo;

int heartBeat;

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
  
  heartBeat = 0;
}
int lightValue = 10;
float lat = 15.01, lon = 100 , utc = 1000;
byte byteArray[sizeof(float) * 3];
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
    pointInfo.lightLevel = (float)analogRead(A9);

    int ret = radio.sendPointInfo(pointInfo.latitude, pointInfo.longitude, pointInfo.lightLevel);
  }

  

  // position.setMotorSpeed(256, 256, 256, 256);
  // delay(1000);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(1000);
  // position.setMotorSpeed(-256, -256, -256, -256);
  // delay(1000);
  // position.setMotorSpeed(0, 0, 0, 0);
  // delay(1000);

 
  // delay(1000);
  // lightValue = analogRead(A9);
  // byte byteArray[2*sizeof(int)];
  // memcpy(byteArray, &lightValue, sizeof(int));
  // lightValue+=100;
  // memcpy(byteArray + sizeof(int), &lightValue, sizeof(int));
  // // memcpy(byteArray + sizeof(float), &lon, sizeof(float));
  // // memcpy(byteArray + 2 * sizeof(float), &utc, sizeof(float));

  // // Send the data over the radio.
  // radio.write(byteArray, sizeof(byteArray));
  
  // // const char text[] = "Hello World";
  // lat++;
  // lon++;
  // utc++;
  // lightValue = analogRead(A9);

  // byte byteArray[2*sizeof(int)]; // * 3 + sizeof(int)];
  
  // memcpy(byteArray, &lightValue, 2 * sizeof(int));
  // memcpy(byteArray + sizeof(int), &lightValue, sizeof(int));
  // // memcpy(byteArray + sizeof(float), &lon, sizeof(float));
  // // memcpy(byteArray + 2 * sizeof(float), &utc, sizeof(float));
  // // memcpy(byteArray + 3 * sizeof(float), &lightValue, sizeof(int));
  
  // radio.write(byteArray, sizeof(byteArray));
  
  // Serial.print(sizeof(int));
  // Serial.print(", ");
  // // Serial.print(sizeof(test));
  // Serial.print(", ");
  // Serial.print(lightValue);
  // Serial.println(", ");
  // // Serial.println(test[1]);
  // delay(1000);
  
  // if (gps.read(true)) {
  //   Serial.print(gps.UTCtime,3);
  //   Serial.print(',');
  //   Serial.print(gps.latitude,5);
  //   Serial.print(',');
  //   Serial.println(gps.longitude,5);
  // }

  if (++heartBeat%5000 == 0) {
    heartBeat = 0;
    // Print long debug status string.
    printf("GPS info:\n\tvalid: %s\n\tlat: %f\n\tlon: %f\n\tUTC: %f\n", gps.valid ? "Valid" : "Invalid", gps.latitude, gps.longitude, gps.UTCtime);
    printf("Base Station info:\n\tlat: %f\n\tlon: %f\n\tUTC: %f\n", radio.deviation.latitudeDeviation, radio.deviation.longitudeDeviation, radio.deviation.UTCtime);
    printf("Light Level: %f\n", pointInfo.lightLevel);
    printf("Sent to Base: %s\n", responseSent ? "Message sent" : "Message not sent");
    printf("-----------------------------\n");
  }
}
