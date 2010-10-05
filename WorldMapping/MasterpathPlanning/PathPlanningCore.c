#include "PathPlanningCore.h"
#include "NormalizePath.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

enum Directions
{
  UP_LEFT=1,
  UP,
  UP_RIGHT,
  LEFT,
  MIDDLE,
  RIGHT,
  DOWN_LEFT,
  DOWN,
  DOWN_RIGHT,
  /*--------------*/
  TOTAL_DIRECTIONS
};


/* This function adds a radious around an obstacle */
inline int PathPlanCore_ObstacleRadiousCalculations(struct Map * themap,unsigned char add_operation,unsigned int x,unsigned int y,unsigned int mem_ptr,unsigned int total_safety_radious)
{
   if ( total_safety_radious == 0 ) { themap->world[mem_ptr].in_unpassable_radious+=1; return 1; } /* Radious is a point */

   unsigned int safety_radious = (unsigned int) total_safety_radious/2;
   if ( safety_radious == 0 ) { safety_radious=1; } /* :P rounded */
   unsigned int start_x=x,start_y=y,end_x=x,end_y=y;


   /* Add left radious */
  if ( start_x>safety_radious ) {  start_x-=safety_radious; } else
                                { start_x=0; }

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
       if (add_operation) { ++themap->world[mem_ptr_i].in_unpassable_radious; ++themap->world_neighbors[mem_ptr_i].total; } else
                          { --themap->world[mem_ptr_i].in_unpassable_radious; --themap->world_neighbors[mem_ptr_i].total; }
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


inline void swap_2_list_references(struct NodeRef * openlist,unsigned int ref1,unsigned int ref2)
{
  // ACTUAL SWAPING
  struct NodeRef tmpref;
  tmpref=openlist[ref1];
  openlist[ref1]=openlist[ref2];
  openlist[ref2]=tmpref;
  return;
}

inline void quickSortNodes(struct NodeRef *arr, int elements)
{
  #define  MAX_LEVELS  400

  struct NodeRef piv;
  int  beg[MAX_LEVELS], end[MAX_LEVELS], i=0, L, R, swap ;

  beg[0]=0;
  end[0]=elements;
  while (i>=0)
    {
      L=beg[i];
      R=end[i]-1;
      if (L<R)
        {
          piv=arr[L];
          while (L<R)
            {
              while ( arr[R].score>=piv.score && L<R ) R--;
              if (L<R) arr[L++]=arr[R];
              while (arr[L].score<=piv.score && L<R) L++;
              if (L<R) arr[R--]=arr[L];
            }
          arr[L]=piv;
          beg[i+1]=L+1;
          end[i+1]=end[i];
          end[i++]=L;
          if (end[i]-beg[i]>end[i-1]-beg[i-1])
            {
              swap=beg[i];
              beg[i]=beg[i-1];
              beg[i-1]=swap;
              swap=end[i];
              end[i]=end[i-1];
              end[i-1]=swap;
            }
        }
      else
        {
          i--;
        }
    }
  return;
}


unsigned int inline ReturnDistanceFromNodeToNode(struct NodeData * world_matrix,unsigned int start_node,unsigned int end_node,unsigned int should_not_be_over)
{ /*This function returns the number of hops from end_node to start_node , if end_node is not connected to start_node it will eventually
    reach a node with parent 0 */
  unsigned char done=0;
  unsigned int current_node=end_node,distance=0;
  while ( (!done) && ( distance < should_not_be_over)  )
    {
      if ( world_matrix[current_node].parent_node==0 ) { done=1; distance=0; /*Dead end*/ }
      else
        { /* Yparxei parent node! */
          ++distance;
          current_node=world_matrix[current_node].parent_node;
          if ( current_node==start_node ) { done=1; }
        }
    }
  return distance;
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

inline unsigned int SamePosition(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
  if ( ( x1 == x2 ) && ( y1 == y2 ) ) { return 1; }
  return 0;
}

inline unsigned int ManhattanDistance(unsigned int from_x,unsigned int from_y,unsigned int to_x,unsigned int to_y)
{
  unsigned int totres=0;
  if (to_y>from_y) { totres+=to_y-from_y; } else { totres+=from_y-to_y; }
  if (to_x>from_x) { totres+=to_x-from_x; } else { totres+=from_x-to_x; }
  //fprintf(stderr,"ManhattanDistance(%u,%u,%u,%u) == %u\n",from_x,from_y,to_x,to_y,totres);
  return totres;
}

inline unsigned int abs_val(signed int num)
{
  if ( num < 0 ) { return (0-num); }
  return num;
}

inline void ExpandNodeFromNode(struct NodeData * world_matrix,unsigned int from_node,unsigned int to_node,unsigned char current_heading,unsigned int turning_penalty)
{
  unsigned int new_score=world_matrix[from_node].score;

  new_score += world_matrix[from_node].node_penalty;
  if ( current_heading!= world_matrix[from_node].arrived_direction )
    {
      // TO PARAKATW DOULEVEI SWSTA
      new_score += abs_val(current_heading-world_matrix[from_node].arrived_direction)*turning_penalty;
    }
  // Den theloume polles peristrofes giayto k gia kathe mia plironoume penalty

  if ((new_score<world_matrix[to_node].score) ||  (world_matrix[to_node].score==0) )
    {
      //Antikathistoume ton parent tou komvou to_node me ton from_node afou yparxei kalytero monopati!..!
      world_matrix[to_node].parent_node = from_node;
      world_matrix[to_node].score = new_score;
      world_matrix[to_node].arrived_direction = current_heading;
    }

}


void inline OpenNode(struct Map * themap,struct Path * route,unsigned int the_node)
{
  if ( ( !themap->world[the_node].opened ) && (themap->world[the_node].unpassable==0) && (themap->world_neighbors[the_node].total==0) )
    {
      if (route->openlist_top+1<route->openlist_size)
        {
          themap->world[the_node].opened=1;

          // ADDING NEW NODE TO PENDING LIST!
          route->openlist[route->openlist_top].node=the_node;
          route->openlist[route->openlist_top].score=ManhattanDistance(route->cur_x,route->cur_y,route->target_x,route->target_y);

          ++route->openlist_top;
          // WE HEAVE SUCCESSFULLY ADDED THE NEW NODE TO THE TAIL OF THE OPENLIST
        }
      else
        {
          fprintf(stderr,"Out of space on open list , lets hope open nodes have a solution ( it might not be optimal )!!!\n");
          /* Mas teleiwse o xoros.. krima.. as elpisoume oti ta open nodes exoun lysi!! */
        }
    }
}

inline unsigned int GetNextNode(struct Map * themap,struct Path * route)
{
  unsigned int retres=0;
  if (route->openlist_top>0)
    {
      quickSortNodes(route->openlist,route->openlist_top-1);
      retres=route->openlist[0].node;
      swap_2_list_references(route->openlist,0,route->openlist_top-1);
      --route->openlist_top;
    }
  return retres;
}

inline int ProcessNode(struct Map * themap,struct Path * route,unsigned int node_to_proc)
{
  route->proc_node = node_to_proc;
  if ( (route->done==0) && (route->proc_node>=0) && ( route->proc_node < themap->world_total_size) ) // IF NOT DONE AND INSIDE WORLD!
    {
      ExpandNodeFromNode( themap->world , route->last_node , route->proc_node, route->node_direction , 1); // False ennooume oti einai diagwnia kinisi!
      if (route->proc_node == route->target)
        {
          //printf("Target Just Reached :D\n"); // Vrikame to target! :D
          ++themap->world[route->target].opened;
          ++route->solutions_gathered;
        }
      else
        {
          OpenNode(themap,route,route->proc_node);
        }
    }
  return 0;
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

         ++ptr;
      }
     }
    return 1;
}

int PathPlanCore_FindPath(struct Map * themap,struct Path * theroute,unsigned int x1,unsigned int y1,unsigned int start_direction,unsigned int oursize,unsigned int x2,unsigned int y2,unsigned int timelimit_ms)
{
  struct Path * route=theroute;

  route->source=(y1*themap->world_size_x)+(x1) , route->source_x = x1 , route->source_y = y1;
  route->target=(y2*themap->world_size_x)+(x2) , route->target_x = x2 , route->target_y = y2;

  route->done=0;
  route->out_of_bounds=0;

  route->cur_x = 0 ,route->cur_y = 0, route->proc_node = 0, route->last_node = 0 , route->node_direction = 0;


  if ( SamePosition(x1,y1,x2,y2) == 1 )
    {
      printf("FindPath called and Source==Target , skipping calculations\n");
      return 0;
    }


  if ( route->openlist==0 )
    {
        route->openlist_size=(unsigned int) themap->world_total_size / 2; // < - MAX LIST SIZE , IT HAS A BIG EFFECT IN ALGORITHM SUCCESS RATIO!
        route->openlist= (struct NodeRef * ) malloc ( sizeof(struct NodeRef) * ( route->openlist_size+1 ) );
    }


  if ( !FindPathCommandIsSane(themap,route,route->source_x,route->source_y,route->target_x,route->target_y) )
    {
      printf("FindPath called with incorrect parameters , not starting find routine..\n");
      return 0;
    }

  clear_open_nodes(themap);

  if ( route->str8_resultlist!=0 ) { free(route->str8_resultlist); route->str8_resultlist_size=0; route->str8_resultlist=0; }
  if ( route->resultlist!=0 ) { free(route->resultlist); route->resultlist=0; route->resultlist=0; }

  route->resultlist_size = 0;
  route->openlist_top = 0;

  route->last_node=route->source;
  route->solutions_gathered = 0;
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // START FINDING THE PATH!
  printf (" Starting Path Finding..! \n");

  unsigned int loops=0;
  struct timeval start, end;
  long mtime=0, seconds, useconds;

  gettimeofday(&start, NULL);
  while ( route->done == 0 )
    {

      // MAIN A* LOOP START

      /*            UP
                   X X X
                X  1 2 3  X
          LEFT  X  4 5 6  X RIGHT      We consider the current node , node (5) and we check all the neighboring nodes!
                X  7 8 9  X
                   X X X
                    DOWN
      */


      // WE WILL SCAN THE NODES WITH THE FOLLOWING SERIES
      // (5) -> 4 1 2 3 6 9 8 7
      route->cur_x=route->last_node % themap->world_size_x; // Ypologizoume tin syntetagmeni x , y
      route->cur_y=route->last_node / themap->world_size_x; // Ypologizoume tin syntetagmeni x , y
      //fprintf(stderr,"%u/%u ",route->cur_x,route->cur_y);

      if ( ( route->cur_x > 0 ) && ( route->cur_x < themap->world_size_x ) && ( route->cur_y > 0 ) && ( route->cur_y < themap->world_size_y ) )
        {
          /* OUR CUR_X,CUR_Y location is DEEP INSIDE the array so WE CAN SKIP boundary CHECKS */
          route->node_direction=LEFT; route->proc_node = route->last_node - 1;
          ProcessNode(themap,route,route->proc_node);

          route->node_direction=UP_LEFT; route->proc_node = route->proc_node - themap->world_size_x;
          ProcessNode(themap,route,route->proc_node);

          route->node_direction=UP; ++route->proc_node;
          ProcessNode(themap,route,route->proc_node);

          route->node_direction=UP_RIGHT; ++route->proc_node;
          ProcessNode(themap,route,route->proc_node);

          route->node_direction=RIGHT; route->proc_node = route->proc_node + themap->world_size_x;
          ProcessNode(themap,route,route->proc_node);

          route->node_direction=DOWN_RIGHT; route->proc_node = route->proc_node + themap->world_size_x;
          ProcessNode(themap,route,route->proc_node);

          route->node_direction=DOWN; --route->proc_node;
          ProcessNode(themap,route,route->proc_node);

          route->node_direction=DOWN_LEFT; --route->proc_node;
          ProcessNode(themap,route,route->proc_node);
          // ---------------------
          // ---------------------
        }
      else
        {
          /*            UP
                       X X X
                    X  1 2 3  X
              LEFT  X  4 5 6  X RIGHT      We consider the current node , node (5) and we check all the neighboring nodes!
                    X  7 8 9  X
                       X X X
                        DOWN
          */
          // BORDER
          // FULL CHECKS
          route->proc_node = route->last_node - 1;
          if (route->cur_x>0 )
            {
              route->node_direction=LEFT; ProcessNode(themap,route,route->proc_node);
            }

          route->proc_node = route->proc_node - themap->world_size_x;
          if ( (route->cur_x>0) && (route->cur_y>0) )
            {
              route->node_direction=UP_LEFT; ProcessNode(themap,route,route->proc_node);
            }

          ++route->proc_node;
          if (route->cur_x>0 )
            {
              route->node_direction=UP; ProcessNode(themap,route,route->proc_node);
            }

          ++route->proc_node;
          if ( (route->cur_x>0) && (route->cur_y<themap->world_size_y) )
            {
              route->node_direction=UP_RIGHT; ProcessNode(themap,route,route->proc_node);
            }

          route->proc_node = route->proc_node + themap->world_size_x;
          if (route->cur_x<themap->world_size_x )
            {
              route->node_direction=RIGHT; ProcessNode(themap,route,route->proc_node);
            }

          if ( (route->cur_x<themap->world_size_x) && (route->cur_y<themap->world_size_y) )
            {
              route->node_direction=DOWN_RIGHT;
              route->proc_node = route->proc_node + themap->world_size_x;
              ProcessNode(themap,route,route->proc_node);
            }

          --route->proc_node;
          if (route->cur_y<themap->world_size_y )
            {
              route->node_direction=DOWN; ProcessNode(themap,route,route->proc_node);
            }

          --route->proc_node;
          if ( (route->cur_x>0) && (route->cur_y<themap->world_size_y) )
            {
              route->node_direction=DOWN_LEFT; ProcessNode(themap,route,route->proc_node);
            }
          // ----------------------------------
          // ----------------------------------
        }

      //FORTWSI KAINOURGIOU NODE PROS ANAZITISI!
      if (route->done==0)
        {
          if (route->openlist_top>0) //AN EXOUME KOMVO PROS PROSTHIKI
            {
              route->last_node=GetNextNode(themap,route); // to idio einai :P -> open_list[0].node;
              route->cur_x=route->last_node % themap->world_size_x; // Ypologizoume tin syntetagmeni x , y
              route->cur_y=route->last_node / themap->world_size_x; // Ypologizoume tin syntetagmeni x , y
            }
          else
            {
              route->done=1;  // AN DEN YPARXEI ALLOS KOMVOS PROS PROSTHIKI TELEIWSAME!
            }
        }

      ++loops;
      if ( loops % 1000 == 0 )
        {
          gettimeofday(&end, NULL);
          seconds  = end.tv_sec  - start.tv_sec;
          useconds = end.tv_usec - start.tv_usec;
          mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
          //printf("Elapsed time: %ld milliseconds\n", mtime);
        }

      if ( ( mtime > timelimit_ms ) && (themap->world[route->target].opened!=0) )
        {
          printf("Performance Shutdown ( %ld ms passed )  :) Hope solution is good enough \n",mtime);
          route->done=1;
        }


    } // MAIN A* LOOP END

  // PATH SHOULD BE FOUND ( OR NOT :P )!
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  printf("Total %d loops , and found total %d solutions \n",loops,route->solutions_gathered);

  unsigned int hops=0;

  if ( themap->world[route->target].opened!=0 )
    {
      hops=ReturnDistanceFromNodeToNode(themap->world,route->source,route->target,themap->world_total_size);
      route->resultlist_size = hops;
      route->resultlist = (struct TraceNode *) malloc(sizeof(struct TraceNode) * (route->resultlist_size+2) );

      fprintf(stderr,"resultlist_size = %u \n",route->resultlist_size);

      FillResultPath(themap->world,themap->world_size_x,route->resultlist,route->resultlist_size,route->source,route->target);
      // WE HAVE THE RAW PATH , WE ARE GOING FOR A LINED PATH!

      // 1st level line extraction
      route->str8_resultlist=GetANormalizedLineFromNodes(route->resultlist,route->resultlist_size,&route->str8_resultlist_size);


      if ( route->str8_resultlist == 0 )
       {
         fprintf(stderr,"Could not extract a normalized line\n");
       } else
       {
          // 2nd level line compression/extraction
           unsigned int start_str8_resultlist_size=route->str8_resultlist_size;
           GetTheShortestNormalizedLineFromNodes(themap->world,themap->world_neighbors,themap->world_size_x,themap->world_total_size,route->str8_resultlist,&route->str8_resultlist_size);
           printf("Compressed route , removed %d checkpoints now has %d total \n",start_str8_resultlist_size-route->str8_resultlist_size,route->str8_resultlist_size);

           /*char * storage;
           unsigned int storagelen=0;
           unsigned int commands= ConvertPathToLogo(route->str8_resultlist,route->str8_resultlist_size,storage,storagelen);*/
       }

      printf("Total Distance is %d \n",hops);
    }
  else
    {
      printf("Could not calculate a route! :S \n");
    }


  free(route->openlist); route->openlist=0; route->openlist_size=0; route->openlist_top=0;
  return hops;
}

