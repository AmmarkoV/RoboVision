#ifndef STATISTICSGENERATION_H_INCLUDED
#define STATISTICSGENERATION_H_INCLUDED

int UpdateFrameProcessingRateStatistics(unsigned int last_frame_microseconds, float last_fps);
int UpdateDisparityMapStatistics(unsigned int time_elapsed,unsigned int coverage,unsigned int comparisons_small,unsigned int comparisons_medium,unsigned int comparisons_large);

#endif // STATISTICSGENERATION_H_INCLUDED
