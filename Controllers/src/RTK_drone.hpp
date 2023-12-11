#ifndef RTK_DRONE_HPP
#define RTK_DRONE_HPP

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "NMEA.hpp"

class RTK_drone : public NMEA {
    public:
        RTK_drone(Stream *serial, int SPI_chip_enable, int SPI_chip_select);
        bool read();

    private:
        RF24 radio;
        bool newData;
        int status; // 0 invalid, 1 depreciated, 2 valid.
        float latiudeDeviation;
        float longitudeDeviation;
        float baseUTCtime;

};

RTK_drone::RTK_drone(Stream *serial, int SPI_chip_enable = 7, int SPI_chip_select = 8) : NMEA(serial){
    RF24 radio(SPI_chip_enable, SPI_chip_select); // CE, CSN
    const byte address[6] = "00001";
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
    latiudeDeviation = 0;
    longitudeDeviation = 0;
    baseUTCtime = 0;
};

bool RTK_drone::read(){
    bool gpsRead = NMEA::read();
    Serial.println("Got NMEA data.");

    bool baseRead;
    if (radio.available()) {
        Serial.println("radio is available.");
        baseRead = true;
        char text[32] = "";
        radio.read(&text, sizeof(text));
        Serial.println(text);
    } else {
        baseRead = false;
    }

    if ((gpsRead || baseRead) && UTCtime == baseUTCtime){
        return true;
    }
    return false;

}

#endif