#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../RoboKernel/RoboKernel.h"
#include <time.h>
#include <string.h>

using namespace std;

int main()
{
    struct timespec start,end;
    clock_gettime(CLOCK_REALTIME,&start);

    printf("Starting RoboKernel!\n");
    if ( StartRoboKernel() )
    {
      usleep( 5000 * 1000 ); /* Allow 5 seconds for guarddog scripts , etc. to startup*/ 
    
      char buffer[2048]={0};
      char *cptr;

      fprintf(stderr,"RoboVisionCLI is now running in the background\n"); 
      do {
          printf("Enter command ( exit stops RoboVision ) :\n");
          cptr = fgets(buffer,2048, stdin);
          if ( cptr != 0 )
           {
             IssueCommand(cptr,0,0,(char *)"CLI");
           }

         } while ( (strncmp(cptr,"exit",4)!=0 ) && (RoboKernelAlive()==1) );


      printf("Stoping RoboKernel!\n");
      StopRoboKernel();
    } 
      else 
    { 
      fprintf(stderr,"RoboKernel failed to start\n"); 
    }
    

    printf("Waiting for de-initialization!\n");
    while ( RoboKernelAlive()==1 ) { usleep(100); }

    clock_gettime(CLOCK_REALTIME,&end);

    fprintf(stderr,"de-initialization complete, complete halt of robovision!\n");

    return 0;
}
