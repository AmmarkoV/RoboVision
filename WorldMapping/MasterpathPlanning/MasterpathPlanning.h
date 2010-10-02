#ifndef MASTERPATHPLANNING_H_INCLUDED
#define MASTERPATHPLANNING_H_INCLUDED

struct Map
{
   unsigned int world_size_x,world_size_y,world_unit_in_cm;


};


struct Path
{
   unsigned int world_size_x,world_size_y,world_unit_in_cm;


};

struct Map * CreateMap(unsigned int world_size_x,unsigned int world_size_y);

#include "MasterpathPlanning.h"
#include <stdio.h>

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

int FindPath(struct Map * world,unsigned int agentnum) ;
int FindPathToPosition(struct Map * world,unsigned int agentnum,unsigned int x,unsigned int y) ;
int FindSponteneousPath(struct Map * world,unsigned int agentnum,struct Path * thepath,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2) ;

int AddLocation(struct Map * world,char * name,unsigned int pos_x,unsigned int pos_y) ;
int DeleteLocation_Number(struct Map * world,unsigned int position_id) ;
int DeleteLocation(struct Map * world,char * name) ;

int ExtractRouteToLogo(struct Map * world,struct Path * thepath,char * filename) ;
int GetRoutePoints(struct Map * world,struct Path * thepath) ;
int GetRouteWaypoint(struct Map * world,struct Path * thepath,unsigned int *x,unsigned int *y) ;
#endif // MASTERPATHPLANNING_H_INCLUDED
