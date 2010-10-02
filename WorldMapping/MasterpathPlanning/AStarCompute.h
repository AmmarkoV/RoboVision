#ifndef ASTARCOMPUTE_H
#define ASTARCOMPUTE_H
#include <sys/time.h>
#include <unistd.h>


struct NodeData    // Declare NODE struct type
  {
    unsigned int parent_node;
    unsigned char arrived_direction;
    unsigned int score;
    bool opened;
    bool unpassable;
    unsigned short node_penalty;
  };

struct NodeRef
  {
    // Declare NODE struct type
    unsigned int node;
    unsigned int score;
  };

struct NodeNeighborsCount
  {
    unsigned int total;
  };

struct TraceNode
  {
    // Declare NODE struct type
    unsigned int nodex,nodey;
    unsigned int score;
  };

class AStarCompute
{
    public:
        AStarCompute(NodeData * world_IN,unsigned int world_x_IN,unsigned int world_y_IN,NodeNeighborsCount *world_neighbors_IN,NodeRef *openlist_IN,unsigned int openlist_size_IN);
        virtual ~AStarCompute();
        int FindPath(unsigned int x1,unsigned int y1,unsigned int start_direction,unsigned int oursize,unsigned int x2,unsigned int y2,unsigned int timelimit_ms);
        unsigned int CheckFindPathCommandSanity();
        unsigned int GetRoutePoint(unsigned char level,unsigned int pt_num,unsigned int &from_x,unsigned int &from_y);
    protected:
    private:
    inline unsigned int GetNextNode();
    inline int ProcessNode(unsigned int node_to_proc);
    void inline OpenNode(unsigned int the_node);

    //          ||
    // VARS     \/
    NodeNeighborsCount * world_neighbors;
    NodeData * world;
    unsigned int world_size,world_x,world_y;


    NodeRef *openlist;
    unsigned int openlist_size,openlist_top;


    TraceNode *resultlist;
    unsigned int resultlist_size;

    TraceNode *str8_resultlist;
    unsigned int str8_resultlist_size;

    unsigned int source,source_x,source_y;
    unsigned int target,target_x,target_y,solutions_gathered;

    unsigned char done,out_of_bounds;

    unsigned int cur_x,cur_y,proc_node,last_node,node_direction;

};

#endif // ASTARCOMPUTE_H
