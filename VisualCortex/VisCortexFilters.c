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
#include "VisCortexFilters.h"
#include "VisCortexConvolutionFilters.h"
#include "VisCortexOptimizedConvolutionFilters.h"
#include "Precalculations.h"
#include "FeatureTracking.h"
#include "VisCortexTimer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned int inline CountEdges(unsigned int edges_required_to_process , unsigned int x , unsigned int y,unsigned int size_x , unsigned int size_y,unsigned char * edge_array)
{
    /*
                THIS CAN BE DONE MUCH FASTER , SEE IntegralImageConversion.c
              THIS FUNCTION IS DEPRECIATED , USED ONLY FOR DEBUGGING THE NEW ONES
    */
   unsigned int counted_edges=0;
   unsigned int x_c=x ,  y_c=y;
   register BYTE *px;
   register BYTE *stopx;

	     while (y_c<y+size_y)
				{
                  px= (BYTE *) edge_array+precalc_memplace_3byte[x_c][y_c];
				  stopx=px+(size_x*3);
				  while (px<stopx) { if ( *px!=0 ) { ++counted_edges; }  px+=3;  }
				  if ( edges_required_to_process < counted_edges ) { return counted_edges+1; } // ++PERFORMANCE --RESULT
				  ++y_c;
			 	}
  return counted_edges;
}



unsigned int inline PixelsOverThresholdSetAsOne(int image_reg,unsigned int threshold)
{
   if (video_register[image_reg].depth!=1) { fprintf(stderr,"Function PixelsOverThresholdSetAsOne assumes 1byte array\n"); return 0; }

 unsigned int image_size=metrics[RESOLUTION_MEMORY_LIMIT_1BYTE];

 register BYTE *start_px = (BYTE *) video_register[image_reg].pixels;
 register BYTE *px = (BYTE *) video_register[image_reg].pixels;

 while ( px < start_px+image_size)
 {
        if ( *px > threshold ) { *px = 1; } else { *px = 0; }
        ++px;
 }

  return 1;
}

unsigned int HistogramPatch(struct Histogram *hist_data,unsigned char *img,unsigned int px,unsigned int py,unsigned int patch_x,unsigned int patch_y)
{
    /*
      THERE ALSO EXISTS A COMPRESSED HISTOGRAM PATCH ( MUCH FASTER , SEE IntegralImageConversion.c
              THIS FUNCTION IS DEPRECIATED , USED ONLY FOR DEBUGGING THE NEW ONES
    */
    register BYTE *image_px,*image_stopx;
    register BYTE *r,*g,*b;

    unsigned int tot_r=0,tot_g=0,tot_b=0;

    unsigned int y=0;
    while (y<patch_y)
	{
      image_px= (BYTE *) img+precalc_memplace_3byte[px][y+py];
      image_stopx=image_px+3*patch_x;

      while (image_px<image_stopx)
	  {
        r=image_px++;
        tot_r+=*r;

		g=image_px++;
		tot_g+=*g;

		b=image_px++;
		tot_b+=*b;
      }
      ++y;
	}

  unsigned int tot_pixels=patch_x*patch_y;
  unsigned int tmp;

  tmp = (unsigned int) (tot_r/tot_pixels);
  hist_data->median_r=(unsigned char) tmp;

  tmp = (unsigned int) (tot_g/tot_pixels);
  hist_data->median_g=(unsigned char) tmp;

  tmp = (unsigned int) (tot_b/tot_pixels);
  hist_data->median_b=(unsigned char) tmp;

  tmp = (unsigned int) ( (hist_data->median_r + hist_data->median_g + hist_data->median_b) / 3 );
  hist_data->median=(unsigned char) tmp;
 return 1;
}


void Monochrome(unsigned char * input_frame,int image_x,int image_y)
{
  if (input_frame==0) {return;}
  int col_med;
  unsigned int image_size=metrics[RESOLUTION_MEMORY_LIMIT_3BYTE];

 register BYTE *start_px = (BYTE *) input_frame;
 register BYTE *px = (BYTE *) input_frame;
 register BYTE *r;
 register BYTE *g;
 register BYTE *b;

 while ( px < start_px+image_size)
 {
       r = px++; g = px++; b = px++;

       col_med= ( *r + *g + *b )/3;
*r= (BYTE)col_med ;
       *g=*r;
*b=*r;
 }

 return;
}

void MonochromeL(unsigned char * input_frame,int image_x,int image_y)
{
  if (input_frame==0) {return;}
  int col_med;

 unsigned int image_size=metrics[RESOLUTION_MEMORY_LIMIT_3BYTE];
 register BYTE *start_px = (BYTE *) input_frame;
 register BYTE *px = (BYTE *) input_frame;
 register BYTE *r;
 register BYTE *g;
 register BYTE *b;

  while ( px < start_px+image_size)
 {
       r = px++; g = px++; b = px++;

	   col_med= ( *r * 0.299 + *g * 0.587 + *b * 0.114 );
	   *r= (BYTE)col_med ;
       *g=*r;
	   *b=*r;
 }

 return;
}


void KillDifferentPixels(unsigned char * image,int image_x,int image_y,unsigned char R,unsigned char G,unsigned char B,unsigned char threshold)
{ if (image==0) {return;}

 unsigned int image_size=metrics[RESOLUTION_MEMORY_LIMIT_3BYTE];
 register BYTE *start_px = (BYTE *) image;
 register BYTE *px = (BYTE *) image;
 register BYTE *r;
 register BYTE *g;
 register BYTE *b;
 char mismatch=0;

  while ( px < start_px+image_size)
 {
       r = px++; g = px++; b = px++;

	   mismatch=0;
	   if (R > *r) { if (R-*r > threshold) { ++mismatch; }  } else
                   { if (*r-R > threshold) { ++mismatch; }  }

       if (G > *g) { if (G-*g > threshold) { ++mismatch; }  } else
                   { if (*g-G > threshold) { ++mismatch; }  }

       if (B > *b) { if (B-*b > threshold) { ++mismatch; }  } else
                   { if (*b-B > threshold) { ++mismatch; }  }

       if (mismatch>0) { *r=0;   *g=0;  *b=0; }
 }
 return;

}

void KillPixelsBetween(unsigned int image_reg,int low_threshold,int high_threshold)
{
 unsigned char * image = video_register[image_reg].pixels;
 if (image==0) {return;}
 unsigned int image_depth = video_register[image_reg].depth;


StartTimer(PIXEL_OVER_THRESHOLD_DELAY); // STATISTICS KEEPER FOR HYPERVISOR | START

 register BYTE *start_px = (BYTE *) image;
 register BYTE *px = (BYTE *) image;
 register BYTE *r;
 register BYTE *g;
 register BYTE *b;
 unsigned int image_size=metrics[RESOLUTION_MEMORY_LIMIT_3BYTE];

 if ( image_depth == 3 )
 {
   while ( px < start_px + image_size )
     {
       r = px++;
       g = px++;
       b = px++;

       // The kill Pixels function kills selected channels to help stabilize output
       if ((*r>high_threshold)||(*r<low_threshold)) { *r=0; }
       if ((*g>high_threshold)||(*g<low_threshold)) { *g=0; }
       if ((*b>high_threshold)||(*b<low_threshold)) { *b=0; }
    }
  }
   else
  if ( image_depth == 1 )
 {
   image_size=metrics[RESOLUTION_MEMORY_LIMIT_1BYTE];
   while (px<start_px+image_size)
   {
	  if  ((*px>high_threshold)||(*px<low_threshold)) { *px=0; }
	  px++;
   }
  }

EndTimer(PIXEL_OVER_THRESHOLD_DELAY); // STATISTICS KEEPER FOR HYPERVISOR | END
 return;
}

void KillPixelsBelow(unsigned int image_reg,int threshold)
{
 unsigned char * image = video_register[image_reg].pixels;
 if (image==0) {return;}

 register BYTE *start_px = (BYTE *) image;
 register BYTE *px = (BYTE *) image;
 register BYTE *r;
 register BYTE *g;
 register BYTE *b;
 unsigned int image_size=metrics[RESOLUTION_MEMORY_LIMIT_3BYTE];

 if ( ThisIsA3ByteRegister(image_reg) )
 {
   while ( px < start_px + image_size )
     {
       r = px++;
       g = px++;
       b = px++;

       // The kill Pixels function kills selected channels to help stabilize output
       if (*r<threshold) { *r=0; }
       if (*g<threshold) { *g=0; }
       if (*b<threshold) { *b=0; }
    }
  }
   else
  if ( ThisIsA1ByteRegister(image_reg) )
 {
   image_size=metrics[RESOLUTION_MEMORY_LIMIT_1BYTE];
   while (px<start_px+image_size)
   {
	  if  ( *px<threshold) { *px=0; }
	  px++;
   }
  }

 return;
}


void Kill3PixelsBelow(unsigned int image_reg,int threshold)
// LIKE KILLPIXELSBELOW , BUT WHENEVER EITHER A R , G or B PIXEL IS FOUND UNDER THRESHOLD IT WILL WIPE OUT ALL OF THE COLOR CHANNELS
// IN THE PIXEL!
{
 unsigned char * image = video_register[image_reg].pixels;
  int image_x=video_register[image_reg].size_x;
  int image_y=video_register[image_reg].size_y;
 if (image==0) {return;}
 unsigned int image_depth = video_register[image_reg].depth;

if ( image_depth == 1 )
 {
     KillPixelsBelow(image_reg,threshold);
     return;
 }


 register BYTE * px;
 register BYTE * endx;
 register BYTE *r; register BYTE *g; register BYTE *b;

  px =  (BYTE *)  image;
  endx = ( (BYTE *)    image + (image_x * (image_y-1) *3 ) + (3*(image_x-1)) );
  while (px<endx)
  {
	  r = px++;
      g = px++;
      b = px++;
	  if  ( (*r<threshold) || (*g<threshold) || (*b<threshold) ) { *r=0; *g=0; *b=0; }
  }


 return;
}


void ReducePalette(unsigned int image_reg,int new_palette)
{
 if (!ThisIsA3ByteRegister(image_reg)) { return; }
 unsigned char * image = video_register[image_reg].pixels;
 if (image==0) {return;}
 unsigned int image_size=metrics[RESOLUTION_MEMORY_LIMIT_3BYTE];
 register BYTE *start_px = ( BYTE * ) image;
 register BYTE *px = ( BYTE * ) image;
 register BYTE *r;
 register BYTE *g;
 register BYTE *b;

  while ( px < start_px + image_size )
  {
       r = px++;
       g = px++;
       b = px++;

       // The kill Pixels function kills selected channels to help stabilize output
	   *r=  (BYTE) ( (*r) * new_palette ) / 255;
       *g=  (BYTE) ( (*g) * new_palette ) / 255;
       *b=  (BYTE) ( (*b) * new_palette ) / 255;
  }

 return;
}





int SobelFromSource(unsigned int source_reg,unsigned int target_reg)
{
  if (!ThisIsA3ByteRegister(source_reg)) { return 0; }
  unsigned char * source = video_register[source_reg].pixels;
  unsigned char * target = video_register[target_reg].pixels;
  int image_x=video_register[source_reg].size_x;
  int image_y=video_register[source_reg].size_y;
  video_register[target_reg].depth=1;
  unsigned int x=0,y=0;
  unsigned int x1=1,y1=1,x2=image_x,y2=image_y;


  if ( (target==0) || (source==0) ) {return(0);}

  //unsigned char *proc_image;
  //proc_image = new unsigned char [ image_x * image_y * 3 ];

StartTimer(FIRST_DERIVATIVE_DELAY); // STATISTICS KEEPER FOR HYPERVISOR | START
  BYTE *px;

 BYTE p1=0,p2=0,p3=0,p4=0,p5=0,p6=0,p7=0,p8=0,p9=0;

 int sum1=0,sum2=0;
 int sum=0;
 float acol=0.0;

 //SOBEL EDGE DETECTION

  for (x=x1; x<x2; x++)
 { for (y=y1; y<y2; y++)
  {

	     px = (BYTE *)  source + precalc_memplace_3byte[x-1][y-1];
         //px = ((BYTE *)  image + (effW * (y-1) ) + (3* (x-1) ) );
		 p1 = *px;
		 px+=5;
         p2 = *px++;
         p3 = *px;



		 px = (BYTE *)  source + precalc_memplace_3byte[x-1][y];
         //px = ((BYTE *)  image + (effW * y ) + (3* (x-1) ) );
         p4 = *px;
         px+=5;
		 p5 = *px++;
         p6 = *px;



		 px = (BYTE *)  source + precalc_memplace_3byte[x-1][y+1];
         //px = ((BYTE *)  image + (effW * (y+1) ) + (3* (x-1) ) );
         p7 = *px;
         px+=5;
		 p8 = *px++;
         p9 = *px;


        //Get Pixel Color
        sum1 =( p1 + 2*p2 + p3 - p7 - 2*p8 - p9 );
        sum2 =( p1  + 2*p4 + p7 - p3 - 2*p6 - p9 );
        acol=(sum1*sum1) + (sum2*sum2);
		acol=floor(sqrt(acol));
		sum = (unsigned long)  acol ;

		if (sum> 255) { sum = 255; }



		px = (BYTE *)  target + precalc_memplace_1byte[x][y];
        //px = ((BYTE *)  proc_image + (effW * y ) + (3*x) );
	    *px=  (BYTE) sum ;
   }
 } //SOBEL FILTER DONE

EndTimer(FIRST_DERIVATIVE_DELAY); // STATISTICS KEEPER FOR HYPERVISOR | END
MarkRegistersAsSynced(source_reg,target_reg);

return (1);
}


int  Sobel(unsigned int image_reg)
{
  if (!ThisIsA3ByteRegister(image_reg)) { return 0; }

    unsigned int TMP_REGISTER = GetTempRegister();
    if (TMP_REGISTER == 0 ) { fprintf(stderr," Error Getting a temporary Video Register ( Sobel ) \n"); return 0; }

    CopyRegister(image_reg,TMP_REGISTER,0,0);

    SobelFromSource(TMP_REGISTER,image_reg);

    StopUsingVideoRegister(TMP_REGISTER);

  return 1;
}


int FirstDerivativeIntensitiesFromSource(unsigned int source_reg,unsigned int target_reg)
{
    if (!ThisIsA1ByteRegister(source_reg)) { fprintf(stderr,"FirstDerivative Intensities requires monochrome image conversion"); return 0; }

    StartTimer(FIRST_DERIVATIVE_DELAY); // STATISTICS KEEPER FOR HYPERVISOR | START

    signed char table[9];//={1,-2,1,2,-4,2,1,-2,1};
    signed int divisor = 1;

    /* SCHAR FILTER HORIZONTAL
    table[0]= 3; table[1]= 10; table[2]= 3;
    table[3]= 0; table[4]=  0; table[5]= 0;
    table[6]=-3; table[7]=-10; table[8]=-3;
    */

    /* SCHAR FILTER VERTICAL
    table[0]=  3; table[1]=  0; table[2]= -3;
    table[3]= 10; table[4]=  0; table[5]= -10;
    table[6]=  3; table[7]=  0; table[8]= -3;
    */

    //SOBEL FILTER HORIZONTAL
    table[0]= 1; table[1]= 2; table[2]= 1;
    table[3]= 0; table[4]= 0; table[5]= 0;
    table[6]=-1; table[7]=-2; table[8]=-1;

     /*
    //SOBEL FILTER VERTICAL
    table[0]= 1; table[1]= 0; table[2]= -1;
    table[3]= 2; table[4]= 0; table[5]= -2;
    table[6]= 1; table[7]= 0; table[8]= -1;*/


    unsigned int retres = ConvolutionFilter9_1Byte(source_reg,target_reg,table,divisor);

    EndTimer(FIRST_DERIVATIVE_DELAY); // STATISTICS KEEPER FOR HYPERVISOR | END
    MarkRegistersAsSynced(source_reg,target_reg);

    return retres;
}


int SecondDerivativeIntensitiesFromSource(unsigned int source_reg,unsigned int target_reg)
{
    if (!ThisIsA1ByteRegister(source_reg)) { fprintf(stderr,"SecondDerivative Intensities requires monochrome image conversion"); return 0; }

    StartTimer(SECOND_DERIVATIVE_DELAY); // STATISTICS KEEPER FOR HYPERVISOR | START

    signed char table[9]={-1,0,1,0,0,0,1,0,-1};
    signed int divisor = 3;
    unsigned int retres = ConvolutionFilter9_1Byte(source_reg,target_reg,table,divisor);

    EndTimer(SECOND_DERIVATIVE_DELAY); // STATISTICS KEEPER FOR HYPERVISOR | END
    MarkRegistersAsSynced(source_reg,target_reg);

    return retres;
}



int GaussianBlurFromSource(unsigned int source_reg,unsigned int target_reg,int monochrome)
{
  if (!ThisIsA3ByteRegister(source_reg)) { return 0; }
  if ( (video_register[source_reg].pixels==0) || (video_register[target_reg].pixels==0) ) {return(0);}

  StartTimer(GAUSSIAN_DELAY); // STATISTICS KEEPER FOR HYPERVISOR | START

  signed char table[9];//={1,1,1,1,1,1,1,1,1};
  signed int divisor;

  /* SIMPLE BLUR FILTER
  divisor = 9;
  table[0]=1; table[1]=1; table[2]=1;
  table[3]=1; table[4]=1; table[5]=1;
  table[6]=1; table[7]=1; table[8]=1;
  */

  // GAUSSIAN BLUR FILTER
  divisor = 16;
  table[0]=1; table[1]=2; table[2]=1;
  table[3]=2; table[4]=4; table[5]=2;
  table[6]=1; table[7]=2; table[8]=1;

  ConvolutionFilter9_AutoByteOptimized(source_reg,target_reg,table,divisor);

  EndTimer(GAUSSIAN_DELAY); // STATISTICS KEEPER FOR HYPERVISOR | END
  MarkRegistersAsSynced(source_reg,target_reg);

 return (1);
}

int GaussianBlur(unsigned int image_reg,int monochrome)
{
  return GaussianBlurFromSource(image_reg,image_reg,monochrome);
}

unsigned int FloodPixel(unsigned char * picture_array,unsigned char * result_array,unsigned int point_x,unsigned int point_y,unsigned int size_x,unsigned int size_y,unsigned char size_rgb)
{
	fprintf(stderr,"FloodPixel not implemented!\n");
	return 0;
	register unsigned int ptr_x=0,y,x;

	x=point_x;
	for ( y=point_y-1; point_y>=0; y--)
	   {
	       ptr_x=( (y) * size_x * size_rgb )+( x * size_rgb );
	   }
  return 0;
}



void PrepareCleanSobeledGaussianAndDerivativeOLD(unsigned int rgb_image_reg,unsigned int target_sobel_image_reg,unsigned int target_derivative_image_reg,unsigned int kill_lower_edges_threshold,unsigned int kill_higher_edges_threshold)
{
    GaussianBlurFromSource(rgb_image_reg,target_sobel_image_reg,1);

	Sobel(target_sobel_image_reg);

	KillPixelsBetween(target_sobel_image_reg,kill_lower_edges_threshold,kill_higher_edges_threshold);

    unsigned int TMP_REGISTER = GetTempRegister();
    if (TMP_REGISTER == 0 ) { fprintf(stderr," Error Getting a temporary Video Register ( PrepareCleanSobeledGaussianAndDerivative ) \n"); return; }

    CopyRegister(rgb_image_reg,TMP_REGISTER,0,0);

    ConvertRegisterFrom3ByteTo1Byte(TMP_REGISTER);

	SecondDerivativeIntensitiesFromSource(TMP_REGISTER,target_derivative_image_reg);

    StopUsingVideoRegister(TMP_REGISTER);
}

void PrepareCleanSobeledGaussianAndDerivative(unsigned int rgb_image_reg,unsigned int target_sobel_image_reg,unsigned int target_derivative_image_reg,unsigned int kill_lower_edges_threshold,unsigned int kill_higher_edges_threshold)
{

    unsigned int TMP_REGISTER = GetTempRegister();
    if (TMP_REGISTER == 0 ) { fprintf(stderr," Error Getting a temporary Video Register ( PrepareCleanSobeledGaussianAndDerivative ) \n"); return; }

    GaussianBlurFromSource(rgb_image_reg,TMP_REGISTER,1);

    ConvertRegisterFrom3ByteTo1Byte(TMP_REGISTER);

    FirstDerivativeIntensitiesFromSource(TMP_REGISTER,target_sobel_image_reg);

	KillPixelsBetween(target_sobel_image_reg,kill_lower_edges_threshold,kill_higher_edges_threshold);

	SecondDerivativeIntensitiesFromSource(TMP_REGISTER,target_derivative_image_reg);

    StopUsingVideoRegister(TMP_REGISTER);

}

int CalibrateImage(unsigned int rgb_image,unsigned int rgb_calibrated,unsigned int * M)
{
   MarkVideoRegistersAsUnsynced(rgb_calibrated , rgb_image);


    /* TODO , For now the full register is returned!*/
       if ( settings[INPUT_CALIBRATION]==0 )
         {
           int i = CopyRegister(rgb_image,rgb_calibrated,1,1);
           video_register[rgb_calibrated].time = video_register[rgb_image].time;
           return i;
         }
    /*The array M is the array calculated from Precalculations.c to speed up things*/

    StartTimer(CALIBRATION_DELAY); // STATISTICS KEEPER FOR HYPERVISOR | START

    unsigned int ptr=0 , new_ptr = 0, ptr_end = metrics[RESOLUTION_MEMORY_LIMIT_3BYTE] ;

    video_register[rgb_image].pixels[0]=255; // DEBUG KEEP BLACK PIXEL
    video_register[rgb_image].pixels[1]=0; // DEBUG KEEP BLACK PIXEL
    video_register[rgb_image].pixels[2]=0; // DEBUG KEEP BLACK PIXEL
    video_register[rgb_image].pixels[3]=255; // DEBUG KEEP BLACK PIXEL
    video_register[rgb_image].pixels[4]=0; // DEBUG KEEP BLACK PIXEL
    video_register[rgb_image].pixels[5]=0; // DEBUG KEEP BLACK PIXEL
    memset(video_register[rgb_calibrated].pixels,0,metrics[RESOLUTION_MEMORY_LIMIT_3BYTE]*sizeof(unsigned char));

    while (ptr < ptr_end)
     {
         new_ptr = M[ptr];
         video_register[rgb_calibrated].pixels[new_ptr] = video_register[rgb_image].pixels[ptr];
         ++new_ptr; ++ptr;

         video_register[rgb_calibrated].pixels[new_ptr] = video_register[rgb_image].pixels[ptr];
         ++new_ptr; ++ptr;

         video_register[rgb_calibrated].pixels[new_ptr] = video_register[rgb_image].pixels[ptr];
         ++ptr;
     }

   video_register[rgb_calibrated].time = video_register[rgb_image].time;

   EndTimer(CALIBRATION_DELAY); // STATISTICS KEEPER FOR HYPERVISOR | END

   return 1;
}

