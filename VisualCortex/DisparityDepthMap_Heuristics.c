#include "DisparityDepthMap_Heuristics.h"
#include "VisCortexFilters.h"
#include "Precalculations.h"
#include "../WorldMapping/MasterWorld/MasterWorld.h"



void SaveRegistersStateRequiredForDisparityMapping()
{
   SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_LEFT.ppm",CALIBRATED_LEFT_EYE,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
   SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_RIGHT.ppm",CALIBRATED_RIGHT_EYE,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
   SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_EDGES_LEFT.ppm",EDGES_LEFT,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
   SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_EDGES_RIGHT.ppm",EDGES_RIGHT,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
   SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_SECOND_DERIVATIVE_LEFT.ppm",SECOND_DERIVATIVE_LEFT,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
   SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_SECOND_DERIVATIVE_RIGHT.ppm",SECOND_DERIVATIVE_RIGHT,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
   SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_MOVEMENT_LEFT.ppm",MOVEMENT_LEFT,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
   SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_MOVEMENT_RIGHT.ppm",MOVEMENT_RIGHT,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
}








unsigned char InstantMovementDisparity(unsigned short *left_depth,unsigned short *right_depth)
{
 return 1;
}



unsigned char DisparityMapNormalize(unsigned short *left_depth,unsigned short *right_depth)
{
	// ENA GRIGORO CHECK POU AN ENA SIMEIO EXEI POLY DISPARITY SE SXESI ME TA GEITONIKA
	// TO OMOGENOPOIEI
/*
  unsigned int memplace,y,xa=0,ya=0,xb=0,yb=0,memsize=0; //a simainei aristeri cam , b deksia
  unsigned int x_start_left=15,avg_x;
  xa=x_start_left;
  memsize=max_res_x*max_res_y;
  while (xa<max_res_x)
	   {
		 memplace=xa;
		 ya=0;
		 avg_x=0;
         while (memplace<memsize)
		 {
            memplace+=max_res_x;
			++ya;
		 }
		++xa;
	   }
  */
  return 1;
}





inline unsigned short FindNeighborDepth(
                                               unsigned short * full_depth_map,
                                               unsigned char * rgb_image,
                                               unsigned int search_area_stop,
                                               unsigned char r,unsigned char g,unsigned char b,
                                               unsigned int dpth_ptr,unsigned int img_ptr,
                                               unsigned int x,unsigned int y,unsigned int depth
                                       )
{
  // Heuristic , finds neighborhood depth..

  unsigned int SHIFT_3_BYTE = metrics[RESOLUTION_X_3_BYTE];
  unsigned int SHIFT_1_BYTE = metrics[RESOLUTION_X];

  unsigned int stop_search_up=0,stop_search_down=0;
  unsigned int dpth_ptr_up=dpth_ptr , dpth_ptr_down=dpth_ptr , y_up=y , y_down=y , img_ptr_up=img_ptr , img_ptr_down=img_ptr;
  unsigned int search_area=0;
  unsigned int THRESHOLD=20;
  while (search_area<search_area_stop)
  {
    if ( stop_search_up==0 )
    {
     if (y_up<=1) stop_search_up=1; else
     {
      dpth_ptr_up-=SHIFT_1_BYTE;
      img_ptr_up-=SHIFT_3_BYTE;
      --y_up;

      if (
           (AbsUCharVDiff(r,rgb_image[img_ptr_up])>THRESHOLD) ||
           (AbsUCharVDiff(g,rgb_image[img_ptr_up+1])>THRESHOLD) ||
           (AbsUCharVDiff(b,rgb_image[img_ptr_up+2])>THRESHOLD)
         ) stop_search_up=1; else
      {
        if (full_depth_map[dpth_ptr_up]!=0) return full_depth_map[dpth_ptr_up];
      }
     }
    }

    if ( stop_search_down==0 )
    {
     if (y_down>=metrics[RESOLUTION_Y]) stop_search_down=1; else
     {
      dpth_ptr_down+=SHIFT_1_BYTE;
      img_ptr_down+=SHIFT_3_BYTE;
      ++y_down;
      if ( (AbsUCharVDiff(r,rgb_image[img_ptr_down])>THRESHOLD) ||
           (AbsUCharVDiff(g,rgb_image[img_ptr_down+1])>THRESHOLD) ||
           (AbsUCharVDiff(b,rgb_image[img_ptr_down+2])>THRESHOLD) ) stop_search_down=1; else
      {
        if (full_depth_map[dpth_ptr_down]!=0) return full_depth_map[dpth_ptr_down];
      }
     }
    }

    if ((stop_search_down!=0)&&(stop_search_up!=0)) return 0;

    ++search_area;
  }
  return 0;
}


void EnhanceDepthMapWithEdges(unsigned char * rgb_image,unsigned short *full_depth_map,unsigned char * edges_map,unsigned int image_x,unsigned int image_y)
{
  // Heuristic , fills depth map holes with neighborhood edge depth..

  fprintf(stderr,"Todo : FindNeighborDepth is not complete , fix it ! \n");
  memcpy(l_video_register[GENERAL_LARGE_1].pixels,full_depth_map,metrics[RESOLUTION_MEMORY_LIMIT_3BYTE]);
  unsigned int SHIFT_1_BYTE = metrics[RESOLUTION_X];

  unsigned int SHIFT_1_BYTE2 = SHIFT_1_BYTE*2;
  unsigned int SHIFT_1_BYTE3 = SHIFT_1_BYTE*3;
  unsigned int SHIFT_1_BYTE4 = SHIFT_1_BYTE*4;
  unsigned int SHIFT_1_BYTE5 = SHIFT_1_BYTE*5;
  //unsigned int SHIFT_1_BYTE6 = SHIFT_1_BYTE*6;

  unsigned int ptr=0,ptr3bit=0;//,dpth_lim=image_x*image_y;
  unsigned int xp=0,yp=0;
  unsigned int left_scan=0,right_scan=0,up_scan=0,down_scan=0;
  char  request_matchup=0;

 for (yp=0; yp<image_y; yp++)
  {
    for (xp=0; xp<image_x; xp++)
    {
     if (edges_map[ptr3bit]==0)
     {
       left_scan=0; right_scan=0; up_scan=0; down_scan=0;

      // KANOUME ENA SCAN PANW KATW ARISTERA K DEKSIA GIA NA VROUME MIA KONTINI EDGE
      // VRISKONTAS MIA KONTINI EDGE ELPIZOUME OTI THA EXEI SWSTO DEPTH VALUE

       if (xp <image_x-10 )
      {
       if  ( full_depth_map[ptr+1]>0 )  { ++right_scan; }
       if  ( full_depth_map[ptr+2]>0 )  { ++right_scan; }
       if  ( full_depth_map[ptr+3]>0 )  { ++right_scan; }
       if  ( full_depth_map[ptr+4]>0 )  { ++right_scan; }
       if  ( full_depth_map[ptr+5]>0 )  { ++right_scan; }
      }

      if (xp >10 )
      {
       if  ( full_depth_map[ptr-1]>0 )  { ++left_scan; }
       if  ( full_depth_map[ptr-2]>0 )  { ++left_scan; }
       if  ( full_depth_map[ptr-3]>0 )  { ++left_scan; }
       if  ( full_depth_map[ptr-4]>0 )  { ++left_scan; }
       if  ( full_depth_map[ptr-5]>0 )  { ++left_scan; }
      }

     if (yp <image_y-10 )
      {
       if  ( full_depth_map[ptr+SHIFT_1_BYTE]>0 )  { ++down_scan; }
       if  ( full_depth_map[ptr+SHIFT_1_BYTE2]>0 )  { ++down_scan; }
       if  ( full_depth_map[ptr+SHIFT_1_BYTE3]>0 )  { ++down_scan; }
       if  ( full_depth_map[ptr+SHIFT_1_BYTE4]>0 )  { ++down_scan; }
       if  ( full_depth_map[ptr+SHIFT_1_BYTE5]>0 )  { ++down_scan; }
      }

      if (yp >10 )
      {
       if  ( full_depth_map[ptr-SHIFT_1_BYTE]>0 )  { ++up_scan; }
       if  ( full_depth_map[ptr-SHIFT_1_BYTE2]>0 )  { ++up_scan; }
       if  ( full_depth_map[ptr-SHIFT_1_BYTE3]>0 )  { ++up_scan; }
       if  ( full_depth_map[ptr-SHIFT_1_BYTE4]>0 )  { ++up_scan; }
       if  ( full_depth_map[ptr-SHIFT_1_BYTE5]>0 )  { ++up_scan; }
      }
         // EXOUME PROSMETRISEI POSES EDGES YPARXOUN DEKSIA ARISTER PANW K KATW
         // TWRA AN EIMASTE ANAMESA SE EDGES , PAROTI THA VRISKOMASTE SE KENO SPOT
         // WSTOSO EXOUME KALI PITHANOTITA NA PAROUME KAPOIA GEITONIKI TIMI KAI NA OMOGENOPOIITHEI TO MAP

         request_matchup=0;
         // AN VRISKOMASTE ANAMESA SE EDGES TOTE SE PERIPTWSI POU EXOUME KAPOIO DEPTH VALUE TO DIATIROUME
         // ALLIWS AN DEN EXOUME PSAXNOUME GYRW GYRW NA VROUME ENA
         if ( (up_scan>=1)&&(down_scan>=5) )  { if (full_depth_map[ptr]==0) { request_matchup=1; } } else
         if ( (up_scan>=5)&&(down_scan>=1) )  { if (full_depth_map[ptr]==0) { request_matchup=1; } } else
         if ( (left_scan>=1)&&(right_scan>=5) )  { if (full_depth_map[ptr]==0) { request_matchup=1; } } else
         if ( (left_scan>=5)&&(right_scan>=1) )  { if (full_depth_map[ptr]==0) { request_matchup=1; } } else
         {
           // AMA DEN EIMASTE ANAMESA SE KAPOIA EDGES APLA SVINETAI TO SIMEIO
           // YPARXEI MEGALI PITHANOTITA NA MIN EINAI SWSTO!
           //if ( PixelIsBright(rgb_image,ptr3bit) == 0 )
           if ( PixelBelongsWithSurroundingPixels(rgb_image,xp,yp,ptr3bit) == 0 )
           { //UNRELATED PIXEL , REMOVE IT !
             l_video_register[GENERAL_LARGE_1].pixels[ptr]=0;
           }
         }

         if (request_matchup!=0 )
          {

          }
       } else
       {  // GRAPHICAL DEBUGING
         // l_video_register[GENERAL_LARGE_1].pixels[ptr]= edges_map[ptr3bit];
       }

       ++ptr;
      ptr3bit+=3;

    }
  }

  memcpy(full_depth_map,l_video_register[GENERAL_LARGE_1].pixels,metrics[RESOLUTION_MEMORY_LIMIT_3BYTE]);
  return;
}


unsigned int EnhanceDepthMapFillHoles(unsigned char * rgb_image,unsigned short *full_depth_map,unsigned int image_x,unsigned int image_y)
{
  // Heuristic , fills depth map holes with neighborhood depth..

  fprintf(stderr,"Todo : EnhanceDepthMapFillHoles is not complete , fix it ! \n");
  memcpy(l_video_register[GENERAL_LARGE_1].pixels,full_depth_map,metrics[RESOLUTION_MEMORY_LIMIT_3BYTE]);
 // unsigned int SHIFT_1_BYTE = metrics[RESOLUTION_X];

  unsigned int ptr=0,ptr3bit=0;
  unsigned int xp=0,yp=0,fill_counter=0;
  char r=0 , g=0 , b=0 ;
 for (yp=0; yp<image_y; yp++)
  {
    for (xp=0; xp<image_x; xp++)
    {

      if (full_depth_map[ptr]==0)
      {
          r=rgb_image[ptr3bit]; g=rgb_image[ptr3bit+1]; b=rgb_image[ptr3bit+2];
          l_video_register[GENERAL_LARGE_1].pixels[ptr] = FindNeighborDepth(full_depth_map,rgb_image,25,r,g,b,ptr,ptr3bit,xp,yp,0);
          ++fill_counter;
      }

      ++ptr;
      ptr3bit+=3;
    }
  }
  memcpy(full_depth_map,l_video_register[GENERAL_LARGE_1].pixels,metrics[RESOLUTION_MEMORY_LIMIT_3BYTE]);
  fprintf(stderr,"%u pixels filled \n",fill_counter);
  return fill_counter;
}










int DepthMapToVideoSimple(unsigned int depth_reg,unsigned int vid_reg,unsigned int depth_scale)
{ // TODO depth_scale added as debugging visual aid
  // Convert from the Large ( unsigned short ) to 3 bytes per pixel storage ( r,g,b ) needed for outputting video
  unsigned short *full_depth_map=l_video_register[depth_reg].pixels;
  unsigned char *vid_depth_map=video_register[vid_reg].pixels;
  unsigned int image_x=video_register[vid_reg].size_x;
  unsigned int image_y=video_register[vid_reg].size_y;


  register BYTE *px;
  register BYTE *r;
  register BYTE *g;
  register BYTE *b;
  unsigned char val;
  px = (BYTE *)  vid_depth_map;


  unsigned int ptr,dpth_lim=image_x*image_y;
  for ( ptr = 0; ptr < dpth_lim; ptr++)
   {
       r = px++; g = px++; b = px++;

       //Just a convertion of the numbers to RGB
       if ( full_depth_map[ptr] >= 255 ) { val = 255; } else
                                         { val = ( unsigned char ) (full_depth_map[ptr]); }

       *r= val; *g=val; *b= val;
   }

   video_register[vid_reg].time = video_register[depth_reg].time;
   return 1;
}


float ConvertDepthUnitToCM(unsigned int depth_unit)
{
   unsigned int total_points = 11;
   unsigned int table[total_points][2];
   //Static table for 2 PS3EYES , parallel configuration 6.5cm away
   // Depth Units    Centimeters
   table[0][0]=56;  table[0][1]=25;
   table[1][0]=50;  table[1][1]=30;
   table[2][0]=39;  table[2][1]=35;
   table[3][0]=35;  table[3][1]=40;
   table[4][0]=30;  table[4][1]=45;
   table[5][0]=27;  table[5][1]=50;
   table[6][0]=21;  table[6][1]=55;
   table[7][0]=20;  table[7][1]=60;
   table[8][0]=16;  table[8][1]=65;
   table[9][0]=15;  table[9][1]=70;
   table[10][0]=14; table[10][1]=75;

   if ( table[0][0]<=depth_unit )
     {
         //THIS POINT IS VERY CLOSE
         return 0.0;
     }

   unsigned int i=0;
    while (i<total_points)
     {
         if ( table[i][0]<depth_unit )
           {
               if (i>0)
                {
                   //y = mx+b
                   //m = y2-y1 / x2-x1

                   float m = (float) table[i-1][0]-table[i][0]/table[i-1][1]-table[i][1];
                   float b = (float) table[i][0] - m * table[i][1];
                   if (m==0.0) { fprintf(stderr,"Division by zero avoided\n"); return 0.0; }
                   float real_value = (float) (depth_unit - b) / m;
                   return real_value;
                }
           }
         ++i;
     }


   return 0.0;
}

int PassDepthMapToMasterWorld(unsigned int vid_reg)
{
 register BYTE *px , *r , *g , *b;
 int i=0;

 fprintf(stderr,"Clearing 3D PointCloud For Now\n");
 Clear3DPointCloud();
 px = video_register[vid_reg].pixels;

 for (i=0; i<metrics[RESOLUTION_MEMORY_LIMIT_1BYTE]; i++)
      {
        r = px++; g = px++; b = px++;
        if ( depth_data_array[i].Z != 0.0 )
         {
           //fprintf(fd,"%f\n%f\n%f\n",depth_data_array[i].X,depth_data_array[i].Y,depth_data_array[i].Z); fprintf(fd,"%u\n%u\n%u\n",*r,*g,*b);
           //depth_data_array[i].Z
           Add3DPoint(depth_data_array[i].X,depth_data_array[i].Y,ConvertDepthUnitToCM(depth_data_array[i].depth_raw),*r,*g,*b);
         }
      }
  return 1;
}



int DepthMapToVideo(unsigned int depth_reg,unsigned int vid_reg,unsigned int depth_scale)
{
  return DepthMapToVideoSimple(depth_reg,vid_reg,depth_scale);

  //THIS FUNCTION IS KIND OF MEANINGLESS , SINCE THE VIDEO OUTPUT IS ONLY INTENDED FOR DEBUGGING PURPOSES
  //AND REPERFORMING THE CALCULATIONS HERE IS REDUNDANT..

  // TODO depth_scale added as debugging visual aid
  // Convert from the Large ( unsigned short ) to 3 bytes per pixel storage ( r,g,b ) needed for outputting video
  unsigned short *full_depth_map=l_video_register[depth_reg].pixels;
  unsigned char *vid_depth_map=video_register[vid_reg].pixels;
  unsigned int image_x=video_register[vid_reg].size_x;
  unsigned int image_y=video_register[vid_reg].size_y;


  register BYTE *px;
  register BYTE *r;
  register BYTE *g;
  register BYTE *b;
  unsigned char val;
  px = (BYTE *)  vid_depth_map;


/* Explained nicely here : http://www.societyofrobots.com/programming_computer_vision_tutorial_pt3.shtml#stereo_vision :P
   Z_actual = (b * focal_length) / (x_camL - x_camR)
   X_actual = x_camL * Z_actual / focal_length
   Y_actual = y_camL * Z_actual / focal_length
*/


  unsigned int max_possible_depth = settings[DEPTHMAP_CLOSEST_DEPTH];
  float Baseline_MULT_FocalLength = 0.0;
  float closest_number , furthest_number ;
  if ( (CameraDistanceInMM != 0 ) && (left_calibration_data.fx != 0) )
    {                                   // MM to CM
       Baseline_MULT_FocalLength = (float) CameraDistanceInMM/10;
       Baseline_MULT_FocalLength *= left_calibration_data.fx;

       if (max_possible_depth!=0)
         {
           max_possible_depth = (unsigned int ) Baseline_MULT_FocalLength / max_possible_depth;
         }
    }

  if (Baseline_MULT_FocalLength==0.0)
      {  /*Fall back to classic Video Conversion */
         return DepthMapToVideoSimple(depth_reg,vid_reg,depth_scale);
      }

  closest_number = Baseline_MULT_FocalLength / 255;
  furthest_number = Baseline_MULT_FocalLength;

  unsigned int tmp_val = 0;
  unsigned int ptr,dpth_lim=image_x*image_y;
  for ( ptr = 0; ptr < dpth_lim; ptr++)
   {
       r = px++; g = px++; b = px++;

     // Real calculation based on stereo rig
     if (Baseline_MULT_FocalLength!=0.0)
      {
       if ( full_depth_map[ptr] != 0 )
          {
            // As the depth map gives a larger number ( i.e. more disparity ) the division of Baseline * Focal Length / Disparity returns a smaller result
            // we want the points close to the camera to be lit brightly and the points far from the camera to be dimmed down , so
            // we subtract 255 from their value to invert their colour ..!

            tmp_val = (unsigned int ) Baseline_MULT_FocalLength / full_depth_map[ptr];
            if ( tmp_val > 255 ) { tmp_val = 255; }

            tmp_val = 255-tmp_val;  // Inversion of colouring ..!

          } else
          {
            tmp_val = 0;
          }
       val = (unsigned char) tmp_val;
      } else
      { // NON LINEAR DEPTH SCALE
        if (full_depth_map[ptr]>255) { val = 255; } else
                                     { val = ( unsigned char ) full_depth_map[ptr]; }
      }

       *r= val; *g=val; *b= val;
   }

   video_register[vid_reg].time = video_register[depth_reg].time;

   return 1;
}

unsigned int DisparityMapGetPercentCovered(unsigned int depth_reg)
{
  unsigned int pixels_too_close = 0;
  unsigned int pixels_covered = 0;
  unsigned int ptr,dpth_lim=metrics[RESOLUTION_MEMORY_LIMIT_1BYTE];
    unsigned short *full_depth_map=l_video_register[depth_reg].pixels;

  for ( ptr = 0; ptr < dpth_lim; ptr++)
   {
      if ( full_depth_map[ptr] != 0 ) { ++pixels_covered; }
      if ( full_depth_map[ptr] > DEPTHMAP_CLOSEST_DEPTH-8 ) { ++pixels_too_close; }
   }

   unsigned int percent = (unsigned int) ( ( pixels_too_close *100 ) / ( metrics[RESOLUTION_X]*metrics[RESOLUTION_Y] ) );
   metrics[LAST_DEPTH_MAP_TOO_CLOSE_COVERAGE]=percent;

   percent = (unsigned int) ( ( pixels_covered *100 ) / ( metrics[RESOLUTION_X]*metrics[RESOLUTION_Y] ) );
   metrics[LAST_DEPTH_MAP_COVERAGE]=percent;

  return percent;
}
