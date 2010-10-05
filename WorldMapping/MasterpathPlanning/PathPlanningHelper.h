#ifndef PATHPLANNINGHELPER_H_INCLUDED
#define PATHPLANNINGHELPER_H_INCLUDED

#include "MasterpathPlanning.h"


enum Directions
{
  UP_LEFT=1,
  UP,
  UP_RIGHT,
  LEFT,
  MIDDLE,
  RIGHT,
  DOWN_LEFT,
  DOWN,
  DOWN_RIGHT,
  /*--------------*/
  TOTAL_DIRECTIONS
};

int PathPlanCore_AddObstacleRadious(struct Map * themap,unsigned int x,unsigned int y,unsigned int mem_ptr,unsigned int safety_radious);
int PathPlanCore_RemoveObstacleRadious(struct Map * themap,unsigned int x,unsigned int y,unsigned int mem_ptr,unsigned int safety_radious);

unsigned int FillResultPath(struct NodeData * world_matrix,unsigned int world_x,struct TraceNode * resultlist,unsigned int result_size,unsigned int start_node,unsigned int end_node);
int ClearFindPathState(struct Map * themap,struct Path * route);

int VerifySorting(struct NodeRef *arr, int elements);

unsigned int GetCompressedHeadingFromRealHeading(float heading);

#endif // PATHPLANNINGHELPER_H_INCLUDED
