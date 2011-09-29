#ifndef VISCORTEXTIMER_H_INCLUDED
#define VISCORTEXTIMER_H_INCLUDED



enum VisCortTimerList
{
   TIMER_PROCESSING_DELAY = 0 ,
   TIMER_DEPTH_MAP_DELAY ,
   TIMER_DEPTH_MAP_AVERAGE_DELAY ,

   TOTAL_TIMERS
};


void StartTimer( unsigned int timer_num );
int EndTimer( unsigned int timer_num );

void VisCortxMillisecondsSleep(unsigned int milliseconds);
void VisCortxMicrosecondsSleep(unsigned int microseconds);
#endif // VISCORTEXTIMER_H_INCLUDED
