#ifndef SERVO_H
#define SERVO_H

#include "mbed.h"

class Parallax{
public:
    //create object
    Parallax(PinName const PWM, PinName const pos);

    //delete object
    ~Parallax();

    //initialise device
    void init();

    void move();

private:
    PwmOut *_pwm;
    DigitalIn *_pos;

};

#endif