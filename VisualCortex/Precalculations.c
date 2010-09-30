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


unsigned char precalc_med[256][256];
unsigned char precalc_sub[256][256];
unsigned int precalc_group_block_belong[641][481];
unsigned long precalc_memplace_3byte[641][481];
unsigned long precalc_memplace_1byte[641][481];



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
