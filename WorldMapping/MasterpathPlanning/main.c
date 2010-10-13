#include <stdio.h>
#include <stdlib.h>
#include "MasterpathPlanning.h"
#include "PathPlanningCore.h"
#include "PathPlanningHelper.h"

/*      --------------------------------------------
               MAP STATE INITIALIZATION START
        -------------------------------------------- */
struct Map * CreateMap(unsigned int world_size_x,unsigned int world_size_y,unsigned int actors_number)
{
    FillInTurningOverheads();

    struct Map * newmap=0;
    newmap = (struct Map * ) malloc(sizeof(struct Map));
    newmap->GUARD_BYTE=GUARD_BYTE_VALUE;

        newmap->world_total_size=world_size_x*world_size_y;
        newmap->world_size_x = world_size_x;
        newmap->world_size_y = world_size_y;

        unsigned int i=0;

        newmap->world = (struct NodeData * ) malloc( sizeof(struct NodeData)* ( newmap->world_total_size+1 ) );
       // newmap->world_neighbors = (struct NodeNeighborsCount * ) malloc( sizeof(struct NodeNeighborsCount) * ( newmap->world_total_size+1 ) );


        struct NodeData emptynode={0};
        for ( i=0; i<newmap->world_total_size; i++ ) { newmap->world[i]=emptynode; }
      //  struct NodeNeighborsCount emptyneigh={0};
      //  for ( i=0; i<newmap->world_total_size; i++ ) { newmap->world_neighbors[i]=emptyneigh; }

/*
        newmap->openlist=0; newmap->openlist_current_size=0;
        newmap->openlist_size=(unsigned int) newmap->world_total_size / 2; // < - MAX LIST SIZE , IT HAS A BIG EFFECT IN ALGORITHM SUCCESS RATIO!
        newmap->openlist= (struct NodeRef * ) malloc( sizeof(struct NodeRef) * ( newmap->openlist_size+1 ) );
*/

        newmap->total_actors=actors_number+1; /*We need at least 1 actor*/
        newmap->actors = (struct Actor * ) malloc( sizeof(struct Actor)* ( newmap->total_actors ) );

        struct Actor emptyactor={0};
        struct Path emptypath={0};
        for ( i=0; i<newmap->total_actors; i++ ) { newmap->actors[i]=emptyactor;
                                                   newmap->actors[i].last_route=emptypath;
                                                   newmap->actors[i].last_route.str8_resultlist=0;
                                                 }


    newmap->error_number=0;
    newmap->add_neighbors_mode = 0; // ADD NEIGHBORS WHILE ADDING!
    newmap->diagonal_penalty=0;
    newmap->horizontal_penalty=0;
    newmap->turning_penalty=1;
    newmap->block_perfect_diagonal=1;


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
       if ( themap->world != 0 ) { free(themap->world); themap->world=0; } /* <- This function is epic it frees the world :D */

   //    if ( themap->world_neighbors != 0 ) { free(themap->world_neighbors); themap->world_neighbors=0; }

       if ( themap->actors != 0 ) { free(themap->actors); themap->actors=0; }

    free(themap);
    return 1;
}

int ClearMap(struct Map * themap)
{
  if (!MapIsOk(themap)) { return 0; }

  unsigned int i = 0;
  struct NodeData emptynode={0};
  for ( i=0; i<themap->world_total_size; i++ ) { themap->world[i]=emptynode; }
 // struct NodeNeighborsCount emptyneigh={0};
//  for ( i=0; i<themap->world_total_size; i++ ) { themap->world_neighbors[i]=emptyneigh; }

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

int RemoveObstacle(struct Map * themap,unsigned int x,unsigned int y,unsigned int safety_radious)
{
   if (!MapIsOk(themap)) { return 0; }
   unsigned int memory_ptr=x+(y*themap->world_size_x);
   if (memory_ptr >= themap->world_total_size ) { return 0; /*Overflow protection */ }

   themap->world[memory_ptr].unpassable=0;

   if (!PathPlanCore_RemoveObstacleRadious(themap,x,y,memory_ptr,safety_radious) ) { fprintf(stderr,"Could not remove radious\n"); }
   return 1;
}

/*      --------------------------------------------
               MAP STATE INITIALIZATION END
        -------------------------------------------- */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/*      --------------------------------------------
               AGENT STATE CHANGING START
        -------------------------------------------- */
int SetAgentSize(struct Map * themap,unsigned int agentnum,unsigned int x_size,unsigned int y_size,float differential_length)
{
  if (!MapIsOk(themap))  { return 0; }
  if (themap->actors==0) { return 0; }

  themap->actors[agentnum].size_x=x_size;
  themap->actors[agentnum].size_y=y_size;
  themap->actors[agentnum].size_total=x_size;
  themap->actors[agentnum].differential_length=differential_length;
  return 1;
}

int SetAgentHeading(struct Map * themap,unsigned int agentnum,float heading)
{
  if (!MapIsOk(themap))  { return 0; }
  if (themap->actors==0) { return 0; }

    themap->actors[agentnum].current_heading=GetCompressedHeadingFromRealHeading(heading);
    themap->actors[agentnum].real_heading=heading;

  return 0;
}

float GetAgentHeading(struct Map * themap,unsigned int agentnum)
{
  if (!MapIsOk(themap))  { return 0; }
  if (themap->actors==0) { return 0; }

  return themap->actors[agentnum].real_heading;
}

int SetAgentLocation(struct Map * themap,unsigned int agentnum,unsigned int x,unsigned int y)
{
  if (!MapIsOk(themap))  { return 0; }
  if (themap->actors==0) { return 0; }

  themap->actors[agentnum].current_x_pos=x;
  themap->actors[agentnum].current_y_pos=y;
  themap->actors[agentnum].abs_x_pos=(float) x;
  themap->actors[agentnum].abs_y_pos=(float) y;

  return 1;
}

int SetAgentLocationName(struct Map * themap,unsigned int agentnum,char * name)
{
    fprintf(stderr,"stub called\n");
  return 0;
}

int GetAgentLocation(struct Map * themap,unsigned int agentnum,unsigned int * x,unsigned int * y)
{
  if (!MapIsOk(themap))  { return 0; }
  if (themap->actors==0) { return 0; }

  *x=themap->actors[agentnum].current_x_pos;
  *y=themap->actors[agentnum].current_y_pos;

  return 1;
}


int SetAgentTargetLocation(struct Map * themap,unsigned int agentnum,unsigned int x,unsigned int y)
{
  if (!MapIsOk(themap))  { return 0; }
  if (themap->actors==0) { return 0; }

  themap->actors[agentnum].target_x_pos=x;
  themap->actors[agentnum].target_y_pos=y;
  return 1;
}

int SetAgentTargetLocationName(struct Map * themap,unsigned int agentnum,char * name)
{
  fprintf(stderr,"stub called\n");

  return 0;
}

int GetAgentTargetLocation(struct Map * themap,unsigned int agentnum,unsigned int * x,unsigned int * y)
{
  if (!MapIsOk(themap))  { return 0; }
  if (themap->actors==0) { return 0; }


  *x=themap->actors[agentnum].target_x_pos;
  *y=themap->actors[agentnum].target_y_pos;
  return 1;
}

int MoveAgentForward(struct Map * themap,unsigned int agentnum,int leftwheel_cm,int rightwheel_cm)
{
  return MoveAgentCore(themap,agentnum,leftwheel_cm,rightwheel_cm);
}

int AddObstacleSensedbyAgent(struct Map * themap,unsigned int agentnum,int ultrasonic_left_cm,int ultrasonic_right_cm)
{
  AddSensorDataToMap(themap,agentnum,ultrasonic_left_cm,ultrasonic_right_cm);
  return 0;
}
/*      --------------------------------------------
               AGENT STATE CHANGING END
        -------------------------------------------- */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/*      --------------------------------------------
                     PATH FINDING START
        -------------------------------------------- */
int FindPath(struct Map * themap,unsigned int agentnum,unsigned int timeout_ms)
{
  /* Finds Path from position to target location for agent
     it must be first set by SetAgentTargetLocation
  */
  return PathPlanCore_FindPath(themap,&themap->actors[agentnum].last_route,
                               themap->actors[agentnum].current_x_pos,themap->actors[agentnum].current_y_pos,themap->actors[agentnum].current_heading,
                               themap->actors[agentnum].size_total,
                               themap->actors[agentnum].target_x_pos,themap->actors[agentnum].target_y_pos,
                               timeout_ms);
}

int FindPathToPosition(struct Map * themap,unsigned int agentnum,unsigned int x,unsigned int y,unsigned int timeout_ms)
{
  /* Finds Path from position to target location for agent
     it must be first set by SetAgentTargetLocation
  */
  return PathPlanCore_FindPath(themap,&themap->actors[agentnum].last_route,themap->actors[agentnum].current_x_pos,themap->actors[agentnum].current_y_pos,themap->actors[agentnum].current_heading,themap->actors[agentnum].size_total,x,y,timeout_ms);
}

int FindSponteneousPath(struct Map * themap,unsigned int agentnum,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int timeout_ms)
{
  /* Finds Path from position to target location for agent
     it must be first set by SetAgentTargetLocation
  */
  return PathPlanCore_FindPath(themap,&themap->actors[agentnum].last_route,x1,y1,themap->actors[agentnum].current_heading,themap->actors[agentnum].size_total,x2,y2,timeout_ms);

}


int FindPathCommandIsSane(struct Map * themap,struct Path * route,unsigned int source_x,unsigned int source_y,unsigned int target_x,unsigned int target_y)
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

  if ( ( themap->world_size_x==0) | ( themap->world_size_y==0) )
    {
      fprintf(stderr,"World Matrix Size X/Y reported 0 ?\n");
      return 0;
    }


  if (route->openlist==0)
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

int GetRouteWaypoint(struct Map * themap,unsigned int agentnum,unsigned int count,unsigned int *x,unsigned int *y)
{
  if ( count >= themap->actors[agentnum].last_route.resultlist_size ) { return 0; }
  *x=themap->actors[agentnum].last_route.resultlist[count].nodex;
  *y=themap->actors[agentnum].last_route.resultlist[count].nodey;
  return 1;
}

int GetStraightRouteWaypoint(struct Map * themap,unsigned int agentnum,unsigned int count,unsigned int *x,unsigned int *y)
{
  if ( count >= themap->actors[agentnum].last_route.str8_resultlist_size ) { return 0; }
  *x=themap->actors[agentnum].last_route.str8_resultlist[count].nodex;
  *y=themap->actors[agentnum].last_route.str8_resultlist[count].nodey;
  return 1;
}
/*      --------------------------------------------
                     ROUTE EXTRACTION END
        -------------------------------------------- */
