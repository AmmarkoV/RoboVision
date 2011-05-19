#include "LinearAlgebra.h"
#include "VisionMemory.h"
#include <stdio.h>
#include <gsl/gsl_linalg.h>

int GetCameraCoords_From_Angle(float horizontal_angle,float vertical_angle,unsigned int * uint_pixel_x,unsigned int * uint_pixel_y)
{
 /*
    horizontal_angle == 0 AND vertical_angle == 0 ---> The center of the Image

    horizontal_angle ( left is less , right is more )
    vertical angle ( down is less , up is more )

     When X is 0 horizontal -> angle -field_of_view
     When X is RES_X/2 -> horizontal angle 0
     When X is RES_X -> horizontal angle +field_of_view
  */
  float degree_step_x = 0 , degree_step_y = 0;
  if (camera_horizontal_field_of_view !=0) { degree_step_x = metrics[RESOLUTION_X] / camera_horizontal_field_of_view; } else
                                           { fprintf(stderr,"Camera horizontal field of view NOT set .. , this will probably keep popping up , until you call VisCortx_SetCamerasGeometry\n"); return 0; }
  if ( degree_step_x == 0 ) { fprintf(stderr,"Camera resolution probably not set , cannot continue to get depth \n"); return 0; }


  if (camera_vertical_field_of_view !=0) { degree_step_y = metrics[RESOLUTION_Y] / camera_vertical_field_of_view; } else
                                         { fprintf(stderr,"Camera vertical field of view NOT set .. , this will probably keep popping up , until you call VisCortx_SetCamerasGeometry\n"); return 0; }
  if ( degree_step_y == 0 ) { fprintf(stderr,"Camera resolution probably not set , cannot continue to get depth \n"); return 0; }

  *uint_pixel_x=metrics[RESOLUTION_X] / 2;
  *uint_pixel_y=metrics[RESOLUTION_Y] / 2;

  float abs_horizontal_angle = horizontal_angle; if ( abs_horizontal_angle < 0 ) { abs_horizontal_angle = abs_horizontal_angle  * (-1); }
  float abs_vertical_angle = vertical_angle;     if ( abs_vertical_angle < 0 ) { abs_vertical_angle = abs_vertical_angle  * (-1); }

  float pixel_x = abs_horizontal_angle / degree_step_x  , pixel_y =abs_vertical_angle / degree_step_y;
  if ( horizontal_angle < 0 ) { pixel_x  = (metrics[RESOLUTION_X] / 2) - abs_horizontal_angle; }
  if ( vertical_angle < 0   ) { pixel_y  = (metrics[RESOLUTION_Y] / 2) - abs_vertical_angle;   }

  if ( ( pixel_x >= 0.0 ) && (pixel_x < metrics[RESOLUTION_X] ) ) { *uint_pixel_x = (unsigned int) pixel_x; }
  if ( ( pixel_y >= 0.0 ) && (pixel_y < metrics[RESOLUTION_Y] ) ) { *uint_pixel_y = (unsigned int) pixel_y; }

  return 1;
}









int ComputeFundamentalMatrix(void)
{
       double a_data[] = { 0.18, 0.60, 0.57, 0.96,
                           0.41, 0.24, 0.99, 0.58,
                           0.14, 0.30, 0.97, 0.66,
                           0.51, 0.13, 0.19, 0.85 };

       double b_data[] = { 1.0, 2.0, 3.0, 4.0 };

       gsl_matrix_view m
         = gsl_matrix_view_array (a_data, 4, 4);

       gsl_vector_view b
         = gsl_vector_view_array (b_data, 4);

       gsl_vector *x = gsl_vector_alloc (4);

       int s;

       gsl_permutation * p = gsl_permutation_alloc (4);

       gsl_linalg_LU_decomp (&m.matrix, p, &s);

       gsl_linalg_LU_solve (&m.matrix, p, &b.vector, x);

       printf ("x = \n");
       gsl_vector_fprintf (stdout, x, "%g");

       gsl_permutation_free (p);
       gsl_vector_free (x);
       return 0;
}






