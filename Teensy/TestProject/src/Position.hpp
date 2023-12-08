#ifndef POSITION_HPP
#define POSITION_HPP

#include <Arduino.h>
#include <vector>
#include "MotorEncoder.hpp"

class Position {
public:
    Position(int encoderPinFL, int encoderPinFR, int encoderPinBL, int encoderPinBR);
    std::vector<float> getPosition();
    void update();
    int compareMotionProfiles();

private:
    MotorEncoder encFL, encFR, encBL, encBR;
    static float position[2];
    static float angle;
    static int currentState[4];
    static int motionProfiles[17][4];
};

// Initialize static members
float Position::position[2] = {0.0, 0.0};
float Position::angle = 0;
int Position::currentState[4] = {0, 0, 0, 0};
int Position::motionProfiles[17][4] = {
    {1,1,1,1},      // Forward
    {-1,-1,-1,-1},  // Reverse
    {1,-1,-1,1},    // Strafe Right
    {-1,1,1,-1},    // Strafe Left
    {1,0,0,1},      // Forward and Strafe Right
    {-1,0,0,-1},    // Backward and Strafe Left
    {0,1,1,0},      // Forward and Strafe Left
    {0,-1,-1,0},    // Backward and Strafe Right
    {1,0,1,0},      // Rotate on Right side CW
    {-1,0,-1,0},    // Rotate on Right side CCW
    {0,1,0,1},      // Rotate of Left side CCW
    {0,-1,0,-1},    // Rotate on Left side CW
    {1,-1,0,0},     // Rotate behind CW
    {-1,1,0,0},     // Rotate behind CCW
    {0,0,1,-1},     // Rotate ahead CW
    {0,0,-1,1},     // Rotate ahead CCW
    {0,0,0,0}       // No motion
}; 

Position::Position(int encoderPinFL, int encoderPinFR, int encoderPinBL, int encoderPinBR)
    : encFL(encoderPinFL), encFR(encoderPinFR), encBL(encoderPinBL), encBR(encoderPinBR){

};

void Position::update(){
    // Update tics for each wheel
    currentState[0] += encFL.getCount();
    currentState[1] += encFR.getCount();
    currentState[2] += encBL.getCount();
    currentState[3] += encBR.getCount();

    int motion = compareMotionProfiles();

    float distancePerTic = 0.009974548;
    float mult45Degree = cos(45.0);
    switch (motion)
    {
    case 0: // Forward
        position[0] += cos(angle)*distancePerTic;
        position[1] += sin(angle)*distancePerTic;
        break;
    case 1: // Reverse
        position[0] -= cos(angle)*distancePerTic;
        position[1] -= sin(angle)*distancePerTic;
        break;
    case 2: // Strafe Right
        position[0] += cos(angle+90)*distancePerTic*mult45Degree;
        position[1] += sin(angle+90)*distancePerTic*mult45Degree;
        break;
    case 3: // Strafe Left
        position[0] -= cos(angle+90)*distancePerTic*mult45Degree;
        position[1] -= sin(angle+90)*distancePerTic*mult45Degree;
        break;
    case 4: // Forward and Strafe Right
        break;
    case 5: // Backward and Strafe Left
        break;
    case 6: // Forward and Strafe Left
        break;
    case 7: // Backward and Strafe Right
        break;
    case 8: // Rotate on Right side CW
        break;
    case 9: // Rotate on Right side CCW
        break;
    case 10: // Rotate of Left side CCW
        break;
    case 11: // Rotate on Left side CW
        break;
    case 12: // Rotate behind CW
        break;
    case 13: // Rotate behind CCW
        break;
    case 14: // Rotate ahead CW
        break;
    case 15: // Rotate ahead CCW
        break;
    case 16: // No motion
        break;
    default:    
        break;
    }

}

int Position::compareMotionProfiles(){
    // Get the total number of motion profiles defined.
    int numProfiles = sizeof(motionProfiles) / sizeof(motionProfiles[0]);
    
    // Compare motion profiles to the current state of the robot.
    int sum[numProfiles];
    for ( int i = 0; i < numProfiles; i++ ){
        sum[i] = 0;
        for ( int j = 0; j < 4; j++ )
        sum[i] += abs( currentState[j] - motionProfiles[i][j] );
    }

    // Find the index of the minimum value in the 'sum' array
    int minIndex = 0;
    int minValue = sum[0];
    for (int i = 1; i < numProfiles; i++) {
        if (sum[i] < minValue) {
            minValue = sum[i];
            minIndex = i;
        }
    }

    // Return the corresponding motion profile.
    return minIndex;
}

#endif