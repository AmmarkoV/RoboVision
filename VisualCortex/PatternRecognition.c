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
#include "PatternRecognition.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned short CompareToHAARWavelet(int wavelet_id,unsigned char * patch,unsigned int x,unsigned int y , unsigned int patch_x,unsigned int patch_y)
{
    unsigned int cur_x=x,cur_y=y;
    unsigned int x_mid,y_mid;
    unsigned char * ptr=patch;
    unsigned char monochrome_byte,r,g,b;
    unsigned short retres=0;

    switch (wavelet_id)
     {


        case 1 :
           /*  PATTERN
               --------
              |XXXX    |
              |XXXX    |
              |XXXX    |
              |XXXX    |
               --------
           */
           x_mid = patch_x / 2;
           while ( cur_y < y+patch_y )
            {
              cur_x = x;
              while ( cur_x < x+patch_x )
              {
                  r = *ptr++;
                  g = *ptr++;
                  b = *ptr++;
                  monochrome_byte = (unsigned char) ( ( r + g + b )/3 );
                  if ( cur_x < x_mid )
                   {
                      if ( monochrome_byte < 128 ) { retres+=128-monochrome_byte; }
                   } else
                   {
                      if ( monochrome_byte > 128 ) { retres+=monochrome_byte-128; }
                   }
                ++cur_x;
              }
              ++cur_y;
            }

        case 2 :
           /*  PATTERN
               --------
              |    XXXX|
              |    XXXX|
              |    XXXX|
              |    XXXX|
               --------
           */
           x_mid = patch_x / 2;
           while ( cur_y < y+patch_y )
            {
              cur_x = x;
              while ( cur_x < x+patch_x )
              {
                  r = *ptr++;
                  g = *ptr++;
                  b = *ptr++;
                  if ( cur_x > x_mid )
                   {
                      if ( monochrome_byte < 128 ) { retres+=128-monochrome_byte; }
                   } else
                   {
                      if ( monochrome_byte > 128 ) { retres+=monochrome_byte-128; }
                   }
                ++cur_x;
              }
              ++cur_y;
            }
        break;

        case 3 :
           /*  PATTERN
               --------
              |XXXXXXXX|
              |XXXXXXXX|
              |        |
              |        |
               --------
           */
           y_mid = patch_y / 2;
           while ( cur_y < y+patch_y )
            {
              cur_x = x;
              while ( cur_x < x+patch_x )
              {
                  r = *ptr++;
                  g = *ptr++;
                  b = *ptr++;
                  monochrome_byte = (unsigned char) ( ( r + g + b )/3 );
                  if ( cur_y < y_mid )
                   {
                      if ( monochrome_byte < 128 ) { retres+=128-monochrome_byte; }
                   } else
                   {
                      if ( monochrome_byte > 128 ) { retres+=monochrome_byte-128; }
                   }
                ++cur_x;
              }
              ++cur_y;
            }
        break;

        case 4 :
           /*  PATTERN
               --------
              |        |
              |        |
              |XXXXXXXX|
              |XXXXXXXX|
               --------
           */
           y_mid = patch_y / 2;
           while ( cur_y < y+patch_y )
            {
              cur_x = x;
              while ( cur_x < x+patch_x )
              {
                  r = *ptr++;
                  g = *ptr++;
                  b = *ptr++;
                  monochrome_byte = (unsigned char) ( ( r + g + b )/3 );
                  if ( cur_y > y_mid )
                   {
                      if ( monochrome_byte < 128 ) { retres+=128-monochrome_byte; }
                   } else
                   {
                      if ( monochrome_byte > 128 ) { retres+=monochrome_byte-128; }
                   }
                ++cur_x;
              }
              ++cur_y;
            }
        break;


        case 5 :
           /*  PATTERN
             0,0     ---> PATCH_X
                --------
               |X       |
            |  |XX      |
            |  |XXX     |
            |  |XXXX    |
            \/  --------
            PATCH_Y
           */
           while ( cur_y < y+patch_y )
            {
              cur_x = x;
              while ( cur_x < x+patch_x )
              {
                  r = *ptr++;
                  g = *ptr++;
                  b = *ptr++;
                  monochrome_byte = (unsigned char) ( ( r + g + b )/3 );
                  if ( cur_x <= cur_y )
                   {
                      if ( monochrome_byte < 128 ) { retres+=128-monochrome_byte; }
                   } else
                   {
                      if ( monochrome_byte > 128 ) { retres+=monochrome_byte-128; }
                   }
                ++cur_x;
              }
              ++cur_y;
            }
        break;


        case 6 :
           /*  PATTERN
             0,0     ---> PATCH_X
                --------
               | XXXXXXX|
            |  |  XXXXXX|
            |  |   XXXXX|
            |  |    XXXX|
            \/  --------
            PATCH_Y
           */
           while ( cur_y < y+patch_y )
            {
              cur_x = x;
              while ( cur_x < x+patch_x )
              {
                  r = *ptr++;
                  g = *ptr++;
                  b = *ptr++;
                  monochrome_byte = (unsigned char) ( ( r + g + b )/3 );
                  if ( cur_x > cur_y )
                   {
                      if ( monochrome_byte < 128 ) { retres+=128-monochrome_byte; }
                   } else
                   {
                      if ( monochrome_byte > 128 ) { retres+=monochrome_byte-128; }
                   }
                ++cur_x;
              }
              ++cur_y;
            }
        break;

        default : break;
     };

    return retres;
}




unsigned int GetPatchSignature(unsigned char * patch, unsigned int image_x, unsigned int image_y ,  unsigned int x,unsigned int y , unsigned int patch_x,unsigned int patch_y , struct PatchSignature * result)
{
    unsigned char * real_patch_start=patch + ( image_x * (y) *3 ) + 3*(x) ;
    if ( image_x <= x ) { fprintf(stderr,"Patch out of bounds X \n"); return 0; }
    if ( image_y <= y ) { fprintf(stderr,"Patch out of bounds Y \n"); return 0; }
    if ( x+patch_x >= image_x ) { return 0; }
    if ( y+patch_y >= image_y ) { return 0; }

    unsigned char * monochrome_patch = (unsigned char * ) malloc( (patch_x+1)*(patch_y+1)*3*sizeof(unsigned char) );
    if ( monochrome_patch == 0 ) { fprintf(stderr,"Could not allocate monochrome patch \n"); return 0; }

    /*
       ADD MONOCHROME CONVERSION HERE :P
    */


    result->segment[0]=CompareToHAARWavelet(1,real_patch_start,x,y,patch_x,patch_y);
    result->segment[1]=CompareToHAARWavelet(2,real_patch_start,x,y,patch_x,patch_y);
    result->segment[2]=CompareToHAARWavelet(3,real_patch_start,x,y,patch_x,patch_y);
    result->segment[3]=CompareToHAARWavelet(4,real_patch_start,x,y,patch_x,patch_y);
    result->segment[4]=CompareToHAARWavelet(5,real_patch_start,x,y,patch_x,patch_y);
    result->segment[5]=CompareToHAARWavelet(6,real_patch_start,x,y,patch_x,patch_y);
    result->total_segments=6;

    free(monochrome_patch);

    return 1;
}
