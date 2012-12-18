#ifndef INTEGRALIMAGECONVERSION_H_INCLUDED
#define INTEGRALIMAGECONVERSION_H_INCLUDED

#include "VisionMemory.h"

inline void GenerateCompressHistogramOfImage_AddUpHorizontal(unsigned char * input_img,unsigned short * output_img,unsigned int block_x,unsigned int block_y);
inline void GenerateCompressHistogramOfImage_AddUpVertical(unsigned short * output_img,unsigned int block_x,unsigned int block_y);
void GenerateCompressHistogramOfImage(unsigned char * input_img,unsigned short * output_img,unsigned int block_x,unsigned int block_y);
//unsigned int CompressedHistogramPatch(unsigned short * compimg,struct Histogram * hist , unsigned int x,unsigned int y);

unsigned int inline GetRegisterPatchPresenceSum(int comp_register, unsigned int x , unsigned int y,unsigned int width , unsigned int  height,unsigned int depth,unsigned int threshold);

int TestIntegralImaging();

unsigned int inline GetCompressedRegisterPatchSum1Byte(int comp_register,int x,int y,int width,int height);
unsigned int GetCompressedRegisterPatchSum(int comp_register,int x,int y,int width,int height);
unsigned int CompressRegister(struct VideoRegister *  input,struct ExtraLargeVideoRegister *  output);




inline static unsigned int CompressedHistogramPatch(unsigned short * compimg,struct Histogram * hist , unsigned int x,unsigned int y)
{
  unsigned short * histpx;
  unsigned int ptr_push = metrics[RESOLUTION_X_3_BYTE]*y+(x*3);
  histpx = compimg + ptr_push;
  hist->median_r = *histpx;
  histpx++;
  hist->median_g = *histpx;
  histpx++;
  hist->median_b = *histpx;
  return 1;
};

#endif // INTEGRALIMAGECONVERSION_H_INCLUDED
