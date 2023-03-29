/* max_limit_isr is being triggered as confirmed by the activation of the on board LED, however the while loop within
* extrude() is not being exited by the flag being set. the variable g_max_limit is being set as confirmed by the resetting of the LED upon 
* exiting the whilke loop using key input 'x'. */


#include "mbed.h"
#include "MD10C.h"
//#include "Parallax_360.h"
#include "Parallax_180.h"

//for lpc use
          //dir, PWM, AnIn
MD10C motor(p24, p23, p20);
                  //PWM, pos
//Parallax_360 servo(p25, p26); 
Parallax_180 servo(p25);
BufferedSerial pc(USBTX, USBRX);        //establish serial communications between PC and NUCLEO
InterruptIn max_limit_switch(p11);
InterruptIn min_limit_switch(p22);
DigitalOut LED_max(LED1);
DigitalOut LED_min(LED2);

char mainMenu[] = "Main Menu\n a. Extrude Material\n b. Retract Actuator\n c. Retract Servo\n d. Manual Operation\n e. Set Duty Cyle\n\n";

//option A messages
char ext1[] = "Extrude material\nPress 'X' to exit\n\n";
char ext2[] = "Extruding...\n\n";
char ext3[] = "Extrusion stopped\n\n";

//option C messages
char retractingActuator[] = "Actuator Retraction\nPress 'X' to exit\n\n";
char retracting[] = "Retracting...\n\n";
char retractionStopped[] =  "Retraction Stopped\n\n";

//option E messages
char manual[] = "Manual mode: Ensure Pot is initially set to 0\n Press 'Y' to confirm\n\n";
char manual2[] = "Entering manual mode:\n Use 'E' to extend the actuator\n Use 'R' to retract the actuator\n Use 'P' to pause actuator motion\n Press 'X' to exit\n\n";
char manual3[] = "Exiting manual mode...\n\n";
char manual4[] = "Extending Actuator...\n\n";
char manual5[] = "Retracting actuator...\n\n";
char manual6[] = "Actuation Paused\n\n";

//option F messages
char set1[] = "Set the Duty Cycle of the actuator\nThe current Duty Cycle value is: ";
char set3[] = "\n\nSet new duty cycle: ";
char set4[] = "\n\nNew duty cycle value is: ";

char error1[] = "Error: Invalid selection\n\n";
char error2[] = "Error: Duty Cycle set to minimum\n\n";
char error3[] = "Error: Duty Cycle set to maximum\n\n";

char *input = new char[1];

volatile int g_max_limit = 0;
volatile int g_min_limit = 0;


bool inputConfirmation = false;

void max_limit_isr();
void min_limit_isr();
void initComms();
void extrude();
void retractActuator();
void manualOperation();
void setDuty();

int main(){
    initComms();
    motor.init();
    servo.init();
    max_limit_switch.mode(PullUp);
    min_limit_switch.mode(PullUp);

    LED_max = 0;
    LED_min = 0;

    min_limit_switch.fall(&min_limit_isr);
    max_limit_switch.fall(&max_limit_isr);

    while(1){
        pc.write(mainMenu, sizeof(mainMenu));
        pc.read(input, sizeof(input));

        switch(*input){
            case 'a':
                //extrude material
                extrude();
                break;

            case 'b':
                //Retract Actuator
                retractActuator();
                break;
            
            case 'c':
                //Retract Servo
                servo.test();
                break;

            case 'd':
                //Manual Operation
                manualOperation();
                break;

            case 'e':
                //set duty cycle
                setDuty();
                break;

            default:
                pc.write(error1, sizeof(error1));
                break;
        }
    }
}

//init comms protocols
void initComms(){
    pc.set_baud(9600);
    pc.set_format(8, BufferedSerial::None, 1); //8-N-1
}

void max_limit_isr(){
    g_max_limit = 1;
    LED_max = 1;
}

void min_limit_isr(){
    g_min_limit = 1;
    //LED_min = 1;
}
//option A
void extrude(){
    pc.write(ext1, sizeof(ext1));
    ThisThread::sleep_for(500ms);
    pc.write(ext2, sizeof(ext2));

    motor.motorOn(EXTENSION);                                       //extend actuator

    while(inputConfirmation == false){                              //while input != x or X || g_max_limit has been reached
        pc.read(input, sizeof(input));                              //read input

        if(*input == 'x' || *input == 'X'){     //check for valid exit character *input == 'x' || *input == 'X' ||
            inputConfirmation = true;                               //set valid input to true

        }else if(g_max_limit == 1){
            inputConfirmation = true;
        }
    }

    /* implement function to retract actuator slightly to reset switch */

    if(g_max_limit == 1){
        g_max_limit = 0; 
        LED_max = 0;                                           //reset g_max_limit flag
    }

    motor.stop();                                                   //stop actuation
    inputConfirmation = false;                                      //reset inputConfirmation flag
    pc.write(ext3, sizeof(ext3));
    ThisThread::sleep_for(500ms);
}

//option B
void retractActuator(){
    pc.write(retractingActuator, sizeof(retractingActuator));       //general message output
    ThisThread::sleep_for(500ms);
    pc.write(retracting, sizeof(retracting));

    while(inputConfirmation == false){          //while input != x or X || g_min_limit has been reached
        motor.motorOn(RETRACTION);                                  //turn actuator on with 10% duty cycle
        pc.read(input, sizeof(input));                              //read input

        if(*input == 'x' || *input == 'X'){                         //check for valid exit character
            inputConfirmation = true;                               //set valid input to true
        }
    }

    inputConfirmation = false;                                      //reset input flag
    pc.write(retractionStopped, sizeof(retractionStopped));         //print message
    motor.stop();                                                   //stop actuation
    ThisThread::sleep_for(500ms);               
}

//option C

//option D
void manualOperation(){
    char dutyValue[1];
    char dutymess[] = "The current duty cyle is: ";
    char doubleReturn[] = "\n\n";

    pc.write(manual, sizeof(manual));               //print general message

    //wait for confirmation that pot is set to 0
    while(inputConfirmation == false){              //while input != x or X
        pc.read(input, sizeof(input));              //read input

        if(*input != 'y' || *input != 'Y'){         //check for valid exit character
            inputConfirmation = true;               //set valid input to true
        }
    }

    inputConfirmation = false;                      //reset input flag
    pc.write(manual2, sizeof(manual2));             //print general message

    while(inputConfirmation == false){              //while input != x or X
        pc.read(input, sizeof(input));              //read input
        dutyValue[0] = motor.getDutyCycle();

        if((*input == 'r' || *input == 'R') && g_min_limit == 0){         //if input == r and g_min_limit hasnt been reached, retract actuator
            pc.write(manual5, sizeof(manual5));     //print general message
            motor.manualMode(RETRACTION);

        }else if((*input == 'e' || *input == 'E') && g_max_limit == 0){   //if input == e and g_max_limit hasnt been reached, extend actuator
            pc.write(manual4, sizeof(manual4));     //print general message
            motor.manualMode(EXTENSION);

        }else if(*input == 't' || *input == 'T'){   //if input == p, pause actuator motion
            pc.write(manual6, sizeof(manual6));     //print general message
            pc.write(dutymess, sizeof(dutymess));   //Display current duty cycle
            pc.write(doubleReturn, sizeof(doubleReturn));   
            motor.stop();

        }else if(*input == 'x' || *input == 'X'){   //check for valid exit character
            inputConfirmation = true;               //set valid input to true
        }
    }

    inputConfirmation = false;                      //reset input flag
    pc.write(manual3, sizeof(manual3));             //print general message
    motor.stop();                                   //stop motor

    ThisThread::sleep_for(500ms);
}

//option E
void setDuty(){
    /*char buffer[32];
    float Newdutycycle;
    float set2;

    char* floatInput = buffer;

    set2 = motor.getDutyCycle();
    buffer[0] = set2;

    pc.write(set1, sizeof(set1));   //general message output
    pc.write(buffer, sizeof(buffer));   //print current duty cycle
    pc.write(set3, sizeof(set3));   //general message output

    Newdutycycle = pc.read(floatInput, sizeof(floatInput));

    pc.write(set4, sizeof(set4));
    pc.write(buffer, sizeof(buffer));

    while(inputConfirmation == false){          //while input != x or X
        pc.read(input, sizeof(input));

        if(*input == 'x' || *input == 'X'){     //check for valid exit character
            inputConfirmation = true;           //set valid input to true
        }
    }*/
}

/*//option F messages
char set1[] = "Set the Duty Cycle of the actuator\nThe current Duty Cycle value is: ";
char set2[1];    //use to read the current value of the duty cycle value
char set3[] = "\n\nSet new duty cycle: ";
char set4[] = "\n\nNew duty cycle value is: ";*/