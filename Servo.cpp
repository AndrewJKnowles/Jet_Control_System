#include "mbed.h"
#include "Servo.h"

Parallax::Parallax(PinName const PWM, PinName const pos) 
                    :
                    _pwm(new PwmOut(PWM)),
                    _pos(new DigitalIn(pos))
                    {};

Parallax::~Parallax(){
    delete _pwm;
    delete _pos;
}