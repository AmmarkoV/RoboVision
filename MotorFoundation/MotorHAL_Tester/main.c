#include "../MotorHAL.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Hello world!\n");
    RobotInit("/dev/ttyUSB1");

    printf("Moving!\n");

    RobotMove(10,10);
    while ( RobotManoeuvresPending() == 1 ) { RobotWait(1000); fprintf(stderr,"."); }
    fprintf(stderr,"done move forward!\n");
    RobotPrintPosition();

    RobotMove(10,-10);
    while ( RobotManoeuvresPending() == 1 ) { RobotWait(1000); fprintf(stderr,"."); }
    fprintf(stderr,"done move backward!\n");
    RobotPrintPosition();

    RobotMove(10,10);
    while ( RobotManoeuvresPending() == 1 ) { RobotWait(1000); fprintf(stderr,"."); }
    fprintf(stderr,"done move forward!\n");
    RobotPrintPosition();

    fprintf(stderr,"DONE!\n");

   // getchar();

    RobotClose();
    return 0;
}
