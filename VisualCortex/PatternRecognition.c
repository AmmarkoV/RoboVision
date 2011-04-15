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

unsigned short CompareToHAARWavelet(int wavelet_id,unsigned char * patch,unsigned int x,unsigned int y , unsigned int patch_x,unsigned int patch_y)
{
    unsigned int cur_x=x,cur_y=y;
    unsigned int x_mid,y_mid;
    unsigned char * ptr=patch;

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
                  if ( cur_x < x_mid )
                   {
                      if ( *ptr < 128 ) { retres+=128-*ptr; }
                   } else
                   {
                      if ( *ptr > 128 ) { retres+=*ptr-128; }
                   }
                ptr+=3;
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
                  if ( cur_x > x_mid )
                   {
                      if ( *ptr < 128 ) { retres+=128-*ptr; }
                   } else
                   {
                      if ( *ptr > 128 ) { retres+=*ptr-128; }
                   }
                ptr+=3;
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
                  if ( cur_y < y_mid )
                   {
                      if ( *ptr < 128 ) { retres+=128-*ptr; }
                   } else
                   {
                      if ( *ptr > 128 ) { retres+=*ptr-128; }
                   }
                ptr+=3;
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
                  if ( cur_y > y_mid )
                   {
                      if ( *ptr < 128 ) { retres+=128-*ptr; }
                   } else
                   {
                      if ( *ptr > 128 ) { retres+=*ptr-128; }
                   }
                ptr+=3;
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

    result->segment[0]=CompareToHAARWavelet(1,real_patch_start,x,y,patch_x,patch_y);
    result->segment[1]=CompareToHAARWavelet(2,real_patch_start,x,y,patch_x,patch_y);
    result->segment[2]=CompareToHAARWavelet(3,real_patch_start,x,y,patch_x,patch_y);
    result->segment[3]=CompareToHAARWavelet(4,real_patch_start,x,y,patch_x,patch_y);
    result->total_segments=4;

    return 1;
}
