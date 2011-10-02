#include "Points3d.h"


int Multiply3DPointsWithMatrix(struct FeatureList * list,struct TransformationMatrix * rotation_and_translation_matrix)
{
    if  (!FeatureListIsOk(list)) { fprintf(stderr,"Multiply3DPointsWithMatrix called with a zero list \n");  return 0; }
    if ( list->current_features == 0 ) { fprintf(stderr,"Cannot Multiply3DPointsWithMatrix from feature list , feature list is empty\n"); return 0; }

    // WITH HOMOGENOUS COORDINATES
   //SUPPOSE 4x4 MATRIX



/*        4x4          *    4*1     =               1*4
     _______________
    | a   b   c   d |       | x |
    | e   f   g   h |       | y |
    | i   j   k   l |   *   | z |   =      | new_x  new_y new_z new_i |
    | m   n   o   p |       | 1 |
     ---------------
*/




   int i=0;
   float new_x,new_y,new_z,new_i;
   while ( i < list->current_features )
     {

        new_x=
        new_y=
        new_z=
        new_i=

        ++i;
     }


   return 1;
}
