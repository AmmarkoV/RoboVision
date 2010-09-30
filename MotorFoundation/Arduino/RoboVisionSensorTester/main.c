#include <stdio.h>
#include <stdlib.h>
#include "../RoboVisionSensorLib.h"
#include <unistd.h>
int main()
{
    printf("Hello world , I will now start sampling the sensors!!\n");
    ConnectRoboVisionSensors("/dev/ttyUSB0");

    fprintf(stderr,"Waiting for Arduino to transmit something meaningfull \n");
    unsigned int tries=0;
    while ( (!RoboVisionSensorsOK())&&(tries<100) ) { fprintf(stderr,"."); usleep(100000); ++tries; } fprintf(stderr,"\n\n");
    if ( !RoboVisionSensorsOK() ) { fprintf(stderr,"Could not get any data off arduino check , port / connections \n"); return 1;  }

      while (1)
       {
        printf("Ultrasonic 1 : %u \n ", GetUltrasonicValue(0) );
        printf("Ultrasonic 2 : %u \n ", GetUltrasonicValue(1) );
        printf("Accelerometers ( X : %u , Y : %u ) \n ", GetAccelerometerX(0), GetAccelerometerY(0) );
        usleep(10000);
       }
    DisconnectRoboVisionSensors("/dev/ttyUSB0");
    return 0;
}
