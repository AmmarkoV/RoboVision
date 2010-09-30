#include <stdio.h>
#include <stdlib.h>
#include "../RoboVisionSensorLib.h"
#include <unistd.h>
int main()
{
    printf("Hello world , I will now start sampling the sensors!!\n");
    ConnectRoboVisionSensors("/dev/ttyUSB0");


      while (1)
       {
        printf("Ultrasonic 1 : %u \n ", GetUltrasonicValue(0) );
        printf("Ultrasonic 2 : %u \n ", GetUltrasonicValue(1) );
        printf("Accelerometers ( X : %u , Y : %u ) \n ", GetAccelerometerX(0), GetAccelerometerY(0) );
        usleep(10000);
       }
    DisconnectRoboVisionSensors();
    return 0;
}
