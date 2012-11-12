#include "tools.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char FileExistsVideoInput(char * filename)
{
 FILE *fp = fopen(filename,"r");
 if( fp ) { /* exists */
            fclose(fp);
            return 1;
          }
          else
          { /* doesnt exist */ }
 return 0;
}


int IncreasePriority()
{
     /*We want higher priority now..! :)*/
     if ( nice(-4) == -1 ) { fprintf(stderr,"Error increasing priority on main video capture loop\n");} else
                           { fprintf(stderr,"Increased priority \n"); return 1; }
     return 0;
}
