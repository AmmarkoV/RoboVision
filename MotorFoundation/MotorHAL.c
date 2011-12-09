#include "MotorHAL.h"
#include "MD23/MD23.h"
#include "Arduino/RoboVisionSensorLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



struct md23_device * guard_base=0;

pthread_t monitor_thread_id=0;

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

    int  ret=system((const char * ) "ls /dev/ttyUSB*");
    if ( ret == 0 ) { printf("These are the possible usb ports.. \n"); }

    ret=system((const char * ) "ls /dev/ttyUSB* | wc -l");
    if ( ret == 0 ) { printf("total usb ports devices .. \n"); }

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

    if ( pthread_create(&monitor_thread_id, NULL,  HAL_Monitor ,0) != 0 )
      {
          fprintf(stderr,"Could not create monitor thread for Motor HAL \n");
          return 0;
      }

return 1;
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


unsigned int RobotBaseOk()
{
  if ( guard_base == 0) { return 0; }
  if ( !MD23_ok(guard_base) ) { return 0; }
  return 1;
}


unsigned int RobotHeadOk()
{
  if ( !RoboVisionSensorsOK() ) { return 0; }
  return 1;
}

void RobotWait(unsigned int msecs)
{
  usleep(msecs);
  return;
}

/*    -------------------------------------------------
    ~~~~~~~~~~~~~~~~~~MOVEMENT CONTROL ~~~~~~~~~~~~~~~~~~
      -------------------------------------------------*/
inline unsigned int _abs1(signed int num)
{
  if ( num < 0 ) return 0-num;
  return num;
}

// WE EXPECT VALUES FROM -255 TO 255 IN VARIABLE LEFT_RIGHT SIGNALING FULL LEFT  (-255) TO FULL RIGHT (255)
// WE EXPECT VALUES FROM -255 TO 255 IN VARIABLE FRONT_REAR SIGNALING FULL FRONT (-255) TO FULL REAR  (255)
unsigned int RobotMoveJoystick(signed int joy_x,signed int joy_y)
{
 if (!RobotBaseOk()) { return 0;}
  unsigned int MAX_POWER=123,BASE=100;
  unsigned int powerleft=_abs1(joy_y) , powerright=_abs1(joy_y);
  signed int degrees_left=0,degrees_right=0,lr_powercross=0;

  lr_powercross = ( _abs1(joy_x) * _abs1(joy_y) / 255);

  if ( joy_y < 0 )
    {
      degrees_left = BASE , degrees_right = BASE;
      if ( joy_x > 0 )
        {
          powerleft  += lr_powercross , powerright -= lr_powercross;
        }
      else
        if ( joy_x < 0 )
          {
            powerleft  -= lr_powercross , powerright += lr_powercross;
          }
    }
  else
    if ( joy_y > 0 )
      {
        degrees_left = -BASE , degrees_right = -BASE;
        if ( joy_x > 0 )
          {
            powerleft  += lr_powercross , powerright -= lr_powercross;
          }
        else
          if ( joy_x < 0 )
            {
              powerleft  -= lr_powercross , powerright += lr_powercross;
            }
      }

  printf("With Joystick Balance %d , %d i give signal 0( %d,%d ) 1( %d,%d )\n",joy_x,joy_y,powerleft,degrees_left,powerright,degrees_right);
  if ( powerleft > MAX_POWER ) powerleft = MAX_POWER;
  if ( powerright > MAX_POWER ) powerright = MAX_POWER;
  printf("With Enforced limits %d , %d i give signal 0( %d,%d ) 1( %d,%d )\n",joy_x,joy_y,powerleft,degrees_left,powerright,degrees_right);

  //MD23_MoveMotors(guard_base,0,powerleft,degrees_left);
  //MD23_MoveMotors(guard_base,1,powerright,degrees_right);

  MD23_MoveMotorsDegrees(guard_base,0,powerleft,(signed int) DistanceToWheelDegreesTurn(degrees_left*100));
  MD23_MoveMotorsDegrees(guard_base,1,powerright,(signed int) DistanceToWheelDegreesTurn(degrees_right*100));

  return 0;
}


unsigned int RobotSetHeadNod(char * pose_string)
{
  if ( !RoboVisionSensorsOK() ) { return 0; }
  if ( strcmp(pose_string,"YES") == 0 )
      {
           SetCameraNod(0,110);
           fprintf(stderr,"Nod yes\n");

      } else
  if ( strcmp(pose_string,"NO") == 0 )
      {
           SetCameraNod(0,70);
           fprintf(stderr,"Nod no\n");
      }
  return 1;
}

unsigned int RobotSetHeadPose(unsigned int heading,unsigned int pitch)
{
    if ( !RoboVisionSensorsOK() ) { return 0; }
    return SetCameraPose(heading,pitch);
}

unsigned int RobotGetHeadPose(unsigned int * heading,unsigned int * pitch)
{
    if ( !RoboVisionSensorsOK() ) { return 0; }
    return GetCameraPose(heading,pitch);
}



unsigned int RobotStartRotating(unsigned char power,signed int direction)
{
 if (!RobotBaseOk()) { return 0;}

 signed int direct=0;
 if ( direction > 0 ) direct=1; else
 if ( direction < 0 ) direct=-1;

 MD23_MoveMotors(guard_base,0,power,(signed int)-1*direct);
 MD23_MoveMotors(guard_base,1,power,(signed int) 1*direct);

 return 0;
}

unsigned int RobotRotate(unsigned char power,signed int degrees)
{
 if (!RobotBaseOk()) { return 0;}

 signed int dist_to_travel = (degrees * pi * motors_distance) / 180 ;
 fprintf(stderr,"Distance to turn %u \n",dist_to_travel);
 MD23_MoveBothMotorsDifferentDegrees(guard_base,power,(signed int) -DistanceToWheelDegreesTurn(dist_to_travel),power,(signed int) DistanceToWheelDegreesTurn(dist_to_travel));
 return 0;
}

unsigned int RobotMove(unsigned char power,signed int distance)
{
   if (!RobotBaseOk()) { return 0;}

   fprintf(stderr,"RobotMove %u %d ( in degrees %d ) \n",power,distance,DistanceToWheelDegreesTurn(distance));
   return  MD23_MoveMotorsDegrees(guard_base,2,power,(signed int) DistanceToWheelDegreesTurn(distance));
}

unsigned int RobotStartMoving(unsigned char power,signed int direction)
{
  if (!RobotBaseOk()) { return 0;}

  fprintf(stderr,"MD23_MoveMotors %u %d \n",power,direction);
  return MD23_MoveMotors(guard_base,2,power,direction);
}

unsigned int RobotGetEncoders(signed int * left_encoder,signed int * right_encoder)
{
  if (!RobotBaseOk()) { return 0;}

  *left_encoder =(signed int ) MD23_GetEncoder(guard_base,0); //guard_base->motors[0].encoder;
  *right_encoder =(signed int ) MD23_GetEncoder(guard_base,0); //guard_base->motors[1].encoder;
  return 1;
}

unsigned int RobotManoeuvresPending()
{
  if (!RobotBaseOk()) { return 0;}

  unsigned int man_flag=0;
  if ( MD23_MovementDone(guard_base)==0 ) man_flag=1;
  return  man_flag;
}

void RobotStopMovement()
{
  if (!RobotBaseOk()) { return;}

  MD23_MoveMotors(guard_base,2,0,0);
}

int RobotCanRotate(unsigned char power,signed int degrees)
{
  if (!RobotBaseOk()) { return 0;}

  fprintf(stderr,"Stub called CanRobotRotate not implemented yet\n");
  return 1;
}

int RobotCanMove(unsigned char power,signed int distance)
{
  if (!RobotBaseOk()) { return 0;}

  fprintf(stderr,"Stub called CanRobotMove not implemented yet\n");
  return 1;
}


/*    -------------------------------------------------
    ~~~~~~~~~~~~~~~~~~SENSOR CONTROL ~~~~~~~~~~~~~~~~~~
      -------------------------------------------------*/
int RobotGetUltrasonic(unsigned int which_one)
{
  if (!RobotBaseOk()) { return 0;}

  return GetUltrasonicValue(which_one);
}

int RobotGetAccelerometerX()
{
  if (!RobotBaseOk()) { return 0;}

  return GetAccelerometerX();
}

int RobotGetAccelerometerY()
{
  if (!RobotBaseOk()) { return 0;}

  return GetAccelerometerY();
}

int RobotSetLightsState(unsigned int light_num,unsigned int light_state)
{
  SetLights(light_num,light_state);
  return 0;
}


int RobotIRTransmit(char * code,unsigned int code_size)
{
  if (!RobotBaseOk()) { return 0;}

  if ((code==0)||(code_size==0)) { return 0; }
  fprintf(stderr,"Stub called SetHeadLights not implemented yet\n");

  return 0;
}


/*    -------------------------------------------------
     ~~~~~~~~~~~~~~~~~~STATUS CONTROL ~~~~~~~~~~~~~~~~~~
      -------------------------------------------------*/
unsigned int RobotPrintPosition()
{
 if (!RobotBaseOk()) { return 0;}

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
  if (!RobotBaseOk()) { return 0;}

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


     usleep(200 * 1000); // 100 ms
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
