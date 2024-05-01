#ifndef RADIOMANAGER_HPP
#define RADIOMANAGER_HPP

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

struct deviationInformation {
  float latitudeDeviation;
  float longitudeDeviation;
  float UTCtime;
};
struct positionInformation {
  float latitude;
  float longitude;
  float UTCtime;
  float lightLevel;
};

/// @brief Enables GPS RTK on the drone platform.
class RadioManager {
    public:
        RadioManager(int SPI_chip_enable, int SPI_chip_selec);
        void startRadio(const byte readAddress[6], const byte writeAddress[6]);
        int sendDeviation(float latitudeDeviation, float longitudeDeviation, float UTCtime);
        int sendPointInfo(float latitude, float longitude, float UTCtime, float lightLevel);
        deviationInformation deviation;
        deviationInformation getDeviation();
        positionInformation pointInfo;
        positionInformation getPointInfo();
        bool available();
        bool read(bool allow_depreciated);

    private:
        RF24 radio;
};

/// @brief Constructor for drone RTK class. Enables RTK on the drone platform.
/// @param SPI_chip_enable pin designated as SPI CE. Used for radio communication with base station.
/// @param SPI_chip_select pin designated as SPI CS. Used for radio communication with base station.
RadioManager::RadioManager(int SPI_chip_enable, int SPI_chip_select) : radio(SPI_chip_enable, SPI_chip_select){

};

void RadioManager::startRadio(const byte readAddress[6], const byte writeAddress[6]){
    radio.begin();
    radio.openWritingPipe(writeAddress);
    radio.openReadingPipe(1, readAddress);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
}
int RadioManager::sendDeviation(float latitudeDeviation, float longitudeDeviation, float UTCtime){
    // Stop radio from listening.
    radio.stopListening();

    // Copy variables to structure for writing.
    // deviationInformation deviation;
    deviation.latitudeDeviation = latitudeDeviation;
    deviation.longitudeDeviation = longitudeDeviation;
    deviation.UTCtime = UTCtime;

    // Write deviation information to radio.
    int ret = radio.write(&deviation, sizeof(deviationInformation));
    
    // Tell radio to start listening again.
    radio.startListening();
    // Return if successful write.
    return ret;
}

int RadioManager::sendPointInfo(float latitude, float longitude, float UTCtime, float lightLevel){
    // Stop radio from listening.
    radio.stopListening();

    // Copy variables to structure for writing.
    pointInfo.latitude = latitude;
    pointInfo.longitude = longitude;
    pointInfo.UTCtime = UTCtime;
    pointInfo.lightLevel = lightLevel;

    // Write position information to radio.
    int ret = radio.write(&pointInfo, sizeof(deviationInformation));
    
    // Tell radio to start listening again.
    radio.startListening();

    // Return if successful write.
    return ret;
}

deviationInformation RadioManager::getDeviation(){
    radio.read(&deviation, sizeof(deviationInformation));
    return deviation;
}

positionInformation RadioManager::getPointInfo(){
    radio.read(&pointInfo, sizeof(positionInformation));
    return pointInfo;
}

bool RadioManager::available(){
    return radio.available();
}





#endif