#ifndef NORMALIZEPATH_H_INCLUDED
#define NORMALIZEPATH_H_INCLUDED

#include "AStarCompute.h"
TraceNode *  GetANormalizedLineFromNodes(TraceNode * rawnodes,unsigned int rawnodes_size,unsigned int &str8nodes_size);
unsigned char ClearLinePath(NodeData * world,NodeNeighborsCount *world_neighbors,unsigned int world_x,unsigned int world_size,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void GetTheShortestNormalizedLineFromNodes(NodeData * world,NodeNeighborsCount *world_neighbors,unsigned int world_x,unsigned int world_size,TraceNode * str8nodes,unsigned int &str8nodes_size);

#endif // NORMALIZEPATH_H_INCLUDED
