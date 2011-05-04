#ifndef VISCORTEXCONVOLUTIONFILTERS_H_INCLUDED
#define VISCORTEXCONVOLUTIONFILTERS_H_INCLUDED

int ConvolutionFilter9_1Byte(unsigned int monochrome_reg,unsigned int target_reg,signed char * table);
int ConvolutionFilter9_3Byte(unsigned int rgb_reg,unsigned int target_reg,signed char * table);

#endif // CONVOLUTIONFILTERS_H_INCLUDED
