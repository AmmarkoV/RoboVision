#include "MotorHAL.h"
#include "MD23/MD23.h"
#include "Arduino/RoboVisionSensorLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



struct md23_device * guard_base=0;

pthread_t monitor_thread_id;

unsigned int AutoMapping=1;
unsigned int StopMonitorThread=0;

struct Map * worldmap=0;

unsigned int robot_height = 50*1000; // mm
unsigned int robot_length = 45*1000; // mm
unsigned int robot_width = 40*1000; // mm
unsigned int motors_distance = 35*1000; // mm
unsigned int wheel_diameter = 10*1000; // mm
float pi = 3.14159;

signed int change_left_encoder=0,change_right_encoder=0,change_left_ultrasonic=0,change_right_ultrasonic=0,change_x_accelerometer=0,change_y_accelerometer=0;

void * HAL_Monitor(void * ptr);

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

    if (AutoMapping)
     {
       worldmap=CreateMap(500,500,3);
       SetMapUnit_In_cm(worldmap,15);
       SetAgentLocation(worldmap,0,250,250);
     }

    pthread_create(&monitor_thread_id, NULL,  HAL_Monitor ,0);

return 0;
}


unsigned int RobotClose()
{
    fprintf(stderr,"Close MD23\n");
    MD23_Close(guard_base);
    DisconnectRoboVisionSensors();
    StopMonitorThread=1;

if (AutoMapping)
     {
      DeleteMap(worldmap);
      worldmap=0;
     }

    return 0;
}


void RobotWait(unsigned int msecs)
{
  usleep(msecs);
  return;
}

/*    -------------------------------------------------
    ~~~~~~~~~~~~~~~~~~MOVEMENT CONTROL ~~~~~~~~~~~~~~~~~~
      -------------------------------------------------*/
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
   fprintf(stderr,"RobotMove %u %d ( in degrees %d ) \n",power,distance,DistanceToWheelDegreesTurn(distance));
   return  MD23_MoveMotorsDegrees(guard_base,2,power,(signed int) DistanceToWheelDegreesTurn(distance));
}

unsigned int RobotStartMoving(unsigned char power,signed int direction)
{
fprintf(stderr,"MD23_MoveMotors %u %d \n",power,direction);
return MD23_MoveMotors(guard_base,2,power,direction);
}

unsigned int RobotGetEncoders(signed int * left_encoder,signed int * right_encoder)
{
  *left_encoder =(signed int ) MD23_GetEncoder(guard_base,0); //guard_base->motors[0].encoder;
  *right_encoder =(signed int ) MD23_GetEncoder(guard_base,0); //guard_base->motors[1].encoder;
  return 1;
}

unsigned int RobotManoeuvresPending()
{
  unsigned int man_flag=0;
  if ( MD23_MovementDone(guard_base)==0 ) man_flag=1;
  return  man_flag;
}

void RobotStopMovement()
{
 MD23_MoveMotors(guard_base,2,0,0);
}

int RobotCanRotate(unsigned char power,signed int degrees)
{
  fprintf(stderr,"Stub called CanRobotRotate not implemented yet\n");
  return 1;
}

int RobotCanMove(unsigned char power,signed int distance)
{
  fprintf(stderr,"Stub called CanRobotMove not implemented yet\n");
  return 1;
}


/*    -------------------------------------------------
    ~~~~~~~~~~~~~~~~~~SENSOR CONTROL ~~~~~~~~~~~~~~~~~~
      -------------------------------------------------*/
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

int RobotSetHeadlightsState(unsigned int scale_1_on,unsigned int scale_2_on,unsigned int scale_3_on)
{
  if ((scale_1_on)||(scale_2_on)||(scale_3_on))
    {
       fprintf(stderr,"Stub called SetHeadLights not implemented yet\n");
    }
  return 0;
}

int RobotIRTransmit(char * code,unsigned int code_size)
{
  if ((code==0)||(code_size==0)) { return 0; }
  fprintf(stderr,"Stub called SetHeadLights not implemented yet\n");

  return 0;
}


/*    -------------------------------------------------
     ~~~~~~~~~~~~~~~~~~STATUS CONTROL ~~~~~~~~~~~~~~~~~~
      -------------------------------------------------*/
unsigned int RobotPrintPosition()
{
 if (AutoMapping) {
                     if (worldmap!=0)
                     {
                       ExtractMaptoHTML(worldmap,"map.html",100);
                     } else
                     {
                        fprintf(stderr,"Map not initialized :( , rescuing segfault \n");
                     }
                  } else
                  {
                    fprintf(stderr,"AutoMapping is switched off , no map.html output possible.. \n");
                  }
 if ( guard_base == 0 ) { return 0; }
 fprintf(stderr,"Robot Encoders :  %f deg , %f deg\n",MD23_GetEncoder(guard_base,0),MD23_GetEncoder(guard_base,1));

 return 0;
}

inline int AbsDifferenceHigherThan(signed int difference,unsigned int low)
{
  if ( difference < 0 ) { difference=-1*difference; }
  if ( difference > low ) { return 1; }
  return 0;
}


inline unsigned int GetTickCount()
{
  struct timespec clock_count_ts;
  unsigned int clock_count=0;
  unsigned long nano_convert=1000000,clock_countbig=0;

  /* CONVERT TIME TO MILLISECONDS WILL WRAP AROUND AT SOME POINT!*/
   clock_gettime(CLOCK_REALTIME,&clock_count_ts);
   clock_countbig = (unsigned long) clock_count_ts.tv_nsec/nano_convert;
   clock_count = (unsigned int) clock_countbig;
   clock_count += clock_count_ts.tv_sec * 1000;
  /* CONVERT TIME TO MILLISECONDS */

   return clock_count;
}

void * HAL_Monitor(void * ptr)
{
  signed int last_left_encoder=0,last_right_encoder=0;
  unsigned int last_left_ultrasonic=0,last_right_ultrasonic=0;
  signed int last_x_accelerometer=0,last_y_accelerometer=0;

  signed int new_left_encoder=0,new_right_encoder=0;
  unsigned int new_left_ultrasonic=0,new_right_ultrasonic=0;
  signed int new_x_accelerometer=0,new_y_accelerometer=0;

  unsigned int clock_count=0,start_clock_count=0;


  start_clock_count = GetTickCount();

  while (!StopMonitorThread)
   {
        clock_count = GetTickCount();
        clock_count = clock_count - start_clock_count;


     usleep(100000);
     RobotGetEncoders(&new_left_encoder,&new_right_encoder);
     new_left_ultrasonic=RobotGetUltrasonic(0);
     new_right_ultrasonic=RobotGetUltrasonic(1);
     new_x_accelerometer=RobotGetAccelerometerX(0);
     new_y_accelerometer=RobotGetAccelerometerY(0);


     change_x_accelerometer=new_x_accelerometer-last_x_accelerometer;
     change_y_accelerometer=new_y_accelerometer-last_y_accelerometer;
     change_left_encoder=new_left_encoder-last_left_encoder;
     change_right_encoder=new_right_encoder-last_right_encoder;
     change_left_ultrasonic=new_left_ultrasonic-last_left_ultrasonic;
     change_right_ultrasonic=new_right_ultrasonic-last_right_ultrasonic;

     last_left_encoder=new_left_encoder;
     last_right_encoder=new_right_encoder;
     last_left_ultrasonic=new_left_ultrasonic;
     last_right_ultrasonic=new_right_ultrasonic;
     last_x_accelerometer=new_x_accelerometer;
     last_y_accelerometer=new_y_accelerometer;

     if ( (AbsDifferenceHigherThan(change_x_accelerometer,8)!=0) || (AbsDifferenceHigherThan(change_y_accelerometer,8)!=0)  )
       {
          fprintf(stderr,"%u ms : Change in Accelerometers %d/%d\n",clock_count,change_x_accelerometer,change_y_accelerometer);
       }

     if ( (AbsDifferenceHigherThan(change_left_encoder,3)!=0) || (AbsDifferenceHigherThan(change_right_encoder,3)!=0) )
       {
          fprintf(stderr,"%u ms : Change in Encoders %d/%d\n",clock_count,change_left_encoder,change_right_encoder);
          if (AutoMapping)
          {
              //MoveAgentForward(struct Map * themap,unsigned int agentnum,int leftwheel_cm,int rightwheel_cm) ;
          }
       }

     if ( (AbsDifferenceHigherThan(change_left_ultrasonic,3)!=0) || (AbsDifferenceHigherThan(change_right_ultrasonic,3)!=0)  )
       {
         fprintf(stderr,"%u ms : Change in Ultrasonics %d/%d\n",clock_count,change_left_ultrasonic,change_right_ultrasonic);
         if (AutoMapping)
         {
          AddObstacleSensedbyAgent(worldmap,0,1,new_left_ultrasonic,new_right_ultrasonic);
         }
       }


   }
   fprintf(stderr,"HAL_Monitor thread stopping \n");
  return 0;
}



/*    -------------------------------------------------
     ~~~~~~~~~~~~~~~~~~ FOR EXTERNAL MAPPING FUNCTIONS ~~~~~~~~~~~~~~~~~~
      -------------------------------------------------*/


struct Map * RobotGetMapPointer()
{
  return worldmap;
}
