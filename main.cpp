#include "mbed.h"
#include "MD10C.h"

MD10C motor(p19, p21, p20);

float val;

int main(){
    motor.init();

    while(1){
        motor.motorOn(clockwise);

    };
}