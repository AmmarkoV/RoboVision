#ifndef MASTERPATHPLANNING_H_INCLUDED
#define MASTERPATHPLANNING_H_INCLUDED

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GUARD_BYTE_VALUE 12345


struct Path
{
    struct TraceNode *resultlist;
    unsigned int resultlist_size;

    struct TraceNode *str8_resultlist;
    unsigned int str8_resultlist_size;

    unsigned int cur_x,cur_y,proc_node,last_node,node_direction;
    unsigned int source,source_x,source_y;
    unsigned int target,target_x,target_y,solutions_gathered;

    unsigned char done,out_of_bounds;
};



struct Actors    // Declare NODE struct type
  {
    float abs_x_pos,abs_y_pos;
    unsigned int current_x_pos,current_y_pos;
    unsigned int target_x_pos,target_y_pos;

    unsigned int reached_target;
    struct Path * target_path;

    int current_heading,target_heading;
  };



struct NodeData    // Declare NODE struct type
  {
    unsigned int parent_node;
    unsigned char arrived_direction;
    unsigned int score;
    unsigned char opened;
    unsigned char unpassable;
    unsigned short node_penalty;
  };


struct NodeRef
  {
    // Declare NODE struct type for open list
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

struct Map
{
   unsigned int world_size_x,world_size_y,world_unit_in_cm;

   unsigned int world_total_size;
   struct NodeData  * world;

   unsigned int GUARD_BYTE;
};






struct Map * CreateMap(unsigned int world_size_x,unsigned int world_size_y);

/*      --------------------------------------------
               MAP STATE INITIALIZATION START
        -------------------------------------------- */
struct Map * CreateMap(unsigned int world_size_x,unsigned int world_size_y);
struct Map * LoadMap(char * filename) ;
int SaveMap(struct Map * world) ;
int DeleteMap(struct Map * world) ;
int SetMapUnit_In_cm(struct Map * world,unsigned int cm_per_unit) ;
int ObstacleExists(struct Map * world,unsigned int x,unsigned int y) ;
int SetObstacle(struct Map * world,unsigned int x,unsigned int y,unsigned int safety_radious) ;
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
int SetAgentSize(struct Map * world,unsigned int agentnum,unsigned int x_size,unsigned int y_size)  ;
int SetAgentHeading(struct Map * world,unsigned int agentnum,unsigned int heading) ;
unsigned int GetAgentHeading(struct Map * world,unsigned int agentnum) ;
int SetAgentLocation(struct Map * world,unsigned int agentnum,unsigned int x,unsigned int y) ;
int SetAgentLocationName(struct Map * world,unsigned int agentnum,char * name) ;
int GetAgentLocation(struct Map * world,unsigned int agentnum,unsigned int * x,unsigned int * y) ;
int SetAgentTargetLocation(struct Map * world,unsigned int agentnum,unsigned int x,unsigned int y) ;
int SetAgentTargetLocationName(struct Map * world,unsigned int agentnum,char * name) ;
int MoveAgentForward(struct Map * world,unsigned int agentnum,int leftwheel_cm,int rightwheel_cm) ;
int SetObstacleSensedbyAgent(struct Map * world,unsigned int agentnum,int ultrasonic_left_cm,int ultrasonic_right_cm) ;
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
int FindPath(struct Map * world,unsigned int agentnum) ;
int FindPathToPosition(struct Map * world,unsigned int agentnum,unsigned int x,unsigned int y) ;
int FindSponteneousPath(struct Map * world,unsigned int agentnum,struct Path * thepath,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2) ;
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
int AddLocation(struct Map * world,char * name,unsigned int pos_x,unsigned int pos_y) ;
int DeleteLocation_Number(struct Map * world,unsigned int position_id) ;
int DeleteLocation(struct Map * world,char * name) ;
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
int ExtractRouteToLogo(struct Map * world,struct Path * thepath,char * filename) ;
int GetRoutePoints(struct Map * world,struct Path * thepath) ;
int GetRouteWaypoint(struct Map * world,struct Path * thepath,unsigned int *x,unsigned int *y) ;

#ifdef __cplusplus
}
#endif

#endif // MASTERPATHPLANNING_H_INCLUDED
