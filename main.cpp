#include "mbed.h"
#include "MD10C.h"
#include "Parallax_360.h"

MD10C motor(p20, p21, p19);
Parallax_360 servo(p22, p23);
BufferedSerial pc(USBTX, USBRX);        //establish serial communications between PC and NUCLEO

char mainMenu[] = "Main Menu\n 1. Extrude Material\n 2. Fill Hopper\n 3. Retract Actuator\n 4. Retract Servo\n 5. Manual Operation\n 6. Set Duty Cyle\n";
char retractingActuator[] = " Retracting Actuator...   To stop Press 'X'\n ";
char retractionStopped[] =  "Retraction Stopped\n";
char manual[] = "Manual mode: Ensure Pot is initially set to 0\n Press 'Y' to confirm\n";
char manual2[] = "Entering manual mode:\n Use 'E' to extend and 'R' to retract the actuator\n Press 'X' to exit\n";
char manual3[] = "Exiting manual mode\n";
char error1[] = "Error: Invalid selection\n";
char *input = new char[1];
int *menuSelection;

void initComms();
void retractActuator();
void manualOperation();

int main(){
    initComms();

    while(1){
        pc.write(mainMenu, sizeof(mainMenu));
        pc.read(menuSelection, sizeof(menuSelection));

        switch(*menuSelection){
            case 1:
                //extrude material
                break;

            case 2:
                //Fill hopper
                break;

            case 3:
                //Retract Actuator
                retractActuator();
                break;
            
            case 4:
                //Retract Servo
                break;

            case 5:
                //Manual Operation
                manualOperation();
                break;

            case 6:
                //set duty cycle
                break;

            default:
                pc.write(error1, sizeof(error1));
                break;
        }
    }
}

void initComms(){
    pc.set_baud(9600);
    pc.set_format(8, BufferedSerial::None, 1); //8-N-1
}

void retractActuator(){
    pc.write(retractingActuator, sizeof(retractingActuator));

    do{
        pc.read(input, sizeof(input));
        motor.motorOn(RETRACTION, 0.25);
        ThisThread::sleep_for(20ms);
    }while(*input != 'x' || *input != 'X'); //retract actuator while stop command is not given

    pc.write(retractionStopped, sizeof(retractionStopped));
    motor.stop();
}

void manualOperation(){
    pc.write(manual, sizeof(manual));

    //wait for confirmation that pot is set to 0
    do{
        pc.read(input, sizeof(input));
    }while(*input != 'y' || *input != 'Y');

    pc.write(manual2, sizeof(manual2));

    //allow manual operation
    do{
        pc.read(input, sizeof(input));

        if(*input == 'r' || *input == 'R'){
            motor.manualMode(RETRACTION);

        }else if(*input == 'e' || *input == 'E'){
            motor.manualMode(EXTENSION);

        }else{
            pc.write(error1, sizeof(error1));
            
        }

    }while(*input != 'x' || *input != 'X');

    pc.write(manual3, sizeof(manual3));
    motor.stop();
}