#ifndef PATHPLANNINGCORE_H_INCLUDED
#define PATHPLANNINGCORE_H_INCLUDED

#include "MasterpathPlanning.h"


int PathPlanCore_CleanMap(struct Map * themap);
int PathPlanCore_AddObstacleRadious(struct Map * themap,unsigned int x,unsigned int y,unsigned int mem_ptr,unsigned int safety_radious);

#endif // PATHPLANNINGCORE_H_INCLUDED
