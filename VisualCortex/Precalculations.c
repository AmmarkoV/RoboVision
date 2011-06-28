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
unsigned int PrecalcOCVResectioning(unsigned int * frame ,  double fx,double fy , double cx,double cy ,
                                                         double k1,double k2 , double p1,double p2 , double k3   )
{
  if ( settings[INPUT_CALIBRATION]==0)
    {
        fprintf(stderr,"Calibration is disabled , please set settings[INPUT_CALIBRATION]=1");
        return 0;
    }

  unsigned int x   , y , mem , new_mem;
  unsigned int undistorted_x,undistorted_y;

  mem = 0;


  const double a1 = 1.f / fx , b1 = 1.f / fy;
  double p22 = 2.f * p2;

  // SEE http://opencv.willowgarage.com/documentation/camera_calibration_and_3d_reconstruction.html
  // https://code.ros.org/trac/opencv/browser/trunk/opencv/src/cv/cvundistort.cpp?rev=18
  // https://code.ros.org/trac/opencv/browser/trunk/opencv/modules/imgproc/src/undistort.cpp?rev=4885
  // Also Learning OpenCV page 375-377

  for (y=0; y<metrics[RESOLUTION_Y]; y++)
  {

     double dy = y - cy;
     double y0 = b1 * dy;
     double y1 ;
     if ( y0 == 0 ) { y1 = 0; } else
                    { y1 = p1 / y0; }
     double y2 = y0 * y0;
     double y3 = 2.f * p1 * y0;

     for (x=0; x<metrics[RESOLUTION_X]; x++)
        {

          double dx = x - cx;
          double x0 = a1 * dx;
          double x1;
          if ( x0 == 0 ) { x1 = 0; } else
                         { x1 = p2 / x0; }
          double x2 = x0 * x0;
          double x3 = p22 * x0;
          double r2 = x2 + y2;
          double bx = r2 * (k1 + r2 * k2) + x3 + y3;
          double by = bx + r2 * y1;
          bx += r2 * x1;

          int xd = x0, yd = y0;


           xd += round( bx * dx );
           yd += round( by * dy );

          undistorted_x  = (unsigned int) xd;
          undistorted_y  = (unsigned int) yd;



          if ( ( undistorted_x >= metrics[RESOLUTION_X] ) || ( undistorted_y >= metrics[RESOLUTION_Y] ) )
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
  //return PrecalcOCVResectioning (frame,fx,fy,cx,cy,k1,k2,p1,p2,k3);

  if ( settings[INPUT_CALIBRATION]==0)
    {
        fprintf(stderr,"Calibration is disabled , please set settings[INPUT_CALIBRATION]=1");
        return 0;
    }

  unsigned int x = metrics[RESOLUTION_X] ,y=metrics[RESOLUTION_Y] , mem , new_mem;
  unsigned int undistorted_x,undistorted_y;

  mem = 0;
  double dx,dy;
  double r_sq  = 0;
  double k_coefficient = 0;
  double new_x,new_y;

  // SEE http://opencv.willowgarage.com/documentation/camera_calibration_and_3d_reconstruction.html
  // https://code.ros.org/trac/opencv/browser/trunk/opencv/src/cv/cvundistort.cpp?rev=18
  // https://code.ros.org/trac/opencv/browser/trunk/opencv/modules/imgproc/src/undistort.cpp?rev=4885
  // Also Learning OpenCV page 375-377

  for (y=0; y<metrics[RESOLUTION_Y]; y++)
  {
     for (x=0; x<metrics[RESOLUTION_X]; x++)
        {
          //Well this is supposed to rectify lens distortions based on calibration done with my image sets
          //but the values returned are way off ..

          dx = ( x - cx );// / fx;
          dy = ( y - cy );// / fy;

          r_sq = dx*dx + dy*dy;

          k_coefficient = 1;
          k_coefficient += k1 * r_sq;
          k_coefficient += k2 * r_sq * r_sq;
          k_coefficient += k3 * r_sq * r_sq * r_sq ;

          new_x = dx * k_coefficient;
          new_x +=   ((2 * p1 * dy) * ( r_sq + 2 * dx * dy));

          new_y = dy * k_coefficient;
          new_y +=  p1 * ( r_sq + 2 * dy * dy) + 2 * p2 * dx;

          undistorted_x  = (unsigned int) (new_x/* *fx */ + cx);
          undistorted_y  = (unsigned int) (new_y/* *fy */ + cy);



          if ( ( undistorted_x >= metrics[RESOLUTION_X] ) || ( undistorted_y >= metrics[RESOLUTION_Y] ) )
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
                                               287.3169556, // fx
                                               288.1220093 , // fy
                                               167.1293335 , // cx
                                               127.3991699 , // cy

                                                -0.0031672, //k1
                                                0.0166765,  //k2
                                                -0.1753691, //p1
                                                0.0774923 , //p2
                                                0.0         //k3
                                                );

  // CAMERA 1
  PrecalcResectioning(resection_right_precalc,
                                                285.3517456 , // fx
                                                284.4697876 , // fy
                                                173.1338501 , // cx
                                                126.5036926 , // cy

                                                -0.0022525 , //k1
                                                0.0097993 ,  //k2
                                                -0.1602673 , //p1
                                                -0.0077100 , //p2
                                                0.0          //k3
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
