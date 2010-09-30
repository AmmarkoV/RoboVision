#include "RoboVisionSensorLib.h"
#include "arduino_serial.h"
#include <stdio.h>
#include <stdlib.h>

int ConnectRoboVisionSensors(char * devname)
{
 kickstart_arduino_thread(devname);
 return 0;
}

int DisconnectRoboVisionSensors(char * devname)
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
