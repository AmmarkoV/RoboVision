#include "AStarCompute.h"
#include <stdio.h>
#include "NormalizePath.h"
#include "LogoHighLevelPath.h"

inline void swap_2_list_references(NodeRef * openlist,unsigned int &openlist_top,unsigned int ref1,unsigned int ref2)
{
// printf (" swap_2_list_references ( %d , %d ) \n",ref1,ref2);

  // ACTUAL SWAPING
  NodeRef tmpref;//={0}; dn xreiazetai na einai clean :P
  tmpref=openlist[ref1];
  openlist[ref1]=openlist[ref2];
  openlist[ref2]=tmpref;

// printf (" swap_2_list_references done \n");
  return;
}



//  quickSort
//
//  This public-domain C implementation by Darel Rex Finley.
//
//  * This function assumes it is called with valid parameters.
//
//  * Example calls:
//    quickSort(&myArray[0],5); // sorts elements 0, 1, 2, 3, and 4
//    quickSort(&myArray[3],5); // sorts elements 3, 4, 5, 6, and 7

inline void quickSortNodes(NodeRef *arr, int elements)
{

#define  MAX_LEVELS  300

  NodeRef piv;
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


unsigned int inline ReturnDistanceFromNodeToNode(NodeData * world_matrix,unsigned int start_node,unsigned int end_node)
{
  bool done=false;
  unsigned int current_node=end_node,distance=0;
  while (!done)
    {
      if ( world_matrix[current_node].parent_node==0 )
        {
          done=true;
        }
      else
        {
          // Yparxei parent node!
          ++distance;
          current_node=world_matrix[current_node].parent_node;
          if ( current_node==start_node )
            {
              done=true;
            }
        }
    }
  return distance;
}


unsigned int inline FillResultPath(NodeData * world_matrix,unsigned int world_x,TraceNode * resultlist,unsigned int result_size,unsigned int start_node,unsigned int end_node)
{
  bool done=false;
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
          done=true;
        }
      else
        {
          // Yparxei parent node!
          ++distance;
          current_node=world_matrix[current_node].parent_node;
          if ( current_node==start_node )
            {
              done=true;
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




AStarCompute::AStarCompute(NodeData * world_IN,unsigned int world_x_IN,unsigned int world_y_IN,NodeNeighborsCount *world_neighbors_IN,NodeRef *openlist_IN,unsigned int openlist_size_IN)
{
  printf("A* Computer is intializing\n");
  world=world_IN;
  world_neighbors=world_neighbors_IN;
  world_x=world_x_IN , world_y=world_y_IN;
  world_size=world_x*world_y;


  openlist=openlist_IN;
  openlist_size=openlist_size_IN;
  openlist_top=0;

  resultlist=0;
  resultlist_size = 0;

  str8_resultlist = 0;
  str8_resultlist_size = 0;
  printf("A* Computer constructed..\n");

}

AStarCompute::~AStarCompute()
{

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
}


inline unsigned int
AStarCompute::CheckFindPathCommandSanity()
{
  if ( world==0 )
    {
      fprintf(stderr,"World Matrix Not Initialized\n");
      return 1;
    }

  if ( (source_x>=world_x)|(source_y>=world_y)|(target_x>=world_x)|(target_y>=world_y) )
    {
      fprintf(stderr,"Positions out of bounds!\n");
      return 1;
    }

  if ( ( world_x==0) | ( world_y==0) )
    {
      fprintf(stderr,"World Matrix Size X/Y reported 0 ?\n");
      return 1;
    }


  if (openlist==0)
    {
      fprintf(stderr,"OpenList Not Initialized!\n");
      return 1;
    }


  return 0;
}

inline unsigned int abs(signed int num)
{
  if ( num < 0 )
    {
      return 0-num;
    }
  return num;
}

inline void ExpandNodeFromNode(NodeData * world_matrix,unsigned int from_node,unsigned int to_node,unsigned char current_heading,unsigned int turning_penalty)
{
  unsigned int new_score=world_matrix[from_node].score;


  new_score += world_matrix[from_node].node_penalty;
  if ( current_heading!= world_matrix[from_node].arrived_direction )
    {
      // TO PARAKATW DOULEVEI SWSTA
      new_score += abs(current_heading-world_matrix[from_node].arrived_direction)*turning_penalty;
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

inline unsigned int AStarCompute::GetNextNode()
{
  unsigned int retres=0;
  if (openlist_top>0)
    {
      quickSortNodes(openlist,openlist_top-1);
      retres=openlist[0].node;
      swap_2_list_references(openlist,openlist_top,0,openlist_top-1);
      --openlist_top;
    }
  return retres;
}



void inline AStarCompute::OpenNode(unsigned int the_node)
{
  if ( ( !world[the_node].opened ) && (world[the_node].unpassable==0) && (world_neighbors[the_node].total==0) )
    {
      if (openlist_top+1<openlist_size)
        {
          unsigned int cur_x=the_node % world_x; // Ypologizoume tin syntetagmeni x , y
          unsigned int cur_y=the_node / world_x; // Ypologizoume tin syntetagmeni x , y

          world[the_node].opened=true;

          // ADDING NEW NODE TO PENDING LIST!
          openlist[openlist_top].node=the_node;
          openlist[openlist_top].score=ManhattanDistance(cur_x,cur_y,target_x,target_y);


          ++openlist_top;
          // WE HEAVE SUCCESSFULLY ADDED THE NEW NODE TO THE TAIL OF THE OPENLIST

        }
      else
        {
          fprintf(stderr,"Out of space on open list , lets hope open nodes have a solution ( it might not be optimal )!!!\n");
          /* Mas teleiwse o xoros.. krima.. as elpisoume oti ta open nodes exoun lysi!! */
        }
    }
}


inline int AStarCompute::ProcessNode(unsigned int node_to_proc)
{
  proc_node=node_to_proc;
  if ( (done==0) && (proc_node>=0) && (proc_node<world_size) ) // IF NOT DONE AND INSIDE WORLD!
    {
      ExpandNodeFromNode(world,last_node,proc_node,node_direction,1); // False ennooume oti einai diagwnia kinisi!
      if (proc_node==target)
        {
          //printf("Target Just Reached :D\n"); // Vrikame to target! :D
          ++world[target].opened;
          ++solutions_gathered;
        }
      else
        {
          OpenNode(proc_node);
        }

    }

  return 0;
}



int AStarCompute::FindPath(unsigned int x1,unsigned int y1,unsigned int start_direction,unsigned int oursize,unsigned int x2,unsigned int y2,unsigned int timelimit_ms)
{
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
          proc_node = last_node - 1;
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



  return hops;
}



unsigned int AStarCompute::GetRoutePoint(unsigned char level , unsigned int pt_num,unsigned int &from_x,unsigned int &from_y)
{
  unsigned int retval=0;

  switch ( level )
    {
    case 0 :

      if ( (resultlist==0) | (resultlist_size <=   pt_num ) )
        {
          retval = 0;
        }
      else
        {
          from_x=resultlist[pt_num].nodex;
          from_y=resultlist[pt_num].nodey;
          retval = 1;
        }
      break;

    case 1 :
      if ( (str8_resultlist==0) | ( str8_resultlist_size <=  pt_num ) )
        {
          retval = 0;
        }
      else
        {
          from_x=str8_resultlist[pt_num].nodex;
          from_y=str8_resultlist[pt_num].nodey;
          retval = 1;
        }
      break;

    }
  return retval;
}
