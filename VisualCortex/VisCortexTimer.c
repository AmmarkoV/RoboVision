#include "VisCortexTimer.h"
#include <sys/time.h>

struct timeval starttime,endtime,timediff;


long timeval_diff ( struct timeval *difference, struct timeval *end_time, struct timeval *start_time )
{
   struct timeval temp_diff;

   if(difference==0) { difference=&temp_diff; }

  difference->tv_sec =end_time->tv_sec -start_time->tv_sec ;
  difference->tv_usec=end_time->tv_usec-start_time->tv_usec;

  /* Using while instead of if below makes the code slightly more robust. */

  while(difference->tv_usec<0)
  {
    difference->tv_usec+=1000000;
    difference->tv_sec -=1;
  }

  return 1000000LL*difference->tv_sec+ difference->tv_usec;

}

void StartTimer()
{
  gettimeofday(&starttime,0x0);
}

int EndTimer()
{
  gettimeofday(&endtime,0x0);
  return timeval_diff(&timediff,&endtime,&starttime);
}
