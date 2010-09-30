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
#include "PatchTracking.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void CopyPartOfImageToImage(unsigned char * input_img,unsigned char * output_img,unsigned int px,unsigned int py,unsigned int tx,unsigned int ty,unsigned int size_x,unsigned int size_y)
{
  register unsigned char *input_pointer,*input_pointer_startline,*input_pointer_endline,*input_pointer_stop;
  register unsigned char *output_pointer,*output_pointer_startline,*output_pointer_stop;

  if (px>=metrics[RESOLUTION_X]) { fprintf(stderr,"CopyPartOfImageToImage : Source X out of bounds\n"); return; }
  if (px+size_x>=metrics[RESOLUTION_X]) { fprintf(stderr,"CopyPartOfImageToImage : Source X Size out of bounds\n");
                                          size_x=metrics[RESOLUTION_X]-px;}

  if (py>=metrics[RESOLUTION_Y]) { fprintf(stderr,"CopyPartOfImageToImage : Source Y out of bounds\n"); return; }
  if (py+size_y>=metrics[RESOLUTION_Y]) { fprintf(stderr,"CopyPartOfImageToImage : Source Y Size out of bounds\n");
                                          size_y=metrics[RESOLUTION_Y]-py;}

  if (tx>=metrics[RESOLUTION_X]) { fprintf(stderr,"CopyPartOfImageToImage : Target X out of bounds\n"); return; }
  if (tx+size_x>=metrics[RESOLUTION_X]) { fprintf(stderr,"CopyPartOfImageToImage : Target X Size out of bounds\n");
                                          size_x=metrics[RESOLUTION_X]-tx;}

  if (ty>=metrics[RESOLUTION_Y]) { fprintf(stderr,"CopyPartOfImageToImage : Target Y out of bounds\n"); return; }
  if (ty+size_y>=metrics[RESOLUTION_Y]) { fprintf(stderr,"CopyPartOfImageToImage : Target Y Size out of bounds\n");
                                          size_y=metrics[RESOLUTION_Y]-ty;}

  unsigned int line_size =size_x * 3;

  input_pointer=(BYTE *) input_img+precalc_memplace_3byte[px][py];
  input_pointer_startline=input_pointer;
  input_pointer_endline=input_pointer_startline+line_size;
  input_pointer_stop=(BYTE *) input_img+precalc_memplace_3byte[px+size_x][py+size_y];

  output_pointer=(BYTE *) output_img+precalc_memplace_3byte[tx][ty];
  output_pointer_startline=output_pointer;
  output_pointer_stop=(BYTE *) output_img+precalc_memplace_3byte[tx+size_x][ty+size_y];

  register unsigned char *r,*g,*b;
  while ( (input_pointer<input_pointer_stop) && (input_pointer<input_pointer_stop)  && (output_pointer<output_pointer_stop) )
  {
    while (input_pointer<input_pointer_endline)
    {
       r=(unsigned char*) input_pointer++; *output_pointer = *r; ++output_pointer;
       g=(unsigned char*) input_pointer++; *output_pointer = *g; ++output_pointer;
       b=(unsigned char*) input_pointer++; *output_pointer = *b; ++output_pointer;
    }

    input_pointer_startline+=metrics[RESOLUTION_X_3_BYTE];
    input_pointer_endline=input_pointer_startline+line_size;
    input_pointer=input_pointer_startline;

    output_pointer_startline+=metrics[RESOLUTION_X_3_BYTE];
    output_pointer=output_pointer_startline;

  }
}

inline void GenerateCompressHistogramOfImage_AddUpHorizontal(unsigned char * input_img,unsigned short * output_img,unsigned int block_x,unsigned int block_y)
{
    register unsigned char *startblock_px,*image_px;
    unsigned short * store_px;
    unsigned short * store_r,*store_g,*store_b;
    register unsigned char *r,*g,*b;

    unsigned int tot_r=0,tot_g=0,tot_b=0;
    unsigned int first_r=0,first_g=0,first_b=0;
    unsigned int x=0,xlim,y=0;

    unsigned int res_x=metrics[RESOLUTION_X];
    unsigned int vertical_position=0,vertical_step=metrics[RESOLUTION_X_3_BYTE];
    while (y<metrics[RESOLUTION_Y])
	{

      image_px=input_img+vertical_position;
      startblock_px=input_img+vertical_position;
      store_px=output_img+vertical_position;


      vertical_position+=vertical_step;

      tot_r=0,tot_g=0,tot_b=0; // THE LINE IS INITIALLY EMPTY

       r=startblock_px++; first_r = *r;
       g=startblock_px++; first_g = *g;
       b=startblock_px++; first_b = *b;
      // WE PREPARE OUR BUFFER WE SUM UP THE FIRST PATCH_X R , G AND B VALUES!
      x=0;
      xlim=block_x;
      while (x<xlim)
	  {
        r=image_px++; tot_r+=*r;
		g=image_px++; tot_g+=*g;
		b=image_px++; tot_b+=*b;
		++x;
      }


      // WE SET THE END , THE X END OF THE CURRENT Y LINE OF THE  IMAGE
      xlim=res_x; // metrics[RESOLUTION_X]
      // HORIZONTAL MAIN LOOP THAT ADDS UP EVERYTHING
      while (x<xlim)
	  {
       //STORE THE HORIZONTAL SUM AT THE FIRST PIXEL
       store_r=store_px++; *store_r = (unsigned short) tot_r;
       store_g=store_px++; *store_g = (unsigned short) tot_g;
       store_b=store_px++; *store_b = (unsigned short) tot_b;

       //SUBTRACT FIRST PIXEL FROM NEW SUM
       tot_r-=first_r; tot_g-=first_g; tot_b-=first_b;

       //NEW FIRST PIXEL IS LAST AT IMAGE_PX
       r=startblock_px++; first_r = *r;
       g=startblock_px++; first_g = *g;
       b=startblock_px++; first_b = *b;

       r=image_px++; tot_r+=*r;
       g=image_px++; tot_g+=*g;
	   b=image_px++; tot_b+=*b;
	   ++x;
	  }
      ++y;
	}
return;
}

inline void GenerateCompressHistogramOfImage_AddUpVertical(unsigned short * output_img,unsigned int block_x,unsigned int block_y)
{

    unsigned short *start_store_px,*store_px;
    unsigned short *r,*g,*b,*store_r,*store_g,*store_b;

    unsigned int tot_r=0,tot_g=0,tot_b=0;
    unsigned int first_tot_r=0,first_tot_g=0,first_tot_b=0;
    unsigned int xlim,ylim;
    unsigned int total_area=block_x*block_y;
    unsigned int vertical_step=metrics[RESOLUTION_X_3_BYTE];


  /* THELOUME GIA KATHE ENA APO TA PIXELS STON X AKSONA NA ATHROISOUME TO PRECALCULATION
     TO OPOIO EXEI DIMIOURGITHEI KATHETA!
  */
  unsigned int x=0,y=0;
  xlim=metrics[RESOLUTION_X];
  while ( x<xlim )
  {
    y=0; // EFOSON EIMASTE STO NEO X PIXEL KSEKINAME APO Y=0

    store_px=output_img+(x*3); // Y=0 OPOTE O POINTER PREPEI NA DEIXNEI POINTER + X * 3
    // ||
    // \/ DEIXNEI PANTA STIN METAVLITI POU EXEI TO first_tot_r ,first_tot_g ,first_tot_b
    start_store_px =output_img+(x*3); // Y=0 OPOTE O POINTER PREPEI NA DEIXNEI POINTER + X * 3

    /*
     APOTHIKEYOUME STA R G B k meta sta FIRST_TOT_R , FIRST_TOT_G, FIRST_TOT_B
     TO PIXEL X,0 TO OPOIO PERIEXEI TA ATHROISMATA APO X,0 EWS X+PATCH_X,0
    */
    r=start_store_px;   first_tot_r = *r;
    g=start_store_px+1; first_tot_g = *g;
    b=start_store_px+2; first_tot_b = *b;

    tot_r=0,tot_g=0,tot_b=0;
    /*
     GEMIZOUME OLO TO PATCH_Y ATHROISMA ETSI WSTE NA MPOROUME NA GLYTWSOUME TIS EXTRA PRAKSEIS :)
    */
    ylim=metrics[VERTICAL_BUFFER];
    while (y<ylim)
	{
	  ++y;
	  store_r=store_px;   tot_r+=*store_r;
	  store_g=store_px+1; tot_g+=*store_g;
	  store_b=store_px+2; tot_b+=*store_b;
	  store_px+=vertical_step;
	}
    /*
      ANTI NA ATHROIZOUME OLO TO PATCH KATHE FORA AFAIROUME TO FIRST_TOT_X k PROSTHETOUME TO LAST_TOT_X kathe fora!
    */
	ylim=metrics[RESOLUTION_Y];
	while (y<ylim)
	{
      /*
        APOTHIKEYOUME TO APOTELESMA TOU ISTOGRAMMATOS
        STA PRWTA 3 SHORTS STA OPOIA DEIXNEI TO START_STORE_PX
      */
      r=start_store_px;   *r=(unsigned short) (tot_r/total_area);
      g=start_store_px+1; *g=(unsigned short) (tot_g/total_area);
      b=start_store_px+2; *b=(unsigned short) (tot_b/total_area);
      start_store_px+=vertical_step;
      /*
        AFAIROUME TIN TIMI TWN TRIWN PROTWN METRISEWN APO TO ATHROISMA
        GIA NA PROSTHESOUME MONO TIN TELEYTAIA KAI NA GLYTWSOUME POLLES PRAKSEIS!
      */
      tot_r-=first_tot_r; tot_g-=first_tot_g; tot_b-=first_tot_b;

      /*
        H NEA PRWTI TIMI EINAI AYTI STIN OPOIA DEIXNEI TWRA POIA O DEIKTIS
      */
      r=start_store_px;   first_tot_r = *r;
      g=start_store_px+1; first_tot_g = *g;
      b=start_store_px+2; first_tot_b = *b;

      /*
        PROSTHETOUME TIN NEA TELEYTAIA TIMI STO ATHROISMA KAI H DIADIKASIA SYNEXIZETAI
      */
	  store_r=store_px;   tot_r+=*store_r;
	  store_g=store_px+1; tot_g+=*store_g;
	  store_b=store_px+2; tot_b+=*store_b;
	  store_px+=vertical_step;

	  ++y;
	}
   ++x;
  }
return;
}


void GenerateCompressHistogramOfImage(unsigned char * input_img,unsigned short * output_img,unsigned int block_x,unsigned int block_y)
{
   /*
        WE WANT EVERY PIXEL OF OUTPUT_IMG TO CONTAIN THE MEDIAN VALUE OF ALL THE R, G AND B PIXELS OF THE BLOCK
        For example to find out the median R value of the block starting at x,y and ending at x+block_x,y+block_y
        you basically read the value output_img[x][y] without any additions to speed up the Disparity Depth map
        functions :)

        THE CODE IS BROKEN IN TWO PARTS TO HELP CLARITY :)
   */
   GenerateCompressHistogramOfImage_AddUpHorizontal(input_img,output_img,block_x,block_y);
   	/*
	   AFTER THIS CODE EVERY PIXEL CONTAINS THE R , G AND B SUM OF THE NEXT BLOCK_X PIXELS
       ON THE HORIZONTAL AXIS

       NEXT THING TO DO SUM THEM UP VERTICALLY AND DIVIDE THEM BY BLOCK_X*BLOCK_Y
       AND STORE THE RESULT AS A PRECALCULATION! :)
    */
   GenerateCompressHistogramOfImage_AddUpVertical(output_img,block_x,block_y);
   /*
      WE SHOULD HAVE ACHIEVED OUR GOAL :)
   */
}

unsigned int HistogramPatch(struct Histogram *hist_data,unsigned char *img,unsigned int px,unsigned int py,unsigned int patch_x,unsigned int patch_y)
{
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



unsigned int CompressedHistogramPatch(unsigned short * compimg,struct Histogram * hist , unsigned int x,unsigned int y)
{
  unsigned short * histpx;
  unsigned int ptr_push = metrics[RESOLUTION_X_3_BYTE]*y+(x*3);
  histpx = compimg + ptr_push;
  hist->median_r = *histpx;
  histpx++;
  hist->median_g = *histpx;
  histpx++;
  hist->median_b = *histpx;
  return 1;
}

void Monochrome(unsigned char * input_frame,int image_x,int image_y)
{
  if (input_frame==0) {return;}
  int x,y,col_med;
  int x1=0,y1=0,x2=image_x,y2=image_y;

 fprintf(stderr,"Stupid code for monochrome .. can be improved needs fix..\n");


 register BYTE *px;
 register BYTE *r;
 register BYTE *g;
 register BYTE *b;
 for (x=x1; x<x2; x++)
 { for (y=y1; y<y2; y++)
	 {
	   px=0;
	   px = (BYTE *)  input_frame + precalc_memplace_3byte[x][y];
       // px = ((BYTE *)  input_frame + (image_x * y *3 ) + (3*x) );
	   r = px++;
       g = px++;
       b = px;
       //Get Pixel Color
	   col_med=  ( *r + *g + *b )/3;
	   *r= (BYTE)col_med ;
       *g=*r;
	   *b=*r;
   }
 } //MONOCHROME DONE

 return;
}

void MonochromeL(unsigned char * input_frame,int image_x,int image_y)
{
  if (input_frame==0) {return;}
  int x,y,col_med;
  int x1=0,y1=0,x2=image_x,y2=image_y;

  fprintf(stderr,"Stupid code for weighted monochrome .. can be improved needs fix..\n");


 register BYTE *px;
 register BYTE *r;
 register BYTE *g;
 register BYTE *b;
 for (x=x1; x<x2; x++)
 { for (y=y1; y<y2; y++)
	 {
	   px=0;
	    px = (BYTE *)  input_frame + precalc_memplace_3byte[x][y];
        //px = ((BYTE *)  input_frame + (image_x * y *3 ) + (3*x) );
	   r = px++;
       g = px++;
       b = px;
       //Get Pixel Color
	   col_med= ( *r * 0.299 + *g * 0.587 + *b * 0.114 );
	   *r= (BYTE)col_med ;
       *g=*r;
	   *b=*r;
   }
 } //MONOCHROME DONE

 return;
}


void KillDifferentPixels(unsigned char * image,int image_x,int image_y,unsigned char R,unsigned char G,unsigned char B,unsigned char threshold)
{ if (image==0) {return;}
  int x,y;
  int x1=0,y1=0,x2=image_x,y2=image_y;

 fprintf(stderr,"KillDifferentPixels .. RGB (%u,%u,%u) , threshold %u ..\n",R,G,B,threshold);
 fprintf(stderr,"Stupid code for killing pixels .. can be improved needs fix..\n");


 register BYTE *px;
 register BYTE *r;
 register BYTE *g;
 register BYTE *b;
 char mismatch=0;
 for (x=x1; x<x2; x++)
 { for (y=y1; y<y2; y++)
	 {
	   px=0;
	   //px = ((BYTE *)  image + (image_x * y *3 ) + (3*x) );
	    px = (BYTE *)  image + precalc_memplace_3byte[x][y];
       r = px++;
       g = px++;
       b = px;
       //Get Pixel Color

	   mismatch=0;
	   if (R > *r) { if (R-*r > threshold) { ++mismatch; }  } else
                   { if (*r-R > threshold) { ++mismatch; }  }

       if (G > *g) { if (G-*g > threshold) { ++mismatch; }  } else
                   { if (*g-G > threshold) { ++mismatch; }  }

       if (B > *b) { if (B-*b > threshold) { ++mismatch; }  } else
                   { if (*b-B > threshold) { ++mismatch; }  }

       if (mismatch>0) { *r=0;   *g=0;  *b=0; }

   }
 } //KILLING IS DONE

 return;

}


void KillPixelsBelow(unsigned char * image,int image_x,int image_y,int threshold)
{ if (image==0) {return;}
  int x,y;
  int x1=0,y1=0,x2=image_x,y2=image_y;

 fprintf(stderr,"Stupid code for killing pixels .. can be improved needs fix..\n");


 register BYTE *px;
 register BYTE *r;
 register BYTE *g;
 register BYTE *b;
 for (x=x1; x<x2; x++)
 { for (y=y1; y<y2; y++)
	 {
	   px=0;
	   //px = ((BYTE *)  image + (image_x * y *3 ) + (3*x) );
	    px = (BYTE *)  image + precalc_memplace_3byte[x][y];
       r = px++;
       g = px++;
       b = px;
       //Get Pixel Color

	   if (*r<threshold) { *r=0; }
       if (*g<threshold) { *g=0; }
       if (*b<threshold) { *b=0; }

   }
 } //MONOCHROME DONE

 return;

}


void Kill3PixelsBelow(unsigned char * image,int image_x,int image_y,int threshold)
// LIKE KILLPIXELSBELOW , BUT WHENEVER EITHER A R , G or B PIXEL IS FOUND UNDER THRESHOLD IT WILL WIPE OUT ALL OF THE COLOR CHANNELS
// IN THE PIXEL!
{ if (image==0) {return;}

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


void ReducePalette(unsigned char * image,int image_x,int image_y,int new_palette)
{ if (image==0) {return;}
  int x,y;
  int x1=0,y1=0,x2=image_x,y2=image_y;

 fprintf(stderr,"Stupid code for reducing palette.. can be improved needs fix..\n");
 register BYTE *px;
 register BYTE *r;
 register BYTE *g;
 register BYTE *b;
 for (x=x1; x<x2; x++)
 { for (y=y1; y<y2; y++)
	 {
	   px=0;
	   px = (BYTE *)  image + precalc_memplace_3byte[x][y];
       //px = ((BYTE *)  image + (image_x * y *3 ) + (3*x) );
	   r = px++;
       g = px++;
       b = px;
       //Get Pixel Color

	   *r=  (BYTE) ( (*r) * new_palette ) / 255;
       *g=  (BYTE) ( (*g) * new_palette ) / 255;
       *b=  (BYTE) ( (*b) * new_palette ) / 255;

   }
 } //REPALETTING IS DONE

 return;

}



BOOLEAN  Sobel(unsigned char * image,int image_x,int image_y)
{

  unsigned int x=0,y=0;
  unsigned int x1=1,y1=1,x2=image_x,y2=image_y;


  if (image==0) { return(0); }

  unsigned char *proc_image;
  //proc_image = new unsigned char [ image_x * image_y * 3 ];
  proc_image = ( unsigned char * ) malloc ( sizeof(unsigned char) * image_x * image_y * 3 );

  BYTE *px;

  BYTE *r;
  BYTE *g;
  BYTE *b;

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



		px = (BYTE *)  proc_image + precalc_memplace_3byte[x][y];
        //px = ((BYTE *)  proc_image + (effW * y ) + (3*x) );
        r= px++;
        g = px++;
        b = px;

	   *r=  (BYTE) sum ;
       *g=*r;
	   *b=*r;
   }
 } //SOBEL FILTER DONE


memcpy(image,proc_image,image_x*image_y*3*sizeof(BYTE));


free(proc_image);

return (1);
}


BOOLEAN SobelFromSource(unsigned char * source,unsigned char * target,int image_x,int image_y)
{

  unsigned int x=0,y=0;
  unsigned int x1=1,y1=1,x2=image_x,y2=image_y;


  if ( (target==0) || (source==0) ) {return(0);}

  //unsigned char *proc_image;
  //proc_image = new unsigned char [ image_x * image_y * 3 ];

  BYTE *px;

  BYTE *r;
  BYTE *g;
  BYTE *b;

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



		px = (BYTE *)  target + precalc_memplace_3byte[x][y];
        //px = ((BYTE *)  proc_image + (effW * y ) + (3*x) );
        r= px++;
        g = px++;
        b = px;

	   *r=  (BYTE) sum ;
       *g=*r;
	   *b=*r;
   }
 } //SOBEL FILTER DONE


//memcpy(image,proc_image,image_x*image_y*3*sizeof(BYTE));
//delete[] proc_image;

return (1);
}







BOOLEAN GaussianBlurFromSource(unsigned char * source,unsigned char * target,int image_x,int image_y,BOOLEAN monochrome)
{
  //return 1;
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

BOOLEAN GaussianBlur(unsigned char * image,int image_x,int image_y,BOOLEAN monochrome)
{
  return GaussianBlurFromSource(image,image,image_x,image_y,monochrome);
}

unsigned int FloodPixel(unsigned char * picture_array,unsigned char * result_array,unsigned int point_x,unsigned int point_y,unsigned int size_x,unsigned int size_y,unsigned char size_rgb)
{
	fprintf(stderr,"FloodPixel not implemented!\n");
	return 0;
	register unsigned int ptr_x=0,y,x;

	x=point_x;
	for ( y=point_y-1; point_y>=0; y--)
	   { ptr_x=( (y) * size_x * size_rgb )+( x * size_rgb );

	   }
  return 0;
}



BOOLEAN FindGoodTrackingPoints(unsigned char * edge_source,unsigned char * target,int image_x,int image_y,BOOLEAN monochrome)
{
  if ( (edge_source==0) || ( target == 0 ) ) { return 0; }
  fprintf(stderr,"FindGoodTrackingPoints , beta function hope it doesn`t crash..\n");

 //Source
 register BYTE * source_p;
 register BYTE * source_end;
 register BYTE *source_r; register BYTE *source_g; register BYTE *source_b;

 //Target
 register BYTE * target_p;
 register BYTE * target_end;
 register BYTE *target_r; register BYTE *target_g; register BYTE *target_b;

 source_p =  (BYTE *)  edge_source;
 source_end = ( (BYTE *)    edge_source + (image_x * (image_y-1) *3 ) + (3*(image_x-1)) );

 target_p =  (BYTE *)  target;
 target_end = ( (BYTE *)    target + (image_x * (image_y-1) *3 ) + (3*(image_x-1)) );


 memset(target,0,image_x*image_y*3); // CLEAR TARGET

 unsigned int line_width = image_x*3;
 unsigned int skip_step_abs = 20;
 unsigned int skip_step = 3 * skip_step_abs;
 unsigned int x=0,y=1;

 while (source_p<source_end)
 {
	  source_r = source_p++; source_g = source_p++; source_b = source_p++;
      target_r = target_p++; target_g = target_p++; target_b = target_p++;
      ++x;

	  if ( (*source_r>20)||(*source_g>20)||(*source_b>20) )
	   {
	     *target_r=255; *target_g=255; *target_b=255;

         source_p+=skip_step;
         target_p+=skip_step;
         x=x+skip_step_abs;

         if ( x >= image_x )
           { x=x-image_x;
             ++y;

           ++y;   source_p+=line_width;   target_p+=line_width;
           ++y;   source_p+=line_width;   target_p+=line_width;
           }

         AddPointToTrackList(0,x,y,0);
	   }

     if ( x >= image_x )
           { x=x-image_x;
             ++y;

             ++y;   source_p+=line_width;   target_p+=line_width;
             ++y;   source_p+=line_width;   target_p+=line_width;
           }
 }

  fprintf(stderr,"FindGoodTrackingPoints , survived , total of %u points ..\n",GetPointTrackList());
  return 1;
}


void PrepareCleanSobeledGaussian(unsigned char * rgb_image,unsigned char * target_image,unsigned int kill_lower_edges_threshold)
{
    GaussianBlurFromSource(rgb_image,target_image,metrics[RESOLUTION_X],metrics[RESOLUTION_Y],1);
	Sobel(target_image,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
	Kill3PixelsBelow(target_image,metrics[RESOLUTION_X],metrics[RESOLUTION_Y],kill_lower_edges_threshold);
}


inline char PixelsSameColour(unsigned char * rgb_image,unsigned int memplace1,unsigned int memplace2)
{
  char res=0;
  if ( precalc_sub [ rgb_image[memplace1] ] [ rgb_image[memplace2] ] < 20 ) ++res;
  if ( precalc_sub [ rgb_image[memplace1+1] ] [ rgb_image[memplace2+1] ] < 20 ) ++res;
  if ( precalc_sub [ rgb_image[memplace1+2] ] [ rgb_image[memplace2+2] ] < 20 ) ++res;

  if ( res == 3 ) res=1; else
                  res=0;

  return res;
}

BOOLEAN PixelIsBright(unsigned char * rgb_image,unsigned int memplace_3_byte)
{
  char res=0;
  if ( precalc_sub [ rgb_image[memplace_3_byte] ] [ rgb_image[255] ] < 150 ) ++res;
  if ( precalc_sub [ rgb_image[memplace_3_byte] ] [ rgb_image[255] ] < 150 ) ++res;
  if ( precalc_sub [ rgb_image[memplace_3_byte] ] [ rgb_image[255] ] < 150 ) ++res;

  if ( res == 3 ) res=1; else
                  res=0;

  return res;
}


BOOLEAN PixelBelongsWithSurroundingPixels(unsigned char * rgb_image,unsigned int x,unsigned int y,unsigned int memplace)
{
    char resemblence=0;
    unsigned int memplace2;
    if ( (x>1)&&(y>1)&&(y<metrics[RESOLUTION_Y])&&(x<metrics[RESOLUTION_X]) )
     {
       unsigned int SHIFT_3_BYTE = metrics[RESOLUTION_X_3_BYTE];

       memplace2=memplace-1; // LEFT
       if ( PixelsSameColour(rgb_image,memplace,memplace2)==1 ) ++resemblence;

       memplace2-=SHIFT_3_BYTE; // LEFT UP
       if ( PixelsSameColour(rgb_image,memplace,memplace2)==1 ) ++resemblence;

       ++memplace2; // UP
       if ( PixelsSameColour(rgb_image,memplace,memplace2)==1 ) ++resemblence;

       ++memplace2; // RIGHT UP
       if ( PixelsSameColour(rgb_image,memplace,memplace2)==1 ) ++resemblence;

       memplace2+=SHIFT_3_BYTE; // RIGHT
       if ( PixelsSameColour(rgb_image,memplace,memplace2)==1 ) ++resemblence;

       memplace2+=SHIFT_3_BYTE;// RIGHT DOWN
       if ( PixelsSameColour(rgb_image,memplace,memplace2)==1 ) ++resemblence;

       --memplace2; // DOWN
       if ( PixelsSameColour(rgb_image,memplace,memplace2)==1 ) ++resemblence;

       --memplace2; // LEFT DOWN
       if ( PixelsSameColour(rgb_image,memplace,memplace2)==1 ) ++resemblence;


     }

    if ( resemblence>7) return 1;

    return 0;
}

