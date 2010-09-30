#ifndef MOVEMENT_REGISTRATION_H
#define MOVEMENT_REGISTRATION_H

#include "VisionMemory.h"

unsigned int PatchDistanceCompare_RGB(struct ImageRegion *block_1,struct ImageRegion *block_2,unsigned char *patchdata1,unsigned char *patchdata2,unsigned long max_distance,unsigned long *fast_edges);
unsigned int PatchHasMovement(struct ImageRegion *block_1,unsigned char *patchdata1,unsigned char *patchdata_compressed);
unsigned int RegisterMovements(BOOLEAN lefteye,unsigned char *last_source_block,unsigned char *source_block,unsigned char *background_block,unsigned char* target_difference,unsigned char *mobility_array);


#endif
