#ifndef PATCHSHAPES_H_INCLUDED
#define PATCHSHAPES_H_INCLUDED

#include "VisualCortex.h"

unsigned int GetPatchSignature(unsigned char * patch, unsigned int image_x, unsigned int image_y ,  unsigned int x,unsigned int y , unsigned int patch_x,unsigned int patch_y , struct PatchSignature * result);

#endif // PATCHSHAPES_H_INCLUDED
