#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../RoboKernel/RoboKernel.h"
#include <time.h>

using namespace std;

int main()
{
    struct timespec start,end;
    clock_gettime(CLOCK_REALTIME,&start);

    printf("Starting RoboKernel!\n");
    StartRoboKernel();

    char c;
    printf("q quits robovision !!\n");
    do { c=getchar(); } while ( (c != 'q')&&(RoboKernelAlive()==1) );


    printf("Stoping RoboKernel!\n");
    StopRoboKernel();


    printf("Waiting for de-initialization!\n");
    while ( RoboKernelAlive()==1 ) { usleep(100); }

    clock_gettime(CLOCK_REALTIME,&end);

    printf("de-initialization complete, complete halt of robovision!\n");

    return 0;
}
