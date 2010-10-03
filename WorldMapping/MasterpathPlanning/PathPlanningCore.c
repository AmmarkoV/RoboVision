#include "PathPlanningCore.h"

int PathPlanCore_CleanMap(struct Map * themap)
{
   if ( !MapIsOk(themap) ) { return 0; }
   unsigned int x,y,ptr=0;

   for ( y=0; y<themap->world_size_y; y++ )
    {
      for ( x=0; x<themap->world_size_x; x++ )
      {
         themap->world[ptr].parent_node=0;
         themap->world[ptr].arrived_direction=0;
         themap->world[ptr].score=0;
         themap->world[ptr].opened=0;
         themap->world[ptr].unpassable=0;
         themap->world[ptr].in_unpassable_radious=0;
         themap->world[ptr].node_penalty=0;
         ++ptr;
      }
    }
   return 1;
}



int PathPlanCore_AddObstacleRadious(struct Map * themap,unsigned int x,unsigned int y,unsigned int mem_ptr,unsigned int safety_radious)
{
   unsigned int start_x=x,start_y=y,end_x=x,end_y=y;
   if ( start_x>safety_radious )
    { /* Add left radious */
      if ( start_y > safety_radious )
      {  /* Add up radious */
         start_y-=safety_radious;
      } else
      {
         start_y=0;
      }
    }

   if ( end_x+safety_radious > themap->world_size_x )
    { /* Add right radious */
      if ( end_y+safety_radious >= themap->world_size_y )
      {  /* Add down radious */
         end_y=themap->world_size_y;
      } else
      {
         end_y+=safety_radious;
      }
    }

  unsigned int xi=start_x,yi=start_y,mem_ptr_i=start_x+start_y*themap->world_size_x;
  for ( yi = start_y; yi < end_y; yi++ )
   {
    for ( xi = start_x; xi < end_x; xi++ )
    {
      themap->world[mem_ptr_i].in_unpassable_radious+=1;
    }
   }

   return 1;
}
