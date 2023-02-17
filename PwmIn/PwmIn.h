/*  PwmIn
*   =====
*   Library to calculate the period and duty cycle of an incoming PWM signal
*
*   Author:         Andrew Knowles 17/02/2023
*   Version:        1.0
*   Last updated:   17/02/2023
*   MBED OS:        6.16.0
*   MBED Studio:    v1.4.4
*/

#ifndef PWMIN_H
#define PWMIN_H

#include "mbed.h"

class PwmIn{
    public:

    //create PWM input object
    PwmIn(PinName pwm);

    //returns the period of the signal in seconds
    float readPeriod();

    //returns the period of the signal in ms
    float readPeriod_ms();

    //returns the period of the signal in us
    float readPeriod_us();

    //returns the pulse width of the signal in seconds
    float readPulseWidth();

    //returns the pulse width of the signal in ms
    float readPulseWidth_ms();

    //returns the pulse width of the signal in us
    float readPulseWidth_us();

    //returns the ducty cycle of the signal as a decimal percentage
    float readDutyCycle();

    private:
    InterruptIn _pwm;
    Timer _timer;

    void rise();
    void fall();

    float _period;
    float _periodms;
    float _periodus;

    float _pulseWidth;
    float _pulseWidthms;
    float _pulseWidthus;
};

#endif