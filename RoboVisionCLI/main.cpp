#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../RoboKernel/RoboKernel.h"
#include <time.h>
#include <string.h>


#define MAX_INPUT_STRING 2048
#define START_UP_SLEEP_TIME 7000
#define SLEEP_TIME_PER_LOOP_MS 1000


/*
   This is the non wxWidgets/non GUI , yes daemon/yes CLI ;P based binary for guarddog , the RoboVisionX executable basically does exactly the same
   ( since the underlying stack is the same ) , but it draws user friendly widgets accross the screen and polls and displays the camera/video reg input..

   What is done here is basically Initialize GuarddoG , wait and if we receive a termination request close GuarddoG
   if the program is called with the parameter noinput the program will not "fget" input from the terminal and so when guarddog starts as a daemon
   the parameter noinput should be appended since trying to get an input without a tty would result in an error..
*/


using namespace std;
unsigned int read_input = 1;

unsigned int wait_milliseconds(unsigned int time_milli)
{
  return  usleep( time_milli * 1000 ); /* Allow 5 seconds for guarddog scripts , etc. to startup*/
}

int LoopWithInput()
{
  char buffer[MAX_INPUT_STRING]= {0};
  char *cptr=0;
  fprintf(stderr,"RoboVisionCLI is now running in the background , the client is waiting for input \n");

  do
    {
      printf("Enter command ( exit stops RoboVision ) :\n");
      cptr = fgets(buffer,MAX_INPUT_STRING,stdin);
      if ( cptr != 0 )
        {
          IssueCommand(cptr,0,0,(char *)"CLI");
        }
    }
  while ( (strncmp(cptr,"exit",4)!=0 ) && (RoboKernelAlive()==1) );
  return 1;
}

int Loop()
{
  fprintf(stderr,"RoboVisionCLI is now running in the background \n");
  do { wait_milliseconds(SLEEP_TIME_PER_LOOP_MS); } while (RoboKernelAlive()==1);
  fprintf(stderr,"RoboVisionCLI has now stopped running in the background \n");
  return 1;
}

int main(int argc, const char* argv[])
{
  if ( argc > 1 )
    {
      int i=0;
      for (i=0; i<argc; i++)
        {
          fprintf(stderr,"Arg %u - %s\n",i,argv[i]);
          if ( strcmp("noinput",argv[i])==0 )
            {
              printf("ReadInput is disabled with switch!\n");
              read_input = 0;
            }
        }
    }

  struct timespec start,end;
  clock_gettime(CLOCK_REALTIME,&start);

  printf("Starting RoboKernel!\n");
  if ( StartRoboKernel() )
    {
      wait_milliseconds(START_UP_SLEEP_TIME); // Dead time until RoboKernel Kicks in , robot is working we just dont receive commands ( and that only in case of looping with input )
      //Depending on if we want to sample for input we choose the main loop..!
      if ( read_input ) { LoopWithInput(); }
                           else
                        { Loop();          }

      printf("Stoping RoboKernel!\n");
      StopRoboKernel();
    }
  else
    { fprintf(stderr,"RoboKernel failed to start\n"); }

  printf("Waiting for de-initialization!\n");
  while ( RoboKernelAlive()==1 ) { usleep(SLEEP_TIME_PER_LOOP_MS); }
  usleep(SLEEP_TIME_PER_LOOP_MS*2*1000);
  clock_gettime(CLOCK_REALTIME,&end);
  fprintf(stderr,"De-initialization complete, complete halt of robovision!\n");
  return 0;
}
