#include "mbed.h"
#include "MD10C.h"
#include "Servo.H"

MD10C motor(PC_10, PA_15);

//          cont, feed
Parallax servo(PB_7, PC_11);


int main(){
    servo.init();
    ThisThread::sleep_for(500ms);

    while(1){
        servo.move();
    }
}