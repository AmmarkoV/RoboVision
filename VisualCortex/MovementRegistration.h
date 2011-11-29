#ifndef MOVEMENT_REGISTRATION_H
#define MOVEMENT_REGISTRATION_H

#include "VisionMemory.h"

unsigned int GetMovementAtBlock(unsigned int movement,struct ImageRegion * source_rgn);
unsigned int PrepareRegisterMovementsRegisters(unsigned char left_eye);
unsigned int RegisterMovementsInternal(BOOLEAN lefteye,unsigned int last_source_block_reg,unsigned int source_block_reg,unsigned int mobility_array_reg,unsigned int movement_target_grouped);
unsigned int RegisterMovements(unsigned char left_eye);

int IfMovementRegistrationEnabledAndOverOrDisabled(int number_over);
int MoveRegEnabledAndNoMovement(int number_over);
#endif
