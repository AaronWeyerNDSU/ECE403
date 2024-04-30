#ifndef MOTORS_HPP
#define MOTORS_HPP

#include <Arduino.h>

class Motors {
    public:
        Motors();
        void FL(int speed);
        void FR(int speed);
        void BL(int speed);
        void BR(int speed);
        void setAll(int speed);
        void forward(int speed);
        void backward(int speed);
        void strafeLeft(int speed);
        void strafeRight(int speed);
        bool dir[4] = {false, false, false, false};
        
};

Motors::Motors(){
    // Set ouputs for motor controls
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);

    // BR motor
    digitalWrite(2, HIGH);
    digitalWrite(6, LOW);

    // BL motor
    digitalWrite(3, HIGH);
    digitalWrite(7, LOW);

    // FR motor
    digitalWrite(4, HIGH);
    digitalWrite(8, LOW);

    // FL motor
    digitalWrite(5, HIGH);
    digitalWrite(9, LOW);
};

void Motors::FL(int speed){
    if(speed > 0){
        digitalWrite(9, LOW);
        analogWrite(5, min(speed, 256));
    } else if(speed < 0) {
        digitalWrite(9, HIGH);
        analogWrite(5, min(-speed, 256));
    } else {
        analogWrite(5, 0);
    }
};

void Motors::FR(int speed){
    if(speed > 0){
        digitalWrite(8, LOW);
        analogWrite(4, min(speed, 256));
    } else if(speed < 0) {
        digitalWrite(8, HIGH);
        analogWrite(4, min(-speed, 256));
    } else {
        analogWrite(4, 0);
    }
};

void Motors::BL(int speed){
    if(speed < 0){
        digitalWrite(7, LOW);
        analogWrite(3, min(-speed, 256));
    } else if(speed > 0) {
        digitalWrite(7, HIGH);
        analogWrite(3, min(speed, 256));
    } else {
        analogWrite(3, 0);
    }
};

void Motors::BR(int speed){
    if(speed < 0){
        digitalWrite(6, LOW);
        analogWrite(2, min(-speed, 256));
    } else if(speed > 0) {
        digitalWrite(6, HIGH);
        analogWrite(2, min(speed, 256));
    } else {
        analogWrite(2, 0);
    }
};

void setAll(int speed){
    // FL(speed);

}

#endif