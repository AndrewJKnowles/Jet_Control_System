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

#include "PwmIn.h"
#include <chrono>

PwmIn::PwmIn(PinName pwm)
                : _pwm(pwm){

                _pwm.rise(callback(this, &PwmIn::rise));
                _pwm.fall(callback(this, &PwmIn::fall));
                _period = 0.0;
                _pulseWidth = 0.0;
                _timer.start();
                }

float PwmIn::readPeriod() {
    return _period;
}

float PwmIn::readPeriod_ms() {
    return _periodms;
}

float PwmIn::readPeriod_us() {
    return _periodus;
}

float PwmIn::readPulseWidth() {
    return _pulseWidth;
}

float PwmIn::readPulseWidth_ms() {
    return _pulseWidthms;
}

float PwmIn::readPulseWidth_us() {
    return _pulseWidthus;
}

float PwmIn::readDutyCycle() {
    return _pulseWidth / _period;
}

void PwmIn::rise() {
    _period = chrono::duration_cast<std::chrono::seconds>(_timer.elapsed_time()).count();
    _periodms = chrono::duration_cast<std::chrono::milliseconds>(_timer.elapsed_time()).count();
    _periodus = chrono::duration_cast<std::chrono::microseconds>(_timer.elapsed_time()).count();
    _timer.reset();
}

void PwmIn::fall() {
    _pulseWidth = chrono::duration_cast<std::chrono::seconds>(_timer.elapsed_time()).count();
    _pulseWidthms = chrono::duration_cast<std::chrono::milliseconds>(_timer.elapsed_time()).count();
    _pulseWidthus = chrono::duration_cast<std::chrono::microseconds>(_timer.elapsed_time()).count();
}