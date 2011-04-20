#ifndef DISPARITY_DEPTH_MAP_H
#define DISPARITY_DEPTH_MAP_H

#include "VisionMemory.h"
unsigned int HistogramPatch(struct Histogram *hist_data,unsigned char *img,unsigned int px,unsigned int py,unsigned int patch_x,unsigned int patch_y);

void DepthMapFull  ( unsigned int left_view_reg,
                     unsigned int right_view_reg,
                     unsigned int left_depth_reg,
                     unsigned int right_depth_reg,
                     unsigned int image_x,
                     unsigned int image_y,
                     unsigned char clear_depth_arrays /*Cleaning the depth arrays takes a little longer :) */
                    );

unsigned char InstantMovementDisparity(unsigned short *left_depth,unsigned short *right_depth);
unsigned char DisparityMapNormalize(unsigned short *left_depth,unsigned short *right_depth);
int DepthMapToVideo(unsigned int depth_reg,unsigned int vid_reg);

void EnhanceDepthMapWithEdges(unsigned char * rgb_image,unsigned short *full_depth_map,unsigned char * edges_map,unsigned int image_x,unsigned int image_y);
unsigned int EnhanceDepthMapFillHoles(unsigned char * rgb_image,unsigned short *full_depth_map,unsigned int image_x,unsigned int image_y);
#endif
