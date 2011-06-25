/*
The Visual Cortex of guarddog
URLs: http://ammarkov.ath.cx
Written by Ammar Qammaz a.k.a. AmmarkoV 2010

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Precalculations.h"
#include "VisionMemory.h"
#include <limits.h>
#include <math.h>

unsigned int resection_left_precalc[321*241*3];
unsigned int resection_right_precalc[321*241*3];

unsigned char precalc_med[256][256];
unsigned char precalc_sub[256][256];
unsigned int precalc_group_block_belong[641][481];
unsigned long precalc_memplace_3byte[641][481];
unsigned long precalc_memplace_1byte[641][481];



/*

   RectifiedPoint = M * OriginalPoint

   Rectified Point = [ new_x new_y new_w ]
   Original Point  = [ x y w ]

         |fx  0   cx|       a   b   c
   M =   |0   fy  cy|       d   e   f
         |0   0   1 |       g   h   i
*/

unsigned int PrecalcResectioning(unsigned int * frame ,  double fx,double fy , double cx,double cy ,
                                                         double k1,double k2 , double p1,double p2 , double k3   )
{



  double new_x,new_y,new_w;
  unsigned int x,y , mem;
  unsigned int undistorted_x,undistorted_y, new_mem ;
  mem = 0;

  double r = 0;
  double k_coefficient = 0;

  double scale_x , scale_y;

 /* FIRE IT UP FOR 320x240 TO HAVE A SCALE MEASURE*/
  x=metrics[RESOLUTION_X]; y=metrics[RESOLUTION_Y];
  r = sqrt(x*x + y*y);
  k_coefficient = 1 + ( k1 * r * r ) + ( k2 * r * r * r * r ) + ( k3 * r * r * r * r * r * r);
  new_x =x*k_coefficient + ((2 * p1 * y) * ( r*r + 2 * x * x));
  new_y =y*k_coefficient + (p1 * ( r * r + 2 * y * y) + 2 * p2 * x);
  scale_x =  (new_x * fx + cx ) / metrics[RESOLUTION_X];
  scale_y =  (new_y * fy + cy ) / metrics[RESOLUTION_Y];


  for (y=0; y<metrics[RESOLUTION_Y]; y++)
  {
     for (x=0; x<metrics[RESOLUTION_X]; x++)
        {
         // SEE http://opencv.willowgarage.com/documentation/camera_calibration_and_3d_reconstruction.html

          r = sqrt(x*x + y*y);
          k_coefficient = 1 + ( k1 * r * r ) + ( k2 * r * r * r * r ) + ( k3 * r * r * r * r * r * r);
          new_x =x*k_coefficient + ((2 * p1 * y) * ( r*r + 2 * x * x));
          new_y =y*k_coefficient + (p1 * ( r * r + 2 * y * y) + 2 * p2 * x);


          undistorted_x =(unsigned int) ((new_x * fx + cx )/scale_x);
          undistorted_y =(unsigned int) ((new_y * fy + cy )/scale_y);


          if ( ( undistorted_x > metrics[RESOLUTION_X] ) || ( undistorted_y > metrics[RESOLUTION_Y] ) )
             {
                 fprintf(stderr,"!!%u,%u to %u,%u!!",x,y,undistorted_x,undistorted_y);
                 new_mem = 0;
                 // new_mem = mem;
             } else
             {
                new_mem = undistorted_y * metrics[RESOLUTION_X_3_BYTE] + undistorted_x * 3 ;
                fprintf(stderr,"%u,%u -> %u,%u .. \n",x,y,undistorted_x,undistorted_y);
             }



          frame [mem] = new_mem;
          ++mem;  ++new_mem;

          frame [mem] = new_mem;
          ++mem;  ++new_mem;

          frame [mem] = new_mem;
          ++mem;  ++new_mem;
       }
   }
 return 1;

}

unsigned int RGB(unsigned char R,unsigned char G,unsigned char B)
{
  return (B + (G * 256) + (R * 65536) );
}

void TestPrecalculations()
{
  int i,z,errors=0;
  for (i=0; i<=255; i++)
   { for (z=0; z<=255; z++)
     {

		 if (precalc_med[i][z] != (short)  ( (i + z) / 2 )) { ++errors; }

	 }
   }

   if (errors>0) { fprintf(stderr,"Precalculation errors\n"); }
}

void Precalculations()
{

  //fprintf(stderr,"Signed/Unsigned short max %u/%u \n",SHRT_MAX,USHRT_MAX);
  //fprintf(stderr,"Signed/Unsigned int max %u/%u \n",INT_MAX,UINT_MAX);
  //fprintf(stderr,"Signed/Unsigned long max %u/%u \n",LONG_MAX,ULONG_MAX);

/*
         |fx  0   cx|       a   b   c
   M =   |0   fy  cy|       d   e   f
         |0   0   1 |       g   h   i
  PrecalcResectioning(unsigned int * frame , double cx,double cy, double fx,double fy )
*/
  /* CAMERA 0
  resolution=[320 240]
  cx=1.671293e+02
  cy=1.273992e+02
  fx=2.873170e+02
  fy=2.881220e+02
  */

  PrecalcResectioning(resection_left_precalc,
                                                287.3170 ,
                                                288.1220  ,
                                                167.1293,
                                                188.1220 ,

                                                0.1753691,
                                                0.07749228,
                                                0.000167197,
                                                0.01667649 ,
                                                0.0
                                                );

  // CAMERA 1
  PrecalcResectioning(resection_right_precalc,
                                                232.97080393892657 ,
                                                275.24989549871043 ,
                                                163.78706430546723 ,
                                                129.05282322284381 ,

                                                0.1753691,
                                                0.07749228,
                                                0.000167197,
                                                0.01667649 ,
                                                0.0
                                                );



  unsigned int div_res;
  unsigned char i,z;
  for (i=0; i<255; i++)
   { for (z=0; z<255; z++)
     {
		 if ( z - i > 0 )  { precalc_sub[i][z] = (unsigned char) z - (unsigned char) i  ; } else
		 if ( i - z > 0  )  { precalc_sub[i][z] = (unsigned char) i - (unsigned char) z  ; } else
                             { precalc_sub[i][z] = 0 ; }
		 div_res=(unsigned int)  ( (i + z) / 2 );
		 precalc_med[i][z] = (unsigned char) div_res;
	 }
   }

   if ((metrics[HORIZONTAL_BUFFER]==0)||(metrics[VERTICAL_BUFFER]==0)) { fprintf(stderr,"Problematic Group Block Size , may crash me :(  (division by 0) !"); return; }

   unsigned int total_blocks_x=(unsigned int) (metrics[RESOLUTION_X]/metrics[HORIZONTAL_BUFFER]);
   unsigned int total_blocks_y=(unsigned int) (metrics[RESOLUTION_Y]/metrics[VERTICAL_BUFFER]);

   if ( (total_blocks_x==0)||(total_blocks_y==0)) { fprintf(stderr,"Problematic Group Block Size , Something went wrong dividing MaxResX/horizontal Block , MaxResY/vertical Block!"); }

   unsigned int x,y;

   for (x=0; x<=640; x++)
   { for (y=0; y<=480; y++)
     {
		  precalc_group_block_belong[x][y]= (unsigned int) (       (  ( y/metrics[VERTICAL_BUFFER] ) * total_blocks_x    ) + (x/metrics[HORIZONTAL_BUFFER])   );
		  precalc_memplace_3byte[x][y] = (unsigned long) ( y * ( metrics[RESOLUTION_X] * 3 ) + x*3 ) ;
		  precalc_memplace_1byte[x][y] = (unsigned long) ( y * ( metrics[RESOLUTION_X] ) + x ) ;
	 }
   }

 //TestPrecalculations();
}
