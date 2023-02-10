#include "mbed.h"

PwmOut PWM(PA_15);
DigitalOut dir(PC_10);

int main(){
    //dir.write(1);
    
    while(1){
        for(float i = 0.1f; i < 0.9f; i = i + 0.1){
            PWM.period_us(67.0f); //provides a pwm frequency of 15KHz
            PWM.write(i);
            ThisThread::sleep_for(1s);
        }
    }
}