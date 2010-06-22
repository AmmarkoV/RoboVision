#include "Map.h"
#include <stdio.h>
// MAP IS SUPPOSED TO BE A LOW LEVEL A* IMPLEMENTATION THAT SOLVES
// MAZES AND FINDS THE BEST ROUTE
// written by Ammar Qammaz as part of the GuardDog Project!

Map::Map(unsigned int size_x,unsigned int size_y)
{
  error_number=0;
  add_neighbors_mode = 0; // ADD NEIGHBORS WHILE ADDING!
  diagonal_penalty=0;
  horizontal_penalty=0;
  turning_penalty=1;
  block_perfect_diagonal=true;

  printf("Creating New Map World\n");
  // WORLD INITIALIZATION
  world_x=size_x;
  world_y=size_y;
  world_size=(size_x+1)*(size_y+1);
  world=new NodeData[world_size+1];
  world_neighbors=new NodeNeighborsCount[world_size+1];


  NodeData empty={0};
  for ( unsigned int i=0; i<world_size; i++) world[i]=empty;
  // WORLD INITIALIZATION

  NodeNeighborsCount emptyneigh={0};
  for (unsigned int i=0; i<world_size; i++) world_neighbors[i]=emptyneigh;

  printf("Creating New A* List \n");
  // A* LIST INITIALIZATION
    openlist=0 , openlist_current_size=0;


    openlist_size=(unsigned int) world_size / 2; // < - MAX LIST SIZE , IT HAS A BIG EFFECT IN ALGORITHM SUCCESS RATIO!
    openlist=new NodeRef [openlist_size+1];

  // A* LIST INITIALIZATION

   pathcomputer = new AStarCompute(world,world_x,world_y,world_neighbors,openlist,openlist_size);
}

Map::~Map()
{
   if (world!=0)
    { delete[] world , world=0; }

   if (openlist!=0)
    { delete[] openlist , openlist=0 , openlist_current_size=0; }

   if (world_neighbors!=0)
    { delete[] world_neighbors; }

   delete pathcomputer;
}



void Map::SetActorCurrentPosition(unsigned int ptx,unsigned int pty,unsigned int heading)
{
  us.x=ptx , us.y=pty , us.heading=heading;
}

void Map::SetActorPhysicalSize(unsigned int units)
{
  us.logical_size = units;
  us.size_to_sub = (unsigned int) units / 2;
}

unsigned int Map::GetObjectAt(unsigned int ptx,unsigned int pty)
{
  unsigned int mem = (pty*world_x)+(ptx);
  if ( mem < world_size ) return world[mem].unpassable;
  printf("GetObjectAt requested data out of world sizes\n");
  return 0;
}

unsigned int Map::GetNeighborsAt(unsigned int ptx,unsigned int pty)
{
  unsigned int mem = (pty*world_x)+(ptx);
  if ( mem < world_size ) return world_neighbors[mem].total;
  printf("GetNeighborsAt requested data out of world sizes\n");
  return 0;
}

void inline Map::AddNeighbors(unsigned int x,unsigned int y,unsigned char state)
{
  if ( add_neighbors_mode == 1 ) { return; }
  if ( us.logical_size <= 1 ) { return ; }

  unsigned int x1,y1,x2,y2;
  if ( x - us.size_to_sub <= 0 )  x1 = 0;  else  x1 = x - us.size_to_sub;
  if ( y - us.size_to_sub <= 0 )  y1 = 0;  else  y1 = y - us.size_to_sub;

  if ( x + us.size_to_sub >= world_x )  x2 = world_x-1;  else  x2 = x + us.size_to_sub;
  if ( y + us.size_to_sub >= world_y )  y2 = world_y-1;  else  y2 = y + us.size_to_sub;


  unsigned int mem =0;
  // ILITHIOS K ARGOS KWDIKAS , TO BE OPTIMIZED :P
  for (unsigned int cur_x=x1; cur_x<=x2; cur_x++ )
  {
     for (unsigned int cur_y=y1; cur_y<=y2; cur_y++ )
     {
       mem = (cur_y*world_x)+(cur_x);
       if ( state == 0 ) {
                           if ( world_neighbors [ mem ] . total>0 )
                           {
                               --world_neighbors [ mem ] . total;
                           }
                         } else
       if ( state == 1 ) {
                           ++ world_neighbors [ mem ] . total;
                         }

     }
  }
  //printf(" done\n");

}

void inline Map::CalculateAllNeighbors()
{
  if ( add_neighbors_mode == 0 ) { return; }
  fprintf(stderr,"CalculateAllNeighbors not implemented \n");
  return;
  if ( us.logical_size <= 1 ) { return ; }

  printf("PreCalculating Neighbors for faster perfomance .. ");
  unsigned int ptr=0;
  for ( unsigned int x = 1; x< world_x; x++ )
  {
   for ( unsigned int y = 1; y< world_y; y++ )
   {
    if ( world[ptr].unpassable != 0 )
     {
       // AddNeighbors();
     }
    ++ptr;
   }
  }
  //printf(" done\n");

}


unsigned int Map::SetObjectAt(unsigned int ptx,unsigned int pty,unsigned int obj)
{
  unsigned int mem = (pty*world_x)+(ptx);
   if ( mem < world_size )
   {
     if ( ( world[mem].unpassable == 0 ) && ( obj == 0 ) ) {} else // gia na min ginontai polla adds!
     if ( ( world[mem].unpassable == 1 ) && ( obj == 1 ) ) {} else // gia na min ginontai polla adds!
     {
      world[mem].unpassable=obj;
      AddNeighbors(ptx,pty,obj);
     }
     return 0;
   }
  printf("GetObjectAt requested data out of world sizes\n");
  return 0;
}



unsigned int Map::FindPathTo(unsigned int to_x,unsigned int to_y,unsigned int timelimit_ms)
{

  for ( unsigned int i=0; i<world_size; i++)
  { world[i].opened=false;
    world[i].parent_node = 0;
    world[i].arrived_direction = 0;
    world[i].score = 0;
  }

  CalculateAllNeighbors();
  return pathcomputer->FindPath(us.x,us.y,us.heading,us.logical_size,to_x,to_y,timelimit_ms);
}



unsigned int Map::GetRoutePoint(unsigned char level,unsigned int pt_num,unsigned int &from_x,unsigned int &from_y)
{
  return pathcomputer->GetRoutePoint(level,pt_num,from_x,from_y);
}




void Map::Clear()
{
  NodeData empty={0};
  for ( unsigned int i=0; i<world_size; i++) world[i]=empty;
  return;
}

