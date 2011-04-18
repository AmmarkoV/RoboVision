#ifndef VISCORTEXHEURISTICS_H_INCLUDED
#define VISCORTEXHEURISTICS_H_INCLUDED

#include "VisCortexHeuristics.h"


char PixelsSameColour(unsigned char * rgb_image,unsigned int memplace1,unsigned int memplace2);
char PixelIsBright(unsigned char * rgb_image,unsigned int memplace_3_byte);
char PixelBelongsWithSurroundingPixels(unsigned char * rgb_image,unsigned int x,unsigned int y,unsigned int memplace);


#endif // VISCORTEXHEURISTICS_H_INCLUDED
