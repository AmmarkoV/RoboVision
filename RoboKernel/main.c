#include <pthread.h>
#include "RoboKernel.h"
#include "command_hal.h"
#include "command_implementation.h"
#include "visual_system.h"
#include "mapping_system.h"
#include "motor_system.h"
#include "webinterface.h"
#include "configuration.h"
#include "activity_coordination.h"
#include "script_runner.h"
#include "joystick.h"
#include "irc_interface.h"
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../MotorFoundation/MotorHAL.h"


#define PRINT_DOTS_ALIVE 1


struct ThreadPassParam { int feednum; };
int go_to_sleep=0;

unsigned int clock_count=0;

pthread_t kernel_loop_id=0;
void * KernelLoop(void *ptr );

int IssueCommand(char * cmd,char * res,unsigned int resmaxsize,char * from)
{
  char output_string[512]={0};
 return IssueCommandInternal((char *) cmd,(char *) from,output_string,512);
}


unsigned char * GetVideoRegister(unsigned int num)
{
    return VisCortx_ReadFromVideoRegister(num,VisCortx_GetMetric(RESOLUTION_X),VisCortx_GetMetric(RESOLUTION_Y),VisCortx_GetMetric(RESOLUTION_DEPTH));
}

unsigned int GetCortexSetting(unsigned int option)
{
    return VisCortx_GetSetting(option);
}

void SetCortexSetting(unsigned int option,unsigned int value)
{
  VisCortx_SetSetting(option,value);
}

unsigned int GetCortexMetric(unsigned int option)
{
    return VisCortx_GetMetric(option);
}

unsigned int GetVideoData(unsigned int num)
{
    return 0;
}

void InitSenses()
{
   LoadConfiguration(); // Set Correct values to VisCortx :P 2nd time but cannot make something better right now :P

   InitMotorSystem();
   InitVisualSystem();
   OpenWebInterface(); //<- this will get discontinued..!
   StartEmbeddedWebInterface();
   StartIRCInterface();

   StartJoystickControl();
   StartScriptRunnerServer();
   EngageActivity(FACE_TRACKING);

   fprintf(stderr,"Senses initialized\n");
}

void CloseSenses()
{
   StopScriptRunnerServer();
   StopJoystickControl();


   StopIRCInterface();
   StopEmbeddedWebInterface();
   CloseWebInterface();  //<- this will get discontinued..!
   CloseMotorSystem();
   CloseVisualSystem();

}


void find_something_to_do(unsigned int clock_time)
{
  /* THIS FUNCTION WILL CREATE A SCRIPT OF FUNCTIONS TO BE PASSED TO DO_SOMETHING */
   ActivityLoop();
}

void do_something(unsigned int clock_time)
{
  /* THIS FUNCTION WILL EXECUTE SCRIPT OF FUNCTIONS CREATED BY FIND_SOMETHING_TO_DO */

  if ( motion_lock_on == 1 )
    {
      if ( CheckAlarm(VisCortx_GetMetric(CHANGES_LEFT) , VisCortx_GetMetric(CHANGES_RIGHT)) )
      {
          Danger();
      }
    }


  if ( keep_snapshots == 1 )
    {
        if (clock_time<last_snapshot_activation ) { last_snapshot_activation = 0; } /* SLOPPY TRUNCATION :P */ else
        if (clock_time-last_snapshot_activation> snapshot_activation_interval )
         {
              char output_string[512];
              IssueCommandInternal("RECORD SNAPSHOT(1)","system",output_string,512);
              last_snapshot_activation = clock_time;
         }
    }

  if ( system_autonomous == 1)
         {
             if ( clock_time % 5 == 0 ) { fprintf(stderr,"Reminder, that Hypervisor status is recorded\n"); VisCortx_GetHyperVisorStatus(0,1,0); } else
             if ( clock_time % 5000 == 0 ) { RobotStartRotating(50,1); } else
             if ( clock_time % 10000 == 0 ) { RobotStartRotating(50,-1); }
         }

}


unsigned int DropRootUID()
{
   fprintf(stderr,"Not Dropping Root UID , until I fix permissions on audio devices and other things \n ");
   // return setuid(1001); // Non Root UID :-P
   return 0;
}

int StartRoboKernel()
{
    clock_count = 0;
    struct ThreadPassParam param={0};
    param.feednum=0;
    EngageActivity(FACE_TRACKING);
    if ( pthread_create( &kernel_loop_id , NULL,  KernelLoop ,(void*) &param) != 0 )
     {
         fprintf(stderr,"Error creating kernel loop \n");
         DropRootUID();
         return 0;
     }

    DropRootUID();
    return 1;
}


unsigned int SanityCheck()
{
  unsigned int check_returned_ok=1;
  char message[1024]={0};
  if (  (GetEmptyFrame() ==  GetFrame(1)) && (GetEmptyFrame() ==  GetFrame(0)) ) { strcat(message," I cannot see anything."); } else
  if (  GetEmptyFrame() ==  GetFrame(0)) { strcat(message," Camera 0 is dead."); } else
  if (  GetEmptyFrame() ==  GetFrame(1)) { strcat(message," Camera 1 is dead."); }

  if (  !RobotBaseOk() ) {  strcat(message," I Cannot move my body."); }
  if (  !RobotHeadOk() ) {  strcat(message," I Cannot move my head or receive input from sensors."); }

  if (strlen(message)>0) // IF SOMETHING IS SET WE ARE NOT COOL
   {  check_returned_ok = 0;  }

   strcat(message," Robovision started.");
   char final_message[1200]={0};

   sprintf(final_message,"SAY(%s)",message);
   IssueCommand(final_message,0,0,"RoboKernel");

  return check_returned_ok;
}


void * KernelLoop(void *ptr )
{
  InitSenses();
  struct timespec clock_count_ts;
  unsigned int  loopcount=0;
  unsigned long nano_convert=1000000,clock_countbig=0;


  SanityCheck();

  DropRootUID();


   while ( go_to_sleep == 0 )
    {
        /* CONVERT TIME TO MILLISECONDS WILL WRAP AROUND AT SOME POINT!*/
        clock_gettime(CLOCK_REALTIME,&clock_count_ts);
        clock_countbig = (unsigned long) clock_count_ts.tv_nsec/nano_convert;
        clock_count = (unsigned int) clock_countbig;
        clock_count += clock_count_ts.tv_sec * 1000;
        /* CONVERT TIME TO MILLISECONDS */


        /* PASS VIDEO REFERENCES AROUND MEMORY */
        PassVideoInputToCortex(clock_count);

        /* PERFORM GUARDDOG DUTIES -----------------*/
        find_something_to_do(clock_count);

        do_something(clock_count);
        /* -----------------------------------------*/


        ++loopcount;
        usleep(1000);

        if ( (PRINT_DOTS_ALIVE) && (loopcount%20000 == 0) )  { fprintf(stderr,"."); }
    }

  IssueCommand("SAY(Robovision closing)",0,0,"RoboKernel");

  CloseSenses();

  go_to_sleep=2;
  return 0;
}

int StopRoboKernel()
{

    char filename[512]={0};
    strcpy(filename,"echo \" \" > ");
    strcat(filename,CONSOLE_OUT_PATH);

    fprintf(stderr,"NOTE : Flushing consoleout.dat to prevent it from beeing refreshed over and over on github.. :P\n");
    int i = system(filename);
    if ( i!=0 ) { fprintf(stderr,"It failed.. well , no big deal \n"); }

  go_to_sleep=1;
  return 1;
}

int CheckThatRoboKernelStopped()
{
   //STUB todo here

  if (!IRCInterfaceStopped()) { return 0;}
  if (!JoystickControlStopped()) { return 0;}
  if (!MotorSystemIsClosed()) { return 0;}
  if (!ScriptRunnerStopped()) { return 0;}
  if (!WebInterfaceStopped()) { return 0;}


   if ( go_to_sleep == 2 )
     {
         return 1;
     }

   return 0;
}


int RoboKernelAlive()
{

  if ( go_to_sleep==2 ) { return 0; }
  return 1;
}


struct Map * GetWorldHandler()
{
   return RobotGetMapPointer();
}

char * GetRoboKernelEnvPath()
{
   return ENVDIR;
}

