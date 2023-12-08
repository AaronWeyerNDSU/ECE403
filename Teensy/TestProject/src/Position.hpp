#ifndef POSITION_HPP
#define POSITION_HPP

#include <Arduino.h>
#include <vector>
#include "Encoder.hpp"

class Position {
public:
    Position(int encoderPinFL, int encoderPinFR, int encoderPinBL, int encoderPinBR);
    std::vector<float> getPosition();
    void update();

private:
    int digitalPin;
    Encoder encFL, encFR, encBL, encBR;
    std::vector<float> position;
};

Position::Position(int encoderPinFL, int encoderPinFR, int encoderPinBL, int encoderPinBR)
    : encFL(encoderPinFL), encFR(encoderPinFR), encBL(encoderPinBL), encBR(encoderPinBR){

    this->position = {0.0, 0.0};
};

void Position::update(){

}

#endif