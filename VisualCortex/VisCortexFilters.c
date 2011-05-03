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
#include "Precalculations.h"
#include "FeatureTracking.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned int inline CountEdges(unsigned int edges_required_to_process , unsigned int x , unsigned int y,unsigned int size_x , unsigned int size_y,unsigned char * edge_array)
{
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
                                THIS FUNCTION IS DEPRECIATED
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

       col_med=  ( *r + *g + *b )/3;
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


void KillPixelsBelow(unsigned int image_reg,int image_x,int image_y,int threshold)
{
 unsigned char * image = video_register[image_reg].pixels;
 if (image==0) {return;}
 unsigned int image_depth = video_register[image_reg].depth;

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
       if (*r<threshold) { *r=0; }
       if (*g<threshold) { *g=0; }
       if (*b<threshold) { *b=0; }
    }
  }
   else
  if ( image_depth == 1 )
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


void Kill3PixelsBelow(unsigned int image_reg,int image_x,int image_y,int threshold)
// LIKE KILLPIXELSBELOW , BUT WHENEVER EITHER A R , G or B PIXEL IS FOUND UNDER THRESHOLD IT WILL WIPE OUT ALL OF THE COLOR CHANNELS
// IN THE PIXEL!
{
 unsigned char * image = video_register[image_reg].pixels;
 if (image==0) {return;}
 unsigned int image_depth = video_register[image_reg].depth;

if ( image_depth == 1 )
 {
     KillPixelsBelow(image_reg,image_x,image_y,threshold);
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


void ReducePalette(unsigned int image_reg,int image_x,int image_y,int new_palette)
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



BOOLEAN  Sobel(unsigned int image_reg,int image_x,int image_y)
{
  if (!ThisIsA3ByteRegister(image_reg)) { return 0; }
  unsigned char * image = video_register[image_reg].pixels;
  video_register[image_reg].depth=1;
  unsigned int x=0,y=0;
  unsigned int x1=1,y1=1,x2=image_x,y2=image_y;


  if (image==0) { return(0); }

  unsigned char *proc_image;
  //proc_image = new unsigned char [ image_x * image_y * 3 ];
  proc_image = ( unsigned char * ) malloc ( sizeof(unsigned char) * image_x * image_y * 3 );

  BYTE *px;

 BYTE p1=0,p2=0,p3=0,p4=0,p5=0,p6=0,p7=0,p8=0,p9=0;

 int sum1=0,sum2=0;
 int sum=0;
 float acol=0.0;

 //SOBEL EDGE DETECTION

  for (x=x1; x<x2; x++)
 { for (y=y1; y<y2; y++)
  {

	     px = (BYTE *)  image + precalc_memplace_3byte[x-1][y-1];
         //px = ((BYTE *)  image + (effW * (y-1) ) + (3* (x-1) ) );
		 p1 = *px;
		 px+=5;
         p2 = *px++;
         p3 = *px;



		 px = (BYTE *)  image + precalc_memplace_3byte[x-1][y];
         //px = ((BYTE *)  image + (effW * y ) + (3* (x-1) ) );
         p4 = *px;
         px+=5;
		 p5 = *px++;
         p6 = *px;



		 px = (BYTE *)  image + precalc_memplace_3byte[x-1][y+1];
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



		px = (BYTE *)  proc_image + precalc_memplace_1byte[x][y];
        //px = ((BYTE *)  proc_image + (effW * y ) + (3*x) );
	    *px=  (BYTE) sum ;
   }
 } //SOBEL FILTER DONE


memcpy(image,proc_image,image_x*image_y*sizeof(BYTE));


free(proc_image);

return (1);
}


BOOLEAN SobelFromSource(unsigned int source_reg,unsigned int target_reg,int image_x,int image_y)
{
  if (!ThisIsA3ByteRegister(source_reg)) { return 0; }
  unsigned char * source = video_register[source_reg].pixels;
  unsigned char * target = video_register[target_reg].pixels;
  video_register[target_reg].depth=1;
  unsigned int x=0,y=0;
  unsigned int x1=1,y1=1,x2=image_x,y2=image_y;


  if ( (target==0) || (source==0) ) {return(0);}

  //unsigned char *proc_image;
  //proc_image = new unsigned char [ image_x * image_y * 3 ];

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


return (1);
}



BOOLEAN SobelNDegreeDerivative(int n,unsigned int source_reg,unsigned int target_reg,int image_x,int image_y)
{
    SobelFromSource(source_reg,target_reg,image_x,image_y);
    int i=1;
    while ( i < n )
     {
         Sobel(target_reg,image_x,image_y);
         ++i;
     }
    return 1;
}



BOOLEAN GaussianBlurFromSource(unsigned int source_reg,unsigned int target_reg,int image_x,int image_y,BOOLEAN monochrome)
{
  if (!ThisIsA3ByteRegister(source_reg)) { return 0; }
  unsigned char * source = video_register[source_reg].pixels;
  unsigned char * target = video_register[target_reg].pixels;
  video_register[target_reg].depth = 3;
  int effW = (image_x*3);

  if ( (target==0) || (source==0) ) {return(0);}

  unsigned char *proc_image;
  proc_image = (unsigned char *) malloc( sizeof(unsigned char) * image_x * image_y * 3);

  BYTE *px,*px2;
  BYTE *r,*r2;
  BYTE *g,*g2;
  BYTE *b,*b2;

  double linc_r=0.0,linc_g=0.0,linc_b=0.0;

     //
     const int gauss_w = 7;                            // Level(+1)
     int mask[7]={1,6,15,20,15,6,1}; // Mask
     int gauss_sum=64;                           // Sum

     register int i,k,j;
     //For every pixel on the temporary bitmap ...
     for(i=gauss_w-1;i<image_y-1;i++)
	 {
       for(j=0;j<image_x;j++)
	   {
         linc_r=0.0 , linc_g=0.0 , linc_b=0.0;

		 for(k=0;k<gauss_w;k++)
		 {
          // color=o.getPixel(j,i-(gauss_w-1)+k);
 	      px = ((BYTE *)  source + (effW * (i-(gauss_w-1)+k) ) + (3* (j) ) );

          r= px++ , g = px++ , b = px;

          linc_r+=(*r)*mask[k] , linc_g+=(*g)*mask[k] , linc_b+=(*b)*mask[k];
         }

        px = ((BYTE *)  proc_image + (effW * (i) ) + (3* (j) ) );
        r2= px++ , g2 = px++ , b2 = px;

		*r2=linc_r/gauss_sum , *g2=linc_g/gauss_sum , *b2=linc_b/gauss_sum;

        // tmp.drawPixel(Pixel(,linc_g/gauss_sum,linc_b/gauss_sum),j,i);
      }
     }
     //For every pixel on the output bitmap ...
     for(i=0;i<image_y;i++)
	 {
       for(j=gauss_w;j<image_x-1;j++)
	   {
         linc_r=0.0 , linc_g=0.0 , linc_b=0.0;

         for(k=0;k<gauss_w;k++){
           px = ((BYTE *)  proc_image + (effW * (i) ) + (3* (j-(gauss_w-1)+k) ) );

		   r= px++ , g = px++ , b = px;

           linc_r+=(*r)*mask[k] , linc_g+=(*g)*mask[k] , linc_b+=(*b)*mask[k];
         }

         linc_r/=gauss_sum , linc_g/=gauss_sum , linc_b/=gauss_sum;

       //  result.drawPixel(Pixel((byte)link_r,(byte)link_g,(byte)link_b),j,i);

	     px = ((BYTE *)  target + (effW * (i) ) + (3* (j) ) );
		 r2= px++ , g2 = px++ , b2 = px;

	    if ( (i>gauss_w) && (j>gauss_w) )
		  {
			*r2=(BYTE) linc_r , *g2=(BYTE) linc_g , *b2=(BYTE) linc_b;
		  }
	   }
     }

   //
   //    FILL IN PARTS OF THE BLUR THAT ARE UNDER THE THRESHOLD AND ARE LEFT EMPTY!
   //
     unsigned int offset=3; // ( gauss_w / 2 )
     // VERTICAL LEFT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
     for(i=0;i<image_y;i++)
	 {
       for(j=0;j<=gauss_w;j++)
	   {
	     px  = ((BYTE *)  target + (effW * (i+offset) ) + (3* (j) ) );
		 px2 = ((BYTE *)  source + (effW * (i) ) + (3* (j) ) );

         r= px++ , g = px++ , b = px;
         r2= px2++ , g2 = px2++ , b2 = px2;
         *r = *r2 , *g = *g2 ,  *b = *b2;
	   }
	 }
     // HORIZONTAL UP!<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	 for(i=0; i<=gauss_w; i++)
	 {
       for(j=0; j<image_x; j++)
	   {
	     px  = ((BYTE *)  target + (effW * (i) ) + (3* (j+offset) ) );
		 px2 = ((BYTE *)  source + (effW * (i) ) + (3* (j) ) );

         r= px++ , g = px++ , b = px;
         r2= px2++ , g2 = px2++ , b2 = px2;
         *r = *r2 , *g = *g2 ,  *b = *b2;
	   }
	 }

 free(proc_image);

 return (1);
}

BOOLEAN GaussianBlur(unsigned int image_reg,int image_x,int image_y,BOOLEAN monochrome)
{
  return GaussianBlurFromSource(image_reg,image_reg,image_x,image_y,monochrome);
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



void PrepareCleanSobeledGaussian(unsigned int rgb_image_reg,unsigned int target_image_reg,unsigned int kill_lower_edges_threshold)
{
    GaussianBlurFromSource(rgb_image_reg,target_image_reg,metrics[RESOLUTION_X],metrics[RESOLUTION_Y],1);
	Sobel(target_image_reg,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
	KillPixelsBelow(target_image_reg,metrics[RESOLUTION_X],metrics[RESOLUTION_Y],kill_lower_edges_threshold);
}


int CalibrateImage(unsigned int rgb_image,unsigned int rgb_calibrated)
{
    /*TODO HERE ADD CALIBRATION CODE*/
    return GaussianBlurFromSource(rgb_image,rgb_calibrated,video_register[rgb_image].size_x,video_register[rgb_image].size_y,0);
}

