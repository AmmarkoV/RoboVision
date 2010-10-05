#include "PathPlanningHelper.h"


/* This function adds a radious around an obstacle */
inline int PathPlanCore_ObstacleRadiousCalculations(struct Map * themap,unsigned char add_operation,unsigned int x,unsigned int y,unsigned int mem_ptr,unsigned int total_safety_radious)
{
   if ( total_safety_radious == 0 ) { themap->world[mem_ptr].in_unpassable_radious+=1; return 1; } /* Radious is a point */

   unsigned int safety_radious = (unsigned int) total_safety_radious/2;
   if ( safety_radious == 0 ) { safety_radious=1; } /* :P rounded */
   unsigned int start_x=x,start_y=y,end_x=x,end_y=y;


   /* Add left radious */
  if ( start_x>safety_radious ) {  start_x-=safety_radious; } else
                                {  start_x=0; }

  if ( start_y > safety_radious ) { start_y-=safety_radious; /* Add up radious */  } else
                                  { start_y=0; }

  if ( end_x+safety_radious >= themap->world_size_x ) { end_x=themap->world_size_x-1; /* Add right radious */ } else
                                                      { end_x+=safety_radious; }

  if ( end_y+safety_radious < themap->world_size_y )  { end_y+=safety_radious;  /* Add down radious */   } else
                                                      { end_y=themap->world_size_y-1; }


  unsigned int xi=start_x,yi=start_y,mem_ptr_i=0;
  for ( yi = start_y; yi<=end_y; yi++ )
   {
     mem_ptr_i=start_x+(yi*themap->world_size_x);
     for ( xi = start_x; xi<=end_x; xi++ )
     {
       if (add_operation) { if (themap->world[mem_ptr_i].in_unpassable_radious<255)
                             { ++themap->world[mem_ptr_i].in_unpassable_radious; } } else
                          { if (themap->world[mem_ptr_i].in_unpassable_radious>0)
                             { --themap->world[mem_ptr_i].in_unpassable_radious; } }
       ++mem_ptr_i;
     }
   }
   return 1;
}

int PathPlanCore_AddObstacleRadious(struct Map * themap,unsigned int x,unsigned int y,unsigned int mem_ptr,unsigned int safety_radious)
{
   return PathPlanCore_ObstacleRadiousCalculations(themap,1,x,y,mem_ptr,safety_radious);
}

int PathPlanCore_RemoveObstacleRadious(struct Map * themap,unsigned int x,unsigned int y,unsigned int mem_ptr,unsigned int safety_radious)
{
   return PathPlanCore_ObstacleRadiousCalculations(themap,0,x,y,mem_ptr,safety_radious);
}

unsigned int FillResultPath(struct NodeData * world_matrix,unsigned int world_x,struct TraceNode * resultlist,unsigned int result_size,unsigned int start_node,unsigned int end_node)
{
  if (result_size==0) { return 0; } /*We dont have a path , none of the following makes sense*/
  fprintf(stderr,"FillResultPath result_size is %u \n",result_size);
  unsigned char done=0,loop_counter=0;
  unsigned int current_node=end_node,distance=0;
  unsigned int cur_x=0,cur_y=0;

  --result_size;

  while (!done)
    {
      ++loop_counter;
      cur_x=current_node % world_x; // Ypologizoume tin syntetagmeni x , y
      cur_y=current_node / world_x; // Ypologizoume tin syntetagmeni x , y

      if ( result_size >= 0 )
        {
          resultlist[result_size].nodex = cur_x;
          resultlist[result_size].nodey = cur_y;
          if ( result_size != 0 ) { --result_size; }
        }

      if ( world_matrix[current_node].parent_node==0 )
        {
          done=1;
        }
      else
        {
          /* Parent node exists ! */
          ++distance;
          current_node=world_matrix[current_node].parent_node;
          if ( current_node==start_node )
            {
              done=1;
            }
        }
    }

  fprintf(stderr,"FillResultPath returning %u distance \n",distance);
  return distance;
}




int ClearFindPathState(struct Map * themap,struct Path * route)
{
    unsigned int x,y,ptr=0;
    for (y=0; y<themap->world_size_y; y++)
     {
      for (x=0; x<themap->world_size_x; x++)
      {
         themap->world[ptr].opened=0;
         themap->world[ptr].score=0;
         themap->world[ptr].heuristic=0;
         themap->world[ptr].movement_cost=0;
         themap->world[ptr].parent_node=0;
         themap->world[ptr].arrived_direction =0;
         ++ptr;
      }
     }

    route->cur_x=0;
    route->cur_y=0;
    route->node_direction=0;
    route->source=0; route->source_x=0; route->source_y=0;
    route->target=0; route->target_x=0; route->target_y=0;
    route->solutions_gathered=0;

    route->done=0;
    route->out_of_bounds=0;
    return 1;
}


int VerifySorting(struct NodeRef *arr, int elements)
{
  if ( elements<=1 ) { return 1; } /*For obvious reasons*/
  unsigned int i=0;
  for (i=0; i<elements-1; i++)
   {
        if (arr[i].score>arr[i+1].score) { return 0; }
   }
   return 1;
}
