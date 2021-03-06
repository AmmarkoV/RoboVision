#include "RoboVisionSensorLib.h"
#include "arduino_serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int ConnectRoboVisionSensors(char * devname)
{
    if (strlen(devname)<=1)
      {
          fprintf(stderr,"Invalid Device Name For Arduino\n");
          return 0;
      }
    if (!ArduinoThreadStart(devname) )
      {
        fprintf(stderr,"ConnectRoboVisionSensors failed\n");
        return 0;
      }

    fprintf(stderr,"Waiting for Arduino to transmit something meaningfull \n");
    unsigned int tries=0 , max_tries = 200;
    while ( (!RoboVisionSensorsOK())&&(tries<max_tries) )
    {
        fprintf(stderr,".");
        usleep(100000);
        ++tries;
    }
    if ( tries > 0 ) fprintf(stderr,"\n\n");
    if ( !RoboVisionSensorsOK() ) { fprintf(stderr,"Could not get any data off arduino check , port / connections \n"); return 0;  }

    fprintf(stderr,"We are finished waiting \n");

 return 1;
}

int DisconnectRoboVisionSensors()
{
 ArduinoStopThread();
 return 0;
}


int RoboVisionSensorsAreDisconnected()
{
  return (!ARDUINO_THREAD_RUNNING);
}

int RoboVisionSensorsOK()
{
  return ArduinoOk();
}


int SetCameraNod(int motor,int pitch)
{
  return ArduinoInternalSetNod(motor,pitch,1);
}

int SetCameraPose(int heading,int pitch)
{
  return ArduinoInternalSetCameraPose(heading,pitch,1);
}


int GetCameraPose(unsigned int * heading,unsigned int * pitch)
{
  *heading=activated_state.camera_pose_heading;
  *pitch=activated_state.camera_pose_pitch;

  return 1;
}


int SetLights(int light_num,int light_state)
{
  return ArduinoInternalSetLights(light_num,light_state,0);
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
