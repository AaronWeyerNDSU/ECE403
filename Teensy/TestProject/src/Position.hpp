#ifndef POSITION_HPP
#define POSITION_HPP

#include <Arduino.h>
#include <vector>
#include "MotorEncoder.hpp"

#define FL 0
#define FR 1
#define BL 2
#define BR 3

class Position {
public:
    Position(int encoderPinFL, int encoderPinFR, int encoderPinBL, int encoderPinBR);
    static void ticFL();
    static void ticFR();
    static void ticBL();
    static void ticBR();
    
    void update();
    int compareMotionProfiles();
    void setMotorSpeed(int FLSpeed, int FRSpeed, int BLSpeed, int BRSpeed);
    String getMotorSpeed();
    String getCurrentState();
    static float X;
    static float Y;
    static float angle;

private:
    static int currentState[4];
    static int velocity[4];
    static int motionProfiles[17][4];
};

// Initialize static members
float Position::X = 0.0;
float Position::Y = 0.0;
float Position::angle = 0;
int Position::currentState[4] = {0, 0, 0, 0};
int Position::velocity[4] = {0, 0, 0, 0};
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

Position::Position(int encoderPinFL, int encoderPinFR, int encoderPinBL, int encoderPinBR){
    pinMode(encoderPinFL, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(encoderPinFL), ticFL, CHANGE);
    pinMode(encoderPinFR, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(encoderPinFR), ticFR, CHANGE);
    pinMode(encoderPinBL, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(encoderPinBL), ticBL, CHANGE);
    pinMode(encoderPinBR, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(encoderPinBR), ticBR, CHANGE);
};

void Position::ticFL() {
    if (velocity[FL] > 0) {
        currentState[FL]++;
    } else if (velocity[FL] < 0) {
        currentState[FL]--;
    } else {
        // do nothing because we don't know the direction of spin.
    }
}
void Position::ticFR() {
    if (velocity[FR] > 0) {
        currentState[FR]++;
    } else if (velocity[FR] < 0) {
        currentState[FR]--;
    } else {
        // do nothing because we don't know the direction of spin.
    }
}
void Position::ticBL() {
    if (velocity[BL] > 0) {
        currentState[BL]++;
    } else if (velocity[BL] < 0) {
        currentState[BL]--;
    } else {
        // do nothing because we don't know the direction of spin.
    }
}
void Position::ticBR() {
    if (velocity[BR] > 0) {
        currentState[BR]++;
    } else if (velocity[BR] < 0) {
        currentState[BR]--;
    } else {
        // do nothing because we don't know the direction of spin.
    }
}

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
void Position::setMotorSpeed(int speedFL = velocity[FL], int speedFR = velocity[FR], int speedBL = velocity[BL], int speedBR = velocity[BR]){
    velocity[FL] = speedFL;
    velocity[FR] = speedFR;
    velocity[BL] = speedBL;
    velocity[BR] = speedBR;

    for (int motor : {FL, FR, BL, BR}) {
        // Set the speed of the motor.
        if (velocity[motor] > 0){
            // Do things that make motor spin forward.
            // DAC or poorDAC using digital outputs.
        } else if (velocity[motor] < 0){
            // Do things that make motor spin backward.
            // DAC or poorDAC using digital outputs.
        } else {
            // Do things that make motor stop spinning.
            // DAC or poorDAC using digital outputs.
        }
    }
}
String Position::getMotorSpeed(){
    String result = "";
    for (int motor : {FL, FR, BL, BR}) {
        if (motor > 0) {
            result += ",";
        }
        result += String(velocity[motor]);
    }
    return result;
}
void Position::update(){
    int motion = compareMotionProfiles();

    float distancePerTic = 0.009974548;
    float mult45Degree = cos(45.0);
    switch (motion) {
        case 0: // Forward
            X += cos(angle)*distancePerTic;
            Y += sin(angle)*distancePerTic;
            break;
        case 1: // Reverse
            X -= cos(angle)*distancePerTic;
            Y -= sin(angle)*distancePerTic;
            break;
        case 2: // Strafe Right
            X += cos(angle+90)*distancePerTic*mult45Degree;
            Y += sin(angle+90)*distancePerTic*mult45Degree;
            break;
        case 3: // Strafe Left
            X -= cos(angle+90)*distancePerTic*mult45Degree;
            Y -= sin(angle+90)*distancePerTic*mult45Degree;
            break;
        case 4: // Forward and Strafe Right
            X = 0;
            Y = 0;
            break;
        case 5: // Backward and Strafe Left
            X = 1;
            Y = 1;
            break;
        case 6: // Forward and Strafe Left
            X = 2;
            Y = 2;
            break;
        case 7: // Backward and Strafe Right
            X = 3;
            Y = 3;
            break;
        case 8: // Rotate on Right side CW
            X = 4;
            Y = 4;
            break;
        case 9: // Rotate on Right side CCW
            X = 5;
            Y = 5;
            break;
        case 10: // Rotate of Left side CCW
            X = 6;
            Y = 6;
            break;
        case 11: // Rotate on Left side CW
            X = 7;
            Y = 7;
            break;
        case 12: // Rotate behind CW
            X = 8;
            Y = 8;
            break;
        case 13: // Rotate behind CCW
            X = 9;
            Y = 9;
            break;
        case 14: // Rotate ahead CW
            X = 10;
            Y = 10;
            break;
        case 15: // Rotate ahead CCW
            X = 11;
            Y = 11;
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
        for ( int motor : {FL, FR, BL, BR}){
            sum[i] += abs( currentState[motor] - motionProfiles[i][motor] );
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