#ifndef PATCHCOMPARISON_H_INCLUDED
#define PATCHCOMPARISON_H_INCLUDED

#include "VisionMemory.h"

int ComparePatchesUsingHistogram(int hist_reg_left,int hist_reg_right,unsigned int *source_x1,unsigned int *source_y1,unsigned int *target_x1,unsigned int *target_y1);

unsigned int ComparePatches(       struct ImageRegion * source_block,    struct ImageRegion * target_block,
                                   unsigned char *rgb1,                  unsigned char *rgb2,
                                   unsigned char *sobel1,                unsigned char *sobel2,
                                   unsigned char *secondderiv1,          unsigned char *secondderiv2,
                                   unsigned char *movement1,             unsigned char *movement2,
                                   unsigned int width,                   unsigned int height,

                                   unsigned int image_x,unsigned int image_y,
                                   unsigned int best_result_yet
                                  );

#endif // PATCHCOMPARISON_H_INCLUDED
