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

unsigned int CompareToHAARWavelet(int wavelet_id,unsigned char * patch,unsigned int patch_x,unsigned int patch_y)
{
    unsigned int cur_x=0,cur_y=0;
    unsigned int x_mid=0,y_mid=0;
    unsigned char * ptr=patch;
    unsigned int retres=0;

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
           while ( cur_y < patch_y )
            {
              cur_x = 0;
              while ( cur_x < patch_x )
              {
                  if ( cur_x < x_mid ) { if ( *ptr < 128 ) { retres+=128-*ptr; } } else
                                       { if ( *ptr > 128 ) { retres+=*ptr-128; } }
                ++ptr;
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
           while ( cur_y < patch_y )
            {
              cur_x = 0;
              while ( cur_x < patch_x )
              {
                  if ( cur_x > x_mid ) { if ( *ptr < 128 ) { retres+=128-*ptr; } } else
                                       { if ( *ptr > 128 ) { retres+=*ptr-128; } }
                ++ptr;
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
           while ( cur_y < patch_y )
            {
              cur_x = 0;
              while ( cur_x < patch_x )
              {
                  if ( cur_y < y_mid ) { if ( *ptr < 128 ) { retres+=128-*ptr; } } else
                                       { if ( *ptr > 128 ) { retres+=*ptr-128; } }
                ++ptr;
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
           while ( cur_y < patch_y )
            {
              cur_x = 0;
              while ( cur_x < patch_x )
              {
                  if ( cur_y > y_mid ) { if ( *ptr < 128 ) { retres+=128-*ptr; } } else
                                       { if ( *ptr > 128 ) { retres+=*ptr-128; } }
                ++ptr;
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
           while ( cur_y < patch_y )
            { cur_x = 0;
              while ( cur_x < patch_x )
              {
                  if ( cur_x <= cur_y ) { if ( *ptr < 128 ) { retres+=128-*ptr; } } else
                                        { if ( *ptr > 128 ) { retres+=*ptr-128; } }
                ptr++;
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
           while ( cur_y < patch_y )
            { cur_x = 0;
              while ( cur_x < patch_x )
              {
                  if ( cur_x > cur_y ) { if ( *ptr < 128 ) { retres+=128-*ptr; } } else
                                       { if ( *ptr > 128 ) { retres+=*ptr-128; } }
                ++ptr;
                ++cur_x;
              }
              ++cur_y;
            }
        break;

        default : break;
     };

    return retres;
}


int ClearPatchSignature(struct PatchSignature * result)
{
    if ( result == 0 ) { fprintf(stderr,"ClearSegments called without result structure..\n"); return 0; }
    result->total_segments=0;
    int i; for ( i=0; i<MAX_SEGMENTS_SIGNATURE; i++) { result->segment[i]=0; }
    return 1;
}

unsigned int GetPatchSignature(unsigned char * patch, unsigned int image_x, unsigned int image_y ,  unsigned int x,unsigned int y , unsigned int patch_x,unsigned int patch_y , struct PatchSignature * result)
{

    //fprintf(stderr,"GetPatchSignature(patch,%u,%u,%u,%u,%u,%u,result)\n",image_x,image_y,x,y,patch_x,patch_y );

    if ( result == 0 ) { fprintf(stderr,"GetPatchSignature called without result structure..\n"); return 0; }
    ClearPatchSignature(result);


    if ( image_x <= x ) { fprintf(stderr,"Patch out of bounds X \n"); return 0; }
    if ( image_y <= y ) { fprintf(stderr,"Patch out of bounds Y \n"); return 0; }
    if ( x+patch_x >= image_x ) { return 0; }
    if ( y+patch_y >= image_y ) { return 0; }
    if ( ( patch_x == 0 ) || ( patch_y == 0 ) ) { return 0; }

    unsigned char * monochrome_patch = (unsigned char * ) malloc( (patch_x+1)*(patch_y+1)*sizeof(unsigned char) );
    if ( monochrome_patch == 0 ) { fprintf(stderr,"Could not allocate monochrome patch \n"); return 0; }
    unsigned char * ptr=monochrome_patch;
    //unsigned char * monochrome_patch_end =monochrome_patch + ( patch_x * patch_y ) ;

    unsigned char * r , * g , * b;
    unsigned char * real_patch_start = patch + ( image_x * y * 3 ) + 3 * x ;
    unsigned char * real_patch_tmp   = patch + ( image_x * y * 3 ) + 3 * x ;
    //unsigned int real_patch_incrementation = (image_x-patch_x) * 3  ;
    int col_med;


   // fprintf(stderr,"Each patch begins at (%u,%u) and has a size of (%u,%u)\n",x,y,patch_x,patch_y);
   // fprintf(stderr,"Patch incrementation is %u that means that after %u we go to %u\n",real_patch_incrementation,real_patch_tmp,real_patch_tmp+real_patch_incrementation);

    /* THIS SHOULD RUN FASTER , BUT IT SEGFAULTS ( FOR NOW :P )
    ptr=monochrome_patch;
    while ( ptr < monochrome_patch_end )
     {
         r = real_patch_tmp++;
         g = real_patch_tmp++;
         b = real_patch_tmp++;
         col_med = (unsigned char) ((*r+*g+*b)/3) ;

         *ptr = (unsigned char ) col_med;
         ptr+=1;
         real_patch_tmp += real_patch_incrementation;
     }*/


   int xi,yi;
   for (xi=x; xi<x+patch_x; xi++)
   { for (yi=y; yi<patch_y; yi++)
	 {

	   real_patch_tmp = (unsigned char *)  real_patch_start + ( image_x * yi * 3 ) + 3 * xi ;

	   r = real_patch_tmp++;
       g = real_patch_tmp++;
       b = real_patch_tmp;
       //Get Pixel Color
	   col_med=  ( *r + *g + *b )/3;
	   *ptr= (unsigned char )col_med ;
	   ++ptr;
    }
  } //MONOCHROME DONE



    ptr=monochrome_patch;
    result->total_segments=6;
    result->segment[0]=CompareToHAARWavelet(1,ptr,patch_x,patch_y);
    result->segment[1]=CompareToHAARWavelet(2,ptr,patch_x,patch_y);
    result->segment[2]=CompareToHAARWavelet(3,ptr,patch_x,patch_y);
    result->segment[3]=CompareToHAARWavelet(4,ptr,patch_x,patch_y);
    result->segment[4]=CompareToHAARWavelet(5,ptr,patch_x,patch_y);
    result->segment[5]=CompareToHAARWavelet(6,ptr,patch_x,patch_y);

    free(monochrome_patch);

    return 1;
}
