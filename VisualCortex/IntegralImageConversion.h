#ifndef INTEGRALIMAGECONVERSION_H_INCLUDED
#define INTEGRALIMAGECONVERSION_H_INCLUDED

#include "VisionMemory.h"

inline void GenerateCompressHistogramOfImage_AddUpHorizontal(unsigned char * input_img,unsigned short * output_img,unsigned int block_x,unsigned int block_y);
inline void GenerateCompressHistogramOfImage_AddUpVertical(unsigned short * output_img,unsigned int block_x,unsigned int block_y);
void GenerateCompressHistogramOfImage(unsigned char * input_img,unsigned short * output_img,unsigned int block_x,unsigned int block_y);
unsigned int CompressedHistogramPatch(unsigned short * compimg,struct Histogram * hist , unsigned int x,unsigned int y);


int TestIntegralImaging();

unsigned int GetCompressedRegisterPatchSum1Byte(int comp_register,int x,int y,int width,int height);
unsigned int GetCompressedRegisterPatchSum(int comp_register,int x,int y,int width,int height);
unsigned int CompressRegister(int input,int output);
unsigned int CompressPresenceRegister(int input,int output,int threshold);

#endif // INTEGRALIMAGECONVERSION_H_INCLUDED
