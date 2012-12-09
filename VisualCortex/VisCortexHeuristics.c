#include "VisCortexHeuristics.h"
#include "Precalculations.h"
#include "VisionMemory.h"





char PixelsSameColour(struct VideoRegister * rgb_reg,unsigned int memplace1,unsigned int memplace2)
{
  unsigned char * rgb_image = rgb_reg->pixels;
  char res=0;
  if ( ABSDIFF(rgb_image[memplace1],rgb_image[memplace2]) < 20 ) ++res;
  if ( ABSDIFF(rgb_image[memplace1+1],rgb_image[memplace2+1]) < 20 ) ++res;
  if ( ABSDIFF(rgb_image[memplace1+2],rgb_image[memplace2+2]) < 20 ) ++res;

  if ( res == 3 ) res=1; else
                  res=0;

  return res;
}

char PixelIsBright(struct VideoRegister * rgb_reg,unsigned int memplace_3_byte)
{
  unsigned char * rgb_image = rgb_reg->pixels;
  char res=0;
  if ( ABSDIFF(rgb_image[memplace_3_byte],rgb_image[255]) < 150 ) ++res;
  if ( ABSDIFF(rgb_image[memplace_3_byte],rgb_image[255]) < 150 ) ++res;
  if ( ABSDIFF(rgb_image[memplace_3_byte],rgb_image[255]) < 150 ) ++res;

  if ( res == 3 ) res=1; else
                  res=0;

  return res;
}


char PixelBelongsWithSurroundingPixels(struct VideoRegister * rgb_reg,unsigned int x,unsigned int y,unsigned int memplace)
{
    char resemblence=0;
    unsigned int memplace2;
    if ( (x>1)&&(y>1)&&(y<metrics[RESOLUTION_Y])&&(x<metrics[RESOLUTION_X]) )
     {
       unsigned int SHIFT_3_BYTE = metrics[RESOLUTION_X_3_BYTE];

       memplace2=memplace-1; // LEFT
       if ( PixelsSameColour(rgb_reg,memplace,memplace2)==1 ) ++resemblence;

       memplace2-=SHIFT_3_BYTE; // LEFT UP
       if ( PixelsSameColour(rgb_reg,memplace,memplace2)==1 ) ++resemblence;

       ++memplace2; // UP
       if ( PixelsSameColour(rgb_reg,memplace,memplace2)==1 ) ++resemblence;

       ++memplace2; // RIGHT UP
       if ( PixelsSameColour(rgb_reg,memplace,memplace2)==1 ) ++resemblence;

       memplace2+=SHIFT_3_BYTE; // RIGHT
       if ( PixelsSameColour(rgb_reg,memplace,memplace2)==1 ) ++resemblence;

       memplace2+=SHIFT_3_BYTE;// RIGHT DOWN
       if ( PixelsSameColour(rgb_reg,memplace,memplace2)==1 ) ++resemblence;

       --memplace2; // DOWN
       if ( PixelsSameColour(rgb_reg,memplace,memplace2)==1 ) ++resemblence;

       --memplace2; // LEFT DOWN
       if ( PixelsSameColour(rgb_reg,memplace,memplace2)==1 ) ++resemblence;
     }

    if ( resemblence>7) return 1;

    return 0;
}
