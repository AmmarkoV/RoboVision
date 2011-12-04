#ifndef MOVEMENT_REGISTRATION_H
#define MOVEMENT_REGISTRATION_H

#include "VisionMemory.h"

unsigned int GetMovementAtBlock(unsigned int movement,struct ImageRegion * source_rgn);
unsigned int PrepareRegisterMovementsRegisters(unsigned char left_eye);
unsigned int RegisterMovementsInternal(unsigned char lefteye,unsigned int last_reg,unsigned int current_reg,unsigned int movement_target,unsigned int generate_grouped,unsigned int movement_target_grouped);
unsigned int RegisterMovements(unsigned char left_eye);

int IfMovementRegistrationEnabledAndOverOrDisabled(int number_over);
int MoveRegEnabledAndNoMovement(int number_over);
#endif
