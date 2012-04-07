#ifndef VISCORTEXHEURISTICS_H_INCLUDED
#define VISCORTEXHEURISTICS_H_INCLUDED

#include "VisCortexHeuristics.h"
#include "VisionMemory.h"

char PixelsSameColour(struct VideoRegister * rgb_reg,unsigned int memplace1,unsigned int memplace2);
char PixelIsBright(struct VideoRegister * rgb_reg,unsigned int memplace_3_byte);
char PixelBelongsWithSurroundingPixels(struct VideoRegister * rgb_reg,unsigned int x,unsigned int y,unsigned int memplace);


#endif // VISCORTEXHEURISTICS_H_INCLUDED
