#include "StatisticsGeneration.h"

#include "VisionMemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int UpdateFrameProcessingRateStatistics(unsigned int last_frame_microseconds, float last_fps)
{
    FILE *fd=0;
    fd = fopen("../DataSets/Statistics/hypervisor_performance.dat","a");

    if (fd!=0)
	{
	  float uptime_in_seconds = (float) (TIME_INC-TIME_START) / 1000;
      fprintf(fd, "%0.2f %u %0.2f\n", uptime_in_seconds , last_frame_microseconds, last_fps);

     fflush(fd);
     fclose(fd);
     return 1;
	}

  return 1;
}


int UpdateDisparityMapStatistics(unsigned int time_elapsed,unsigned int coverage,unsigned int too_close_coverage,unsigned int comparisons_small,unsigned int comparisons_medium,unsigned int comparisons_large)
{
    FILE *fd=0;
    fd = fopen("../DataSets/Statistics/hypervisor_disparity_mapping_performance.dat","a");

    if (fd!=0)
	{
	  float uptime_in_seconds = (float) (TIME_INC-TIME_START) / 1000;
      fprintf(fd, "%0.2f %u %u %u %u %u\n", uptime_in_seconds , time_elapsed , coverage , comparisons_small, comparisons_medium, comparisons_large);

     fflush(fd);
     fclose(fd);
     return 1;
	}

  return 1;
}
