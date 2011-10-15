#include "RoboVisionSensorLib.h"
#include "arduino_serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int ConnectRoboVisionSensors(char * devname)
{
    if (!ArduinoThreadStart(devname) )
      {
        fprintf(stderr,"ConnectRoboVisionSensors failed\n");
        return 0;
      }

    fprintf(stderr,"Waiting for Arduino to transmit something meaningfull \n");
    unsigned int tries=0 , max_tries = 200;
    while ( (!RoboVisionSensorsOK())&&(tries<max_tries) ) { fprintf(stderr,"."); usleep(100000); ++tries; } fprintf(stderr,"\n\n");
    if ( !RoboVisionSensorsOK() ) { fprintf(stderr,"Could not get any data off arduino check , port / connections \n"); return 0;  }


 return 1;
}

int DisconnectRoboVisionSensors()
{
 ArduinoStopThread();
 return 0;
}

int RoboVisionSensorsOK()
{
  return ArduinoOk();
}

int SetCameraPose(int heading,int pitch)
{
  return ArduinoInternalSetCameraPose(heading,pitch);
}

int GetUltrasonicValue(int which_one)
{
  return ArduinoInternalGetUltrasonicValue(which_one);
}

int GetAccelerometerX()
{ /*Dev parameter is for future use , to use the same cheap dual axis sensor for more axis :P*/
  return ArduinoInternalGetAccelerometerX();
}

int GetAccelerometerY()
{
  /*Dev parameter is for future use , to use the same cheap dual axis sensor for more axis :P*/
  return ArduinoInternalGetAccelerometerY();
}

int SendIRCode(char * ircodes,unsigned int numberofcodes,unsigned int microsecondpause)
{
  fprintf(stderr,"STUB , todo add SendIRCode");
  return 0;
}
