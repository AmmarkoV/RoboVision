#ifndef VISCORTEXCONVOLUTIONFILTERSOPTIMIZED_H_INCLUDED
#define VISCORTEXCONVOLUTIONFILTERSOPTIMIZED_H_INCLUDED
#include "VisionMemory.h" 
int ConvolutionFilter9_1ByteOptimized(struct VideoRegister * monochrome_reg,struct VideoRegister * target_reg,signed char * table,signed int divisor);
int ConvolutionFilter9_3ByteOptimized(struct VideoRegister * rgb_reg,struct VideoRegister * target_reg,signed char * table,signed int divisor);
int ConvolutionFilter9_AutoByteOptimized(struct VideoRegister * rgb_reg,struct VideoRegister * target_reg,signed char * table,signed int divisor);
#endif // CONVOLUTIONFILTERS_H_INCLUDED


