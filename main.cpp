#include "mbed.h"
#include "MD10C.h"
#include "Parallax_360.h"

/*MD10C motor(p20, p21, p19);
Parallax_360 servo(p22, p23);*/ //for lpc use

MD10C motor(PA_13, PA_15, PA_0);
Parallax_360 servo(PB_7, PC_13);   //for nucleo use


BufferedSerial pc(USBTX, USBRX);        //establish serial communications between PC and NUCLEO

char mainMenu[] = "Main Menu\n a. Extrude Material\n b. Fill Hopper\n c. Retract Actuator\n d. Retract Servo\n e. Manual Operation\n f. Set Duty Cyle\n\n";

//option C messages
char retractingActuator[] = " Retracting Actuator...   To stop Press 'X'\n\n";
char retractionStopped[] =  "Retraction Stopped\n\n";

//option E messages
char manual[] = "Manual mode: Ensure Pot is initially set to 0\n Press 'Y' to confirm\n\n";
char manual2[] = "Entering manual mode:\n Use 'E' to extend\n Use 'R' to retract the actuator\n Use 'P' to pause actuator motion\n Press 'X' to exit\n\n";
char manual3[] = "Exiting manual mode...\n\n";
char manual4[] = "Extending Actuator...\n\n";
char manual5[] = "Retracting actuator...\n\n";
char manual6[] = "Actuation Paused\n\n";

//option F messages
char set1[] = "Set the Duty Cycle of the actuator\nThe current Duty Cycle value is: ";
char set3[] = "\n\nSet new duty cycle: ";
char set4[] = "\n\nNew duty cycle value is: ";

char error1[] = "Error: Invalid selection\n\n";

char *input = new char[1];

bool inputConfirmation = false;

void initComms();
void retractActuator();
void manualOperation();
void setDuty();

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

//option A

//option B

//option C
void retractActuator(){
    pc.write(retractingActuator, sizeof(retractingActuator));   //general message output

    while(inputConfirmation == false){          //while input != x or X
        motor.motorOn(RETRACTION);        //turn actuator on with 10% duty cycle
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
    char dutyValue[1];
    char dutymess[] = "The current duty cyle is: ";
    char doubleReturn[] = "\n\n";

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
        dutyValue[0] = motor.getDutyCycle();

        if(*input == 'r' || *input == 'R'){     //if input == r, retract actuator
            pc.write(manual5, sizeof(manual5)); //print general message
            pc.write(dutymess, sizeof(dutymess));
            pc.write(doubleReturn, sizeof(doubleReturn));
            motor.manualMode(RETRACTION);

        }else if(*input == 'e' || *input == 'E'){   //if input == e, extend actuator
            pc.write(manual4, sizeof(manual4));     //print general message
            pc.write(dutymess, sizeof(dutymess));
            pc.write(doubleReturn, sizeof(doubleReturn));
            motor.manualMode(EXTENSION);

        }else if(*input == 'p' || *input == 'P'){   //if input == p, pause actuator motion
            pc.write(manual6, sizeof(manual6));     //print general message
            pc.write(dutymess, sizeof(dutymess));
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

//option F
void setDuty(){
    char buffer[32];
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
    }
}

/*//option F messages
char set1[] = "Set the Duty Cycle of the actuator\nThe current Duty Cycle value is: ";
char set2[1];    //use to read the current value of the duty cycle value
char set3[] = "\n\nSet new duty cycle: ";
char set4[] = "\n\nNew duty cycle value is: ";*/