#include "PathPlanningHelper.h"

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



int clear_open_nodes(struct Map * themap)
{
    unsigned int x,y,ptr=0;
    for (y=0; y<themap->world_size_y; y++)
     {
      for (x=0; x<themap->world_size_x; x++)
      {
         themap->world[ptr].opened=0;
         themap->world[ptr].score=0;
         themap->world[ptr].parent_node=0;
         themap->world[ptr].arrived_direction =0;
         ++ptr;
      }
     }
    return 1;
}
