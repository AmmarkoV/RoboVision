#include "VisCortexConvolutionFilters.h"
#include "VisCortexFilters.h"
#include "Precalculations.h"
#include "FeatureTracking.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


signed int SumTable(signed char * table , signed int total_blocks)
{
   signed int sum=0;
   unsigned int i=0;
   while ( i<total_blocks )
    {
        sum+=table[i];
        ++i;
    }

   return sum;
}



int ConvolutionFilter9_1Byte(unsigned int monochrome_reg,unsigned int target_reg,signed char * table,signed int divisor)
{
    if (!ThisIsA1ByteRegister(monochrome_reg)) { return 0; }
    video_register[target_reg].depth=1;
    // 3 x 3 = 9 :P
    unsigned char * out_px=video_register[target_reg].pixels;
    unsigned char * cur_px=video_register[monochrome_reg].pixels;
    unsigned char * end_of_line_cur_px=cur_px+metrics[RESOLUTION_X];
    unsigned char * end_of_memory_cur_px=video_register[monochrome_reg].pixels+metrics[RESOLUTION_MEMORY_LIMIT_1BYTE]-metrics[RESOLUTION_X]-1;
    unsigned char * px=video_register[monochrome_reg].pixels;

    if ( divisor == 0 ) { divisor = 1; }

    //We skip the first line , no value can be calculated for it anyway
    cur_px += metrics[RESOLUTION_X];
    out_px += metrics[RESOLUTION_X];
    //We skip the first pixel since the whole first column cant be calculated
     ++cur_px;
     ++out_px;

    // TABLE i,j i=line , j=column , this is the convention used
    // table_1_2 means first line second column
    signed char * table_1_1 = table;
    signed char * table_1_2 = table+1;
    signed char * table_1_3 = table+2;

    signed char * table_2_1 = table+3;
    signed char * table_2_2 = table+4;
    signed char * table_2_3 = table+5;

    signed char * table_3_1 = table+6;
    signed char * table_3_2 = table+7;
    signed char * table_3_3 = table+8;
    //----------------------------------------------------------

   /*

       XXXXXXXXXXXXXXXXXXXXXXXX
       XOOOOOOOOOOOOOOOOOOOOOOX
       XOOOOOOOOOOOOOOOOOOOOOOX
       XOOOOOOOOOOOOOOOOOOOOOOX
       XOOOOOOOOOOOOOOOOOOOOOOX
       XOOOOOOOOOOOOOOOOOOOOOOX
       XXXXXXXXXXXXXXXXXXXXXXXX
       We try to apply a 3x3 convolution vector only on the O's
   */

   signed int cur_value ;
   while (cur_px<end_of_memory_cur_px)
    {
            px = cur_px - metrics[RESOLUTION_X];

            cur_value   = (signed int) *px * *table_1_1; ++px;
            cur_value  += (signed int) *px * *table_1_2; ++px;
            cur_value  += (signed int) *px * *table_1_3; ++px;

            px += metrics[RESOLUTION_X]-3;
            cur_value  += (signed int) *px * *table_2_1; --px;
            cur_value  += (signed int) *px * *table_2_2; --px;
            cur_value  += (signed int) *px * *table_2_3; --px;

            px += metrics[RESOLUTION_X]-3;
            cur_value  += (signed int) *px * *table_3_1; ++px;
            cur_value  += (signed int) *px * *table_3_2; ++px;
            cur_value  += (signed int) *px * *table_3_3; ++px;

            cur_value = (signed int) cur_value / divisor;

            if ( cur_value < 0   ) { *out_px=0; } else
            if ( cur_value > 255 ) { *out_px=255; } else
                                   { *out_px=(unsigned char) cur_value; }


        ++out_px;
        ++cur_px;

      if (cur_px>end_of_line_cur_px)
      {
          end_of_line_cur_px+=metrics[RESOLUTION_X];
          out_px+=1; /*This because we go until 1 byte before the edge of the array*/
          cur_px+=1; /*This because we go until 1 byte before the edge of the array*/
      }
    }

   return 1;
}


int ConvolutionFilter9_3Byte(unsigned int rgb_reg,unsigned int target_reg,signed char * table,signed int divisor)
{
    //fprintf(stderr,"ConvolutionFilter9_3Byte is not tested yet and its disabled \n");
    //return 0;

    if (!ThisIsA3ByteRegister(rgb_reg)) { return 0; }
    video_register[target_reg].depth=3;
    // 3 x 3 = 9 :P
    unsigned char * out_px=video_register[target_reg].pixels;
    unsigned char * cur_px=video_register[rgb_reg].pixels;
    unsigned char * px=video_register[rgb_reg].pixels;



    fprintf(stderr,"Using table \n%i %i %i\n%i %i %i\n%i %i %i\n Each: 1/%i\n",table[0],table[1],table[2],table[3],table[4],table[5],table[6],table[7],table[8],divisor);

    if ( divisor == 0 ) { divisor=1; /*If zero we wont make any divisions :P */   }
    signed int cur_value_r , cur_value_g , cur_value_b  ;
    unsigned int x=1,y=1;

    cur_px += metrics[RESOLUTION_X_3_BYTE];
    out_px += metrics[RESOLUTION_X_3_BYTE];


    // TABLE i,j i=line , j=column , this is the convention used
    // table_1_2 means first line second column
    signed char * table_1_1 = table;
    signed char * table_1_2 = table+1;
    signed char * table_1_3 = table+2;

    signed char * table_2_1 = table+3;
    signed char * table_2_2 = table+4;
    signed char * table_2_3 = table+5;

    signed char * table_3_1 = table+6;
    signed char * table_3_2 = table+7;
    signed char * table_3_3 = table+8;
    //----------------------------------------------------------

    while ( y < video_register[rgb_reg].size_y-1 )
     {
         x=1;
         out_px+=3; /*This because we go until 3 bytes before the edge of the array*/
         cur_px+=3; /*This because we go until 3 bytes before the edge of the array*/

         while ( x < video_register[rgb_reg].size_x-1 )
          {
            px = cur_px - metrics[RESOLUTION_X_3_BYTE];

            cur_value_r   = *px * *table_1_1; ++px;       cur_value_g   = *px * *table_1_1; ++px;       cur_value_b   = *px * *table_1_1; ++px;
            cur_value_r  += *px * *table_1_2; ++px;       cur_value_g  += *px * *table_1_2; ++px;       cur_value_b  += *px * *table_1_2; ++px;
            cur_value_r  += *px * *table_1_3; ++px;       cur_value_g  += *px * *table_1_3; ++px;       cur_value_b  += *px * *table_1_3; ++px;


            px += metrics[RESOLUTION_X_3_BYTE]-9;
            cur_value_r  += *px * *table_2_1; --px;       cur_value_g  += *px * *table_2_1; ++px;       cur_value_b  += *px * *table_2_1; ++px;
            cur_value_r  += *px * *table_2_2; --px;       cur_value_g  += *px * *table_2_2; ++px;       cur_value_b  += *px * *table_2_2; ++px;
            cur_value_r  += *px * *table_2_3; --px;       cur_value_g  += *px * *table_2_3; ++px;       cur_value_b  += *px * *table_2_3; ++px;

            px += metrics[RESOLUTION_X_3_BYTE]-9;
            cur_value_r  += *px * *table_3_1; ++px;       cur_value_g  += *px * *table_3_1; ++px;       cur_value_b  += *px * *table_3_1; ++px;
            cur_value_r  += *px * *table_3_2; ++px;       cur_value_g  += *px * *table_3_2; ++px;       cur_value_b  += *px * *table_3_2; ++px;
            cur_value_r  += *px * *table_3_3; ++px;       cur_value_g  += *px * *table_3_3; ++px;       cur_value_b  += *px * *table_3_3; ++px;

            cur_value_r = cur_value_r / divisor;
            cur_value_g = cur_value_g / divisor;
            cur_value_b = cur_value_b / divisor;
            //px = px - metrics[RESOLUTION_X_3_BYTE] - 6;

            if ( cur_value_r < 0 )   { *out_px=0;  } else
            if ( cur_value_r > 255 ) { *out_px=255; } else
                                     { *out_px=cur_value_r; }
            ++out_px;

            if ( cur_value_g < 0 )   { *out_px=0; } else
            if ( cur_value_g > 255 ) { *out_px=255; } else
                                     { *out_px=cur_value_g; }
            ++out_px;

            if ( cur_value_b < 0 )   { *out_px=0; } else
            if ( cur_value_b > 255 ) { *out_px=255; } else
                                     { *out_px=cur_value_b; }
            ++out_px;
            cur_px+=3;
            ++x;
          }

            out_px+=3; /*This because we go until 3 bytes before the edge of the array*/
            cur_px+=3; /*This because we go until 3 bytes before the edge of the array*/

         ++y;
     }


     return 1;
}
