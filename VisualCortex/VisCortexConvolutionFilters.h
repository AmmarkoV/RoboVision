#ifndef VISCORTEXCONVOLUTIONFILTERS_H_INCLUDED
#define VISCORTEXCONVOLUTIONFILTERS_H_INCLUDED

int ConvolutionFilter9_1Byte(unsigned int monochrome_reg,unsigned int target_reg,signed char * table,signed int divisor);
int ConvolutionFilter9_3Byte(unsigned int rgb_reg,unsigned int target_reg,signed char * table,signed int divisor);

#endif // CONVOLUTIONFILTERS_H_INCLUDED
