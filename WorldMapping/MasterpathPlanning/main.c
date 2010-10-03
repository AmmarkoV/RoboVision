#include <stdio.h>
#include <stdlib.h>
#include "MasterpathPlanning.h"
#include "PathPlanningCore.h"

/*      --------------------------------------------
               MAP STATE INITIALIZATION START
        -------------------------------------------- */
struct Map * CreateMap(unsigned int world_size_x,unsigned int world_size_y,unsigned int actors_number)
{
    struct Map * newmap=0;
    newmap = (struct Map * ) malloc(sizeof(struct Map));
    newmap->GUARD_BYTE=GUARD_BYTE_VALUE;

        newmap->world_total_size=world_size_x*world_size_y;
        newmap->world = (struct NodeData * ) malloc( sizeof(struct NodeData)*newmap->world_total_size );
        newmap->world_size_x = world_size_x;
        newmap->world_size_y = world_size_y;

        /*We need at least 1 actor*/
        actors_number+=1;


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

int ClearMap(struct Map * themap)
{
     if (!MapIsOk(themap)) { return 0; }

  return PathPlanCore_CleanMap(themap);
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

int ObstacleRadiousExists(struct Map * themap,unsigned int x,unsigned int y)
{
   /* Tactic is to return 1 when failing that will effectively disable movement , and is less catastrophic */
   if (!MapIsOk(themap)) { return 1; }
   unsigned int memory_ptr=x+(y*themap->world_size_x);
   if (memory_ptr >= themap->world_total_size ) { return 1; /*Overflow protection */ }

   if ( themap->world[memory_ptr].in_unpassable_radious ) { return 1; } else
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
   return 1;
}
/*      --------------------------------------------
               MAP STATE INITIALIZATION END
        -------------------------------------------- */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/*      --------------------------------------------
               AGENT STATE CHANGING START
        -------------------------------------------- */
int SetAgentSize(struct Map * themap,unsigned int agentnum,unsigned int x_size,unsigned int y_size)
{
   fprintf(stderr,"stub called\n");
  return 0;
}

int SetAgentHeading(struct Map * themap,unsigned int agentnum,unsigned int heading)
{
   fprintf(stderr,"stub called\n");
  return 0;
}

unsigned int GetAgentHeading(struct Map * themap,unsigned int agentnum)
{
    fprintf(stderr,"stub called\n");
  return 0;
}

int SetAgentLocation(struct Map * themap,unsigned int agentnum,unsigned int x,unsigned int y)
{
    fprintf(stderr,"stub called\n");
  return 0;
}

int SetAgentLocationName(struct Map * themap,unsigned int agentnum,char * name)
{
    fprintf(stderr,"stub called\n");
  return 0;
}

int GetAgentLocation(struct Map * themap,unsigned int agentnum,unsigned int * x,unsigned int * y)
{
    fprintf(stderr,"stub called\n");
  return 0;
}


int SetAgentTargetLocation(struct Map * themap,unsigned int agentnum,unsigned int x,unsigned int y)
{
    fprintf(stderr,"stub called\n");
  return 0;
}

int SetAgentTargetLocationName(struct Map * themap,unsigned int agentnum,char * name)
{
    fprintf(stderr,"stub called\n");
  return 0;
}

int MoveAgentForward(struct Map * themap,unsigned int agentnum,int leftwheel_cm,int rightwheel_cm)
{
    fprintf(stderr,"stub called\n");
  return 0;
}

int SetObstacleSensedbyAgent(struct Map * themap,unsigned int agentnum,int ultrasonic_left_cm,int ultrasonic_right_cm)
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
int FindPath(struct Map * themap,unsigned int agentnum)
{
  /* Finds Path from position to target location for agent
     it must be first set by SetAgentTargetLocation
  */
  return 0;
}

int FindPathToPosition(struct Map * themap,unsigned int agentnum,unsigned int x,unsigned int y)
{
  /* Finds Path from position to target location for agent
     it must be first set by SetAgentTargetLocation
  */
  return 0;
}

int FindSponteneousPath(struct Map * themap,unsigned int agentnum,struct Path * thepath,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
  /* Finds Path from position to target location for agent
     it must be first set by SetAgentTargetLocation
  */
  PathPlanCore_FindPath(struct Map * themap,unsigned int x1,unsigned int y1,unsigned int start_direction,unsigned int oursize,x2,y2,5000);
  return 0;
}


unsigned int FindPathCommandIsSane(struct Map * themap,unsigned int source_x,unsigned int source_y,unsigned int target_x,unsigned int target_y)
{
  if (!MapIsOk(themap)) {
                          fprintf(stderr,"World Matrix Not Initialized\n");
                          return 0;
                        }

  if ( (source_x>=themap->world_size_x)|(source_y>=themap->world_size_y)|(target_x>=themap->world_size_x)|(target_y>=themap->world_size_y) )
    {
      fprintf(stderr,"Positions out of bounds!\n");
      return 0;
    }

  if ( ( world_x==0) | ( world_y==0) )
    {
      fprintf(stderr,"World Matrix Size X/Y reported 0 ?\n");
      return 0;
    }


  if (openlist==0)
    {
      fprintf(stderr,"OpenList Not Initialized!\n");
      return 0;
    }


  return 1;
}

/*      --------------------------------------------
                     PATH FINDING END
        -------------------------------------------- */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/*      --------------------------------------------
                     LOCATION MANAGMENT START
        -------------------------------------------- */

int AddLocation(struct Map * themap,char * name,unsigned int pos_x,unsigned int pos_y)
{
  return 0;
}

int DeleteLocation_Number(struct Map * themap,unsigned int position_id)
{
  return 0;
}

int DeleteLocation(struct Map * themap,char * name)
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
int ExtractRouteToLogo(struct Map * themap,struct Path * thepath,char * filename)
{
  return 0;
}

int GetRoutePoints(struct Map * themap,struct Path * thepath)
{
  return 0;
}

int GetRouteWaypoint(struct Map * themap,struct Path * thepath,unsigned int *x,unsigned int *y)
{
  return 0;
}
/*      --------------------------------------------
                     ROUTE EXTRACTION END
        -------------------------------------------- */
