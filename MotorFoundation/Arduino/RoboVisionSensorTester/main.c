#include <stdio.h>
#include <stdlib.h>
#include "../RoboVisionSensorLib.h"
#include <unistd.h>
int main()
{
    printf("Hello world , I will now start sampling the sensors!!\n");
    ConnectRoboVisionDevice("/dev/ttyUSB0");

      while (1)
       {
      //  printf(" %u \n ", GetUltrasonicValue(0) );
        usleep(10000);
       }
    DisconnectRoboVisionDevice("/dev/ttyUSB0");
    return 0;
}
