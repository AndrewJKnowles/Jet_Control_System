#include "mbed.h"
#include "MD10C.h"

MD10C::MD10C(PinName const dir, PinName const PWM) 
            :
            _dir(new DigitalOut(dir)), 
            _pwm(new PwmOut(PWM))
            {};

MD10C::~MD10C(){
    delete _dir;
    delete _pwm;
}

void MD10C::init(){
    _pwm->period_us(67.0f); //set pwm frequency to 15KHz
    stop(); //ensure the motor is off
    _dir->write(CLOCKWISE);
}

void MD10C::test(){
    for(int i = 0; i < 3; i++){
        motorOn(0.2, clockwise);
        ThisThread::sleep_for(1s);

        stop();
        ThisThread::sleep_for(1s);

        motorOn(0.2, anticlockwise);
        ThisThread::sleep_for(1s);

        stop();
        ThisThread::sleep_for(1s);
    }
}

void MD10C::motorOn(float duty, DIRECTION const Dir){
    if(Dir == clockwise){
        _dir->write(CLOCKWISE);

    }else{
        _dir->write(ANTICLOCKWISE);
    }

    _pwm->write(duty); //default to 50%
}

void MD10C::stop(){
    _pwm->write(0); //set duty cycle to 0%
}