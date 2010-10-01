#include "PathPlan_core.h"
#include "PathPlan_printouts.h"
#include "PathPlan_error_codes.h"
#include <unistd.h>
#include <stdio.h>





struct NodeData    // Declare NODE struct type
  {
    unsigned int parent_node;
    unsigned char arrived_direction;
    unsigned int score;
    bool opened;
    bool unpassable;
    unsigned short node_penalty;
  };

struct NodeRef
  {
    // Declare NODE struct type
    unsigned int node;
    unsigned int score;
  };

unsigned int error_no=666;
unsigned int source_block=0;

unsigned int target_block=0;
unsigned int target_block_x=0,target_block_y=0;

unsigned char agent_source_direction=0;
unsigned char agent_direction=0;

unsigned int matrixes_loaded=0;
unsigned int matrix_size_x=0;
unsigned int matrix_size_y=0;

unsigned short diagonal_penalty=0;
unsigned short horizontal_penalty=0;
unsigned short turning_penalty=1;
bool block_perfect_diagonal=true;



unsigned int world_matrix_size=0;
unsigned int last_run_time=0;
NodeData * world_matrix;

unsigned int openlist_size=0;
unsigned int open_list_current_size=0;
NodeRef *open_list;


unsigned int best_path_length=0;
unsigned int * best_path;
int tickc=0;

unsigned int GetTickCount()
{
  ++tickc;
  return tickc;
}


unsigned int inline ReturnDistanceFromNodeToNode(unsigned int start_node,unsigned int end_node)
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


unsigned short PathPlanningStatus(unsigned short type_of_data)
{
  unsigned short retres=0;
  switch (type_of_data)
    {
    case STATUS_ERROR: //ERROR CHECK
      retres=error_no;
      break;
    case STATUS_WORLDX: //WORLD SIZE X
      retres=matrix_size_x;
      break;
    case STATUS_WORLDY: //WORLD SIZE Y
      retres=matrix_size_y;
      break;
    case STATUS_WORLD_MATRIX_SIZE: //WORLD MATRIX SIZE
      retres=world_matrix_size;
      break;
    case STATUS_LAST_SOURCE_NODE: //LAST SOURCE NODE
      retres=source_block;
      break;
    case STATUS_LAST_TARGET_NODE: //LAST TARGET NODE
      retres=target_block;
      break;
    case STATUS_WORLD_MEMORY_OK: //WORLD MEMORY OK  ( RESULT = 1 -> MEMORY ALLOCATED  /  RESULT = 0 -> MEMORY FREE )
      if (world_matrix!=0)
        {
          retres=1;
        }
      else
        {
          retres=0;
        }
      break;
    case STATUS_LIST_MEMORY_OK: //LIST MEMORY OK   ( RESULT = 1 -> MEMORY ALLOCATED  /  RESULT = 0 -> MEMORY FREE )
      if (open_list!=0)
        {
          retres=1;
        }
      else
        {
          retres=0;
        }
      break;
    case STATUS_LIST_MEMORY_SIZE: //LIST MEMORY SIZE
      retres=open_list_current_size;
      break;
    case STATUS_LAST_RUN_ELAPSED_TIME: //LAST RUN ELAPSED TIME
      retres=last_run_time;
      break;
    case STATUS_TARGET_NODE_PARENT: //TARGET NODE PARENT
      if ( (target_block>world_matrix_size) || (target_block<1) )
        {
          error_no=TARGET_OUT_OF_BOUNDS;
        }
      else
        {
          retres=world_matrix[target_block].parent_node;
        }
      break;
    case STATUS_TARGET_NODE_DISTANCE: //TARGET NODE DISTANCE IF TARGET REACHED!
      if ( (source_block>world_matrix_size) || (source_block<1) )
        {
          error_no=SOURCE_OUT_OF_BOUNDS;
        }
      else
        if ( (target_block>world_matrix_size) || (target_block<1) )
          {
            error_no=TARGET_OUT_OF_BOUNDS;
          }
        else
          {
            retres=ReturnDistanceFromNodeToNode(source_block,target_block);
          }
      break;
    default:
      retres=type_of_data;
    }
  return retres;
}



unsigned short PathPlanning_Get_ObjectMem(unsigned short mem_place,unsigned short type)
{
  unsigned short retres=0;
  switch (type)
    {
    case TYPE_SCORE:
      retres=world_matrix[mem_place].score;
      break;
    case TYPE_PARENTNODE:
      retres=world_matrix[mem_place].parent_node;
      break;
    case TYPE_PLACE_X:
      retres=mem_place % matrix_size_x;
      break;
    case TYPE_PLACE_Y:
      retres=mem_place / matrix_size_x;
      break;
    case TYPE_ARRIVED_DIRECTION:
      retres=world_matrix[mem_place].arrived_direction;
      break;
    case TYPE_NODE_PENALTY:
      retres=world_matrix[mem_place].node_penalty;
      break;
    case TYPE_UNPASSABLE:
      if (world_matrix[mem_place].unpassable)
        {
          retres=1;
        }
      else
        {
          retres=0;
        }
      break;

    default :
      retres=0;
    }
  return retres;
}

unsigned short PathPlanning_Get_Object(unsigned short atx,unsigned short aty,unsigned short type)
{
  unsigned int  mem_place=(unsigned int) (aty*matrix_size_x)+(atx);
  return PathPlanning_Get_ObjectMem(mem_place,type);
}






void CreatePathPlanningMatrix(unsigned int x_size,unsigned int y_size)
{
  if (world_matrix!=0)
    {
      delete[] world_matrix;
      world_matrix=0;
    }

  world_matrix_size=(x_size+1)*(y_size+1);
  world_matrix=new NodeData[world_matrix_size];

  if (world_matrix!=0)
    {
      ++matrixes_loaded; // We have our world!
      matrix_size_x=x_size;
      matrix_size_y=y_size;

      openlist_size=(unsigned int) world_matrix_size / 5;

      best_path_length=0;
      error_no=0;

      NodeData empty={0};
      for ( int i=0; i<world_matrix_size; i++)
        {
          world_matrix[i]=empty;
        }
    }
  else
    {
      error_no=OUT_OF_MEMORY_WHILE_CREATING_MATRIX;
      /*MessageBox(0,"Out of memory while creating the matrix!","",0);*/
    }

}


void DeletePathPlanningMatrix()
{
  //MessageBox(0,"Deleting everything","",0);
  if (world_matrix!=0)
    {
      delete[] world_matrix;
      world_matrix=0;
    }
  if (open_list!=0)
    {
      delete[] open_list;
      open_list=0;
    }
  world_matrix_size=0;
  matrix_size_x=0;
  matrix_size_y=0;

  openlist_size=0;
  open_list_current_size=0;
  best_path_length=0;
  --matrixes_loaded; // We have our world!
  error_no=0;

}

void SetAllNodesClear()
{
  if (world_matrix==0)
    {
      return;
    }
  //memset(world_matrix,0,sizeof(NodeData));
}

void SetNodeData(unsigned int x,unsigned int y,bool unpassable,unsigned int penalty)
{
  if (world_matrix==0)
    {
      return;
    }
  unsigned int  mem_place=(unsigned int) (y*matrix_size_x)+(x);

  world_matrix[mem_place].unpassable=unpassable;
  world_matrix[mem_place].node_penalty=penalty;

}


void SetObject(unsigned int x,unsigned int y,unsigned int type_of_obj,unsigned int direction)
{
  // Mporei na prosthetei kinoumena antikeimena ston xarti..
}

unsigned int inline ManhattanDistance(unsigned int from_x,unsigned int from_y,unsigned int to_x,unsigned int to_y)
{
  signed int retres,totres=to_x-from_x;
  if (totres<0)
    {
      totres=-totres;
    }
  retres=to_x-from_x;
  if (retres<0)
    {
      retres=-retres;
    }
  totres+=retres;
  return totres;
}



void inline ExpandNodeFromNode(unsigned int from_node,unsigned int to_node,unsigned char current_heading,bool horizontal_move)
{
  unsigned int new_score=world_matrix[from_node].score;
  if (horizontal_move)
    {
      new_score+=horizontal_penalty;
    }
  else
    {
      new_score+=diagonal_penalty;  // Oi orizonties kiniseis kostizoun elafrws perisotero apo tis kathetes
    }

  new_score+=world_matrix[from_node].node_penalty;
  if ( current_heading!= world_matrix[from_node].arrived_direction )
    {
      new_score+=turning_penalty;
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


void inline swap_2_list_references(unsigned int ref1,unsigned int ref2)
{
  NodeRef tmpref;//={0}; dn xreiazetai na einai clean :P
  tmpref=open_list[ref1];
  open_list[ref1]=open_list[ref2];
  open_list[ref2]=tmpref;
}

void inline OpenNode(unsigned int the_node)
{
  if ( !world_matrix[the_node].opened )
    {
      if (open_list_current_size+1<openlist_size)
        {
          unsigned int cur_x=the_node % matrix_size_x; // Ypologizoume tin syntetagmeni x , y
          unsigned int cur_y=the_node / matrix_size_x; // Ypologizoume tin syntetagmeni x , y

          world_matrix[the_node].opened=true;

          open_list[open_list_current_size].node=the_node;
          open_list[open_list_current_size].score=ManhattanDistance(cur_x,cur_y,target_block_x,target_block_y);

          //BINARY HEAP ADD
          unsigned cur_place=open_list_current_size,swap_place=open_list_current_size;
          while (cur_place>0)
            {
              swap_place=cur_place/2;
              if ( open_list[swap_place].score > open_list[cur_place].score )
                {
                  swap_2_list_references(swap_place,cur_place);
                  cur_place=swap_place;

                }
              else
                {
                  break;
                }
            }
          //BINARY HEAP ADD

          ++open_list_current_size;

        }
      else
        {
          fprintf(stderr,"Out of space on open list , lets hope open nodes carry a solution!!!\n");
          /* Mas teleiwse o xoros.. krima.. as elpisoume oti ta open nodes exoun lysi!! */
        }
    }
}


void inline FloatUpNodeListAtPlace(unsigned int theplace)
{
  bool stopfloat=false;
  theplace+=1; // Giati k kala to prwto slot einai to #1
  unsigned int thecomp=theplace*2;
  unsigned int thecomp2=thecomp+1;
  while (!stopfloat)
    {
      if (open_list_current_size>thecomp2)
        {
          if (open_list[thecomp].score<=open_list[thecomp2].score)
            {
              if (open_list[theplace].score<open_list[thecomp].score)
                {
                  swap_2_list_references(theplace,thecomp);
                }
              else
                {
                  stopfloat=true;
                }
            }
          else
            // to comp2 exei mikrotero score
            {
              if (open_list[theplace].score<open_list[thecomp2].score)
                {
                  swap_2_list_references(theplace,thecomp2);
                }
              else
                {
                  stopfloat=true;
                }
            }
        }
    }
}


unsigned int GetNextNode()
{
  unsigned int retres=0;
  if (open_list_current_size>0)
    {
      retres=open_list[0].node;
      swap_2_list_references(0,open_list_current_size-1);
      FloatUpNodeListAtPlace(0);
      --open_list_current_size;
    }
  return retres;
}


void FindPath(unsigned int x1,unsigned int y1,unsigned int start_direction,unsigned int x2,unsigned int y2)
{
  if ( world_matrix==0 )
    {
      fprintf(stderr,"World Matrix Not Initialized\n");
      error_no=WORLDMATRIX_NOT_INITIALIZED;
      return;
    }
  if ( (x1>=matrix_size_x)|(y1>=matrix_size_y)|(x2>=matrix_size_x)|(y2>=matrix_size_y) )
    {
      error_no=INCORRECT_PARAMETERS;
      return;
    }
  if ( ( matrix_size_x==0) | ( matrix_size_y==0) )
    {
      fprintf(stderr,"World Matrix Size X/Y reported 0 ?\n");
      error_no=WORLDMATRIX_NOT_INITIALIZED;
      return;
    }

  last_run_time=GetTickCount();
  bool local_restrictions=false;

  //COMMIT MEMORY FOR REMAINING NODE LIST!
  if (open_list!=0)
    {
      delete [] open_list;
      open_list=0;
      openlist_size=0;
      open_list_current_size=0;
    }
  open_list=new NodeRef [openlist_size+1];
  open_list_current_size=0;

  if (open_list==0)
    {
      error_no=OUT_OF_MEMORY_WHILE_CREATING_LIST;
      fprintf(stderr,"No Memory!\n");
      return;
    } //No Memory , No results
  //COMMIT MEMORY FOR REMAINING NODE LIST!

  source_block=(y1*matrix_size_x)+(x1);
  if (source_block==0)
    {
      error_no=SOURCE_AT_ZERO;
      fprintf(stderr,"If the source is directly connected to the target there is a bug !\n");
      return;
    }

  target_block=(y2*matrix_size_x)+(x2);
  target_block_x=x2;
  target_block_y=y2;
  agent_source_direction=start_direction;

  bool done=false,out_of_bounds=false;
  unsigned int cur_x=0,cur_y=0,adj_1=0,adj_2=0,proc_node=0,last_node=0,node_direction=0;
  last_node=source_block;

  cur_x=x1; // Arxika vriskomaste sto source x
  cur_y=y1; // Arxika vriskomaste sto source y

  printf("Commencing Start of Pathfinding\n");

  unsigned int loops=0;
  while ( !done )
    {
      /*
              1 2 3
              4 5 6        Theoroume kentriko to node (5) ayto pou eimaste k apo kei k empros elegxoume ta geitonika!
              7 8 9
      */


      // ---------------------------------------------------------------------------------------------------------------
      // --------------------------------------------  KATHETA NODES!  -------------------------------------------------
      // ---------------------------------------------------------------------------------------------------------------

      for ( int i=1; i<=4; i++)
        {
          // EXOUME 4 DIAGWNIA GEITONIKA NODES!
          out_of_bounds=false;
          switch ( i )
            {
            case 1 : // PRWTO GEITONIKO NODE TO KATW DEKSIA ( 9 )
              adj_1=last_node+1;  // DILADI NODE 6
              adj_2=last_node+matrix_size_x; // DILADI NODE 8
              proc_node=adj_2+1; // DILADI NODE 9
              node_direction=9;
              break;
            case 2 :
              //adj_1=last_node+1;  // DILADI NODE 6 alla den xreiazetai giati einai precalc apo to proigoumeno :P elite programming..!
              adj_2=last_node-matrix_size_x; // DILADI NODE 2
              proc_node=adj_2+1; // DILADI NODE 3
              node_direction=3;
              break;
            case 3 :
              adj_1=last_node-1;  // DILADI NODE 4
              //adj_2=last_node-matrix_size_x; // DILADI NODE 2 alla den xreiazetai giati einai precalc apo to proigoumeno :P elite programming..!
              proc_node=adj_2-1; // DILADI NODE 1
              node_direction=1;
              break;
            case 4 :
              //adj_1=last_node-1;  // DILADI NODE 6 alla den xreiazetai giati einai precalc apo to proigoumeno :P elite programming..!
              adj_2=last_node+matrix_size_x; // DILADI NODE 7
              proc_node=adj_2+1; // DILADI NODE 7
              node_direction=7;
              break;
            default :
              out_of_bounds=true;
            } // <- Telos tou SWITCH

          if (proc_node>world_matrix_size)
            {
              out_of_bounds=true;  // ELEGXOUME GIA PITHANI EKSODO APO TO MATRIX MAS!
            }

          if ( (!out_of_bounds) && ( !world_matrix[proc_node].unpassable ) ) // ELEGXOS NODE ->
            {
              // To Block "9" pernietai opote koitamai an ena apo ta block 6,8 einai anoixto gia logous blocking kateytheian diagoniou perasmatos
              local_restrictions=false;
              if (block_perfect_diagonal)
                {
                  if ( (world_matrix[adj_1].unpassable) && (world_matrix[adj_2].unpassable) )
                    {
                      local_restrictions=true;  // Den epitrepoume epektasi kata teleia diagwnia tropo
                    }
                }


              if ( !local_restrictions )
                {
                  ExpandNodeFromNode(last_node,proc_node,node_direction,false); // False ennooume oti einai diagwnia kinisi!
                  if (proc_node==target_block)
                    {
                      printf("Target Just Reached #1 :D\n"); /* Vrikame to target! :D */
                      done=true;
                    }
                  else
                    {
                      OpenNode(proc_node);
                    }

                }
            }       // <- TELOS ELEGXOY NODE


        } // <- TELOS LOOP DIAGWNIWN NODES!



      // ---------------------------------------------------------------------------------------------------------------
      // --------------------------------------------  ORIZONTIA NODES!  -----------------------------------------------
      // ---------------------------------------------------------------------------------------------------------------
      if (!done)
        {
          for ( int i=1; i<=4; i++)
            {
              // EXOUME 4 ORIZONTIA GEITONIKA NODES!
              out_of_bounds=false;
              switch ( i )
                {
                case 1 : // PRWTO GEITONIKO NODE TO DEKSIA ( 6 )
                  proc_node=last_node+1; // DILADI NODE 6
                  node_direction=6;
                  break;
                case 2 :
                  proc_node=last_node-1; // DILADI NODE 4
                  node_direction=4;
                  break;
                case 3 :
                  proc_node=last_node-matrix_size_x; // DILADI NODE 2
                  node_direction=2;
                  break;
                case 4 :
                  proc_node=last_node+matrix_size_x; // DILADI NODE 8
                  node_direction=8;
                  break;
                default :
                  out_of_bounds=true;
                } // <- Telos tou SWITCH

              if (proc_node>world_matrix_size)
                {
                  out_of_bounds=true;  // ELEGXOUME GIA PITHANI EKSODO APO TO MATRIX MAS!
                }

              if ( (!out_of_bounds) && ( !world_matrix[proc_node].unpassable ) ) // ELEGXOS NODE ->
                {
                  ExpandNodeFromNode(last_node,proc_node,node_direction,true); // False ennooume oti einai orizontia kinisi!
                  if (proc_node==target_block)
                    {
                      printf("Target Just Reached #2 :D\n"); /* Vrikame to target! :D */
                      done=true;
                    }
                  else
                    {
                      OpenNode(proc_node);
                    }


                }       // <- TELOS ELEGXOY NODE

            } // <- TELOS LOOP ORIZONTIWN NODES!
        } // TELOS ELEGXOU GIA TO KATA POSO TELEIWSAME SE DIAGWNIA METAKINISI..




      //FORTWSI KAINOURGIOU NODE PROS ANAZITISI!
      if (!done)
        {
          if (open_list_current_size>0) //AN EXOUME KOMVO PROS PROSTHIKI
            {
              last_node=GetNextNode(); // to idio einai :P -> open_list[0].node;
              unsigned int cur_x=last_node % matrix_size_x; // Ypologizoume tin syntetagmeni x , y
              unsigned int cur_y=last_node / matrix_size_x; // Ypologizoume tin syntetagmeni x , y
            }
          else
            {
              done=true;  // AN DEN YPARXEI ALLOS KOMVOS PROS PROSTHIKI TELEIWSAME!
            }
        }

      ++loops;
      printf("%d\n",loops);

    } // TELOS KOMVWN PROS EPEKSERGASIA!

//MessageBox(0,"Finished Search","Trace",0);
  printf("Finished Search!\n");

  /*
         LOGIKA TWRA THA PREPEI NA EXOUME ENA PATH PAIRNONTAS TON TELIKO KOMVO K KSEKINONTAS NA PIGAINOUME PISW!!
  */

  //PROSOXI BUG AN TO SOURCE EINAI CONNECTED STO TARGET!
  if ( (target_block>world_matrix_size) || (target_block<1) )
    {
      error_no=TARGET_OUT_OF_BOUNDS;
      fprintf(stderr,"Out of Bounds target..\n");
    }
  else
    if (world_matrix[target_block].parent_node!=0)
      {
        error_no=PATH_ESTABLISHED;
        printf("Path Established !\n");
      }
    else
      {
        error_no=NO_PATH_ESTABLISHED;
      }





  if (open_list!=0)
    {
      delete[] open_list;
      open_list=0;
      openlist_size=0;
      open_list_current_size=0;
    }
  last_run_time=GetTickCount()-last_run_time;

}




