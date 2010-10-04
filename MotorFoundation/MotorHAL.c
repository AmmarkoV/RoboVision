#include "MotorHAL.h"
#include "MD23/MD23.h"
#include "Arduino/RoboVisionSensorLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct md23_device * guard_base=0;

unsigned int motors_distance = 35*1000; // mm
unsigned int wheel_diameter = 10*1000; // mm
float pi = 3.14159;

signed int DistanceToWheelDegreesTurn(signed int dist)
{
   signed int res=( (180*dist) / (wheel_diameter * pi) );
   return res;
}

unsigned int RobotInit(char * md23_device_id,char * arduino_device_id)
{
    printf("MD23 Code Version %s !\n",MD23_GetVersion());
    guard_base=MD23_Init(md23_device_id,1);
    fprintf(stderr,"MD23 Inited\n");
    MD23_ZeroEncoders(guard_base);


    printf("RoboVisionSensors ( arduino ) beeing initialized \n");
    ConnectRoboVisionSensors(arduino_device_id);
return 0;
}

unsigned int RobotStartRotating(unsigned char power,signed int direction)
{
 signed int direct=0;
 if ( direction > 0 ) direct=1; else
 if ( direction < 0 ) direct=-1;

 MD23_MoveMotors(guard_base,0,power,(signed int)-1*direct);
 MD23_MoveMotors(guard_base,1,power,(signed int) 1*direct);

 return 0;
}

unsigned int RobotRotate(unsigned char power,signed int degrees)
{
 signed int dist_to_travel = (degrees * pi * motors_distance) / 180 ;
 fprintf(stderr,"Distance to turn %u \n",dist_to_travel);
 MD23_MoveBothMotorsDifferentDegrees(guard_base,power,(signed int) -DistanceToWheelDegreesTurn(dist_to_travel),power,(signed int) DistanceToWheelDegreesTurn(dist_to_travel));
 return 0;
}

unsigned int RobotMove(unsigned char power,signed int distance)
{
return  MD23_MoveMotorsDegrees(guard_base,2,power,(signed int) DistanceToWheelDegreesTurn(distance));
}

unsigned int RobotStartMoving(unsigned char power,signed int direction)
{
fprintf(stderr,"MD23_MoveMotors %u %d \n",power,direction);
return MD23_MoveMotors(guard_base,2,power,direction);
}

unsigned int RobotManoeuvresPending()
{
  unsigned int man_flag=0;
  if ( MD23_MovementDone(guard_base)==0 ) man_flag=1;
  return  man_flag;
}

unsigned int RobotPrintPosition()
{
fprintf(stderr,"Robot Encoders :  %f deg , %f deg\n",MD23_GetEncoder(guard_base,0),MD23_GetEncoder(guard_base,1));
return 0;
}

void RobotStopMovement()
{
 MD23_MoveMotors(guard_base,2,0,0);
}

int RobotGetUltrasonic(unsigned int dev)
{
  return GetUltrasonicValue(dev);
}


int RobotGetAccelerometerX(unsigned int dev)
{
  return GetAccelerometerX(dev);
}

int RobotGetAccelerometerY(unsigned int dev)
{
  return GetAccelerometerY(dev);
}

void RobotWait(unsigned int msecs)
{
  usleep(msecs);
  return;
}

unsigned int RobotClose()
{
    fprintf(stderr,"Close MD23\n");
    MD23_Close(guard_base);
    DisconnectRoboVisionSensors();
    return 0;
}
