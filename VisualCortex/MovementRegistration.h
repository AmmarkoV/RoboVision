#ifndef MOVEMENT_REGISTRATION_H
#define MOVEMENT_REGISTRATION_H

#include "VisionMemory.h"

unsigned int GetMovementAtBlock(unsigned int movement,struct ImageRegion * source_rgn);
unsigned int RegisterMovements(BOOLEAN lefteye,unsigned int last_source_block_reg,unsigned int source_block_reg,unsigned int mobility_array_reg,unsigned int movement_target_grouped);

#endif
