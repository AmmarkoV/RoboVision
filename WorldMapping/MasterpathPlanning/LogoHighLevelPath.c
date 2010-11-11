#include "LogoHighLevelPath.h"
#include <math.h>
#include <stdio.h>

#define pi 3.141592653589793238462643

inline unsigned int _abs2(signed int num)
{
  if ( num < 0 )
    {
      return 0-num;
    }
  return num;
}


unsigned int ConvertPathToLogo(struct TraceNode * str8nodes,unsigned int *str8nodes_size,char * storage,unsigned int storagelen)
{
     if ( *str8nodes_size <= 1 ) { return 0; } // No sense in returning path with one or none steps
     double prev_angle=0;


     FILE *p = fopen("logo.txt", "w");
     if (p == 0) {
                      fprintf(stderr,"Error in opening a file for logo high level output..\n");
                      return 0;
                 }


    fprintf(stderr,"Starting logo output..\n");
    unsigned int oldx,oldy,x,y;
    double distance=0,angle=0;


    fprintf(p,"#Starting path with a total of %u nodes\n",*str8nodes_size);
    fprintf(p,"reset\n");
    fprintf(p,"direction %f\n",prev_angle);

    oldx = str8nodes[0].nodex , oldy = str8nodes[0].nodey;
    int i=1;
    for ( i=1; i<*str8nodes_size; i++ )
     {
       x = str8nodes[i].nodex , y = str8nodes[i].nodey;

       if ( x!=oldx )
        {
         distance = sqrt( (y-oldy)*(y-oldy) + (x-oldx)*(x-oldx) );
         angle =   atan ((double) _abs2(y-oldy)/_abs2(x-oldx)) * 180 / (double) pi;

         if ( angle-prev_angle < 0 ) { fprintf(p,"turnleft %f\n",- ( angle-prev_angle ) ); } else
                                     { fprintf(p,"turnright %f\n",angle-prev_angle); }

         prev_angle=angle;

         if ( distance < 0 ) { fprintf(p,"backward %f\n",distance); } else
                             { fprintf(p,"forward %f\n",distance); }

        }

       oldx = x , oldy = y;
     }

    fclose(p);
    return 1;
}


int PrintoutHTML(char * filename,unsigned int actor_x,unsigned int actor_y,unsigned int world_x,unsigned int world_y,unsigned int visible_area,struct NodeData * world)
{

   fprintf(stderr,"Printing out HTML map of world , code is sloppy , check border cases\n");
   FILE * pFile;
   pFile = fopen (filename,"w");

   unsigned int startx=0,starty=0,endx=world_x,endy=world_y;
   if (actor_x-visible_area>=0) { startx=actor_x-visible_area; }
   if (actor_y-visible_area>=0) { starty=actor_y-visible_area; }

   if ( startx+visible_area<world_x) { endx = startx+visible_area; }
   if ( starty+visible_area<world_y) { endy = starty+visible_area; }

   fprintf(stderr,"HTML Viewable Map size %u,%u - %u,%u \n",startx,starty,endx,endy);

   if ( pFile != 0 )
   {
     fprintf (pFile, "<html><head></head><body>");
     fprintf (pFile, "<table>\n");

      int x,y,ptr=0;
       for ( y=starty; y<endy; y++ )
       {
        ptr = world_x * y + x ;
        fprintf (pFile, "<tr>\n   ");
        for ( x=startx; x<endx; x++ )
         {

           if ( world[ptr].unpassable!=0 )
             {
               fprintf (pFile, "<td bgcolor=\"#000000\">");
             } else
             {
               fprintf (pFile, "<td >"); /*bgcolor=\"#FFFFFF\"*/
             }
             fprintf (pFile, "&nbsp;");

           fprintf (pFile, "</td>");
           ++ptr;
         }
         fprintf (pFile, "</tr>\n");
       }


     fprintf (pFile, "</table>\n");
     fprintf (pFile, " \n");
     fprintf (pFile, "</body></html>");
   }

   fclose (pFile);
  return 1;
}
