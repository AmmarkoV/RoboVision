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

unsigned int resection_left_precalc[321*241*3];
unsigned int resection_right_precalc[321*241*3];

unsigned char precalc_med[256][256];
unsigned char precalc_sub[256][256];
unsigned int precalc_group_block_belong[641][481];
unsigned long precalc_memplace_3byte[641][481];
unsigned long precalc_memplace_1byte[641][481];



/*
         |fx  0   cx|       a   b   c
   M =   |0   fy  cy|       d   e   f
         |0   0   1 |       g   h   i
*/

unsigned int PrecalcResectioning(unsigned int * frame , double cx,double cy, double fx,double fy )
{
  double new_w;
  unsigned int x,y , mem;
  unsigned int new_x,new_y , new_mem ;
  mem = 0;
  for (y=0; y<metrics[RESOLUTION_Y]; y++)
  {
     for (x=0; x<metrics[RESOLUTION_X]; x++)
        {

         /* TODO REMOVE STATIC MULTIPLICATIONS , ETC ( MAYBE EVEN MOVE THIS TO VIDEO INPUT :P )
          new_x = x * a + y * d  + 1 * g;
          new_y = x * b + y * e  + 1 * h;
          new_w = x * c + y * f  + 1 * i;*/
          new_w = x * cx + y * cy  + 1 ;
          new_x =(unsigned int)  x*((x * fx + y * 0  + 1 * 0)/new_w);
          new_y =(unsigned int)  y*((x * 0 + y * fy  + 1 * 0)/new_w);
          if ( ( new_x > metrics[RESOLUTION_X] ) || ( new_y > metrics[RESOLUTION_Y] ) )
             {
                 fprintf(stderr,"(!)");
                 new_mem = mem;
             } else
             {
                new_mem = new_y * metrics[RESOLUTION_X_3_BYTE] + new_x * 3 ;
                fprintf(stderr,"%u,%u -> %u,%u .. \n",x,y,new_x,new_y);
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
LEFT CAMERA
cx=3.071862e+02
cy=2.624510e+02
fx=5.690248e+02
fy=5.711376e+02
dist=[-9.984673e-02 4.476950e-02 -9.349797e-04 -6.787937e-03 0.000000e+00]
focal_length=0.000000e+00

RIGHT CAMERA
cx=3.258495e+02
cy=2.456993e+02
fx=6.021574e+02
fy=6.039465e+02
dist=[-8.269976e-02 1.691322e-01 -1.120546e-03 -4.395945e-04 0.000000e+00]
focal_length=0.000000e+00
*/

  PrecalcResectioning(resection_left_precalc,307.1862,262.4510,569.0248,571.1376);
  PrecalcResectioning(resection_right_precalc,325.8495,245.6993,602.1574,603.9465);


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
