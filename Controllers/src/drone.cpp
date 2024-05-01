// Include libraries
#include <Arduino.h>
// #include "RTK_drone.hpp"
// #include "Position.hpp"
#include "RadioManager.hpp"

// Define constants
#define GPS Serial1 // GPS module is connected to serial port 1 on Teensy.
#define SPI_chip_enable 14
#define SPI_chip_select 10
#define ENCODER1 2
#define ENCODER2 3
#define ENCODER3 4
#define ENCODER4 5

// RTK_drone gps(&GPS);
// Position position(ENCODER1, ENCODER2, ENCODER3, ENCODER4);
// RadioManager radio;
// #include <SPI.h>
// #include <nRF24L01.h>
// #include <RF24.h>


deviationInformation deviation;
positionInformation pointInfo;


// RF24 radio(14, 10); // CE, CSN
// const byte addresses[][6] = {"00001", "00002"};
RadioManager radio(SPI_chip_enable, SPI_chip_select);


int heartBeat;
float oldPos[2];

void setup() {
  // Initialize serial communication to USB port and GPS module.
  Serial.begin(9600);
  // GPS.begin(9600);

  // Begin Radio communication
  Serial.println("Radio starting.");
  radio.startRadio("00002", "00001");
  Serial.println("Radio started.");
  
  // position.setMotorSpeed(1,1,1,1);
  // radio.begin();
  // radio.openWritingPipe(address);
  // radio.setPALevel(RF24_PA_MIN);
  // radio.stopListening();
  // heartBeat = 0;
  // radio.begin();
  // radio.openWritingPipe(addresses[0]); // 00001
  // radio.openReadingPipe(1, addresses[1]); // 00002
  // radio.setPALevel(RF24_PA_MIN);
  // radio.startListening();

  // gps.startRadio();
}
int lightValue = 10;
float lat = 15.01, lon = 100 , utc = 1000;
byte byteArray[sizeof(float) * 3];
void loop() {
  bool baseRead = radio.available();
  if (baseRead) {
        
    deviation = radio.getDeviation();

    Serial.println((String)deviation.latitudeDeviation + ", " + (String)deviation.longitudeDeviation + ", " + (String)deviation.UTCtime);

    pointInfo.latitude = deviation.latitudeDeviation;
    pointInfo.longitude = deviation.longitudeDeviation;
    pointInfo.UTCtime = deviation.UTCtime;
    pointInfo.lightLevel = 15.0;//(float)analogRead(A9);

    delay(100);

    int ret = radio.sendPointInfo(pointInfo.latitude, pointInfo.longitude, pointInfo.UTCtime, pointInfo.lightLevel);
    Serial.println((String)ret + ", " + (String)pointInfo.lightLevel);

    // // Send the data over the radio.
    // radio.stopListening();
    // int ret = radio.write(&pointInfo, sizeof(positionInformation));
    // radio.startListening();
    // Serial.println("sent" + (String)ret);
  }

 
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

  // if (++heartBeat%50000 == 0) {
  //   heartBeat = 0;
  //   Serial.println(".");
  // }
}
