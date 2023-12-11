#ifndef RTK_BASE_HPP
#define RTK_BASE_HPP

#include <Arduino.h>
#include "NMEA.hpp"

class RTK_base : public NMEA {
    public:
        RTK_base(Stream *serial);
        void calibrate(int maxPoints);
        bool getDeviation();
        float latitudeDeviation;
        float longitudeDeviation;
        float latitudeExact;
        float longitudeExact;
    private:
        
};

/// @brief Initialize RTK base station.
/// @param serial Serial port used to read GPS.
RTK_base::RTK_base(Stream *serial): NMEA(serial){
    
}
/// @brief Get the exact position of the GPS module by averaging a large number of readings together.
/// @param maxPoints Quantity of points to collect for finding base station exact location.
void RTK_base::calibrate(int maxPoints = 100){
    // Temporary variables
    int points = 0;
    float latitudeArray[maxPoints] = {0};
    float longitudeArray[maxPoints] = {0};
    float latitudeTotal = 0;
    float longitudeTotal = 0;

    // Loop until all points collected.
    while (points < maxPoints){
        // Wait for new NMEA message.
        if (read()){
            // Check to make sure data is valid.
            if (valid){
                // Add data to averaging array and increment points collected.
                latitudeArray[points] = latitude;
                longitudeArray[points] = longitude;
                points++;
            }
        }
    }

    // Sum collected points and take average.
    for (int i = 0; i < maxPoints; i++){
        latitudeTotal += latitudeArray[i];
        longitudeTotal += longitudeArray[i];
    }
    latitudeExact = latitudeTotal/maxPoints;
    longitudeExact = longitudeTotal/maxPoints;
}

bool RTK_base::getDeviation(){
    // If no new data return false.
    if (!(read())) { return false; }

    // If data is not valid return false.
    if (!valid) { return false; }

    // Subtract measured location from exact location to get deviation.
    latitudeDeviation = latitude-latitudeExact;
    longitudeDeviation = longitude-longitudeExact;

    // Return true so controller knows there is new data.
    return true;
}

#endif