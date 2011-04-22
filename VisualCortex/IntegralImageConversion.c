#include "IntegralImageConversion.h"
#include "Precalculations.h"

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

inline int MakePatchFitInsideImage(int *x,int *y ,int *width,int *height)
{
    if ( *x >= metrics[RESOLUTION_X] ) { return 0; }
    if ( *y >= metrics[RESOLUTION_Y] ) { return 0; }
    if ( *x + *width >= metrics[RESOLUTION_X] ) { *width  = metrics[RESOLUTION_X] - *x; }
    if ( *y + *height >= metrics[RESOLUTION_Y]) { *height = metrics[RESOLUTION_Y] - *y; }
    return 1;
}


int GetCompressedRegisterPatchSum1Byte(int comp_register,int x,int y,int width,int height)
{
    if (!MakePatchFitInsideImage(&x,&y,&width,&height)) { return 0; }
    unsigned int ptr1 = metrics[RESOLUTION_X]*y+(x*3);
    unsigned int ptr2 = metrics[RESOLUTION_X]*(y+height)+((x+width)*3);
    int total=l_video_register[comp_register].pixels[ptr1] - l_video_register[comp_register].pixels[ptr2];
    return total;
}




int CompressRegister1Byte(int input,int output)
{
  //This code will add up all the pixels to every other pixel , in order to speed up access
  //Patch procedures when each pixel must be added to the others..!

  if ( video_register[input].depth != 1 ) { fprintf(stderr,"CompressRegister1Bit called with 3bit image\n"); return 0; }

  ClearLargeVideoRegister(output);
  unsigned char *in_ptr_start=video_register[input].pixels,*in_ptr=in_ptr_start;
  unsigned short *out_ptr_start=l_video_register[input].pixels,*out_ptr=out_ptr_start;

  unsigned int x=0,y=0;
  while (y<metrics[RESOLUTION_Y])
	{
	    x = metrics[RESOLUTION_X];
	    in_ptr += metrics[RESOLUTION_X];
	    out_ptr += metrics[RESOLUTION_X];

	    while (x>0)
	     {
           --in_ptr;
           --out_ptr;
           --x;

           *out_ptr += *in_ptr;
           *(out_ptr-1) = *out_ptr;
	     }

	    in_ptr += metrics[RESOLUTION_X];
	    out_ptr += metrics[RESOLUTION_X];
	    ++y;
	}

  out_ptr=out_ptr_start+metrics[RESOLUTION_MEMORY_LIMIT_1BYTE];

  x=0; y=0;
  unsigned int last_val=0;
  while (x<metrics[RESOLUTION_X])
	{
	    y = metrics[RESOLUTION_Y];
	    out_ptr -= metrics[RESOLUTION_X];
        last_val = *out_ptr;

	    while (y>0)
	     {
            out_ptr -= metrics[RESOLUTION_X];
            --y;
            *out_ptr += last_val;
            last_val=*out_ptr;
	     }

	    out_ptr += 1; //depth
	    ++x;
	}


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
