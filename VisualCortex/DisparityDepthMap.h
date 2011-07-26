#ifndef DISPARITY_DEPTH_MAP_H
#define DISPARITY_DEPTH_MAP_H

#include "VisionMemory.h"
unsigned int HistogramPatch(struct Histogram *hist_data,unsigned char *img,unsigned int px,unsigned int py,unsigned int patch_x,unsigned int patch_y);

void DepthMapFull  ( unsigned int left_view_reg,
                     unsigned int right_view_reg,
                     unsigned int left_depth_reg,
                     unsigned int right_depth_reg,
                     unsigned char clear_and_calculate /*Cleaning the depth arrays takes a little longer :) */
                    );

int ExecuteDisparityMappingPyramid();
#endif
