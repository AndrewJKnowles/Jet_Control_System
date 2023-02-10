#include "mbed.h"
#include "MD10C.h"

MD10C motor(PC_10, PA_15);

int main(){
    motor.init();

    ThisThread::sleep_for(1s);

    motor.test();
}