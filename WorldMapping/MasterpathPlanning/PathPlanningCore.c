#include "PathPlanningCore.h"
#include "PathPlanningHelper.h"
#include "NormalizePath.h"
#include "LogoHighLevelPath.h"
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265
unsigned char DEBUGING=0;

unsigned int turning_overheads[TOTAL_DIRECTIONS][TOTAL_DIRECTIONS]={{0}};

void FillInTurningOverheads()
{
  turning_overheads[UP_LEFT][UP]=1; turning_overheads[UP][UP_LEFT]=1;
  turning_overheads[UP][UP_RIGHT]=1; turning_overheads[UP_RIGHT][UP]=1;
  turning_overheads[UP_RIGHT][RIGHT]=1; turning_overheads[RIGHT][UP_RIGHT]=1;
  turning_overheads[RIGHT][DOWN_RIGHT]=1; turning_overheads[DOWN_RIGHT][RIGHT]=1;
  turning_overheads[DOWN_RIGHT][DOWN]=1; turning_overheads[DOWN][DOWN_RIGHT]=1;
  turning_overheads[DOWN][DOWN_LEFT]=1; turning_overheads[DOWN_LEFT][DOWN]=1;
  turning_overheads[DOWN_LEFT][LEFT]=1; turning_overheads[LEFT][DOWN_LEFT]=1;
  turning_overheads[LEFT][UP_LEFT]=1; turning_overheads[UP_LEFT][LEFT]=1;

  turning_overheads[UP_LEFT][UP_RIGHT]=2; turning_overheads[UP_RIGHT][UP_LEFT]=2;
  turning_overheads[UP][RIGHT]=2; turning_overheads[RIGHT][UP]=2;
  turning_overheads[UP_RIGHT][DOWN_RIGHT]=2; turning_overheads[DOWN_RIGHT][UP_RIGHT]=2;
  turning_overheads[RIGHT][DOWN]=2; turning_overheads[DOWN][RIGHT]=2;
  turning_overheads[DOWN_RIGHT][DOWN_LEFT]=2; turning_overheads[DOWN_LEFT][DOWN_RIGHT]=2;
  turning_overheads[DOWN][LEFT]=2; turning_overheads[LEFT][DOWN]=2;
  turning_overheads[DOWN_LEFT][UP_LEFT]=2; turning_overheads[UP_LEFT][DOWN_LEFT]=2;
  turning_overheads[LEFT][UP]=2; turning_overheads[UP][LEFT]=2;

  turning_overheads[UP_LEFT][RIGHT]=3; turning_overheads[RIGHT][UP_LEFT]=3;
  turning_overheads[UP][DOWN_RIGHT]=3; turning_overheads[DOWN_RIGHT][UP]=3;
  turning_overheads[UP_RIGHT][DOWN]=3; turning_overheads[DOWN][UP_RIGHT]=3;
  turning_overheads[RIGHT][DOWN_LEFT]=3; turning_overheads[DOWN_LEFT][RIGHT]=3;
  turning_overheads[DOWN_RIGHT][LEFT]=3; turning_overheads[LEFT][DOWN_RIGHT]=3;
  turning_overheads[DOWN][UP_LEFT]=3; turning_overheads[UP_LEFT][DOWN]=3;
  turning_overheads[DOWN_LEFT][UP]=3; turning_overheads[UP][DOWN_LEFT]=3;
  turning_overheads[LEFT][UP_RIGHT]=3; turning_overheads[UP_RIGHT][LEFT]=3;

  turning_overheads[UP_LEFT][DOWN_RIGHT]=4; turning_overheads[DOWN_RIGHT][UP_LEFT]=4;
  turning_overheads[UP][DOWN]=4; turning_overheads[DOWN][UP]=4;
  turning_overheads[UP_RIGHT][DOWN_LEFT]=4; turning_overheads[DOWN_LEFT][UP_RIGHT]=4;
  turning_overheads[RIGHT][LEFT]=4; turning_overheads[LEFT][RIGHT]=4;

}

unsigned int AddSensorDataToMap(struct Map * themap,unsigned int agentnum,int ultrasonic_left_cm,int ultrasonic_right_cm)
{
  fprintf(stderr,"AddSensorDataToMap stub %u %u \n",ultrasonic_left_cm,ultrasonic_right_cm);
    float cos_degrees=0.0,sin_degrees=0.0;
    float new_ultrasonic_left_x= themap->actors[agentnum].abs_x_pos;  float new_ultrasonic_left_y=themap->actors[agentnum].abs_y_pos;
    float new_ultrasonic_right_x=themap->actors[agentnum].abs_x_pos; float new_ultrasonic_right_y=themap->actors[agentnum].abs_y_pos;

    fprintf(stderr,"  Real position was %u/%u ",(unsigned int) themap->actors[agentnum].abs_x_pos,(unsigned int) themap->actors[agentnum].abs_y_pos);

    cos_degrees=cos(themap->actors[agentnum].real_heading*PI/180) / themap->world_unit_in_cm;
    sin_degrees=sin(themap->actors[agentnum].real_heading*PI/180) / themap->world_unit_in_cm;

    new_ultrasonic_left_x+=(ultrasonic_left_cm * cos_degrees );
    new_ultrasonic_left_y+=(ultrasonic_left_cm * sin_degrees );

    new_ultrasonic_right_x+=(ultrasonic_right_cm * cos_degrees );
    new_ultrasonic_right_y+=(ultrasonic_right_cm * sin_degrees );

    if  ( ( new_ultrasonic_left_x< 0 ) || ( new_ultrasonic_left_y< 0 ) )
      {
        fprintf(stderr,"Error while Sensing with agent , out of memory bounds");
        return 0;
      }

    SetObstacle(themap,new_ultrasonic_left_x,new_ultrasonic_left_y,2);

  return 1;
}

unsigned int MoveAgentCore(struct Map * themap,unsigned int agentnum,int move_left_cm,int move_right_cm)
{
    fprintf(stderr,"  MoveAgentCore stub %u %u \nTODO: add real differential change tracking\n",move_left_cm,move_right_cm);


    float cos_degrees=0.0,sin_degrees=0.0;
    float new_x_left_axel= themap->actors[agentnum].abs_x_pos;  float new_y_left_axel=themap->actors[agentnum].abs_y_pos;
    float new_x_right_axel=themap->actors[agentnum].abs_x_pos; float new_y_right_axel=themap->actors[agentnum].abs_y_pos;

    fprintf(stderr,"  Real position was %u/%u ",(unsigned int) themap->actors[agentnum].abs_x_pos,(unsigned int) themap->actors[agentnum].abs_y_pos);

    cos_degrees=cos(themap->actors[agentnum].real_heading*PI/180) / themap->world_unit_in_cm;
    sin_degrees=sin(themap->actors[agentnum].real_heading*PI/180) / themap->world_unit_in_cm;

    new_x_left_axel+= ( move_left_cm * cos_degrees );
    new_y_left_axel+= ( move_left_cm * sin_degrees );

    new_x_right_axel+= ( move_right_cm * cos_degrees );
    new_y_right_axel+= ( move_right_cm * sin_degrees );

    fprintf(stderr,"  Moving From %u/%u ",themap->actors[agentnum].current_x_pos,themap->actors[agentnum].current_y_pos);

    if  ( ( new_x_left_axel< 0 ) || ( new_y_left_axel< 0 ) )
      {
        fprintf(stderr,"Error while Moving agent , agent out of memory bounds");
        return 0;
      }

    themap->actors[agentnum].abs_x_pos=new_x_left_axel;
    themap->actors[agentnum].abs_y_pos=new_y_left_axel;
    themap->actors[agentnum].current_x_pos=(unsigned int)new_x_left_axel;
    themap->actors[agentnum].current_y_pos=(unsigned int)new_y_left_axel;

    fprintf(stderr," to %u/%u \n",themap->actors[agentnum].current_x_pos,themap->actors[agentnum].current_y_pos);

  return 1;
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


inline void swap_2_list_references(struct NodeRef * openlist,unsigned int ref1,unsigned int ref2)
{
  // ACTUAL SWAPING
  struct NodeRef tmpref;
  tmpref=openlist[ref1];
  openlist[ref1]=openlist[ref2];
  openlist[ref2]=tmpref;
  return;
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

//  quickSort
//  This public-domain C implementation by Darel Rex Finley.
//  * This function assumes it is called with valid parameters.
//  * Example calls:
//    quickSort(&myArray[0],5); // sorts elements 0, 1, 2, 3, and 4
//    quickSort(&myArray[3],5); // sorts elements 3, 4, 5, 6, and 7
inline void quickSortNodes(struct NodeRef *arr, int elements)
{
  if ( elements<=1 ) { return ; } /*For obvious reasons*/
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
              while ( (arr[R].score>=piv.score) && (L<R) ) R--;
              if (L<R) arr[L++]=arr[R];
              while ( (arr[L].score<=piv.score) && (L<R) ) L++;
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

inline unsigned int GetHeadingChangeOverhead(unsigned int start_heading,unsigned int end_heading)
{
  if ( ( start_heading >= TOTAL_DIRECTIONS ) || ( end_heading >= TOTAL_DIRECTIONS ) ) { fprintf(stderr,"Erroneous heading \n"); return 1; }
  if ( start_heading == end_heading ) { return 0; }

  return turning_overheads[start_heading][end_heading];
}

inline unsigned int GetHeadingMovementOverhead(unsigned int heading)
{
  switch (heading)
   {
      case UP_LEFT : return 3; break;
      case UP_RIGHT : return 3; break;
      case DOWN_LEFT : return 3; break;
      case DOWN_RIGHT : return 3; break;
   };
  return 2;
}



inline void ExpandNodeFromNode(struct Map * themap,struct Path * route,unsigned int from_node,unsigned int to_node,unsigned char current_heading,unsigned int turning_penalty)
{
  unsigned int x=from_node % themap->world_size_x; // Ypologizoume tin syntetagmeni x , y
  unsigned int y=from_node / themap->world_size_x; // Ypologizoume tin syntetagmeni x , y

  unsigned int new_heuristic=ManhattanDistance(x,y,route->target_x,route->target_y);
  unsigned int new_movement_cost=ManhattanDistance(x,y,route->target_x,route->target_y);
  unsigned int new_score=themap->world[to_node].movement_cost+themap->world[to_node].heuristic;//=themap->world[from_node].score;

  unsigned int turning_movement_overhead=GetHeadingChangeOverhead(current_heading,themap->world[from_node].arrived_direction);

  new_movement_cost=themap->world[from_node].movement_cost + turning_movement_overhead + GetHeadingMovementOverhead(current_heading);
  new_score = new_movement_cost + new_heuristic;

  if ((new_score<themap->world[to_node].score) ||  (themap->world[to_node].score==0) )
    {
      //Antikathistoume ton parent tou komvou to_node me ton from_node afou yparxei kalytero monopati!..!
      themap->world[to_node].parent_node = from_node;
      themap->world[to_node].score = new_score;
      themap->world[to_node].movement_cost=new_movement_cost;
      themap->world[to_node].heuristic=new_heuristic;
      themap->world[to_node].arrived_direction = current_heading;
    }

}


void inline OpenNode(struct Map * themap,struct Path * route,unsigned int parent_node,unsigned int the_node)
{
  if ( ( !themap->world[the_node].opened ) && (themap->world[the_node].unpassable==0) && (themap->world[the_node].in_unpassable_radious==0) )
    {
      if (route->openlist_top+1<route->openlist_size)
        {
          themap->world[the_node].opened=1;

          // ADDING NEW NODE TO PENDING LIST!
          route->openlist[route->openlist_top].node = the_node;
          unsigned int x=parent_node % themap->world_size_x; // Ypologizoume tin syntetagmeni x , y
          unsigned int y=parent_node / themap->world_size_x; // Ypologizoume tin syntetagmeni x , y
          route->openlist[route->openlist_top].score = themap->world[the_node].movement_cost+ManhattanDistance(x,y,route->target_x,route->target_y);

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

inline int ProcessNode(struct Map * themap,struct Path * route,unsigned int parent_node,unsigned int node_to_proc)
{
  /*
     Each time process node is called we want to check "node_to_proc" and the parent_node is the previous node
  */

  if ( (route->done==0) && ( node_to_proc < themap->world_total_size) ) // IF NOT DONE AND INSIDE WORLD!
    {

      ExpandNodeFromNode( themap,route , parent_node , node_to_proc , route->node_direction , themap->turning_penalty);
      if (node_to_proc == route->target)
        {
          //printf("Target Just Reached :D\n"); // We found the target! :D
          themap->world[route->target].opened=1;
          ++route->solutions_gathered;
        }
      else
        {
          OpenNode(themap,route,parent_node,node_to_proc);
        }
    }
  return 0;
}



inline unsigned int GetNextNode(struct Map * themap,struct Path * route)
{
  unsigned int retres=0;
  if (route->openlist_top>0)
    {
      quickSortNodes(route->openlist,route->openlist_top-1);
      if (DEBUGING) { if (!VerifySorting(route->openlist,route->openlist_top-1)) {fprintf(stderr,"QuickSort malfunctioning\n"); } }
      retres=route->openlist[0].node;
      swap_2_list_references(route->openlist,0,route->openlist_top-1);
      --route->openlist_top;
    }
  return retres;
}


int PathPlanCore_FindPath(struct Map * themap,struct Path * theroute,unsigned int x1,unsigned int y1,unsigned int start_direction,unsigned int oursize,unsigned int x2,unsigned int y2,unsigned int timelimit_ms)
{
  if ( SamePosition(x1,y1,x2,y2) == 1 ) { printf("FindPath called and Source==Target , skipping calculations\n"); return 0; }

  struct Path * route=theroute;
  ClearFindPathState(themap,route);

  route->source=(y1*themap->world_size_x)+(x1);   route->source_x = x1; route->source_y = y1;
  route->target=(y2*themap->world_size_x)+(x2);   route->target_x = x2; route->target_y = y2;
  fprintf(stderr,"Findpath %u/%u -> %u/%u ",x1,y1,x2,y2);
  fprintf(stderr,"Source node is  %u ",route->source);

  if ( route->openlist==0 )
    {
        route->openlist_size=(unsigned int) themap->world_total_size / 2; // < - MAX LIST SIZE , IT HAS A BIG EFFECT IN ALGORITHM SUCCESS RATIO!
        route->openlist = (struct NodeRef * ) malloc ( sizeof(struct NodeRef) * ( route->openlist_size+1 ) );
    }
  route->openlist_top = 0;

  if ( route->str8_resultlist!=0 ) { free(route->str8_resultlist); route->str8_resultlist_size=0; route->str8_resultlist=0; }
  if ( route->resultlist!=0 ) { free(route->resultlist); route->resultlist=0; route->resultlist_size=0; }



  unsigned int last_node=route->source; /* The source node is the parent node */
  route->solutions_gathered = 0;
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // START FINDING THE PATH!

  if ( !FindPathCommandIsSane(themap,route,route->source_x,route->source_y,route->target_x,route->target_y) )
    {
      printf("FindPath called with incorrect parameters , not starting find routine..\n");
      return 0;
    } else
    {
     printf (" Starting Path Finding..! \n");
    }


  unsigned int loops=0;
  struct timeval start, end;
  long mtime=0, seconds, useconds;

  unsigned int memory_ptr=0;

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
      route->cur_x=last_node % themap->world_size_x; // Ypologizoume tin syntetagmeni x , y
      route->cur_y=last_node / themap->world_size_x; // Ypologizoume tin syntetagmeni x , y
      //fprintf(stderr,"%u/%u ",route->cur_x,route->cur_y);

      if ( ( route->cur_x > 0 ) && ( route->cur_x < themap->world_size_x ) && ( route->cur_y > 0 ) && ( route->cur_y < themap->world_size_y ) )
        {
          /* OUR CUR_X,CUR_Y location is DEEP INSIDE the array so WE CAN SKIP boundary CHECKS */
          route->node_direction=LEFT; memory_ptr = last_node - 1;
          ProcessNode(themap,route,last_node,memory_ptr);

          route->node_direction=UP_LEFT; memory_ptr = memory_ptr - themap->world_size_x;
          ProcessNode(themap,route,last_node,memory_ptr);

          route->node_direction=UP; ++memory_ptr;
          ProcessNode(themap,route,last_node,memory_ptr);

          route->node_direction=UP_RIGHT; ++memory_ptr;
          ProcessNode(themap,route,last_node,memory_ptr);

          route->node_direction=RIGHT; memory_ptr = memory_ptr + themap->world_size_x;
          ProcessNode(themap,route,last_node,memory_ptr);

          route->node_direction=DOWN_RIGHT; memory_ptr = memory_ptr + themap->world_size_x;
          ProcessNode(themap,route,last_node,memory_ptr);

          route->node_direction=DOWN; --memory_ptr;
          ProcessNode(themap,route,last_node,memory_ptr);

          route->node_direction=DOWN_LEFT; --memory_ptr;
          ProcessNode(themap,route,last_node,memory_ptr);
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

          if (route->cur_x>0 )
            {
              route->node_direction=LEFT; ProcessNode(themap,route,last_node,last_node - 1);
            }

          if ( (route->cur_x>0) && (route->cur_y>0) )
            {
              route->node_direction=UP_LEFT; ProcessNode(themap,route,last_node,last_node - themap->world_size_x -1);
            }


          if (route->cur_x>0 )
            {
              route->node_direction=UP; ProcessNode(themap,route,last_node,last_node - themap->world_size_x);
            }


          if ( (route->cur_x>0) && (route->cur_y<themap->world_size_y) )
            {
              route->node_direction=UP_RIGHT; ProcessNode(themap,route,last_node,last_node - themap->world_size_x +1);
            }


          if (route->cur_x<themap->world_size_x )
            {
              route->node_direction=RIGHT; ProcessNode(themap,route,last_node,last_node + 1);
            }

          if ( (route->cur_x<themap->world_size_x) && (route->cur_y<themap->world_size_y) )
            {
              route->node_direction=DOWN_RIGHT; ProcessNode(themap,route,last_node,last_node + themap->world_size_x + 1);
            }

          if (route->cur_y<themap->world_size_y )
            {
              route->node_direction=DOWN; ProcessNode(themap,route,last_node,last_node + themap->world_size_x);
            }

          if ( (route->cur_x>0) && (route->cur_y<themap->world_size_y) )
            {
              route->node_direction=DOWN_LEFT; ProcessNode(themap,route,last_node,last_node + themap->world_size_x-1);
            }

          // ----------------------------------
          // ----------------------------------
        }

      //FORTWSI KAINOURGIOU NODE PROS ANAZITISI!
      if (route->done==0)
        {
          if (route->openlist_top>0) //AN EXOUME KOMVO PROS PROSTHIKI
            {
              last_node = GetNextNode(themap,route); // to idio einai :P -> open_list[0].node;
              route->cur_x = last_node % themap->world_size_x; // Ypologizoume tin syntetagmeni x , y
              route->cur_y = last_node / themap->world_size_x; // Ypologizoume tin syntetagmeni x , y
              if ( last_node == 0 )
                { fprintf(stderr,"? NO NEXT NODE ?\n");}
            }
          else
            {
              fprintf(stderr,"Could not find another new node after %u loops \n",loops);
              route->done=1;  // AN DEN YPARXEI ALLOS KOMVOS PROS PROSTHIKI TELEIWSAME!
            }
        }

      ++loops;
      if ( loops % 500 == 0 )
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
  printf("Total %u loops , and found total %u solutions \n",loops,route->solutions_gathered);

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
           GetTheShortestNormalizedLineFromNodes(themap->world,themap->world_size_x,themap->world_total_size,route->str8_resultlist,&route->str8_resultlist_size);
           printf("Compressed route , removed %d checkpoints now has %d total \n",start_str8_resultlist_size-route->str8_resultlist_size,route->str8_resultlist_size);

           char * storage=0;
           unsigned int storagelen=0;
           ConvertPathToLogo(route->str8_resultlist,&route->str8_resultlist_size,storage,storagelen);
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

