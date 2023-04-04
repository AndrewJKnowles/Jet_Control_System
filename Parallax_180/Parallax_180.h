/*  Parallax_180
*   ============
*   Driver file for the Parallax 180 degree servo
*
*   Author:         Andrew Knowles 17/02/2023
*   Version:        1.0
*   Last updated:   04/04/2023
*   MBED OS:        6.16.0
*   MBED Studio:    v1.4.4
*/

#ifndef PARALLAX_180_H
#define PARALLAX_180_H

#include "mbed.h"

/*pwm duty cycle and position relations
* 5% = 0 degrees
* 6.25% = 45 degrees
* 7.5% = 90 degrees
* 8.75% = 135 degrees
* 10% = 180 degrees */

class Parallax_180{
public:
    //create object
    Parallax_180(PinName const PWM);

    //delete object
    ~Parallax_180();

    //initialise device
    void init();

    //test the function of the servo
    void test();

    //move to given position where position is the PWM duty cycle
    void move(float position);

private:
    PwmOut *_pwm;
    float _position;
};

#endif