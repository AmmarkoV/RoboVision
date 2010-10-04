#include "NormalizePath.h"
#include <stdio.h>
#include <stdlib.h>
int debug_normalize_path = 0;

inline unsigned int _abs(signed int num)
{
  if ( num < 0 )
    {
      return 0-num;
    }
  return num;
}


//---------------------------------------
//Bresenham general algorithm !
//---------------------------------------
unsigned char ClearLinePath(struct NodeData * world,struct NodeNeighborsCount *world_neighbors,unsigned int world_x,unsigned int world_size,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
  unsigned int the_node;
  if ( debug_normalize_path ==1 ) printf("Running bresenham algorithm from %d,%d to %d,%d ... ",x1,y1,x2,y2);
  if ( ((y1*world_x)+(x1)>=world_size) || ((y2*world_x)+(x2)>=world_size) )
    {
      return 0; // This means not clear
    }

  int dx, dy, i, e;
  int incx, incy, inc1, inc2 , x , y;

  dx = x2 - x1 , dy = y2 - y1;

  if (dx < 0) dx = -dx;
  if (dy < 0) dy = -dy;

  incx = 1;
  if (x2 < x1) incx = -1;

  incy = 1;
  if (y2 < y1) incy = -1;

  x=x1 , y=y1;

  if (dx > dy)
    {
      // draw_pixel(x,y)
      the_node = (y*world_x)+(x);
      if (  (world[the_node].unpassable==1) || (world_neighbors[the_node].total>=1) )
        {
          if ( debug_normalize_path ==1 ) printf (" not clear\n");
          return 0; // This means not clear
        }
      // draw_pixel(x,y)

      e = 2*dy - dx;
      inc1 = 2*( dy -dx);
      inc2 = 2*dy;
      for (i = 0; i < dx; i++)
        {
          if (e >= 0)
            {
              y += incy;
              e += inc1;
            }
          else e += inc2;
          x += incx;


          // draw_pixel(x,y)
          the_node = (y*world_x)+(x);
          if (  (world[the_node].unpassable==1) || (world_neighbors[the_node].total>=1) )
            {
              if ( debug_normalize_path ==1 ) printf (" not clear\n");
              return 0; // This means not clear
            }
          // draw_pixel(x,y)
        }
    }
  else
    {
      // draw_pixel(x,y)
      the_node = (y*world_x)+(x);
      if (  (world[the_node].unpassable==1) || (world_neighbors[the_node].total>=1) )
        {
          if ( debug_normalize_path ==1 ) printf (" not clear\n");
          return 0; // This means not clear
        }
      // draw_pixel(x,y)

      e = 2*dx - dy;
      inc1 = 2*( dx - dy);
      inc2 = 2*dx;
      for (i = 0; i < dy; i++)
        {
          if (e >= 0)
            {
              x += incx;
              e += inc1;
            }
          else e += inc2;
          y += incy;

          // draw_pixel(x,y)
          the_node = (y*world_x)+(x);
          if (  (world[the_node].unpassable==1) || (world_neighbors[the_node].total>=1) )
            {
              if ( debug_normalize_path ==1 ) printf (" not clear\n");
              return 0; // This means not clear
            }
          // draw_pixel(x,y)
        }
    }
  if ( debug_normalize_path ==1 ) printf (" clear\n");
  return 1; // This means clear
}

struct TraceNode * GetANormalizedLineFromNodes(struct TraceNode * rawnodes,unsigned int rawnodes_size,unsigned int *str8nodes_size)
{
  if ( rawnodes_size  <= 1 ) { return 0; }

  *str8nodes_size=0;

  struct TraceNode tmpnodes[rawnodes_size]; // IN WORST CASE WE WILL HAVE THE SAME NUMBER OF NODES AS RAW NODES

  unsigned int new_diff_x,new_diff_y;
  unsigned int last_diff_x=666,last_diff_y=666;
  unsigned int last_i=0; // STO PRWTO LOOP EXOUME LAST_I == i == 0

  unsigned int i=0;
  for (i=0; i < rawnodes_size; i++ )
    {
      new_diff_x = _abs( rawnodes[last_i].nodex - rawnodes[i].nodex );
      new_diff_y = _abs( rawnodes[last_i].nodey - rawnodes[i].nodey );

      if ( ( new_diff_x != last_diff_x ) | ( new_diff_y != last_diff_y ) )
        {
          tmpnodes[*str8nodes_size].nodex = rawnodes[i].nodex;
          tmpnodes[*str8nodes_size].nodey = rawnodes[i].nodey;
          tmpnodes[*str8nodes_size].score = rawnodes[i].score;
          ++*str8nodes_size;

          last_diff_x = new_diff_x;
          last_diff_y = new_diff_y;
          last_i = i;
        }
    }

  // COPY IT TO THE NEW TRACENODE MEMORY!
  struct TraceNode * str8nodes = (struct TraceNode *) malloc( sizeof(struct TraceNode) * ( *str8nodes_size+1 ) );
  if ( str8nodes == 0 )
    {
      fprintf(stderr,"Could not allocate memory for 1st level Lines \n");
    }
  else
    {
      unsigned int i=0;
      for (i=0; i<*str8nodes_size; i++)
        {
          str8nodes[i].nodex=tmpnodes[i].nodex;
          str8nodes[i].nodey=tmpnodes[i].nodey;
          str8nodes[i].score=tmpnodes[i].score;
        }
    }

  return str8nodes;

}

void GetTheShortestNormalizedLineFromNodes(struct NodeData * world,struct NodeNeighborsCount *world_neighbors,unsigned int world_x,unsigned int world_size,struct TraceNode * str8nodes,unsigned int *str8nodes_size)
{
  unsigned int str8nodes_size_start;
  str8nodes_size_start = *str8nodes_size;

  unsigned int move_steps;
  signed int skip_steps;
  unsigned int x1,y1,x2,y2;
  if ( debug_normalize_path ==1 ) printf ("\n\nCompressing Route!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n\n");
  unsigned int swaps=0;
  unsigned int i;
  for (i=*str8nodes_size-1; i > 0; i-- ) // To i<=0 Dn exei noima giati dn mporw na zipparw ton prwto komvo , prepei na yparxei !!
    {
        if ( debug_normalize_path ==1 ) printf ("Checking for compression to %d ! \n",i);
        move_steps=0;
        x2=str8nodes[i].nodex , y2=str8nodes[i].nodey; // <- Test Target State


       skip_steps=i-1; // ksekiname apo tin timi 1 !
       while ( (skip_steps>0) && ( i-skip_steps > 0 ) )// To  i-skip_steps<=0 Dn exei noima giati dn mporw na zipparw ton prwto komvo , prepei na yparxei !!
       {
        if ( debug_normalize_path ==1 ) printf (" ... range up to %d ! \n",i-skip_steps);
        x1=str8nodes[i-skip_steps].nodex , y1=str8nodes[i-skip_steps].nodey; // <- Test Start State
        if (
               ClearLinePath(world,world_neighbors,world_x,world_size,x1,y1,x2,y2) == 0
           )
           {  } else
           { move_steps=skip_steps;
             break; }
         --skip_steps;
       }

       if ( move_steps > 0 )
        {
          if ( debug_normalize_path ==1 ) printf("I can save %d steps from %d to %d  ( ",move_steps,i,i-move_steps);
          //str8nodes[i-move_steps].nodex=x2 , str8nodes[i-move_steps].nodey=y2;

          int z=0;
          for ( z=0; z<=swaps; z++ )
            {
              if ( debug_normalize_path ==1 ) printf("Mov %d -> %d  ( ",i+z,i-move_steps+z);
              str8nodes[i-move_steps+z].nodex=str8nodes[i+z].nodex , str8nodes[i-move_steps+z].nodey=str8nodes[i+z].nodey;
            }

          ++swaps;
          *str8nodes_size -= move_steps;
          i = i-move_steps; // oxi +1 giati sigoura den tha exoume tha kanoume ena i-- logw tou for loop!
          if ( debug_normalize_path ==1 ) printf(" ) Done!\n");

        }
    }
 // DEBUG PRINTOUT!
  if ( str8nodes_size_start != *str8nodes_size )
    {
       if ( debug_normalize_path ==1 )
       {
        printf("GetTheShortestNormalizedLineFromNodes compressed %d \n",str8nodes_size_start-*str8nodes_size);
        int i=0;
        for (i=0; i <str8nodes_size_start; i++ )
         {
           printf("( %d ) %d,%d ",i,str8nodes[i].nodex,str8nodes[i].nodey);
         }
        printf("\n\n");
        for (i=0; i <*str8nodes_size; i++ )
         {
           printf("( %d ) %d,%d ",i,str8nodes[i].nodex,str8nodes[i].nodey);
         }
        }
    }
  // DEBUG PRINTOUT!
 return;
}
