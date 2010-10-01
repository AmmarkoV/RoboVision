#include "LogoHighLevelPath.h"
#include <math.h>
#include <stdio.h>

const double pi = 3.141592653589793238462643;

inline unsigned int _abs(signed int num)
{
  if ( num < 0 )
    {
      return 0-num;
    }
  return num;
}


unsigned int ConvertPathToLogo(TraceNode * str8nodes,unsigned int &str8nodes_size,char * storage,unsigned int storagelen)
{

     double prev_angle=0;


    if ( str8nodes_size <= 1 ) { return 0; } // No sense in returning path with one or none steps

     FILE *p = NULL;
     char *file = "logo.txt";
     char buffer[255];
     size_t len = 0;
     p = fopen(file, "w");
     if (p== NULL) {
                      printf("Error in opening a file..", file);
                   }


    unsigned int oldx,oldy,x,y;
    double distance,angle;


    fprintf(p,"reset\n");
    fprintf(p,"direction %f\n",prev_angle);

    oldx = str8nodes[0].nodex , oldy = str8nodes[0].nodey;
    for ( int i=1; i<str8nodes_size; i++ )
     {
       x = str8nodes[i].nodex , y = str8nodes[i].nodey;

       if ( x!=oldx )
        {
         distance = sqrt( (y-oldy)*(y-oldy) + (x-oldx)*(x-oldx) );
         angle =   atan ((double) _abs(y-oldy)/_abs(x-oldx)) * 180 / pi;

         if ( angle-prev_angle < 0 )
           fprintf(p,"turnleft %f\n",- ( angle-prev_angle ) );  else
           fprintf(p,"turnright %f\n",angle-prev_angle);

         prev_angle=angle;

         if ( distance < 0 )
           fprintf(p,"backward %f\n",distance); else
           fprintf(p,"forward %f\n",distance);

        }

       oldx = x , oldy = y;
     }

    fclose(p);
    return 0;
}


int PrintoutHTML(char * filename,unsigned int world_x,unsigned int world_y,NodeData * world)
{
   FILE * pFile;
   pFile = fopen ("printout.html","w");

   if ( pFile != 0 )
   {
     fprintf (pFile, "<html><head></head><body>");
     fprintf (pFile, "<table>\n");

      int x,y,ptr=0;
       for ( y=0; y<world_x; y++ )
       {
         fprintf (pFile, "<tr>\n   ");
        for ( x=0; x<world_y; x++ )
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
