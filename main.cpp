#include "mbed.h"
#include "MD10C.h"
#include "Parallax_360.h"

BufferedSerial pc(USBTX, USBRX);

char *input = new char[1];

char mainMenu[] = "Select an option:\n 1. Extrude Material\n 2. Retract Bed\n 3. Retract Actuator\n 4. System Reset\n 5. Refill Hopper\n 6. Test Actuator\n 7. Test Servo\n";
char message1[] = "Option 1";
char message2[] = "Option 2";
char message3[] = "Option 3";
char message4[] = "Option 4";
char message5[] = "Option 5";
char message6[] = "Option 6";
char message7[] = "Option 7";

int main(){
    pc.set_baud(9600);
    pc.write(mainMenu, sizeof(mainMenu));
    while(1){
        pc.read(input, sizeof(input));

        if(*input == '1'){
            pc.write(message1, sizeof(message1));

        }else if(*input == '2'){
            pc.write(message2, sizeof(message2));

        }else if(*input == '3'){
            pc.write(message3, sizeof(message3));

        }else if(*input == '4'){
            pc.write(message4, sizeof(message4));

        }else if(*input == '5'){
            pc.write(message5, sizeof(message5));

        }else if(*input == '6'){
            pc.write(message6, sizeof(message6));

        }else if(*input == '7'){
            pc.write(message7, sizeof(message7));

        }else{
            printf(">> error invalid option");
        }
    }
}