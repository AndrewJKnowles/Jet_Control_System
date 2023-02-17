#include "mbed.h"
#include "MD10C.h"
#include "Parallax_360.h"

//          (dir, pwm, anlog In)
MD10C motor(p19, p21, p20);
Parallax_360 servo(p22, p23);
InterruptIn button(p18);    //artificial limit switch

void interrupt_isr();       //service routine

int direction = 1;          //initial motor direction (clockwise)
volatile int g_button_flag = 0; 

int main(){
    button.rise(&interrupt_isr);    //set isr condition
    button.mode(PullNone);          //set button mode
    motor.init();                   //initialise motor driver
    servo.init();


    /*while(1){
        if(g_button_flag == 1){     //if limit switch active
            g_button_flag = 0;      //clear flag
            direction = !direction; //change direction status
            motor.stop();           //stop motor
            wait_us(500000);        //wait for complete stop
        }

        motor.motorOn(direction);   //begin motor
    };*/

    servo.test();

    while(1){}
}

void interrupt_isr(){
    g_button_flag = 1;
}