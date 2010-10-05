#ifndef PATHPLANNINGHELPER_H_INCLUDED
#define PATHPLANNINGHELPER_H_INCLUDED

#include "MasterpathPlanning.h"

int PathPlanCore_AddObstacleRadious(struct Map * themap,unsigned int x,unsigned int y,unsigned int mem_ptr,unsigned int safety_radious);
int PathPlanCore_RemoveObstacleRadious(struct Map * themap,unsigned int x,unsigned int y,unsigned int mem_ptr,unsigned int safety_radious);

unsigned int FillResultPath(struct NodeData * world_matrix,unsigned int world_x,struct TraceNode * resultlist,unsigned int result_size,unsigned int start_node,unsigned int end_node);
int ClearFindPathState(struct Map * themap,struct Path * route);

int VerifySorting(struct NodeRef *arr, int elements);

#endif // PATHPLANNINGHELPER_H_INCLUDED
