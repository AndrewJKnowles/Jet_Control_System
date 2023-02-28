/*  Parallax_360
*   ============
*   Driver file for the parallax #900-00360 high speed countinous servo
*
*   Author:         Andrew Knowles 17/02/2023
*   Version:        1.0
*   Last updated:   17/02/2023
*   MBED OS:        6.16.0
*   MBED Studio:    v1.4.4
*/

#include "mbed.h"
#include "Parallax_360.h"

Parallax_360::Parallax_360(PinName const PWM, PinName const pos) 
                    :
                    _pwm(new PwmOut(PWM)),
                    _pos(new DigitalIn(pos))
                    {};

Parallax_360::~Parallax_360(){
    delete _pwm;
    delete _pos;
}

void Parallax_360::init(){
    _pwm->period_ms(20.0f); //set frequency to 50Hz
    stop();                 //ensure servo is off as default
}

void Parallax_360::test(){
    //test clockwise functionality
    clockwise(SLOW);
    wait_us(500000);
    stop();
    wait_us(500000);
    clockwise(MEDIUM);
    wait_us(500000);
    stop();
    wait_us(500000);
    clockwise(FAST);
    wait_us(500000);
    stop();
    wait_us(500000);

    //test anti clockwise functionality
    anticlockwise(SLOW);
    wait_us(500000);
    stop();
    wait_us(500000);
    anticlockwise(MEDIUM);
    wait_us(500000);
    stop();
    wait_us(500000);
    anticlockwise(FAST);
    wait_us(500000);
    stop();
    wait_us(500000);
}

void Parallax_360::clockwise(Speed const speed){
    if(speed == SLOW){
        _pwm->write(0.072f);   //duty cycle set to 7.2% 
    }else if(speed == MEDIUM){
        _pwm->write(0.069f);    //set duty cycle to 6.9% for 70rpm approx.
    }else if(speed == FAST){
        _pwm->write(0.064f);    //set duty cycle to 6.4% for 140rpm approx.
    }
}

void Parallax_360::anticlockwise(Speed const speed){
    if(speed == SLOW){
        _pwm->write(0.078f);   //duty cycle set to 7.8% 
    }else if(speed == MEDIUM){
        _pwm->write(0.081f);    //set duty cycle to 8.1% for -70rpm approx.
    }else if(speed == FAST){
        _pwm->write(0.086f);    //set duty cycle to 8.6% for 140rpm approx.
    }
}

void Parallax_360::stop(){
    _pwm->write(0.075f);    //set to 7.5% duty cycle, stop servo
}