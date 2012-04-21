#ifndef OBJECTRECOGNITION_H_INCLUDED
#define OBJECTRECOGNITION_H_INCLUDED

#include "VisionMemory.h"

unsigned int RememberRegisterPart(struct VideoRegister * frame , unsigned int x , unsigned int y , unsigned int width , unsigned int height , char * str , unsigned int length);

#endif // OBJECTRECOGNITION_H_INCLUDED
