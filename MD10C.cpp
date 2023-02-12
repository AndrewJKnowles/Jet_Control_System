#include "mbed.h"
#include "MD10C.h"

MD10C::MD10C(PinName const dir, PinName const PWM, PinName const AnIn) 
            :
            _dir(new DigitalOut(dir)),
            _pwm(new PwmOut(PWM)), 
            _anIn(new AnalogIn(AnIn))
            {};

MD10C::~MD10C(){
    delete _dir;
    delete _pwm;
    delete _anIn;
}

void MD10C::init(){
    _pwm->period_ms(1.0f);
    stop(); //ensure the motor is off
    _dir->write(CLOCKWISE);
}

void MD10C::test(){
    for(int i = 0; i < 3; i++){
        motorOn(clockwise);
        ThisThread::sleep_for(1s);

        stop();
        ThisThread::sleep_for(1s);

        motorOn(anticlockwise);
        ThisThread::sleep_for(1s);

        stop();
        ThisThread::sleep_for(1s);
    }
}

void MD10C::motorOn(DIRECTION const Dir){
    if(Dir == clockwise){
        _dir->write(CLOCKWISE);
    }else{
        _dir->write(ANTICLOCKWISE);
    }

    an_read = _anIn->read();

    if(an_read <= 0.001){
        an_read = 0.0;
    }else if(an_read >= 0.999){
        an_read = 1.0;
    }

    _pwm->write(an_read); //default to 50%
}

void MD10C::stop(){
    _pwm->write(0); //set duty cycle to 0%
}