#include "mbed.h"
#include "MD10C.h"
#include "Parallax_360.h"

/*MD10C motor(p20, p21, p19);
Parallax_360 servo(p22, p23);*/ //for lpc use

MD10C motor(PA_13, PA_15, PA_0);
Parallax_360 servo(PB_7, PC_13);   //for nucleo use


BufferedSerial pc(USBTX, USBRX);        //establish serial communications between PC and NUCLEO

char mainMenu[] = "Main Menu\n a. Extrude Material\n b. Fill Hopper\n c. Retract Actuator\n d. Retract Servo\n e. Manual Operation\n f. Set Duty Cyle\n\n";
char retractingActuator[] = " Retracting Actuator...   To stop Press 'X'\n\n";
char retractionStopped[] =  "Retraction Stopped\n\n";
char manual[] = "Manual mode: Ensure Pot is initially set to 0\n Press 'Y' to confirm\n\n";
char manual2[] = "Entering manual mode:\n Use 'E' to extend\n Use 'R' to retract the actuator\n Use 'P' to pause actuator motion\n Press 'X' to exit\n\n";
char manual3[] = "Exiting manual mode...\n\n";
char manual4[] = "Extending Actuator...\n\n";
char manual5[] = "Retracting actuator...\n\n";
char manual6[] = "Actuation Paused\n\n";
char error1[] = "Error: Invalid selection\n\n";

char *input = new char[1];

bool inputConfirmation = false;

void initComms();
void retractActuator();
void manualOperation();

int main(){
    initComms();
    motor.init();
    servo.init();

    while(1){
        pc.write(mainMenu, sizeof(mainMenu));
        pc.read(input, sizeof(input));

        switch(*input){
            case 'a':
                //extrude material
                break;

            case 'b':
                //Fill hopper
                break;

            case 'c':
                //Retract Actuator
                retractActuator();
                break;
            
            case 'd':
                //Retract Servo
                break;

            case 'e':
                //Manual Operation
                manualOperation();
                break;

            case 'f':
                //set duty cycle
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

//option A

//option B

//option C
void retractActuator(){
    pc.write(retractingActuator, sizeof(retractingActuator));   //general message output

    while(inputConfirmation == false){          //while input != x or X
        motor.motorOn(RETRACTION, 0.10);        //turn actuator on with 10% duty cycle
        pc.read(input, sizeof(input));          //read input

        if(*input == 'x' || *input == 'X'){     //check for valid exit character
            inputConfirmation = true;           //set valid input to true
        }
    }

    inputConfirmation = false;                  //reset input flag
    pc.write(retractionStopped, sizeof(retractionStopped)); //print message
    motor.stop();                               //stop actuation
    ThisThread::sleep_for(500ms);               
}

//option D

//option E
void manualOperation(){
    pc.write(manual, sizeof(manual));           //print general message

    //wait for confirmation that pot is set to 0
    while(inputConfirmation == false){          //while input != x or X
        pc.read(input, sizeof(input));          //read input

        if(*input != 'y' || *input != 'Y'){     //check for valid exit character
            inputConfirmation = true;           //set valid input to true
        }
    }

    inputConfirmation = false;                  //reset input flag
    pc.write(manual2, sizeof(manual2));         //print general message

    while(inputConfirmation == false){          //while input != x or X
        pc.read(input, sizeof(input));          //read input

        if(*input == 'r' || *input == 'R'){     //if input == r, retract actuator
            pc.write(manual5, sizeof(manual5)); //print general message
            motor.manualMode(RETRACTION);

        }else if(*input == 'e' || *input == 'E'){   //if input == e, extend actuator
            pc.write(manual4, sizeof(manual4));     //print general message
            motor.manualMode(EXTENSION);

        }else if(*input == 'p' || *input == 'P'){   //if input == p, pause actuator motion
            pc.write(manual6, sizeof(manual6));     //print general message
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

//option F