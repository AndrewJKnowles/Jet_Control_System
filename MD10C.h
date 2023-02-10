#ifndef MD10C_H
#define MD10C_H

#include "mbed.h"

//rotation of shaft determined when looking at the motor from the direction of the shaft
#define CLOCKWISE 1
#define ANTICLOCKWISE 0 

enum DIRECTION{
    clockwise,
    anticlockwise,
};

class MD10C{
public:

    //create object
    MD10C(PinName const dir, PinName const PWM);

    //delete object
    ~MD10C();

    //initalise device
    void init();

    //test operation of motor
    void test();

    //turn motor. Takes a float 0.0 -> 1.0 acting as duty cycle and direction
    void motorOn(float duty, DIRECTION const Dir);

    //stops motor operation
    void stop();

private:
    DigitalOut *_dir;
    PwmOut *_pwm;

};

#endif
