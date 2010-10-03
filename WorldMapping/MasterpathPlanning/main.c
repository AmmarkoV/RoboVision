#include <stdio.h>
#include <stdlib.h>
#include "MasterpathPlanning.h"
#include "PathPlanningCore.h"

/*      --------------------------------------------
               MAP STATE INITIALIZATION START
        -------------------------------------------- */
struct Map * CreateMap(unsigned int world_size_x,unsigned int world_size_y)
{
    struct Map * newmap=0;
    newmap = (struct Map * ) malloc(sizeof(struct Map));
    newmap->GUARD_BYTE=GUARD_BYTE_VALUE;

        newmap->world_total_size=world_size_x*world_size_y;
        newmap->world = (struct NodeData * ) malloc( sizeof(struct NodeData)*newmap->world_total_size );

    PathPlanCore_CleanMap(newmap);

    return newmap;
}


struct Map * LoadMap(char * filename)
{
    fprintf(stderr,"Function LoadMap is a stub \n");
    return 0;
}


int SaveMap(struct Map * themap)
{
    fprintf(stderr,"Function SaveMap is a stub \n");
    return 0;
}

int MapIsOk(struct Map * themap)
{
    if ( themap == 0 ) { fprintf(stderr,"DeleteMap called for non-existent map\n"); return 0; } else
    if ( themap->GUARD_BYTE!=GUARD_BYTE_VALUE ) { fprintf(stderr,"DeleteMap called for broken map\n"); return 0; }
  return 1;
}

int DeleteMap(struct Map * themap)
{   if (!MapIsOk(themap)) { return 0; }
    themap->GUARD_BYTE=0;

       themap->world_total_size=0;
       free(themap->world); /* <- This function is epic it frees the world :D */

    free(themap);
    return 1;
}

int SetMapUnit_In_cm(struct Map * themap,unsigned int cm_per_unit)
{
    if (!MapIsOk(themap)) { return 0; }
    themap->world_unit_in_cm=cm_per_unit;
    return 1;
}

int ObstacleExists(struct Map * themap,unsigned int x,unsigned int y)
{
   /* Tactic is to return 1 when failing that will effectively disable movement , and is less catastrophic */
   if (!MapIsOk(themap)) { return 1; }
   unsigned int memory_ptr=x+(y*themap->world_size_x);
   if (memory_ptr >= themap->world_total_size ) { return 1; /*Overflow protection */ }

   if ( themap->world[memory_ptr].unpassable ) { return 1; } else
                                               { return 0; }
   return 1;
}

int SetObstacle(struct Map * themap,unsigned int x,unsigned int y,unsigned int safety_radious)
{
   if (!MapIsOk(themap)) { return 0; }
   unsigned int memory_ptr=x+(y*themap->world_size_x);
   if (memory_ptr >= themap->world_total_size ) { return 0; /*Overflow protection */ }

   themap->world[memory_ptr].unpassable=1;
   if (!PathPlanCore_AddObstacleRadious(themap,x,y,memory_ptr,safety_radious) ) { fprintf(stderr,"Could not add radious\n"); }
   fprintf(stderr,"safety_radious is a stub\n");
   return 1;
}
/*      --------------------------------------------
               MAP STATE INITIALIZATION END
        -------------------------------------------- */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/*      --------------------------------------------
               AGENT STATE CHANGING START
        -------------------------------------------- */
int SetAgentSize(struct Map * world,unsigned int agentnum,unsigned int x_size,unsigned int y_size)
{
   fprintf(stderr,"stub called\n");
  return 0;
}

int SetAgentHeading(struct Map * world,unsigned int agentnum,unsigned int heading)
{
   fprintf(stderr,"stub called\n");
  return 0;
}

unsigned int GetAgentHeading(struct Map * world,unsigned int agentnum)
{
    fprintf(stderr,"stub called\n");
  return 0;
}

int SetAgentLocation(struct Map * world,unsigned int agentnum,unsigned int x,unsigned int y)
{
    fprintf(stderr,"stub called\n");
  return 0;
}

int SetAgentLocationName(struct Map * world,unsigned int agentnum,char * name)
{
    fprintf(stderr,"stub called\n");
  return 0;
}

int GetAgentLocation(struct Map * world,unsigned int agentnum,unsigned int * x,unsigned int * y)
{
    fprintf(stderr,"stub called\n");
  return 0;
}


int SetAgentTargetLocation(struct Map * world,unsigned int agentnum,unsigned int x,unsigned int y)
{
    fprintf(stderr,"stub called\n");
  return 0;
}

int SetAgentTargetLocationName(struct Map * world,unsigned int agentnum,char * name)
{
    fprintf(stderr,"stub called\n");
  return 0;
}

int MoveAgentForward(struct Map * world,unsigned int agentnum,int leftwheel_cm,int rightwheel_cm)
{
    fprintf(stderr,"stub called\n");
  return 0;
}

int SetObstacleSensedbyAgent(struct Map * world,unsigned int agentnum,int ultrasonic_left_cm,int ultrasonic_right_cm)
{
    fprintf(stderr,"stub called\n");
  return 0;
}
/*      --------------------------------------------
               AGENT STATE CHANGING END
        -------------------------------------------- */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/*      --------------------------------------------
                     PATH FINDING START
        -------------------------------------------- */
int FindPath(struct Map * world,unsigned int agentnum)
{
  /* Finds Path from position to target location for agent
     it must be first set by SetAgentTargetLocation
  */
  return 0;
}

int FindPathToPosition(struct Map * world,unsigned int agentnum,unsigned int x,unsigned int y)
{
  /* Finds Path from position to target location for agent
     it must be first set by SetAgentTargetLocation
  */
  return 0;
}

int FindSponteneousPath(struct Map * world,unsigned int agentnum,struct Path * thepath,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
  /* Finds Path from position to target location for agent
     it must be first set by SetAgentTargetLocation
  */
  return 0;
}

/*      --------------------------------------------
                     PATH FINDING END
        -------------------------------------------- */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/*      --------------------------------------------
                     LOCATION MANAGMENT START
        -------------------------------------------- */

int AddLocation(struct Map * world,char * name,unsigned int pos_x,unsigned int pos_y)
{
  return 0;
}

int DeleteLocation_Number(struct Map * world,unsigned int position_id)
{
  return 0;
}

int DeleteLocation(struct Map * world,char * name)
{
  return 0;
}

/*      --------------------------------------------
                     LOCATION MANAGMENT END
        -------------------------------------------- */

        /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/*      --------------------------------------------
                     ROUTE EXTRACTION START
        -------------------------------------------- */
int ExtractRouteToLogo(struct Map * world,struct Path * thepath,char * filename)
{
  return 0;
}

int GetRoutePoints(struct Map * world,struct Path * thepath)
{
  return 0;
}

int GetRouteWaypoint(struct Map * world,struct Path * thepath,unsigned int *x,unsigned int *y)
{
  return 0;
}
/*      --------------------------------------------
                     ROUTE EXTRACTION END
        -------------------------------------------- */
