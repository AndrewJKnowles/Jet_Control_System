#ifndef SERVO_H
#define SERVO_H

#include "mbed"

class Parallax{
public:
    //create object
    Parallax(PinName const PWM, PinName const pos);

    //delete object
    ~Parallax();

    void init();

private:
    PwmOut *_pwm;
    DigitalIn *_pos;

};

#endif