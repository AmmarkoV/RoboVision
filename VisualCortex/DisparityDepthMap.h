#ifndef DISPARITY_DEPTH_MAP_H
#define DISPARITY_DEPTH_MAP_H

#include "VisionMemory.h"


struct DisparityMappingContext
{
  unsigned int left_view_reg;
  unsigned int right_view_reg;
  unsigned int left_depth_reg;
  unsigned int right_depth_reg;
  unsigned char clear_and_calculate;

  unsigned int detail;
  unsigned int comparison_threshold;
  unsigned int patch_comparison_edges_percent_required;

  unsigned int vertical_buffer;
  unsigned int horizontal_buffer;
};




int ExecuteDisparityMappingPyramid();
unsigned int ExecuteDisparityMappingOpenCV();
#endif
