#include "mbed.h"
#include "MD10C.h"
#include "Parallax_360.h"

MD10C motor(p20, p21, p19);
Parallax_360 servo(p22, p23);
BufferedSerial pc(USBTX, USBRX);        //establish serial communications between PC and NUCLEO

char mainMenu[] = "Main Menu\n 1. Extrude Material\n 2. Fill Hopper\n 3. Retract Actuator\n 4. Retract Servo\n 5. Manual Operation\n 6. Set Duty Cyle\n";
char error1[] = "Error: Invalid selection\n";
char *input = new char[1];
int *menuSelection;

void initComms();

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
                break;
            
            case 4:
                //Retract Servo
                break;

            case 5:
                //Manual Operation
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