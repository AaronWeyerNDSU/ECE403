#ifndef MOTORENCODER_HPP
#define MOTORENCODER_HPP

#include <Arduino.h>

class MotorEncoder {
public:
    MotorEncoder(int digitalPin);
    static void tic();
    void setCount(int newCount);
    int getCount();
    float ticToMeter(int tics);
    static int count;

private:
    int digitalPin;
};

MotorEncoder::MotorEncoder(int digitalPin) {
    this->digitalPin = digitalPin;
    pinMode(digitalPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(digitalPin), tic, CHANGE);
}

void MotorEncoder::tic() {
    count++;
}

void MotorEncoder::setCount(int newCount){
    this->count = newCount;
};

int MotorEncoder::getCount(){
    int ret = this->count;
    this->count = 0;
    return ret;
}

float MotorEncoder::ticToMeter(int tics){
    return tics*0.009974548; // assuming 20 tics per rotation and wheel diamer of 2.5in
};

#endif