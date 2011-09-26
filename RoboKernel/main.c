#include <pthread.h>
#include "RoboKernel.h"
#include "command_hal.h"
#include "command_implementation.h"
#include "visual_system.h"
#include "mapping_system.h"
#include "motor_system.h"
#include "webinterface.h"
#include "configuration.h"
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../MotorFoundation/MotorHAL.h"

struct ThreadPassParam { int feednum; };
int go_to_sleep=0;

unsigned int clock_count=0;

pthread_t kernel_loop_id=0;
void * KernelLoop(void *ptr );

int IssueCommand(char * cmd,char * res,unsigned int resmaxsize,char * from)
{
 return IssueCommandInternal((char *) cmd,(char *) from);
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

   InitVisualSystem();
   InitMotorSystem();
   OpenWebInterface();
   fprintf(stderr,"Senses initialized\n");
}

void CloseSenses()
{
   CloseWebInterface();
   CloseVisualSystem();
   CloseMotorSystem();
}


void find_something_to_do(unsigned int clock_time)
{
  /* THIS FUNCTION WILL CREATE A SCRIPT OF FUNCTIONS TO BE PASSED TO DO_SOMETHING */
}

void do_something(unsigned int clock_time)
{
  /* THIS FUNCTION WILL EXECUTE SCRIPT OF FUNCTIONS CREATED BY FIND_SOMETHING_TO_DO */
  if ( keep_snapshots == 1 )
    {
        if (clock_time<last_snapshot_activation ) { last_snapshot_activation = 0; } /* SLOPPY TRUNCATION :P */ else
        if (clock_time-last_snapshot_activation> snapshot_activation_interval )
         {
              IssueCommandInternal("RECORD SNAPSHOT(1)","system");
              last_snapshot_activation = clock_time;
         }
    }

}

int StartRoboKernel()
{
    clock_count = 0;
    struct ThreadPassParam param={0};
    param.feednum=0;
    if ( pthread_create( &kernel_loop_id , NULL,  KernelLoop ,(void*) &param) != 0 )
     {
         fprintf(stderr,"Error creating kernel loop \n");
         return 0;
     }

    return 1;
}


unsigned int SanityCheck()
{
  char message[1024]={0};
  if (  GetEmptyFrame() ==  GetFrame(0)) { strcat(message," Camera 0 is dead."); }
  if (  GetEmptyFrame() ==  GetFrame(1)) { strcat(message," Camera 1 is dead."); }
  if (  (GetEmptyFrame() ==  GetFrame(1)) && (GetEmptyFrame() ==  GetFrame(0)) ) { strcat(message," I cannot see anything."); }

  if (  !RobotBaseOk() ) {  strcat(message," I Cannot move."); }

  if (strlen(message)>0)
   {
     strcat(message," Robovision started.");
     char final_message[1200]={0};
     sprintf(final_message,"SAY(%s)",message);
     IssueCommand(final_message,0,0,"RoboKernel");

     return 0;
   } else
   {
     strcat(message," Robovision started.");
     char final_message[1200]={0};
     sprintf(final_message,"SAY(%s)",message);
     IssueCommand(final_message,0,0,"RoboKernel");
   }
  return 1;
}



void * KernelLoop(void *ptr )
{
  InitSenses();
  struct timespec clock_count_ts;
  unsigned int  loopcount=0;
  unsigned long nano_convert=1000000,clock_countbig=0;


  SanityCheck();

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

        //TakeCareOfNetworkInterface(clock_count);

        if ( motion_lock_on == 1 ) { CheckAlarm(VisCortx_GetMetric(CHANGES_LEFT) , VisCortx_GetMetric(CHANGES_RIGHT)); }

        find_something_to_do(clock_count);
        do_something(clock_count);

        //fprintf(stderr,"%u ms ( %u )\n",clock_count,clock_countbig);
        ++loopcount;
        if ( motor_system_autonomous )
         {
             if ( loopcount % 5000 == 0 ) { RobotStartRotating(50,1); } else
             if ( loopcount % 10000 == 0 ) { RobotStartRotating(50,-1); }

         }

        if (loopcount%10000 == 0 )  { fprintf(stderr,"."); }

        usleep(1000);
    }

  IssueCommand("SAY(Robovision closing)",0,0,"RoboKernel");

  CloseSenses();

  go_to_sleep=2;
  return 0;
}

int StopRoboKernel()
{

    fprintf(stderr,"NOTE : Flushing consoleout.dat to prevent it from beeing refreshed over and over on github.. :P\n");
    int i = system((const char *) "echo " " > memfs/public_html/consoleout.dat");
    if ( i!=0 ) { fprintf(stderr,"It failed.. well , no big deal \n"); }

  go_to_sleep=1;
  return 1;
}

int RoboKernelAlive()
{
  if ( go_to_sleep==2 ) { return 0; }
  return 1;
}
