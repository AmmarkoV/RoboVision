#include "PathPlanningCore.h"

/* This function cleans up a Map Structure */
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



/* This function adds a radious around an obstacle */
int PathPlanCore_AddObstacleRadious(struct Map * themap,unsigned int x,unsigned int y,unsigned int mem_ptr,unsigned int safety_radious)
{
   if ( safety_radious == 0 ) { themap->world[mem_ptr].in_unpassable_radious+=1; return 1; } /* Radious is a point */

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
      start_x-=safety_radious;
    } else
    { start_x=0; }

   if ( end_x+safety_radious >= themap->world_size_x )
    { /* Add right radious */
      if ( end_y+safety_radious >= themap->world_size_y )
      {  /* Add down radious */
         end_y=themap->world_size_y-1;
      } else
      {
         end_y+=safety_radious;
      }
      end_x=themap->world_size_x-1;
    } else
    { end_x+=safety_radious; }


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


inline void swap_2_list_references(struct NodeRef * openlist,unsigned int *openlist_top,unsigned int ref1,unsigned int ref2)
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

  #define  MAX_LEVELS  300

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
}


unsigned int inline ReturnDistanceFromNodeToNode(struct NodeData * world_matrix,unsigned int start_node,unsigned int end_node)
{
  unsigned char done=0;
  unsigned int current_node=end_node,distance=0;
  while (!done)
    {
      if ( world_matrix[current_node].parent_node==0 )
        {
          done=1;
        }
      else
        {
          // Yparxei parent node!
          ++distance;
          current_node=world_matrix[current_node].parent_node;
          if ( current_node==start_node )
            {
              done=1;
            }
        }
    }
  return distance;
}

unsigned int inline FillResultPath(struct NodeData * world_matrix,unsigned int world_x,struct TraceNode * resultlist,unsigned int result_size,unsigned int start_node,unsigned int end_node)
{
  unsigned char done=0;
  unsigned int current_node=end_node,distance=0;
  unsigned int cur_x,cur_y;
  --result_size;

  while (!done)
    {
      cur_x=current_node % world_x; // Ypologizoume tin syntetagmeni x , y
      cur_y=current_node / world_x; // Ypologizoume tin syntetagmeni x , y

      if ( result_size >= 0 )
        {
          resultlist[result_size].nodex = cur_x;
          resultlist[result_size].nodey = cur_y;
          --result_size;
        }

      // printf("Node %d,%d\n",cur_x,cur_y);

      if ( world_matrix[current_node].parent_node==0 )
        {
          done=1;
        }
      else
        {
          // Yparxei parent node!
          ++distance;
          current_node=world_matrix[current_node].parent_node;
          if ( current_node==start_node )
            {
              done=1;
            }
        }
    }
  return distance;
}

inline unsigned int SamePosition(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
  if ( ( x1 == x2 ) && ( y1 == y2 ) )
    {
      return 1;
    }
  return 0;
}

inline unsigned int ManhattanDistance(unsigned int from_x,unsigned int from_y,unsigned int to_x,unsigned int to_y)
{
  signed int retres=to_y-from_y,totres=to_x-from_x;
  if (totres<0) totres=-totres;
  if (retres<0) retres=-retres;
  totres+=retres;
  return totres;
}

inline unsigned int abs(signed int num)
{
  if ( num < 0 ) { return 0-num; }
  return num;
}


int PathPlanCore_FindPath(struct Map * themap,unsigned int x1,unsigned int y1,unsigned int start_direction,unsigned int oursize,unsigned int x2,unsigned int y2,unsigned int timelimit_ms)
{/*
  source=(y1*world_x)+(x1) , source_x = x1 , source_y = y1;
  target=(y2*world_x)+(x2) , target_x = x2 , target_y = y2;

  done=0;
  out_of_bounds=0;

  cur_x = 0 ,cur_y = 0, proc_node = 0, last_node = 0 , node_direction = 0;


  if ( SamePosition(x1,y1,x2,y2) == 1 )
    {
      printf("FindPath called and Source==Target , skipping calculations\n");
      return 0;
    }
  if ( CheckFindPathCommandSanity()!=0 )
    {
      printf("FindPath called with incorrect parameters , not starting find routine..\n");
      return 0;
    }


  if ( str8_resultlist!=0 )
    {
      delete str8_resultlist;
      str8_resultlist_size=0;
    }

  if ( resultlist!=0 )
    {
      delete resultlist;
      resultlist=0;
    }

  resultlist_size = 0;
  openlist_top = 0;

  last_node=source;
  solutions_gathered = 0;
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
  while ( done == 0 )
    {*/
      // MAIN A* LOOP START

      /*            UP
                   X X X
                X  1 2 3  X
          LEFT  X  4 5 6  X RIGHT      We consider the current node , node (5) and we check all the neighboring nodes!
                X  7 8 9  X
                   X X X
                    DOWN
      */

/*
      // WE WILL SCAN THE NODES WITH THE FOLLOWING SERIES
      // (5) -> 4 1 2 3 6 9 8 7
      cur_x=last_node % world_x; // Ypologizoume tin syntetagmeni x , y
      cur_y=last_node / world_x; // Ypologizoume tin syntetagmeni x , y

      if ( ( cur_x > 0 ) && ( cur_x < world_x ) && ( cur_y > 0 ) && ( cur_y < world_y ) )
        {
          // BEEING DEEP INSIDE
          // WE CAN SKIP CHECKS
          proc_node = last_node - 1;
          node_direction=4;
          ProcessNode(proc_node);

          proc_node = proc_node - world_x;
          node_direction=1;
          ProcessNode(proc_node);

          node_direction=2;
          ++proc_node;
          ProcessNode(proc_node);

          node_direction=3;
          ++proc_node;
          ProcessNode(proc_node);

          node_direction=6;
          proc_node = proc_node + world_x;
          ProcessNode(proc_node);

          node_direction=9;
          proc_node = proc_node + world_x;
          ProcessNode(proc_node);

          node_direction=8;
          --proc_node;
          ProcessNode(proc_node);

          node_direction=7;
          --proc_node;
          ProcessNode(proc_node);
          // ---------------------
          // ---------------------
        }
      else
        {*/
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
   /*       proc_node = last_node - 1;
          if (cur_x>0 )
            {
              node_direction=4;
              ProcessNode(proc_node);
            }

          proc_node = proc_node - world_x;
          if ( (cur_x>0) && (cur_y>0) )
            {
              node_direction=1;
              ProcessNode(proc_node);
            }

          ++proc_node;
          if (cur_x>0 )
            {
              node_direction=2;
              ProcessNode(proc_node);
            }

          ++proc_node;
          if ( (cur_x>0) && (cur_y<world_y) )
            {
              node_direction=3;
              ProcessNode(proc_node);
            }

          proc_node = proc_node + world_x;
          if (cur_x<world_x )
            {
              node_direction=6;
              ProcessNode(proc_node);
            }

          if ( (cur_x<world_x) && (cur_y<world_y) )
            {
              node_direction=9;
              proc_node = proc_node + world_x;
              ProcessNode(proc_node);
            }

          --proc_node;
          if (cur_y<world_y )
            {
              node_direction=8;
              ProcessNode(proc_node);
            }

          --proc_node;
          if ( (cur_x>0) && (cur_y<world_y) )
            {
              node_direction=7;
              ProcessNode(proc_node);
            }
          // ----------------------------------
          // ----------------------------------
        }

      //FORTWSI KAINOURGIOU NODE PROS ANAZITISI!
      if (done==0)
        {
          if (openlist_top>0) //AN EXOUME KOMVO PROS PROSTHIKI
            {
              last_node=GetNextNode(); // to idio einai :P -> open_list[0].node;
              cur_x=last_node % world_x; // Ypologizoume tin syntetagmeni x , y
              cur_y=last_node / world_x; // Ypologizoume tin syntetagmeni x , y
            }
          else
            {
              done=1;  // AN DEN YPARXEI ALLOS KOMVOS PROS PROSTHIKI TELEIWSAME!
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

      if ( ( mtime > timelimit_ms ) && (world[target].opened!=0) )
        {
          printf("Performance Shutdown ( %ld ms passed )  :) Hope solution is good enough \n",mtime);
          done=1;
        }


    } // MAIN A* LOOP END

  // PATH SHOULD BE FOUND ( OR NOT :P )!
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  printf("Total %d loops , and found total %d solutions \n",loops,solutions_gathered);

  unsigned int hops=0;

  if ( world[target].opened!=0 )
    {
      hops=ReturnDistanceFromNodeToNode(world,source,target);
      resultlist_size = hops;
      resultlist = new TraceNode[resultlist_size+2];

      FillResultPath(world,world_x,resultlist,resultlist_size,source,target);
      // WE HAVE THE RAW PATH , WE ARE GOING FOR A LINED PATH!

      // 1st level line extraction
      str8_resultlist=GetANormalizedLineFromNodes(resultlist,resultlist_size,str8_resultlist_size);
      if ( str8_resultlist == 0 )
       {
         fprintf(stderr,"Could not extract a normalized line\n");
       } else
       {
          // 2nd level line compression/extraction
           unsigned int start_str8_resultlist_size=str8_resultlist_size;
           GetTheShortestNormalizedLineFromNodes(world,world_neighbors,world_x,world_size,str8_resultlist,str8_resultlist_size);
           printf("Compressed route , removed %d checkpoints now has %d total \n",start_str8_resultlist_size-str8_resultlist_size,str8_resultlist_size);

           char * storage;
           unsigned int storagelen=0;
           unsigned int commands= ConvertPathToLogo(str8_resultlist,str8_resultlist_size,storage,storagelen);
       }
      //

      printf("Total Distance is %d \n",hops);
    }
  else
    {
      printf("Could not calculate a route! :S \n");
    }



  return hops;*/
  return 0;
}

