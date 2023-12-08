#ifndef POSITION_HPP
#define POSITION_HPP

#include <Arduino.h>
#include <vector>
#include "MotorEncoder.hpp"

class Position {
public:
    Position(int encoderPinFL, int encoderPinFR, int encoderPinBL, int encoderPinBR);
    String getCurrentState();
    void update();
    int compareMotionProfiles();
    void setVelocity(int speed);
    static float position[2];
    static float angle;

private:
    MotorEncoder encFL, encFR, encBL, encBR;
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

String Position::getCurrentState(){
    String result = "";
    for (size_t i = 0; i < 4; ++i) {
        if (i > 0) {
            result += ",";
        }
        result += String(currentState[i]);
    }
    return result;
}
void Position::setVelocity(int speed){
    encFL.setMotorSpeed(speed);
    encFR.setMotorSpeed(speed);
    encBL.setMotorSpeed(speed);
    encBR.setMotorSpeed(speed);
    
}

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
        position[0] = 0;
        position[1] = 0;
        break;
    case 5: // Backward and Strafe Left
        position[0] = 1;
        position[1] = 1;
        break;
    case 6: // Forward and Strafe Left
        position[0] = 2;
        position[1] = 2;
        break;
    case 7: // Backward and Strafe Right
        position[0] = 3;
        position[1] = 3;
        break;
    case 8: // Rotate on Right side CW
        position[0] = 4;
        position[1] = 4;
        break;
    case 9: // Rotate on Right side CCW
        position[0] = 5;
        position[1] = 5;
        break;
    case 10: // Rotate of Left side CCW
        position[0] = 6;
        position[1] = 6;
        break;
    case 11: // Rotate on Left side CW
        position[0] = 7;
        position[1] = 7;
        break;
    case 12: // Rotate behind CW
        position[0] = 8;
        position[1] = 8;
        break;
    case 13: // Rotate behind CCW
        position[0] = 9;
        position[1] = 9;
        break;
    case 14: // Rotate ahead CW
        position[0] = 10;
        position[1] = 10;
        break;
    case 15: // Rotate ahead CCW
        position[0] = 11;
        position[1] = 11;
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
        for ( int j = 0; j < 4; j++ ){
            sum[i] += abs( currentState[j] - motionProfiles[i][j] );
        }
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

    // Update the current state of the recorded encoder values.
    for ( int i = 0; i < 4; i++ ){
            currentState[i] -= motionProfiles[minIndex][i];
    }

    // Return the corresponding motion profile.
    return minIndex;
}

#endif