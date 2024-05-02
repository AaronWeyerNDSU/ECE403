#ifndef POSITION_HPP
#define POSITION_HPP

#include <Arduino.h>
#include <vector>
#include "MotorEncoder.hpp"

#define FL 0
#define FR 1
#define BL 2
#define BR 3
#define FORWARD HIGH
#define REVERSE LOW

class Position {
    public:
        Position(int encoderPinFL, int encoderPinFR, int encoderPinBL, int encoderPinBR, int speedPinFL, int speedPinFR, int speedPinBL, int speedPinBR, int directionPinFL, int directionPinFR, int directionPinBL, int directionPinBR);
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
        int velocity2Speed(int velocity);
        static int currentState[4];
        static int velocity[4];
        static bool direction[4];
        int velocityPin[4];
        int directionPin[4];
        static int motionProfiles[17][4];
};

// Initialize static members
float Position::X = 0.0;
float Position::Y = 0.0;
float Position::angle = 0;
int Position::currentState[4] = {0, 0, 0, 0};
int Position::velocity[4] = {0, 0, 0, 0};
bool Position::direction[4] = {false, false, false, false};
int Position::motionProfiles[17][4] = {
    {0,0,0,0},       // No motion
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
    {0,0,-1,1}      // Rotate ahead CCW
}; 

/// @brief Constructor for position class. uses encoders to know the position of the drone.
/// @param encoderPinFL digital input pin that the front left encoder is plugged into.
/// @param encoderPinFR digital input pin that the front right encoder is plugged into.
/// @param encoderPinBL digital input pin that the back left encoder is plugged into.
/// @param encoderPinBR digital input pin that the back right encoder is plugged into.
Position::Position(int encoderPinFL, int encoderPinFR, int encoderPinBL, int encoderPinBR, int speedPinFL, int speedPinFR, int speedPinBL, int speedPinBR, int directionPinFL, int directionPinFR, int directionPinBL, int directionPinBR){
    // Initialize encoders
    pinMode(encoderPinFL, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(encoderPinFL), ticFL, CHANGE);
    pinMode(encoderPinFR, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(encoderPinFR), ticFR, CHANGE);
    pinMode(encoderPinBL, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(encoderPinBL), ticBL, CHANGE);
    pinMode(encoderPinBR, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(encoderPinBR), ticBR, CHANGE);

    // Set pinmodes
    pinMode(speedPinFL, OUTPUT);
    pinMode(speedPinFR, OUTPUT);
    pinMode(speedPinBL, OUTPUT);
    pinMode(speedPinBR, OUTPUT);
    pinMode(directionPinFL, OUTPUT);
    pinMode(directionPinFR, OUTPUT);
    pinMode(directionPinBL, OUTPUT);
    pinMode(directionPinBR, OUTPUT);
    
    // Store velocity control pins for each motor.
    velocityPin[FL] = speedPinFL;
    velocityPin[FR] = speedPinFR;
    velocityPin[BL] = speedPinBL;
    velocityPin[BR] = speedPinBR;
    
    // Store direction control pins for each motor.
    directionPin[FL] = directionPinFL;
    directionPin[FR] = directionPinFR;
    directionPin[BL] = directionPinBL;
    directionPin[BR] = directionPinBR;
};

/// @brief Interupt function for front left encoder. 
/// Increment or decrement the counted number of tic's depending on the direction of wheel spin.
void Position::ticFL() {
    if (velocity[FL] > 0) {
        currentState[FL]++;
    } else if (velocity[FL] < 0) {
        currentState[FL]--;
    } else {
        // do nothing because we don't know the direction of spin.
    }
}

/// @brief Interupt function for front right encoder. 
/// Increment or decrement the counted number of tic's depending on the direction of wheel spin.
void Position::ticFR() {
    if (velocity[FR] > 0) {
        currentState[FR]++;
    } else if (velocity[FR] < 0) {
        currentState[FR]--;
    } else {
        // do nothing because we don't know the direction of spin.
    }
}

/// @brief Interupt function for back left encoder. 
/// Increment or decrement the counted number of tic's depending on the direction of wheel spin.
void Position::ticBL() {
    if (velocity[BL] > 0) {
        currentState[BL]++;
    } else if (velocity[BL] < 0) {
        currentState[BL]--;
    } else {
        // do nothing because we don't know the direction of spin.
    }
}

/// @brief Interupt function for back right encoder. 
/// Increment or decrement the counted number of tic's depending on the direction of wheel spin.
void Position::ticBR() {
    if (velocity[BR] > 0) {
        currentState[BR]++;
    } else if (velocity[BR] < 0) {
        currentState[BR]--;
    } else {
        // do nothing because we don't know the direction of spin.
    }
}

/// @brief get the current number of tics counted for each wheel.
/// @return string with the quantity of tics counted for each wheel in the format of "FL,FR,BL,BR".
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

int Position::velocity2Speed(int velocity){
    // Make velocity positive.
    if(velocity < 0) velocity = -velocity;

    // Invert magnitude of velocity.
    velocity = 256 - velocity;

    // return bounded velocity.
    return max(min(velocity, 256), 0);
}

/// @brief set the speed of any motor connected.
/// @param velocityFL new speed of front left motor. default is it retains its speed.
/// @param velocityFR new speed of front right motor. default is it retains its speed.
/// @param velocityBL new speed of back left motor. default is it retains its speed.
/// @param velocityBR new speed of back right motor. default is it retains its speed.
void Position::setMotorSpeed(int velocityFL = velocity[FL], int velocityFR = velocity[FR], int velocityBL = velocity[BL], int velocityBR = velocity[BR]){
    // Check if direction changes.
    bool dirChange = ((velocity[FL] ^ velocityFL) >> 31 || (velocity[FR] ^ velocityFR) >> 31 || (velocity[BL] ^ velocityBL) >> 31 || (velocity[BR] ^ velocityBR) >> 31);
    
    // Set velocity variables.
    velocity[FL] = velocityFL;
    velocity[FR] = velocityFR;
    velocity[BL] = velocityBL;
    velocity[BR] = velocityBR;
    if(dirChange){

    }
    // Set direction of motors.
    for (int motor : {FL, FR, BL, BR}) {
        // 
        if(velocity[motor] > 0){
            digitalWrite(directionPin[motor], FORWARD);
        } else if ( velocity[motor] < 0){
            digitalWrite(directionPin[motor], REVERSE);
        } else {
            // do nothing.
        }
        analogWrite(velocityPin[motor], velocity2Speed(velocity[motor]));
    }

    
}

/// @brief Get the velocity value each motor is currently set to.
/// @return string of the set velocities of each motor.
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

/// @brief Get the motion of the robot and update its coordinates based on
/// the motion detected.
void Position::update(){
    int motion = compareMotionProfiles();

    float distancePerTic = 0.009974548;
    float mult45Degree = cos(45.0);
    switch (motion) {
        case 1: // Forward
            X += cos(angle)*distancePerTic;
            Y += sin(angle)*distancePerTic;
            break;
        case 2: // Reverse
            X -= cos(angle)*distancePerTic;
            Y -= sin(angle)*distancePerTic;
            break;
        case 3: // Strafe Right
            X += cos(angle+90)*distancePerTic*mult45Degree;
            Y += sin(angle+90)*distancePerTic*mult45Degree;
            break;
        case 4: // Strafe Left
            X -= cos(angle+90)*distancePerTic*mult45Degree;
            Y -= sin(angle+90)*distancePerTic*mult45Degree;
            break;
        case 5: // Forward and Strafe Right
            X = 0;
            Y = 0;
            break;
        case 6: // Backward and Strafe Left
            X = 1;
            Y = 1;
            break;
        case 7: // Forward and Strafe Left
            X = 2;
            Y = 2;
            break;
        case 8: // Backward and Strafe Right
            X = 3;
            Y = 3;
            break;
        case 9: // Rotate on Right side CW
            X = 4;
            Y = 4;
            break;
        case 10: // Rotate on Right side CCW
            X = 5;
            Y = 5;
            break;
        case 11: // Rotate of Left side CCW
            X = 6;
            Y = 6;
            break;
        case 12: // Rotate on Left side CW
            X = 7;
            Y = 7;
            break;
        case 13: // Rotate behind CW
            X = 8;
            Y = 8;
            break;
        case 14: // Rotate behind CCW
            X = 9;
            Y = 9;
            break;
        case 15: // Rotate ahead CW
            X = 10;
            Y = 10;
            break;
        case 16: // Rotate ahead CCW
            X = 11;
            Y = 11;
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