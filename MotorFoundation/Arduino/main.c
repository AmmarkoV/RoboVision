#include "RoboVisionSensorLib.h"
#include "arduino_serial.h"
#include <stdio.h>
#include <stdlib.h>

int ConnectRoboVisionDevice(char * devname)
{
 fprintf(stderr,"STUB , todo add Arduino Device KickStart");
 kickstart_arduino_thread(0);
 return 0;
}


int DisconnectRoboVisionDevice(char * devname)
{
 fprintf(stderr,"STUB , todo add Arduino Device Stopping");
 return 0;
}


int GetUltrasonicValue(int dev)
{
  fprintf(stderr,"STUB , todo add GetUltrasonicValue");
  return 0;
}



int GetAccelerometerX(int dev)
{
  fprintf(stderr,"STUB , todo add GetAccelerometerX");
  return 0;
}


int GetAccelerometerY(int dev)
{
  fprintf(stderr,"STUB , todo add GetAccelerometerY");
  return 0;
}

int SendIRCode(char * ircodes,unsigned int numberofcodes,unsigned int microsecondpause)
{
  fprintf(stderr,"STUB , todo add SendIRCode");
  return 0;
}
