#include "mbed.h"
#include "Servo.H"

Parallax::Parallax(PinName const PWM, PinName const pos) 
                    :
                    _pwm(new PwmOut(PWM)),
                    _pos(new DigitalIn(pos))
                    {};

Parallax::~Parallax(){
    delete _pwm;
    delete _pos;
}

void Parallax::init(){
    _pwm->period_ms(20.0f); //set frequency to 50Hz

}

void Parallax::move(){
    _pwm->write(0.05f);    //set to 6.4% duty cycle
}