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

#ifndef MD10C_H
#define MD10C_H

#include "mbed.h"

//rotation of shaft determined when looking at the motor from the direction of the shaft
#define CLOCKWISE 1
#define ANTICLOCKWISE 0 

class MD10C{
public:

    //create object
    //take direction pin and read analog input to control motor speed
    MD10C(PinName const dir, PinName const PWM, PinName const AnIn);

    //delete object
    ~MD10C();

    //initalise device
    void init();

    //test operation of motor
    void test();

    //turn motor. Takes direction
    void motorOn(int direction);

    //stops motor operation
    void stop();

private:
    DigitalOut *_dir;
    PwmOut *_pwm;
    AnalogIn *_anIn;

    float an_read = 0.0;

};

#endif
