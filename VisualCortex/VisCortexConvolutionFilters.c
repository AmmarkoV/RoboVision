#include "VisCortexConvolutionFilters.h"
#include "VisCortexFilters.h"
#include "Precalculations.h"
#include "FeatureTracking.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


unsigned int SumTable(unsigned char * table , unsigned int total_blocks)
{
   unsigned int sum=0;
   unsigned int i=0;
   while ( i<total_blocks )
    {
        sum+=table[i];
        ++i;
    }

   return sum;
}


int ConvolutionFilter9(unsigned int monochrome_reg,unsigned int target_reg,unsigned char * table)
{
    if (!ThisIsA1ByteRegister(monochrome_reg)) { return 0; }
    // 3 x 3 = 9 :P
    unsigned char * out_px=video_register[target_reg].pixels;
    unsigned char * cur_px=video_register[monochrome_reg].pixels;
    unsigned char * px=video_register[monochrome_reg].pixels;

    unsigned int summed_table = SumTable(table,9);
    if ( summed_table == 0 ) { fprintf(stderr,"Convolution filter called with a zero table\n"); return 0; }
    unsigned int cur_value ;
    unsigned int x=1,y=1;

    cur_px += metrics[RESOLUTION_X];
    out_px += metrics[RESOLUTION_X];

    while ( y < video_register[monochrome_reg].size_y-1 )
     {
         x=1;

         while ( x < video_register[monochrome_reg].size_x-1 )
          {
            px = cur_px - metrics[RESOLUTION_X]-1;

            cur_value  = *px * table[0];
            ++px;

            cur_value  += *px * table[1];
            ++px;

            cur_value  += *px * table[2];
            ++px;

            px += metrics[RESOLUTION_X];
            cur_value  += *px * table[5];
            --px;

            cur_value  += *px * table[4];
            --px;

            cur_value  += *px * table[3];
            --px;

            px += metrics[RESOLUTION_X];
            cur_value  += *px * table[6];
            ++px;

            cur_value  += *px * table[7];
            ++px;

            cur_value  += *px * table[8];
            ++px;

            cur_value = cur_value / summed_table;
            px = px - metrics[RESOLUTION_X] - 2;

            ++cur_px;
            ++x;
          }
         ++y;
     }


     return 1;
}
