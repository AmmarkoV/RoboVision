#ifndef VISUAL_CORTEX_FILTERS_H
#define VISUAL_CORTEX_FILTERS_H

#include "VisionMemory.h"
#include <math.h>

void GenerateCompressHistogramOfImage(unsigned char * input_img,unsigned short * output_img,unsigned int block_x,unsigned int block_y);
unsigned int HistogramPatch(struct Histogram *hist_data,unsigned char *img,unsigned int px,unsigned int py,unsigned int patch_x,unsigned int patch_y);
unsigned int inline CountEdges(unsigned int edges_required_to_process , unsigned int x , unsigned int y,unsigned int size_x , unsigned int size_y,unsigned char * edge_array);
unsigned int inline PixelsOverThresholdSetAsOne(int image_reg,unsigned int threshold);

int Sobel(unsigned int image_reg);
int SobelFromSource(unsigned int source_reg,unsigned int target_reg);
int SecondDerivativeIntensitiesFromSource(unsigned int source_reg,unsigned int target_reg);
void ReducePalette(unsigned int image_reg,int new_palette);
void KillDifferentPixels(unsigned char * image,int image_x,int image_y,unsigned char R,unsigned char G,unsigned char B,unsigned char threshold);
void KillPixelsBelow(unsigned int image_reg,int threshold);
void KillPixelsBetween(unsigned int image_reg,int low_threshold,int high_threshold);
void Kill3PixelsBelow(unsigned int image_reg,int threshold);
void Monochrome(unsigned char * input_frame,int image_x,int image_y);
void MonochromeL(unsigned char * input_frame,int image_x,int image_y);
int GaussianBlur(unsigned int image_reg);
int GaussianBlurFromSource(unsigned int source_reg,unsigned int target_reg);
void PrepareCleanSobeledGaussianAndDerivative(unsigned int rgb_image_reg,unsigned int target_sobel_image_reg,unsigned int target_derivative_image_reg,unsigned int kill_lower_edges_threshold,unsigned int kill_higher_edges_threshold);
int CalibrateImage(unsigned int rgb_image,unsigned int rgb_calibrated,unsigned int * M);

//BOOLEAN FindGoodTrackingPoints(unsigned char * edge_source,unsigned char * target,int image_x,int image_y,BOOLEAN monochrome);
int PixelIsBright(unsigned char * rgb_image,unsigned int memplace_3_byte);
int PixelBelongsWithSurroundingPixels(unsigned char * rgb_image,unsigned int x,unsigned int y,unsigned int memplace);
//unsigned int FloodPixel(unsigned char * picture_array,unsigned int point_y,unsigned int point_y,unsigned int size_x,unsigned int size_y);


#endif
