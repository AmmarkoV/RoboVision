#include "Points3d.h"



struct TransformationMatrix left_homography;
struct TransformationMatrix right_homography;

struct TransformationMatrix total_left_rotation;
struct TransformationMatrix total_right_rotation;

struct TransformationMatrix left_rotation;
struct TransformationMatrix right_rotation;


struct TransformationMatrix left_translation;
struct TransformationMatrix right_translation;

struct TransformationMatrix left_rotation_and_translation;
struct TransformationMatrix right_rotation_and_translation;

struct TransformationMatrix total_left_rotation_and_translation;
struct TransformationMatrix total_right_rotation_and_translation;

int CopyMatrixToMatrix(struct TransformationMatrix * target_matrix,struct TransformationMatrix * matrix)
{
    target_matrix->rows = matrix->rows;
    target_matrix->columns = matrix->columns;

    int i=0;
    for (i=0; i<matrix->rows*matrix->columns; i++) { target_matrix->item[i]=matrix->item[i]; }
    return 1;
}


int SaveTransformationMatrixToFile(char * filename,struct TransformationMatrix * matrix)
{
    FILE *fd=0;
    fd = fopen(filename,"w");

    if (fd!=0)
	{
      unsigned int i=0;
       for ( i=0; i< matrix->columns*matrix->rows; i++ )
        {
          fprintf(fd,"%f\n",matrix->item[i]);
        }

       if( (matrix->columns == 3 ) &&  (matrix->rows==3 ) )
        {
         fprintf(fd,"m1=[%f,%f,%f;%f,%f,%f;%f,%f,%f];\n",matrix->item[0] , matrix->item[1] , matrix->item[2]
                                                        ,matrix->item[3] , matrix->item[4] , matrix->item[5]
                                                        ,matrix->item[6] , matrix->item[7] , matrix->item[8]  );
        } else
       if( (matrix->columns == 4 ) &&  (matrix->rows==4 ) )
        {
          fprintf(fd,"m1=[%f,%f,%f,%f;%f,%f,%f,%f;%f,%f,%f,%f;%f,%f,%f,%f];\n",matrix->item[0] , matrix->item[1] , matrix->item[2] , matrix->item[3] ,
                                                                               matrix->item[4] , matrix->item[5] , matrix->item[6] , matrix->item[7] ,
                                                                               matrix->item[8] , matrix->item[9] , matrix->item[10] , matrix->item[11] ,
                                                                               matrix->item[12] , matrix->item[13] , matrix->item[14] , matrix->item[15] );
        }

	  fclose(fd);
	  return 1;
	}
  return 0;
}



int ClearTransformationMatrix(struct TransformationMatrix * matrix)
{
   if ( matrix== 0 ) { return 0; }
   matrix->columns=0;
   matrix->rows=0;
   int i=0;
   for ( i=0; i<16; i++ ) { matrix->item[i]=0.0; }
   return 1;
}



int Multiply4x4Matrices(struct TransformationMatrix * target_matrix,struct TransformationMatrix * mult_1,struct TransformationMatrix * mult_2)
{
    if ( ( mult_1->columns != 4 ) ||
         ( mult_1->rows != 4 ) )
       {
            fprintf(stderr,"Multiply4x4Matrices : Input matrix 1 is not 4x4  but %ux%u \n",mult_1->rows,mult_1->columns);
            return 0;
       }

    if (
         ( mult_2->columns != 4 ) ||
         ( mult_2->rows != 4 )
       )
       {
            fprintf(stderr,"Multiply4x4Matrices : Input matrix 2 is not 4x4 but %ux%u \n",mult_2->rows,mult_2->columns);
            return 0;
       }


    /*        4x4          *        4*4         =               4*4
     _______________
    | a   b   c   d |       | A   B   C   D |         | Q   R   S   T |
    | e   f   g   h |       | E   F   G   H |         | U   V   W   X |
    | i   j   k   l |   *   | I   J   K   L |   =     | Y   Z   !   @ |
    | m   n   o   p |       | M   N   O   P |         | #   $   %   ^ |
     ---------------
*/

target_matrix->rows=4;
target_matrix->columns=4;

//    Q = a*A + b*E + c*I + d*M
 target_matrix->item[0]= mult_1->item[0] * mult_2->item[0] + mult_1->item[1] * mult_2->item[4] + mult_1->item[2] * mult_2->item[8] + mult_1->item[3] * mult_2->item[12];
//    R = a*B + b*F + c*J + d*N
 target_matrix->item[1]= mult_1->item[0] * mult_2->item[1] + mult_1->item[1] * mult_2->item[5] + mult_1->item[2] * mult_2->item[9] + mult_1->item[3] * mult_2->item[13];
//    S = a*C + b*G + c*K + d*O
 target_matrix->item[2]= mult_1->item[0] * mult_2->item[2] + mult_1->item[1] * mult_2->item[6] + mult_1->item[2] * mult_2->item[10] + mult_1->item[3] * mult_2->item[14];
//    T = a*D + b*H + c*L + d*P
 target_matrix->item[3]= mult_1->item[0] * mult_2->item[3] + mult_1->item[1] * mult_2->item[7] + mult_1->item[2] * mult_2->item[11] + mult_1->item[3] * mult_2->item[15];

//    U = e*A + f*E + g*I + h*M
 target_matrix->item[4]= mult_1->item[4] * mult_2->item[0] + mult_1->item[5] * mult_2->item[4] + mult_1->item[6] * mult_2->item[8] + mult_1->item[7] * mult_2->item[12];
//    V = e*B + f*F + g*J + h*N
 target_matrix->item[5]= mult_1->item[4] * mult_2->item[1] + mult_1->item[5] * mult_2->item[5] + mult_1->item[6] * mult_2->item[9] + mult_1->item[7] * mult_2->item[13];
//    W = e*C + f*G + g*K + h*O
 target_matrix->item[6]= mult_1->item[4] * mult_2->item[2] + mult_1->item[5] * mult_2->item[6] + mult_1->item[6] * mult_2->item[10] + mult_1->item[7] * mult_2->item[14];
//    X = e*D + f*H + g*L + h*P
 target_matrix->item[7]= mult_1->item[4] * mult_2->item[3] + mult_1->item[5] * mult_2->item[7] + mult_1->item[6] * mult_2->item[11] + mult_1->item[7] * mult_2->item[15];

//    Y = i*A + j*E + k*I + l*M
 target_matrix->item[8]= mult_1->item[8] * mult_2->item[0] + mult_1->item[9] * mult_2->item[4] + mult_1->item[10] * mult_2->item[8] + mult_1->item[11] * mult_2->item[12];
//    Z = i*B + j*F + k*J + l*N
 target_matrix->item[9]= mult_1->item[8] * mult_2->item[1] + mult_1->item[9] * mult_2->item[5] + mult_1->item[10] * mult_2->item[9] + mult_1->item[11] * mult_2->item[13];
//    ! = i*C + j*G + k*K + l*O
 target_matrix->item[10]= mult_1->item[8] * mult_2->item[2] + mult_1->item[9] * mult_2->item[6] + mult_1->item[10] * mult_2->item[10] + mult_1->item[11] * mult_2->item[14];
//    @ = i*D + j*H + k*L + l*P
 target_matrix->item[11]= mult_1->item[8] * mult_2->item[3] + mult_1->item[9] * mult_2->item[7] + mult_1->item[10] * mult_2->item[11] + mult_1->item[11] * mult_2->item[15];

//    # = m*A + n*E + o*I + p*M
 target_matrix->item[12]= mult_1->item[12] * mult_2->item[0] + mult_1->item[13] * mult_2->item[4] + mult_1->item[14] * mult_2->item[8] + mult_1->item[15] * mult_2->item[12];
//    $ = m*B + n*F + o*J + p*N
 target_matrix->item[13]= mult_1->item[12] * mult_2->item[1] + mult_1->item[13] * mult_2->item[5] + mult_1->item[14] * mult_2->item[9] + mult_1->item[15] * mult_2->item[13];
//    % = m*C + n*G + o*K + p*O
 target_matrix->item[14]= mult_1->item[12] * mult_2->item[2] + mult_1->item[13] * mult_2->item[6] + mult_1->item[14] * mult_2->item[10] + mult_1->item[15] * mult_2->item[14];
//    ^ = m*D + n*H + o*L + p*P
 target_matrix->item[15]= mult_1->item[12] * mult_2->item[3] + mult_1->item[13] * mult_2->item[7] + mult_1->item[14] * mult_2->item[11] + mult_1->item[15] * mult_2->item[15];

return 1;
}


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

     _______________
    | 0   1   2   3 |       | .x |
    | 4   5   6   7 |       | .y |
    | 8   9   10  11|   *   | .z |   =      | new_x  new_y new_z new_i |
    | 12  13  14  15|       |  1 |
     ---------------


     new_x  =  a*x + b*y + c*z + d*1
     new_y  =  e*x + f*y + g*z + h*1
     new_z  =  i*x + j*y + k*z + l*1
     new_i  =  m*x + n*y + o*z + p*1
*/


   struct TransformationMatrix * M = rotation_and_translation_matrix;


   int i=0;
   float old_x,old_y,old_z;
   float new_x,new_y,new_z,new_i;
   while ( i < list->current_features )
     {
        old_x=list->list[i].x;
        old_y=list->list[i].y;
        old_z=list->list[i].z;

        new_i=M->item[12]*old_x  + M->item[13]*old_y + M->item[14]*old_z + M->item[15];
        if ( (new_i==0.0) || (new_i==1.0) )
         {
          new_x=(M->item[0]*old_x  + M->item[1]*old_y + M->item[2]*old_z + M->item[3]);
          new_y=(M->item[4]*old_x  + M->item[5]*old_y + M->item[6]*old_z + M->item[7]);
          new_z=(M->item[8]*old_x  + M->item[9]*old_y + M->item[10]*old_z + M->item[11]);
         }   else
         {
          new_x=(M->item[0]*old_x  + M->item[1]*old_y + M->item[2]*old_z + M->item[3])/new_i;
          new_y=(M->item[4]*old_x  + M->item[5]*old_y + M->item[6]*old_z + M->item[7])/new_i;
          new_z=(M->item[8]*old_x  + M->item[9]*old_y + M->item[10]*old_z + M->item[11])/new_i;
         }

        list->list[i].x=new_x;
        list->list[i].y=new_y;
        list->list[i].z=new_z;
        ++i;
     }


   return 1;
}


int InitCameraPose()
{
 ClearTransformationMatrix(&total_left_rotation);
 total_left_rotation.rows=4 , total_left_rotation.columns=4;
 total_left_rotation.item[0]=1.0;
 total_left_rotation.item[5]=1.0;
 total_left_rotation.item[10]=1.0;
 total_left_rotation.item[15]=1.0;

 ClearTransformationMatrix(&total_right_rotation);
 total_right_rotation.rows=4 , total_right_rotation.columns=4;
 total_right_rotation.item[0]=1.0;
 total_right_rotation.item[5]=1.0;
 total_right_rotation.item[10]=1.0;
 total_right_rotation.item[15]=1.0;

 return 1;
}
