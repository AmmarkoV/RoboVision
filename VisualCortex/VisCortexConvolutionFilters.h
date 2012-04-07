#ifndef VISCORTEXCONVOLUTIONFILTERS_H_INCLUDED
#define VISCORTEXCONVOLUTIONFILTERS_H_INCLUDED

#include "VisionMemory.h"

int ConvolutionFilter9_1Byte(struct VideoRegister * monochrome_reg,struct VideoRegister * target_reg,signed char * table,signed int divisor);
int ConvolutionFilter9_3Byte(struct VideoRegister * rgb_reg,struct VideoRegister * target_reg,signed char * table,signed int divisor);

#endif // CONVOLUTIONFILTERS_H_INCLUDED
