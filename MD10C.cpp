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
    _pwm->period_ms(1.0f);      //initialise pwm frequency to 1KHz
    stop();                     //ensure the motor is off
    _dir->write(CLOCKWISE);     //default direction
}

void MD10C::test(){
    for(int i = 0; i < 3; i++){
        motorOn(1);
        ThisThread::sleep_for(1s);

        stop();
        ThisThread::sleep_for(1s);

        motorOn(0);
        ThisThread::sleep_for(1s);

        stop();
        ThisThread::sleep_for(1s);
    }
}

void MD10C::motorOn(int direction){
    if(direction == 1){                //set direction of motor
        _dir->write(CLOCKWISE);
    }else if(direction == 0){
        _dir->write(ANTICLOCKWISE);
    }else{
        printf("ERROR >> Unspecified Motor Direction <<");
    }

    an_read = _anIn->read();

    if(an_read <= 0.001){           //check analogue reading is within range
        an_read = 0.0;
    }else if(an_read >= 0.999){
        an_read = 1.0;
    }

    _pwm->write(an_read);           //write analogue reading as pwm duty cycle
}

void MD10C::stop(){
    _pwm->write(0);                 //set duty cycle to 0%
}