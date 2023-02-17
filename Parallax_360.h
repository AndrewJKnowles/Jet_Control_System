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

#ifndef PARALLAX_360_H
#define PARALLAX_360_H

#include "mbed.h"

enum Speed {
    SLOW,
    MEDIUM,
    FAST,
};

class Parallax_360{
public:
    //create object
    Parallax_360(PinName const PWM, PinName const pos);

    //delete object
    ~Parallax_360();

    //initialise device
    void init();

    //test the function of the servo
    void test();

    //rotate servo clockwise for given speed
    void clockwise(Speed const speed);

    //rotate servo anticlockwise for a given speed
    void anticlockwise(Speed const speed);

    //write stop condition to servo
    void stop();

private:
    PwmOut *_pwm;
    DigitalIn *_pos;

};

#endif