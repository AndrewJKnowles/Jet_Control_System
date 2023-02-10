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

    void motorOn(DIRECTION const Dir);

    void stop();

private:
    DigitalOut *_dir;
    PwmOut *_pwm;

};

#endif
