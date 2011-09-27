#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../RoboKernel/RoboKernel.h"
#include <time.h>
#include <string.h>

using namespace std;
unsigned int read_input = 1;

unsigned int wait_milliseconds(unsigned int time_milli)
{
  return  usleep( time_milli * 1000 ); /* Allow 5 seconds for guarddog scripts , etc. to startup*/
}


int LoopWithInput()
{
  char buffer[2048]= {0};
  char *cptr=0;

  fprintf(stderr,"RoboVisionCLI is now running in the background , the client is waiting for input \n");

  do
    {
      printf("Enter command ( exit stops RoboVision ) :\n");
      cptr = fgets(buffer,2048, stdin);
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

  do
    {
      wait_milliseconds(500);
    }
  while (RoboKernelAlive()==1);
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
      wait_milliseconds(5000);



      if ( read_input )
        {
          LoopWithInput();
        }
      else
        {
          Loop();
        }



      printf("Stoping RoboKernel!\n");
      StopRoboKernel();
    }
  else
    {
      fprintf(stderr,"RoboKernel failed to start\n");
    }


  printf("Waiting for de-initialization!\n");
  while ( RoboKernelAlive()==1 )
    {
      usleep(100);
    }

  clock_gettime(CLOCK_REALTIME,&end);

  fprintf(stderr,"de-initialization complete, complete halt of robovision!\n");

  return 0;
}
