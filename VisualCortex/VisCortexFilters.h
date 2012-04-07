#ifndef VISUAL_CORTEX_FILTERS_H
#define VISUAL_CORTEX_FILTERS_H

#include "VisionMemory.h"
#include <math.h>

void GenerateCompressHistogramOfImage(unsigned char * input_img,unsigned short * output_img,unsigned int block_x,unsigned int block_y);
unsigned int HistogramPatch(struct Histogram *hist_data,struct VideoRegister * reg,unsigned int px,unsigned int py,unsigned int patch_x,unsigned int patch_y);
unsigned int inline CountEdges(unsigned int edges_required_to_process , unsigned int x , unsigned int y,unsigned int size_x , unsigned int size_y,unsigned char * edge_array);
unsigned int inline PixelsOverThresholdSetAsOne(struct VideoRegister * reg,unsigned int threshold);


int Sobel(struct VideoRegister * image_reg);
int SobelFromSource(struct VideoRegister * source_reg,struct VideoRegister * target_reg);
int SecondDerivativeIntensitiesFromSource(struct VideoRegister * source_reg,struct VideoRegister * target_reg);
void ReducePalette(struct VideoRegister * reg,int new_palette);
void KillDifferentPixels(struct VideoRegister * reg,unsigned char R,unsigned char G,unsigned char B,unsigned char threshold);
void KillPixelsBelow(struct VideoRegister * reg,int threshold);
void KillPixelsBetween(struct VideoRegister * reg,int low_threshold,int high_threshold);
void Kill3PixelsBelow(struct VideoRegister * reg,int threshold);
void Monochrome(struct VideoRegister * reg);
void MonochromeL(struct VideoRegister * reg);
void CollapseLargeRegister(struct LargeVideoRegister * reg);
void CollapseRegister(struct VideoRegister * reg);
int GaussianBlur(struct VideoRegister * image_reg);
int GaussianBlurFromSource(struct VideoRegister * source_reg,struct VideoRegister * target_reg);
void PrepareCleanSobeledGaussianAndDerivative(struct VideoRegister * rgb_reg,struct VideoRegister * target_sobel_reg,struct VideoRegister * target_derivative_reg,unsigned int kill_lower_edges_threshold,unsigned int kill_higher_edges_threshold);
int CalibrateImage(struct VideoRegister * rgb_reg,struct VideoRegister * rgb_calibrated_reg,unsigned int * M);

//BOOLEAN FindGoodTrackingPoints(unsigned char * edge_source,unsigned char * target,int image_x,int image_y,BOOLEAN monochrome);
int PixelIsBright(unsigned char * rgb_image,unsigned int memplace_3_byte);
int PixelBelongsWithSurroundingPixels(unsigned char * rgb_image,unsigned int x,unsigned int y,unsigned int memplace);
//unsigned int FloodPixel(unsigned char * picture_array,unsigned int point_y,unsigned int point_y,unsigned int size_x,unsigned int size_y);


#endif
