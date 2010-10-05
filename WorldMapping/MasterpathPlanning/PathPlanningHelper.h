#ifndef PATHPLANNINGHELPER_H_INCLUDED
#define PATHPLANNINGHELPER_H_INCLUDED

#include "MasterpathPlanning.h"

unsigned int FillResultPath(struct NodeData * world_matrix,unsigned int world_x,struct TraceNode * resultlist,unsigned int result_size,unsigned int start_node,unsigned int end_node);
int clear_open_nodes(struct Map * themap);

#endif // PATHPLANNINGHELPER_H_INCLUDED
