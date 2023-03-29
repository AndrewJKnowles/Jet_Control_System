/*  Parallax_360
*   ============
*
*   Author:         Andrew Knowles 17/02/2023
*   Version:        1.0
*   Last updated:   17/02/2023
*   MBED OS:        6.16.0
*   MBED Studio:    v1.4.4
*/

#include "mbed.h"
#include "Parallax_180.h"

Parallax_180::Parallax_180(PinName const PWM) 
                    :
                    _pwm(new PwmOut(PWM))
                    {};

Parallax_180::~Parallax_180(){
    delete _pwm;
}

void Parallax_180::init(){
    _pwm->period_ms(20.0f); //set frequency to 50Hz
    move(0.03);     //move to 0 degrees
}

void Parallax_180::test(){
    
    for(int i = 0; i < 3; i++){
        move(0.053);    //move to 45 degrees
        //move(0.076);    //move to 90 degrees
        move(0.03);     //move to 0 degrees
        
        ThisThread::sleep_for(100ms);
    }
}

void Parallax_180::move(float position){
    _position = position;
    _pwm->write(_position);
    ThisThread::sleep_for(500ms); //ensure movement has completed
}