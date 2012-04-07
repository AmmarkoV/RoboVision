#ifndef DISPARITY_DEPTH_MAP_H
#define DISPARITY_DEPTH_MAP_H

#include "VisionMemory.h"

unsigned int  DepthMapFull  ( unsigned int left_view_reg,
                              unsigned int right_view_reg,
                              unsigned int left_depth_reg,
                              unsigned int right_depth_reg,
                              unsigned char clear_and_calculate /*Cleaning the depth arrays takes a little longer :) */
                            );

int ExecuteDisparityMappingPyramid();
unsigned int ExecuteDisparityMappingOpenCV();
#endif
