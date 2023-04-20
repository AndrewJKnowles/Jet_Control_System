/*  Jet Control System
    ==================

    This program forms the basic mean of controlling a jetting system intended for use with viscus materials.
    The physical components consist of a Crytron MD10C Rev 2.0 motor driver, a Parallax 180 degree motor and a linear actuator.
    This may be used as a refference but must not be used in any way, in part or in whole without prior premission from the author.
    
    Author:         Andrew Knowles
    Date:           04/04/2023
    Version:        1.0
    Platform:       Arm MBED LPC1768
    Last updated:   04/04/2023
    MBED OS:        6.17.0
    IDE:            MBED Studio v1.4.4  */

#include "mbed.h"
#include "MD10C.h"
#include "Parallax_180.h"

//for lpc use
          //dir, PWM, AnIn
MD10C actuator(p24, p23, p20);             //define actuator object 
Parallax_180 servo(p25);
BufferedSerial pc(USBTX, USBRX);        //establish serial communications between PC and NUCLEO
InterruptIn max_limit_switch(p21);      //generate event interrupt
InterruptIn min_limit_switch(p22);      //generate event interrupt
DigitalOut led(LED1);

//main menu 
char mainMenu[] = "Main Menu\n a. Extrude Material\n b. Retract Actuator\n c. Retract Servo\n d. Manual Operation\n e. Read Duty Cyle\n\n";

//option A messages
char ext1[] = "Extrude material\nPress spacebar to exit\n\n";
char ext2[] = "Extruding...\n\n";
char ext3[] = "Extrusion stopped\n\n";

//option C messages
char retractingActuator[] = "Actuator Retraction\nPress spacebar to exit\n\n";
char retracting[] = "Retracting...\n\n";
char retractionStopped[] =  "Retraction Stopped\n\n";

//option D messages
char manual[] = "Manual mode: Ensure Pot is initially set to 0\n Press 'Y' to confirm\n\n";
char manual2[] = "Entering manual mode:\n Use 'E' to extend the actuator\n Use 'R' to retract the actuator\n Use 'T' to pause actuator motion\n Press spacebar to exit\n\n";
char manual3[] = "Exiting manual mode...\n\n";
char manual4[] = "Extending Actuator...\n\n";
char manual5[] = "Retracting actuator...\n\n";
char manual6[] = "Actuation Paused\n\n";
char dutymess[] = "The current duty cyle is: ";
char doubleReturn[] = "\n\n";

//option E messages
char get1[] = "The current Duty Cycle value is: ";
char get2[] = "Press spacebar to exit\n\n ";
char get3[] = "Exiting ...\n\n";

//error messages
char error1[] = "Error: Invalid selection\n\n";
char error2[] = "Error: Duty Cycle set to minimum\n\n";
char error3[] = "Error: Duty Cycle set to maximum\n\n";

char *input = new char[1];          //key input buffer

volatile int g_max_limit = 0;       //actuator maximum travel flag
volatile int g_min_limit = 0;       //actuator minimum travel flag

bool inputConfirmation = false;     //valid input received flag

//declare functions
void max_limit_isr();
void min_limit_isr();
void initComms();
void systemInit();
void extrude();
void retractActuator();
void manualOperation();
void getDuty();
char convertToChar(int value);

int main(){
    systemInit();

    while(1){
        pc.write(mainMenu, sizeof(mainMenu));
        pc.read(input, sizeof(input));

        switch(*input){
            case 'a':
                //extrude material
                extrude();
                g_max_limit = 0;    //reset g_max_limit flag
                break;

            case 'b':
                //Retract Actuator
                retractActuator();
                g_min_limit = 0;    //reset g_min_limit flag
                break;
            
            case 'c':
                //Retract Servo
                servo.test();
                break;

            case 'd':
                //Manual Operation
                manualOperation();
                g_max_limit = 0;    //reset g_max_limit flag
                g_min_limit = 0;    //reset g_min_limit flag
                break;

            case 'e':
                //set duty cycle
                getDuty();
                break;

            default:
                pc.write(error1, sizeof(error1));
                break;
        }
    }
}

//max limit ISR
void max_limit_isr(){
    g_max_limit = 1;    //set g_max_limit
}

//min limit ISR
void min_limit_isr(){
    g_min_limit = 1;    //set g_min_limit
}

//init comms protocols
void initComms(){
    pc.set_baud(9600);                          //9600 baud rate
    pc.set_format(8, BufferedSerial::None, 1);  //8-N-1
}

//init system
void systemInit(){
    initComms();                                //init comms protocols
    actuator.init();                               //init actuator
    servo.init();                               //init servo
    max_limit_switch.mode(PullUp);              //set weak pull up
    min_limit_switch.mode(PullUp);              //set weak pull up
    max_limit_switch.fall(&max_limit_isr);      //generate callback upon voltage drop
    min_limit_switch.fall(&min_limit_isr);      //generate callback upon voltage drop
}

//option A
void extrude(){
    pc.set_blocking(false);                                         //disable serial blocking status
    pc.write(ext1, sizeof(ext1));                                   //display general message
    ThisThread::sleep_for(500ms);
    pc.write(ext2, sizeof(ext2));                                   //display general message
    actuator.motorOn(EXTENSION);                                    //extend actuator
    *input = 0;                                                     //clear buffer

    while(inputConfirmation == false){                              //while valid input is false
        pc.read(input, sizeof(input));                              //read key input

        if(*input == ' ' || g_max_limit == 1){   //check for valid exit character or limit has been reached
            inputConfirmation = true;                               //set valid input to true
        }
    }

    actuator.stop();                                                //stop actuation

    /*if(g_max_limit){                                                //if limit has been reached, mechanically reset switch
        actuator.motorOn(RETRACTION);
        ThisThread::sleep_for(250ms);
        actuator.stop();
    }*/

    inputConfirmation = false;                                      //reset inputConfirmation flag
    pc.set_blocking(true);                                          //re-enable serial blocking status
    pc.write(ext3, sizeof(ext3));                                   //display general message
    ThisThread::sleep_for(500ms);
}

//option B
void retractActuator(){
    pc.set_blocking(false);                                         //disable serial blocking status
    pc.write(retractingActuator, sizeof(retractingActuator));       //display general message
    ThisThread::sleep_for(500ms);
    pc.write(retracting, sizeof(retracting));                       //display general message
    actuator.motorOn(RETRACTION);                                   //turn actuator on with 10% duty cycle
    *input = 0;                                                     //clear buffer

    while(inputConfirmation == false){                              //while valid input is false
        pc.read(input, sizeof(input));                              //read key input

        if(*input == ' ' || g_min_limit == 1){     //check for valid exit character or limit has been reached
            inputConfirmation = true;                               //set valid input to true
        }
    }

    actuator.stop();                                                //stop actuation

    /*if(g_min_limit){                                                //if limit has been reached, mechanically reset switch
        actuator.motorOn(EXTENSION);
        ThisThread::sleep_for(250ms);
        actuator.stop();
    }*/

    inputConfirmation = false;                                      //reset inputConfirmation flag
    pc.set_blocking(true);                                          //re-enable serial blocking status
    pc.write(retractionStopped, sizeof(retractionStopped));         //display general message      
    ThisThread::sleep_for(500ms);               
}

//option D
void manualOperation(){
    char buffer[3] = {0};
    pc.write(manual, sizeof(manual));                                       //print general message
    *input = 0;                                                     //clear buffer

    //wait for confirmation that pot is set to 0
    while(inputConfirmation == false){                                      //while input != y or Y
        pc.read(input, sizeof(input));                                      //read input

        if(*input == 'y' || *input == 'Y'){                                 //check for valid exit character
            inputConfirmation = true;                                       //set valid input to true
        }
    }

    *input = 0;                                                     //clear buffer
    inputConfirmation = false;                                              //reset input flag
    pc.write(manual2, sizeof(manual2));                                     //print general message

    while(inputConfirmation == false){                                      //while input != x or X
        pc.read(input, sizeof(input));                                      //read input

        if(*input == 'r' || *input == 'R'){           //if input == r and g_min_limit hasnt been reached, retract actuator
            pc.write(manual5, sizeof(manual5));                             //print general message
            actuator.manualMode(RETRACTION);

        }else if(*input == 'e' || *input == 'E'){     //if input == e and g_max_limit hasnt been reached, extend actuator
            pc.write(manual4, sizeof(manual4));                             //print general message
            actuator.manualMode(EXTENSION);

        }else if(*input == 't' || *input == 'T'){                           //if input == t, pause actuator motion () ||
            actuator.stop();
            pc.write(manual6, sizeof(manual6));                             //print general message
            pc.write(dutymess, sizeof(dutymess));                           //Display current duty cycle message

            //calculate current duty cycle
            int cycle = actuator.getDutyCycle();                            //get current duty cycle
            int tens = cycle / 10;                                          //determine ten value of the duty cycle 93% duty cycle = 9 ten's
            int units = cycle % 10;                                         //determine unit value of the duty cycle 93% duty cycle = 3 units

            //store value in buffer
            buffer[0] = convertToChar(tens);                                
            buffer[1] = convertToChar(units);
            buffer[2] = '%';                                                //include a percentage sign for asthetics

            pc.write(buffer, sizeof(buffer));                               //Display current duty cycle as an int
            pc.write(doubleReturn, sizeof(doubleReturn));                   //print double return for asthetics
            

        }else if(*input == ' '){                           //check for valid exit character
            inputConfirmation = true;                                       //set valid input to true
        }

        /*if(g_max_limit){                                                //if limit has been reached, mechanrically reset switch
            actuator.motorOn(RETRACTION);
            ThisThread::sleep_for(250ms);
            actuator.stop();
            g_max_limit = 0;
        }

        if(g_min_limit){                                                //if limit has been reached, mechanically reset switch
            actuator.motorOn(EXTENSION);
            ThisThread::sleep_for(250ms);
            actuator.stop();
            g_min_limit = 0;
        }*/

    }

    actuator.stop();                                                        //stop motor
    inputConfirmation = false;                                              //reset input flag
    pc.write(manual3, sizeof(manual3));                                     //print general message
    pc.set_blocking(true);                                                  //reset serial blocking status

    ThisThread::sleep_for(500ms);
}

//option E
void getDuty(){
    char buffer[3] = {0};

    pc.write(get1, sizeof(get1));

    //calculate current duty cycle
    int cycle = actuator.getDutyCycle();                            //get current duty cycle
    int tens = cycle / 10;                                          //determine ten value of the duty cycle 93% duty cycle = 9 ten's
    int units = cycle % 10;                                         //determine unit value of the duty cycle 93% duty cycle = 3 units

    //store value in buffer
    buffer[0] = convertToChar(tens);                                
    buffer[1] = convertToChar(units);
    buffer[2] = '%';                                                //include a percentage sign for asthetics

    pc.write(buffer, sizeof(buffer));                               //Display current duty cycle as an int
    pc.write(doubleReturn, sizeof(doubleReturn));                   //print double return for asthetics
    pc.write(get2, sizeof(get2));

    while(inputConfirmation == false){                              //while input != y or Y
        pc.read(input, sizeof(input));                              //read input

        if(*input == ' '){                         //check for valid exit character
            inputConfirmation = true;                               //set valid input to true
        }
    }

    pc.write(get3, sizeof(get3));
    inputConfirmation = false;
    ThisThread::sleep_for(500ms);
}


//function to convert int values to char for printing
char convertToChar(int value){
    if(value < 0 || value > 9){
        return '?';

    }else{
        switch(value){
            case 1:
                return '1';
            break;

            case 2:
                return '2';
            break;

            case 3:
                return '3';
            break;

            case 4:
                return '4';
            break;

            case 5:
                return '5';
            break;

            case 6:
                return '6';
            break;

            case 7:
                return '7';
            break;

            case 8:
                return '8';
            break;

            case 9:
                return '9';
            break;

            case 0:
                return '0';
            break;

            default:
                return '!';
            break;
        }
    }
}