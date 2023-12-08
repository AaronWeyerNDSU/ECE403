#ifndef MOTORENCODER_HPP
#define MOTORENCODER_HPP

#include <Arduino.h>

class MotorEncoder {
public:
    MotorEncoder(int digitalPin);
    void tic();
    void setCount(int newCount);
    void setMotorSpeed(int8_t velocity);
    int getCount();
    int8_t getMotorSpeed();
    float ticToMeter(int tics);

private:
    int digitalPin;
    int count;
    int8_t velocity;
    static void staticTicWrapper();
    static MotorEncoder* instance;  // Static pointer to the instance
};

MotorEncoder* MotorEncoder::instance = nullptr;  // Initialize the static pointer

MotorEncoder::MotorEncoder(int digitalPin) {
    this->digitalPin = digitalPin;
    this->count = 0;
    this->velocity = 0;
    instance = this;  // Set the static pointer to this instance

    pinMode(digitalPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(digitalPin), staticTicWrapper, CHANGE);
}

void MotorEncoder::staticTicWrapper() {
    MotorEncoder* instance = reinterpret_cast<MotorEncoder*>(interruptArgs);  // Retrieve the instance from the interrupt context
    if (instance != nullptr) {
        instance->tic();
    }
}

void MotorEncoder::tic() {
    if (velocity > 0) {
        count++;
    } else if (velocity < 0) {
        count--;
    } else {
        // do nothing because we don't know the direction of spin.
    }
}

void MotorEncoder::setCount(int newCount){
    if ( velocity > 0 ){

    }
    this->count = newCount;
}

/// @brief Convenient interface to set the speed of an individual motor.
/// @param velocity Integer speed representing how fast motor should spin. Positive value is forward negative value is backward. -128 to 127 range
void MotorEncoder::setMotorSpeed(int8_t velocity){
    // Potentially split velocity into two separate variables, velocity and direction. 
    // Direction is not channged when velocity is set to zero, this would help if spindown time is a big factor.

    // Set the speed of the motor.
    if (velocity > 0){
        // Do things that make motor spin forward.
        // DAC or poorDAC using digital outputs.
    } else if (velocity < 0){
        // Do things that make motor spin backward.
        // DAC or poorDAC using digital outputs.
    } else {
        // Do things that make motor stop spinning.
        // DAC or poorDAC using digital outputs.
    }

    // Update the velocity variable in the class.
    this->velocity = velocity;
}

int MotorEncoder::getCount(){
    int ret = this->count;
    this->count = 0;
    return ret;
}

float MotorEncoder::ticToMeter(int tics){
    return tics*0.009974548; // assuming 20 tics per rotation and wheel diamer of 2.5in
}

#endif