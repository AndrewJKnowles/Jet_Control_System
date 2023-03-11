/*  MD10C
*   =====
*   Driver file for the Crytron MD10C Rev 2.0 motor driver
*
*   Author:         Andrew Knowles 17/02/2023
*   Version:        1.0
*   Last updated:   17/02/2023
*   MBED OS:        6.16.0
*   MBED Studio:    v1.4.4
*/

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
    _pwm->period_ms(1.0f);      //initialise pwm frequency to 1KHz !!check frequency of actuator!!
    stop();                     //ensure the motor is off
    _dir->write(EXTEND);     //default direction
    duty_cycle = 0.1;
}

void MD10C::test(){
    for(int i = 0; i < 3; i++){
        //test clockwise functionality
        motorOn(EXTENSION);
        wait_us(3000000);
        stop();
        wait_us(3000000);

        //test anti clockwise functionality
        motorOn(RETRACTION);
        wait_us(3000000);
        stop();
        wait_us(3000000);
    }
}

void MD10C::motorOn(DIRECTION const direction){
    if(direction == RETRACTION){                //set direction of motor
        _dir->write(RETRACT);

    }else if(direction == EXTENSION){
        _dir->write(EXTEND);

    }else{
        printf("ERROR >> Unspecified Motor Direction <<");
    }

    _pwm->write(duty_cycle); //write duty cycle 

}

void MD10C::manualMode(DIRECTION const direction){
    if(direction == RETRACTION){                //set direction of motor
        _dir->write(RETRACT);

    }else if(direction == EXTENSION){
        _dir->write(EXTEND);

    }else{
        printf("ERROR >> Unspecified Motor Direction <<");
    }

    an_read = _anIn->read();    //take pot reading

    if(an_read <= 0.001){           //check analogue reading is within range
        an_read = 0.0;
    }else if(an_read >= 0.999){      //max actuator duty cycle 25%
        an_read = 0.999;
    }

    duty_cycle = an_read;
    
    _pwm->write(duty_cycle);           //write analogue reading as pwm duty cycle
}

void MD10C::stop(){
    _pwm->write(0);                 //set duty cycle to 0%
}

void MD10C::setDutyCycle(float _duty){
    //check speed is valid
    if(_duty <= 0.0){
        _duty = 0.0;
    }else if(_duty >= 1.0){
        _duty = 1.0;
    };

    duty_cycle = _duty;
}

float MD10C::getDutyCycle(){
    float currentDutyCycle = duty_cycle;
    return currentDutyCycle;
}