#ifndef VISCORTEXCONVOLUTIONFILTERSOPTIMIZED_H_INCLUDED
#define VISCORTEXCONVOLUTIONFILTERSOPTIMIZED_H_INCLUDED
int ConvolutionFilter9_1ByteOptimized(unsigned int monochrome_reg,unsigned int target_reg,signed char * table,signed int divisor);
int ConvolutionFilter9_3ByteOptimized(unsigned int rgb_reg,unsigned int target_reg,signed char * table,signed int divisor);
int ConvolutionFilter9_AutoByteOptimized(unsigned int rgb_reg,unsigned int target_reg,signed char * table,signed int divisor);
#endif // CONVOLUTIONFILTERS_H_INCLUDED


