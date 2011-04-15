#ifndef PATCHSHAPES_H_INCLUDED
#define PATCHSHAPES_H_INCLUDED

struct PatchSignature
{
   unsigned short segment[20];
};

unsigned int GetPatchSignature(unsigned char * patch,unsigned int x,unsigned int y , unsigned int patch_x,unsigned int patch_y , struct PatchSignature * result);

#endif // PATCHSHAPES_H_INCLUDED
