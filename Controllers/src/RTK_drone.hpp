#ifndef RTK_DRONE_HPP
#define RTK_DRONE_HPP

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "NMEA.hpp"

/// @brief Enables GPS RTK on the drone platform.
class RTK_drone : public NMEA {
    public:
        RTK_drone(Stream *serial, int SPI_chip_enable, int SPI_chip_select);
        void startRadio();
        bool read(bool allow_depreciated);

    private:
        RF24 radio;
        float latitudeDeviation;
        float longitudeDeviation;
        float baseUTCtime;

};

/// @brief Constructor for drone RTK class. Enables RTK on the drone platform.
/// @param serial Serial port that the GPS communicates on.
/// @param SPI_chip_enable pin designated as SPI CE. Used for radio communication with base station.
/// @param SPI_chip_select pin designated as SPI CS. Used for radio communication with base station.
RTK_drone::RTK_drone(Stream *serial, int SPI_chip_enable = 7, int SPI_chip_select = 8) : NMEA(serial), radio(SPI_chip_enable, SPI_chip_select){
    latitudeDeviation = 0;
    longitudeDeviation = 0;
    baseUTCtime = 0;
};

/// @brief Run in setup. Enable the radio communications to talk to the base station.
void RTK_drone::startRadio(){
    const byte address[6] = "00001";
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
}

/// @brief Get GPS information from the local GPS, then apply the RTK correction to the coordinates.
/// @param allow_depreciated Allow GPS values to update even if communication with base station is lost.
/// @return True if new valid data is loaded, False otherwise. 
bool RTK_drone::read(bool allow_depreciated = false){
    // Get information from local GPS.
    bool gpsRead = NMEA::read();

    // See if information is available from base station.
    bool baseRead = radio.available();
    if (baseRead) {
        // Read the byte array
        byte byteArray[sizeof(float) * 3];
        radio.read(byteArray, sizeof(byteArray));

        // Convert byte array to floats
        memcpy(&latitudeDeviation, byteArray, sizeof(float));
        memcpy(&longitudeDeviation, byteArray + sizeof(float), sizeof(float));
        memcpy(&baseUTCtime, byteArray + 2 * sizeof(float), sizeof(float));  
    }

    // If GPS data is valid, and something has been updated, and the UTC time stamps match.
    if (valid && (gpsRead || baseRead) && (UTCtime == baseUTCtime)){
        // Apply the RTK correction to the coordinates. Return true.
        latitude -= latitudeDeviation;
        longitude -= longitudeDeviation;
        return true;
    // If depreciated mode is enabled, and data was read from the local GPS module, return true.
    } else if (allow_depreciated && valid && gpsRead){
        return true;
    // If no data was updated, return false.
    } else {
        return false;
    }
}

#endif