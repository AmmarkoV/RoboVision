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

unsigned int precalc_group_block_belong[ABSOLUTE_MAX_WIDTH+1][ABSOLUTE_MAX_HEIGHT+1];
unsigned long precalc_memplace_3byte[ABSOLUTE_MAX_WIDTH+1][ABSOLUTE_MAX_HEIGHT+1];
unsigned long precalc_memplace_1byte[ABSOLUTE_MAX_WIDTH+1][ABSOLUTE_MAX_HEIGHT+1];


struct CameraCalibrationData left_calibration_data;
struct CameraCalibrationData right_calibration_data;
unsigned int CameraDistanceInMM = 65;




inline unsigned char AbsUCharVDiff(unsigned char param1,unsigned char param2)
{
    if ( param1<param2 ) { return param2-param1; }
    return param1-param2;
}

inline unsigned char AbsUCharDiff(unsigned char * param1,unsigned char * param2)
{
    if ( *param1<*param2 ) { return *param2-*param1; }
    return *param1-*param2;
}


inline unsigned int AbsUIntDiff(unsigned int num1,unsigned int num2)
{
    if (num1>num2 ) { return num1-num2;}
    return num2-num1;
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
 /*   ALLOW PRECALCULATIONS FOR RESECTIONING , BECAUSE IT MAY BE ENABLED LATER
  if ( settings[INPUT_CALIBRATION]==0)
    {
        fprintf(stderr,"Calibration is disabled , please set settings[INPUT_CALIBRATION]=1");
        return 0;
    }
*/

  fprintf(stderr,"Calibrating fx=%f fy=%f cx=%f cy=%f\n",fx,fy,cx,cy);
  fprintf(stderr,"k1=%f k2=%f p1=%f p2=%f k3=%f \n",k1,k2,p1,p2,k3);

  if ( ( fx == 0.0) || ( fy == 0.0) || ( (k1==0.0)&&(k2==0.0)&&(k3==0.0) )) { fprintf(stderr,"Erroneous parameters calibration canceled\n"); return 0; }

  unsigned int i,x = metrics[RESOLUTION_X] ,y=metrics[RESOLUTION_Y] , mem , new_mem , interpolation_mem;
  unsigned int undistorted_x,undistorted_y;

  mem = 0;
  double ifx=1.f/fx,ify=1.f/fy;
  double dstdx,dstdy , distx,disty;
  double dx,dy;
  double r_sq  = 0;  // R Squared
  double r_cu = 0;   // R Cubed
  double k_coefficient = 0;
  double new_x,new_y;

  // SEE http://opencv.willowgarage.com/documentation/camera_calibration_and_3d_reconstruction.html
  // https://code.ros.org/trac/opencv/browser/trunk/opencv/src/cv/cvundistort.cpp?rev=18
  // https://code.ros.org/trac/opencv/browser/trunk/opencv/modules/imgproc/src/undistort.cpp?rev=4885
  // http://tech.groups.yahoo.com/group/OpenCV/message/26019
  // Also Learning OpenCV page 375-377
  /*

        Finaly fixed using code from Philip Gruebele @
            http://tech.groups.yahoo.com/group/OpenCV/message/26019

            archived at 3dpartylibs/code/undistort_point.cpp
  */
  unsigned int PrecisionErrors=0;
  unsigned int OffFrame=0;
  unsigned int OutOfMemory=0;


  for (y=0; y<metrics[RESOLUTION_Y]; y++)
  {
     interpolation_mem=0;
     for (x=0; x<metrics[RESOLUTION_X]; x++)
        {
          //Well this is supposed to rectify lens distortions based on calibration done with my image sets
          //but the values returned are way off ..
          dstdx = ( x - cx );
          dstdx *=  ifx;

          dstdy = ( y - cy );
          dstdy *=  ify;

          new_x = dstdx;
          new_y = dstdy;

          for ( i=0; i<5; i++)
           {
               r_sq =  new_x*new_x;
               r_sq += new_y*new_y;

               r_cu = r_sq*r_sq;

               k_coefficient = 1;
               k_coefficient += k1 * r_sq;
               k_coefficient += k2 * r_cu;
               k_coefficient += k3 * r_cu * r_sq ;

               dx =  2 * p1 * new_x * new_y;
               dx += p2 * ( r_sq + 2 * new_x * new_x);

               dy =  2 * p2 * new_x * new_y;
               dy += p1 * ( r_sq + 2 * new_y * new_y);

               new_x = ( dstdx - dx );
               new_x /= k_coefficient;

               new_y = ( dstdy - dy );
               new_y /= k_coefficient;
           }

          dstdx = new_x;
          dstdy = new_y;

          dstdx *= fx; dstdx += cx;
          dstdy *= fy; dstdy += cy;


          undistorted_x  = (unsigned int) round(dstdx);
          undistorted_y  = (unsigned int) round(dstdy);



                   /* REVERSE CHECK ! >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
                   new_x = dstdx - cx; new_x /= fx;
                   new_y = dstdy - cy; new_y /= fy;

                   r_sq = new_x*new_x + new_y*new_y;
                   distx = new_x ;
                   distx += new_x*(k1*r_sq + k2*r_sq*r_sq);
                   distx += (2*p1*new_x*new_y + p2*(r_sq + 2*new_x*new_x));


                   disty = new_y;
                   disty +=new_y*(k1*r_sq + k2*r_sq*r_sq);
                   disty +=(2*p2*new_x*new_y + p1*(r_sq + 2*new_y*new_y));


                   distx *= fx; distx += cx;
                   disty *= fy; disty += cy;
                   /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/



      // This should never be more than .2 pixels...
      double diffx = x - distx;
      double diffy = y - disty;

         if ( (diffx> 0.1) || (diffy>0.1) )
          {
             /* ACCURACY ERROR , This means that we have a percision error in the way math is done*/
             //fprintf(stderr,"$%u,%u to %u,%u",x,y,undistorted_x,undistorted_y);
             new_mem = 0;
                 new_mem=interpolation_mem ; //TEST THIS USESTHE INTERPOLATED TO GET RID OF SOME BLANK SPOT ARTIFACTS
             ++PrecisionErrors;
          }

          if ( ( undistorted_x >= metrics[RESOLUTION_X] ) || ( undistorted_y >= metrics[RESOLUTION_Y] ) )
             {
                 // OFF RESULTS SHOULD BE INTERPOLATED WITH CLOSE MEMORY SPOTS
                 //fprintf(stderr,"!%u,%u to %u,%u",x,y,undistorted_x,undistorted_y);
                 new_mem = 0;
                 new_mem=interpolation_mem ; //TEST THIS USESTHE INTERPOLATED TO GET RID OF SOME BLANK SPOT ARTIFACTS
                 ++OffFrame;
             } else
             {
                new_mem = undistorted_y * metrics[RESOLUTION_X_3_BYTE] + undistorted_x * 3 ;
                interpolation_mem = new_mem;
                if ( new_mem>= metrics[RESOLUTION_MEMORY_LIMIT_3BYTE])
                 {
                   new_mem = 0;
                   ++OutOfMemory;
                  }
                //fprintf(stderr,"%u,%u -> %u,%u .. \n",x,y,undistorted_x,undistorted_y);
             }



          frame [mem] = new_mem;
          ++mem;  ++new_mem;

          frame [mem] = new_mem;
          ++mem;  ++new_mem;

          frame [mem] = new_mem;
          ++mem;  ++new_mem;

       }
   }

 fprintf(stderr,"PrecalculationErrors - Precision=%u , OffFrame=%u , OutOfMemory=%u\n",PrecisionErrors,OffFrame,OutOfMemory);

 return 1;

}

unsigned int RGB(unsigned char R,unsigned char G,unsigned char B)
{
  return (B + (G * 256) + (R * 65536) );
}

void TestPrecalculations()
{
  int errors=0;
  if (errors>0) { fprintf(stderr,"Precalculation errors\n"); }
}



void Precalculations()
{

  //fprintf(stderr,"Signed/Unsigned short max %u/%u \n",SHRT_MAX,USHRT_MAX);
  //fprintf(stderr,"Signed/Unsigned int max %u/%u \n",INT_MAX,UINT_MAX);
  //fprintf(stderr,"Signed/Unsigned long max %u/%u \n",LONG_MAX,ULONG_MAX);
  unsigned int mem;
   for (mem=0; mem<metrics[RESOLUTION_MEMORY_LIMIT_3BYTE]; mem++)
   {
      resection_left_precalc[mem]=mem;
      resection_right_precalc[mem]=mem;
   }


   if ((metrics[HORIZONTAL_BUFFER]==0)||(metrics[VERTICAL_BUFFER]==0)) { fprintf(stderr,"Problematic Group Block Size , may crash me :(  (division by 0) !"); return; }

   unsigned int total_blocks_x=(unsigned int) (metrics[RESOLUTION_X]/metrics[HORIZONTAL_BUFFER]);
   unsigned int total_blocks_y=(unsigned int) (metrics[RESOLUTION_Y]/metrics[VERTICAL_BUFFER]);

   if ( (total_blocks_x==0)||(total_blocks_y==0)) { fprintf(stderr,"Problematic Group Block Size , Something went wrong dividing MaxResX/horizontal Block , MaxResY/vertical Block!"); }

   unsigned int x,y;

   for (x=0; x<=ABSOLUTE_MAX_WIDTH; x++)
   { for (y=0; y<=ABSOLUTE_MAX_HEIGHT; y++)
     {
		  precalc_group_block_belong[x][y]= (unsigned int) (       (  ( y/metrics[VERTICAL_BUFFER] ) * total_blocks_x    ) + (x/metrics[HORIZONTAL_BUFFER])   );
		  precalc_memplace_3byte[x][y] = (unsigned long) ( y * ( metrics[RESOLUTION_X] * 3 ) + x*3 ) ;
		  precalc_memplace_1byte[x][y] = (unsigned long) ( y * ( metrics[RESOLUTION_X] ) + x ) ;
	 }
   }

 //TestPrecalculations();
}
