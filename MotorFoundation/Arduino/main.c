#include "RoboVisionSensorLib.h"
#include "arduino_serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int ConnectRoboVisionSensors(char * devname)
{
    kickstart_arduino_thread(devname);

    fprintf(stderr,"Waiting for Arduino to transmit something meaningfull \n");
    unsigned int tries=0 , max_tries = 30;
    while ( (!RoboVisionSensorsOK())&&(tries<max_tries) ) { fprintf(stderr,"."); usleep(100000); ++tries; } fprintf(stderr,"\n\n");
    if ( !RoboVisionSensorsOK() ) { fprintf(stderr,"Could not get any data off arduino check , port / connections \n"); return 0;  }


 return 1;
}

int DisconnectRoboVisionSensors()
{
 kill_arduino_thread();
 return 0;
}

int RoboVisionSensorsOK()
{
  return arduino_ok();
}

int GetUltrasonicValue(int dev)
{
  return InternalGetUltrasonicValue(dev);
}

int GetAccelerometerX(int dev)
{ /*Dev parameter is for future use , to use the same cheap dual axis sensor for more axis :P*/
  return InternalGetAccelerometerX(dev);
}

int GetAccelerometerY(int dev)
{
  /*Dev parameter is for future use , to use the same cheap dual axis sensor for more axis :P*/
  return InternalGetAccelerometerY(dev);
}

int SendIRCode(char * ircodes,unsigned int numberofcodes,unsigned int microsecondpause)
{
  fprintf(stderr,"STUB , todo add SendIRCode");
  return 0;
}
