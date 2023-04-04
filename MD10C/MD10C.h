/*  MD10C
*   =====
*   Driver file for the Crytron MD10C Rev 2.0 motor driver
*
*   Author:         Andrew Knowles 17/02/2023
*   Version:        1.0
*   Last updated:   04/04/2023
*   MBED OS:        6.16.0
*   MBED Studio:    v1.4.4
*/

#ifndef MD10C_H
#define MD10C_H

#include "mbed.h"

//rotation of shaft determined when looking at the motor from the direction of the shaft
#define RETRACT 1
#define EXTEND 0 
#define MAXCYCLE 99
#define MINCYCLE 50

enum DIRECTION {
    EXTENSION,
    RETRACTION
};

class MD10C{
public:

    /*create object
    *Take direction pin and read analog input to control motor speed
    *NOTE: the analogue pin is controlled by an external pot and is not a funtionality 
    *of the motor driver itself */
    MD10C(PinName const dir, PinName const PWM, PinName const AnIn);

    //delete object
    ~MD10C();

    //initalise device
    void init();

    //test operation of motor
    void test();

    //turn motor. Takes direction
    void motorOn(DIRECTION const direction);

    //operate actuator in manual mode
    void manualMode(DIRECTION const direction);

    //stops motor operation
    void stop();

    //set the duty cycle
    //void setDutyCycle(unsigned short _duty);  TO BE IMPLEMENTED AT A LATER DATE

    //get the current duty cycle
    int getDutyCycle();

private:
    DigitalOut *_dir;
    PwmOut *_pwm;
    AnalogIn *_anIn;

    float an_read = 0.0;
    float duty_cycle;
};

#endif
