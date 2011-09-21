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
#include "VisionMemory.h"
#include "Precalculations.h"
#include "VisCortexFilters.h"
#include "StateSetting.h"

#define PPMREADBUFLEN 256
unsigned int TIME_INC=0;
unsigned int COLD_START=1;

unsigned int state[STATE_COUNT]={0};
unsigned int settings[SETTINGS_COUNT]={0};
unsigned int metrics[METRICS_COUNT]={0};
struct VideoRegister video_register[REGISTERS_COUNT]={{0},{0},{0},{0},{0},{0}};
struct LargeVideoRegister l_video_register[LARGE_REGISTERS_COUNT]={{0},{0},{0},{0},{0},{0}};
struct ExtraLargeVideoRegister xl_video_register[EXTRA_LARGE_REGISTERS_COUNT]={{0},{0},{0},{0}};
struct DepthData * depth_data_array=0;

float camera_distance=0;
float camera_diagonal_field_of_view=0,camera_horizontal_field_of_view=0,camera_vertical_field_of_view=0;

float depth_units_in_cm[256]={0};

int SetImageRegion( struct ImageRegion * ir , unsigned int x1,unsigned int y1,unsigned int width,unsigned int height)
{
   if ( ir == 0 ) { return 0; }
   ir->x1=x1;
   ir->y1=y1;
   ir->width = width;
   ir->height = height;
   return 1;
}


int VideoRegisterRequestIsOk(unsigned int reg_num, unsigned int res_x,unsigned int res_y,unsigned int depth)
{
    if (reg_num>=REGISTERS_COUNT) { fprintf(stderr,"Register does not exist! \n "); return 0; }
    if (video_register[reg_num].pixels == 0 ) { fprintf(stderr,"Register is dead! \n "); return 0; }
    return 1;
}

int LargeVideoRegisterRequestIsOk(unsigned int reg_num, unsigned int res_x,unsigned int res_y,unsigned int depth)
{
    if (reg_num>=LARGE_REGISTERS_COUNT) { fprintf(stderr,"Large Register does not exist! \n "); return 0; }
    if (l_video_register[reg_num].pixels == 0 ) { fprintf(stderr,"Large Register is dead! \n "); return 0; }
    return 1;
}

int ExtraLargeVideoRegisterRequestIsOk(unsigned int reg_num, unsigned int res_x,unsigned int res_y,unsigned int depth)
{
    if (reg_num>=EXTRA_LARGE_REGISTERS_COUNT) { fprintf(stderr,"ExtraLarge Register does not exist! \n "); return 0; }
    if (xl_video_register[reg_num].pixels == 0 ) { fprintf(stderr,"ExtraLarge Register is dead! \n "); return 0; }
    return 1;
}

int InitRegister( unsigned int reg_num, unsigned int res_x,unsigned int res_y,unsigned int depth )
{
  if (video_register[reg_num].pixels!=0) { fprintf(stderr,"While Allocating Register : Video register %u is not empty !\n",reg_num);
                                           free( (void *) video_register[reg_num].pixels);
                                           video_register[reg_num].pixels=0; }

  video_register[reg_num].pixels = (unsigned char * ) malloc( sizeof(unsigned char) * res_x * res_y * depth);
  if (video_register[reg_num].pixels==0) { fprintf(stderr,"Error allocating memory for video register\n"); return 0; }
  video_register[reg_num].size_x = res_x;
  video_register[reg_num].size_y = res_y;
  video_register[reg_num].depth = depth;
  video_register[reg_num].time = 0;
  video_register[reg_num].used = 0;
  video_register[reg_num].features =  CreateFeatureList(settings[MAX_FEATURES],settings[PATCH_TRACKING_WIDTH],settings[PATCH_TRACKING_HEIGHT]);
  video_register[reg_num].faces = CreateFeatureList(settings[MAX_FACES],settings[PATCH_TRACKING_WIDTH],settings[PATCH_TRACKING_HEIGHT]);

  if ( video_register[reg_num].pixels == 0 ) { fprintf(stderr,"While Allocating Register : Unable to allocate video register %u memory !\n",reg_num);
                                               return 1; }

  return 0;
}


int InitLargeRegister( unsigned int reg_num, unsigned int res_x,unsigned int res_y,unsigned int depth )
{
  if (l_video_register[reg_num].pixels!=0) { fprintf(stderr,"While Allocating Large Register : Video register %u is not empty !\n",reg_num);
                                             free( (void *) l_video_register[reg_num].pixels);
                                             l_video_register[reg_num].pixels=0; }

  l_video_register[reg_num].pixels = (unsigned short * ) malloc( res_x * res_y * depth * sizeof(unsigned short) );
  if (l_video_register[reg_num].pixels ==0) { fprintf(stderr,"Error allocating memory for large video register\n"); return 0; }
  l_video_register[reg_num].size_x = res_x;
  l_video_register[reg_num].size_y = res_y;
  l_video_register[reg_num].depth = depth;
  l_video_register[reg_num].time = 0;
  l_video_register[reg_num].used = 0;
  l_video_register[reg_num].features =  CreateFeatureList(settings[MAX_FEATURES],settings[PATCH_TRACKING_WIDTH],settings[PATCH_TRACKING_HEIGHT]);
  l_video_register[reg_num].faces = CreateFeatureList(settings[MAX_FACES],settings[PATCH_TRACKING_WIDTH],settings[PATCH_TRACKING_HEIGHT]);

  if ( l_video_register[reg_num].pixels == 0 ) { fprintf(stderr,"While Allocating Large Register : Unable to allocate video register %u memory !\n",reg_num);
                                                 return 1; }

  return 0;
}

int InitExtraLargeRegister( unsigned int reg_num, unsigned int res_x,unsigned int res_y,unsigned int depth )
{
  if (xl_video_register[reg_num].pixels!=0) { fprintf(stderr,"While Allocating Extra Large Register : Video register %u is not empty !\n",reg_num);
                                             free( (void *) xl_video_register[reg_num].pixels);
                                             xl_video_register[reg_num].pixels=0; }

  xl_video_register[reg_num].pixels = (unsigned int * ) malloc( res_x * res_y * depth * sizeof(unsigned int) );
  if (xl_video_register[reg_num].pixels==0) { fprintf(stderr,"Error allocating memory for extra large video register\n"); return 0; }
  xl_video_register[reg_num].size_x = res_x;
  xl_video_register[reg_num].size_y = res_y;
  xl_video_register[reg_num].depth = depth;
  xl_video_register[reg_num].time = 0;
  xl_video_register[reg_num].used = 0;
  xl_video_register[reg_num].features =  CreateFeatureList(settings[MAX_FEATURES],settings[PATCH_TRACKING_WIDTH],settings[PATCH_TRACKING_HEIGHT]);
  xl_video_register[reg_num].faces = CreateFeatureList(settings[MAX_FACES],settings[PATCH_TRACKING_WIDTH],settings[PATCH_TRACKING_HEIGHT]);

  if ( xl_video_register[reg_num].pixels == 0 ) { fprintf(stderr,"While Allocating Extra Large Register : Unable to allocate video register %u memory !\n",reg_num);
                                                 return 1; }

  return 0;
}



int ClearVideoRegister(unsigned int reg_num)
{
    if (video_register[reg_num].depth==3)
     {
        memset(video_register[reg_num].pixels,0,metrics[RESOLUTION_MEMORY_LIMIT_3BYTE]*sizeof(unsigned char));
     } else
     {
        memset(video_register[reg_num].pixels,0,video_register[reg_num].size_x*video_register[reg_num].size_y*video_register[reg_num].depth*sizeof(unsigned char));
     }

    video_register[reg_num].lock = 0;
    video_register[reg_num].time = 0;
    video_register[reg_num].used = 0;
    ClearFeatureList(video_register[reg_num].features);
    ClearFeatureList(video_register[reg_num].faces);


    return 1;

}

int ClearLargeVideoRegister(unsigned int reg_num)
{
    if (l_video_register[reg_num].depth==3)
     {
        memset(l_video_register[reg_num].pixels,0,metrics[RESOLUTION_MEMORY_LIMIT_3BYTE]*sizeof(unsigned short));
     } else
     {
       memset(l_video_register[reg_num].pixels,0,l_video_register[reg_num].size_x*l_video_register[reg_num].size_y*l_video_register[reg_num].depth*sizeof(unsigned short));
     }

    l_video_register[reg_num].lock = 0;
    l_video_register[reg_num].time = 0;
    l_video_register[reg_num].used = 0;
    ClearFeatureList(l_video_register[reg_num].features);
    ClearFeatureList(l_video_register[reg_num].faces);

    return 1;

}

int ClearExtraLargeVideoRegister(unsigned int reg_num)
{
    if (xl_video_register[reg_num].depth==3)
     {
        memset(xl_video_register[reg_num].pixels,0,metrics[RESOLUTION_MEMORY_LIMIT_3BYTE]*sizeof(unsigned int));
     } else
     {
       memset(xl_video_register[reg_num].pixels,0,l_video_register[reg_num].size_x*l_video_register[reg_num].size_y*l_video_register[reg_num].depth*sizeof(unsigned int));
     }

    xl_video_register[reg_num].lock = 0;
    xl_video_register[reg_num].time = 0;
    xl_video_register[reg_num].used = 0;
    ClearFeatureList(xl_video_register[reg_num].features);
    ClearFeatureList(xl_video_register[reg_num].faces);

    return 1;

}

int CloseRegister( unsigned int reg_num )
{
    if ( video_register[reg_num].pixels == 0 ) { fprintf(stderr,"While Deallocating Register : video register %u already deallocated !\n",reg_num);
                                                 return 1; }

    free( video_register[reg_num].pixels );
    video_register[reg_num].size_x = 0;
    video_register[reg_num].size_y = 0;
    video_register[reg_num].depth = 0;
    video_register[reg_num].time = 0;
    video_register[reg_num].used = 0;
    DestroyFeatureList(video_register[reg_num].features);
    DestroyFeatureList(video_register[reg_num].faces);

    return 0;
}




int CloseLargeRegister( unsigned int reg_num )
{
    if ( l_video_register[reg_num].pixels == 0 ) { fprintf(stderr,"While Deallocating Large Register : video register %u already deallocated !\n",reg_num);
                                                   return 1; }

    free( l_video_register[reg_num].pixels );
    l_video_register[reg_num].size_x = 0;
    l_video_register[reg_num].size_y = 0;
    l_video_register[reg_num].depth = 0;
    l_video_register[reg_num].time = 0;
    l_video_register[reg_num].used = 0;
    DestroyFeatureList(l_video_register[reg_num].features);
    DestroyFeatureList(l_video_register[reg_num].faces);

    return 0;
}


int CloseExtraLargeRegister( unsigned int reg_num )
{
    if ( xl_video_register[reg_num].pixels == 0 ) { fprintf(stderr,"While Deallocating Extra Large Register : video register %u already deallocated !\n",reg_num);
                                                   return 1; }

    free( xl_video_register[reg_num].pixels );
    xl_video_register[reg_num].size_x = 0;
    xl_video_register[reg_num].size_y = 0;
    xl_video_register[reg_num].depth = 0;
    xl_video_register[reg_num].time = 0;
    xl_video_register[reg_num].used = 0;
    DestroyFeatureList(xl_video_register[reg_num].features);
    DestroyFeatureList(xl_video_register[reg_num].faces);


    return 0;
}


void DefaultSettings()
{


    // Initialize all variables used by Visual Cortex
    settings[INPUT_CALIBRATION]=0; // DEACTIVATED UNTIL FIX :P
    // TEST

   settings[PASS_TO_WORLD_3D]=0; // FOR NOW ONLY PASSES TO 3D Visualization
   settings[PASS_TO_FACE_DETECTOR]=1;
   settings[PASS_TO_FEATURE_DETECTOR]=1;

   settings[REMEMBER_FACES]=1;

   settings[FEATURE_TRACKING_COMPARISON_THRESHOLD]=35000; /* FEATURE Tracking comparison threshold */

   settings[FEATURE_DETECTION_THRESHOLD]=40; // 30

   settings[DEPTHMAP_COMPARISON_ALSO_REVERSE_CHECK]=1;

   settings[DEPTHMAP_RGB_MULTIPLIER]=1;
   settings[DEPTHMAP_MOVEMENT_MULTIPLIER]=10;
   settings[DEPTHMAP_SOBEL_MULTIPLIER]=4;
   settings[DEPTHMAP_SECOND_DERIVATIVE_MULTIPLIER]=39;


    settings[DEPTHMAP_STARTLEFT_X]=15;
    settings[DEPTHMAP_DETAIL]=5;
    settings[DEPTHMAP_EDGE_LOW_STRICTNESS]=45;
    settings[DEPTHMAP_EDGE_HIGH_STRICTNESS]=255; // 255
    settings[DEPTHMAP_INSTANT_DETAIL]=2;
    settings[DEPTHMAP_VERT_SHIFT_UP]=5;
    settings[DEPTHMAP_VERT_SHIFT_DOWN]=0;
    settings[DEPTHMAP_VERT_OFFSET_UP]=0;
    settings[DEPTHMAP_VERT_OFFSET_DOWN]=0;
    settings[DEPTHMAP_COMPARISON_TOO_GOOD_THRESHOLD]=500;
    settings[DEPTHMAP_COMPARISON_THRESHOLD]=30000; // 27000;//18000; //16000;
    settings[DEPTHMAP_COMPARISON_THRESHOLD_LARGE_PATCH]=30000;
    settings[DEPTHMAP_COMPARISON_THRESHOLD_EXTRALARGE_PATCH]=20000;
    // SetThresholdsForAllPatchSizes();
    settings[DEPTHMAP_COMPARISON_THRESHOLD_ADDED]=0;// <- this value is added to comparison_threshold!

    settings[DEPTHMAP_COMPARISON_DO_NOT_PERFORM_FULL_COUNT]=1; /* <- This actually should always be 1 :P */
    settings[DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_PREVIOUS_PATCH_SIZE_DEPTH]=1;
    settings[DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_CLOSEST_DEPTH]=1; /* <- This actually should always be 1 :P */
    settings[DEPTHMAP_CLOSEST_DEPTH]=75; // Praktika dedomena deixnoun oti synithws apotelesmata panw apo 80 einai poly konta kai tha prepe na theorountai thoryvos!
    settings[DEPTHMAP_GUESSES]=0;
    settings[DEPTHMAP_IMPROVE_USING_HISTOGRAM]=1;
    settings[DEPTHMAP_IMPROVE_FILLING_HOLES]=0;
    settings[DEPTHMAP_IMPROVE_USING_EDGES]=0;
    settings[DEPTHMAP_IMPROVE_USING_MOVEMENT]=1;


    settings[PATCH_COMPARISON_LEVELS]=3; /* It will use 3 different size block levels for comparison */
    settings[PATCH_COMPARISON_SCORE_MIN]=35000;
    settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED]=20;
    settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_LARGE_PATCH]=20;
    settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_EXTRALARGE_PATCH]=10;
    settings[PATCH_HIST_THRESHOLD_R]=9; settings[PATCH_HIST_THRESHOLD_G]=9; settings[PATCH_HIST_THRESHOLD_B]=9;

    settings[MAX_FEATURES]=1000;
    settings[MAX_FACES]=100;

    settings[PATCH_TRACKING_WIDTH]=19;
    settings[PATCH_TRACKING_HEIGHT]=19;

    settings[TIME_BETWEEN_TRACKING]=500;

    settings[MOVEMENT_PATCH_SENSITIVITY]=7;
    settings[MOVEMENT_R_THRESHOLD]=45; settings[MOVEMENT_G_THRESHOLD]=45;   settings[MOVEMENT_B_THRESHOLD]=45;
    settings[MOVEMENT_MIN_R_THRESHOLD]=29; settings[MOVEMENT_MIN_G_THRESHOLD]=29; settings[MOVEMENT_MIN_B_THRESHOLD]=29;
}

int InitVisionMemory(unsigned int res_x,unsigned int res_y)
{
    int i=0;
    if ( COLD_START == 1 )
     {
        // THIS MEANS THE WHOLE CORTEX JUST INITIALIZED , SO LETS CLEAN THE VIDEO REGISTERS
        for ( i=0; i<REGISTERS_COUNT; i++) video_register[i].pixels = 0;
        for ( i=0; i<LARGE_REGISTERS_COUNT; i++) l_video_register[i].pixels = 0;
     }

    COLD_START=0;

    //INITIALIZE ALL IMAGE METRICS
    metrics[RESOLUTION_X]=res_x;
    metrics[RESOLUTION_Y]=res_y;
    metrics[RESOLUTION_X_3_BYTE]=res_x * 3;
    metrics[RESOLUTION_MEMORY_LIMIT_1BYTE]=res_x*res_y;
    metrics[RESOLUTION_MEMORY_LIMIT_3BYTE]=metrics[RESOLUTION_MEMORY_LIMIT_1BYTE]*3;
    metrics[RESOLUTION_DEPTH]=3;
    metrics[CHANGES_LEFT]=0;
    metrics[CHANGES_RIGHT]=0;
    metrics[HORIZONTAL_BUFFER]=20; //20
    metrics[VERTICAL_BUFFER]=25; //30
    metrics[HORIZONTAL_BUFFER_LARGE]=20; //50
    metrics[VERTICAL_BUFFER_LARGE]=40; //75
    metrics[HORIZONTAL_BUFFER_EXTRALARGE]=40; //125
    metrics[VERTICAL_BUFFER_EXTRALARGE]=50; // 188
    metrics[GROUP_MOVEMENT_ARRAY_SIZE] = ( ((res_y+1)/metrics[VERTICAL_BUFFER])*((res_x+1)/metrics[HORIZONTAL_BUFFER]) ) + ((res_x+1)/metrics[HORIZONTAL_BUFFER]);

    DefaultSettings(); //Settings must be set after metrics because they take them into account

   fprintf(stderr,"Initializing %u  Video Registers\n",REGISTERS_COUNT+LARGE_REGISTERS_COUNT);

    for ( i=0; i<REGISTERS_COUNT; i++)
     {
        if ( InitRegister(i,res_x,res_y,3)!=0 ) { fprintf(stderr,"Error initializing Vision Memory"); return 1; }
     }
     fprintf(stderr,"%u MB of memory allocated for video registers\n",(unsigned int) ( 3*res_x*res_y*REGISTERS_COUNT*sizeof(char)) / 1048576);

     for ( i=0; i<LARGE_REGISTERS_COUNT; i++)
     {
        if ( InitLargeRegister(i,res_x,res_y,3)!=0 ) { fprintf(stderr,"Error initializing Vision Memory"); return 1; }
     }
     fprintf(stderr,"%u MB of memory allocated for large video registers\n",(unsigned int) ( 3*res_x*res_y*LARGE_REGISTERS_COUNT*sizeof(unsigned short)) / 1048576);

     for ( i=0; i<EXTRA_LARGE_REGISTERS_COUNT; i++)
     {
        if ( InitExtraLargeRegister(i,res_x,res_y,3)!=0 ) { fprintf(stderr,"Error initializing Vision Memory"); return 1; }
     }
     fprintf(stderr,"%u MB of memory allocated for extra large video registers\n",(unsigned int) ( 3*res_x*res_y*LARGE_REGISTERS_COUNT*sizeof(unsigned int)) / 1048576);


  //  unsigned int MEM3BIT = (res_x+1)*(res_y+1)*3;
    unsigned int MEM1BIT = (res_x+1)*(res_y+1);

    depth_data_array = ( struct DepthData * ) malloc ( sizeof(struct DepthData) * MEM1BIT );
    if (depth_data_array==0) { fprintf(stderr,"Error allocating depth_data_array memory \n"); return 1; }


    Precalculations();

  return 0; // 0 Means Success!
}


int CloseVisionMemory()
{
   fprintf(stderr,"Deinitializing Vision Memory \n");
   int i=0;
    for ( i=0; i<REGISTERS_COUNT; i++)
     {
        fprintf(stderr,"Deinit %u ",i );
        if ( CloseRegister(i)!=0 ) { fprintf(stderr,"Error deinitializing Vision Memory"); return 1; }
     }

   fprintf(stderr,"Deinitializing Large Vision Memory \n");
    for ( i=0; i<LARGE_REGISTERS_COUNT; i++)
     {
        if ( CloseLargeRegister(i)!=0 ) { fprintf(stderr,"Error deinitializing Vision Memory"); return 1; }
     }


   fprintf(stderr,"Deinitializing Extra Large Vision Memory \n");
    for ( i=0; i<EXTRA_LARGE_REGISTERS_COUNT; i++)
     {
        if ( CloseExtraLargeRegister(i)!=0 ) { fprintf(stderr,"Error deinitializing Vision Memory"); return 1; }
     }

   fprintf(stderr,"Deinitializing Depth data array \n");
   if ( depth_data_array != 0 )  free ( depth_data_array );

   fprintf(stderr,"Successfull Deinitialization of Visual Cortex!\n");
  return 0; // 0 Means Success!
}







/*
    BITBLT , etc
*/
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

void SwapChars(char * chr1,char *chr2)
{
   char tmp = *chr1;
   *chr1=*chr2;
   *chr2=tmp;
}

void SwapInts(unsigned int * int1,unsigned int *int2)
{
   unsigned int tmp = *int1;
   *int1=*int2;
   *int2=tmp;
}

int SwapRegister(unsigned int source,unsigned int target)
{
  register unsigned char *tmp_reg  = video_register[source].pixels;
  video_register[source].pixels = video_register[target].pixels;
  video_register[target].pixels = tmp_reg;

  SwapInts(&video_register[source].size_x,&video_register[target].size_x);
  SwapInts(&video_register[source].size_y,&video_register[target].size_y);
  SwapInts(&video_register[source].depth,&video_register[target].depth);

  SwapInts(&video_register[source].lock,&video_register[target].lock);
  SwapInts(&video_register[source].time,&video_register[target].time);
  SwapChars(&video_register[source].used,&video_register[target].used);

  struct FeatureList * tmp_features = video_register[source].features;
  video_register[source].features = video_register[target].features;
  video_register[target].features = tmp_features;

  tmp_features = video_register[source].faces;
  video_register[source].faces = video_register[target].faces;
  video_register[target].faces = tmp_features;

  return 1;
}


int CopyRegister(unsigned int source,unsigned int target,unsigned int copy_features,unsigned int copy_faces)
{
  unsigned int image_size=metrics[RESOLUTION_MEMORY_LIMIT_3BYTE];
  if ( video_register[source].depth == 1 ) { image_size=metrics[RESOLUTION_MEMORY_LIMIT_1BYTE]; } else
  if ( video_register[source].depth == 3 ) { image_size=metrics[RESOLUTION_MEMORY_LIMIT_3BYTE]; } else
                                           {
                                               fprintf(stderr,"assuming 3byte size");
                                           }

 register BYTE *start_px = (BYTE *) video_register[source].pixels;
 register BYTE *px = (BYTE *) video_register[source].pixels;
 register BYTE *tpx = (BYTE *) video_register[target].pixels;

 while ( px < start_px+image_size)
 {
       *tpx = *px;
       ++tpx;
       ++px;
 }

  video_register[target].size_x=video_register[source].size_x;
  video_register[target].size_y=video_register[source].size_y;
  video_register[target].depth=video_register[source].depth;

  video_register[target].lock=video_register[source].lock;
  video_register[target].time=video_register[source].time;
  video_register[target].used=video_register[source].used;

  if (copy_features) { CopyFeatureList(video_register[source].features,video_register[target].features); }
  if (copy_faces) { CopyFeatureList(video_register[source].faces,video_register[target].faces); }

  return 1;
}



void ConvertRegisterFrom3ByteTo1Byte(int in_reg)
{
  if (video_register[in_reg].pixels==0) {return;}
  int col_med;
  unsigned int image_size=metrics[RESOLUTION_MEMORY_LIMIT_3BYTE];
  if ( video_register[in_reg].depth == 3 ) { video_register[in_reg].depth = 1; } else
                                           {
                                             fprintf(stderr,"Error , ConvertRegisterFrom3ByteTo1Byte called with register of %u depth\n",video_register[in_reg].depth);
                                             return;
                                           }

 register BYTE *start_px = (BYTE *) video_register[in_reg].pixels;
 register BYTE *opx = (BYTE *) video_register[in_reg].pixels;
 register BYTE *px =  (BYTE *) video_register[in_reg].pixels;
 register BYTE *r;
 register BYTE *g;
 register BYTE *b;

 while ( px < start_px+image_size)
 {
       r = px++; g = px++; b = px++;
       col_med=  ( *r + *g + *b )/3;
       if ( col_med < 255 ) {
                              *opx= (BYTE)col_med ;
                            }
                            else
                            {
                              *opx= 255;
                            }
	   ++opx;
 }
 return;
}


void ConvertRegisterFrom1ByteTo3Byte(int in_reg)
{
  if (video_register[in_reg].pixels==0) {return;}

  if ( video_register[in_reg].depth == 1 ) { video_register[in_reg].depth = 3; } else
                                           {
                                             fprintf(stderr,"Error , ConvertRegisterFrom1ByteTo3Byte called with register of %u depth\n",video_register[in_reg].depth);
                                             return;
                                           }

 register BYTE *start_px = (BYTE *) video_register[in_reg].pixels;
 register BYTE *opx      = (BYTE *) video_register[in_reg].pixels+metrics[RESOLUTION_MEMORY_LIMIT_3BYTE];
 register BYTE *px       = (BYTE *) video_register[in_reg].pixels+metrics[RESOLUTION_MEMORY_LIMIT_1BYTE];

 while ( px >= start_px)
 {
       *opx = *px;   --opx;
       *opx = *px;   --opx;
       *opx = *px;   --opx;
       --px;
 }
 return;
}


int ThisIsA3ByteRegister(int reg)
{
    if ( video_register[reg].depth == 3 )
     {
         return 1;
     }

     fprintf(stderr,"3byte register check failed\n");
     return 0;
}


int ThisIsA1ByteRegister(int reg)
{
    if ( video_register[reg].depth == 1 )
     {
         return 1;
     }

     fprintf(stderr,"1byte register check failed\n");
     return 0;
}


/*
    ---------------------------------------
                FILE OPERATIONS
    ---------------------------------------
*/

int PrintRegister(char * filename,unsigned int reg_num)
{
    FILE *fd=0;
    fd = fopen(filename,"wb");

    if (fd!=0)
	 {
	  fprintf(fd,"<html><head></head><body><h1>Register %u</h1><br>",reg_num);
      fprintf(fd,"<h2>%ux%u : %u </h2>",video_register[reg_num].size_x,video_register[reg_num].size_y,video_register[reg_num].depth);
         int x=0,y=0;
         unsigned char * ptr = video_register[reg_num].pixels;
         fprintf(fd,"<table border=1>\n");
           while (y<metrics[RESOLUTION_Y])
            {
              fprintf(fd," <tr>\n");
              x=0;
              while (x<metrics[RESOLUTION_X])
              {
                  fprintf(fd,"  <td>\n");
                    if (video_register[reg_num].depth == 3 )
                      {
                        fprintf(fd,"%u,",*ptr); ++ptr;
                        fprintf(fd,"%u,",*ptr); ++ptr;
                        fprintf(fd,"%u",*ptr); ++ptr;
                      } else
                    if (video_register[reg_num].depth == 1 )
                      {
                          fprintf(fd,"%u",*ptr);
                          ++ptr;
                      }
                  fprintf(fd,"  </td>\n");
                  ++x;
              }
              fprintf(fd," </tr>\n");
                ++y;
            }
        fprintf(fd,"</table>\n");
	  fprintf(fd,"</body></html>");
	  fclose(fd);
     }
  return 1;
}

int PrintExtraLargeRegister(char * filename,unsigned int reg_num)
{
    FILE *fd=0;
    fd = fopen(filename,"wb");

    if (fd!=0)
	 {
	  fprintf(fd,"<html><head></head><body><h1>Extra Large Register %u</h1><br>",reg_num);
      fprintf(fd,"<h2>%ux%u : %u </h2>",xl_video_register[reg_num].size_x,xl_video_register[reg_num].size_y,xl_video_register[reg_num].depth);
         int x=0,y=0;
         unsigned int * ptr = xl_video_register[reg_num].pixels;
         fprintf(fd,"<table border=1>\n");
           while (y<metrics[RESOLUTION_Y])
            {
              fprintf(fd," <tr>\n");
              x=0;
              while (x<metrics[RESOLUTION_X])
              {
                  fprintf(fd,"  <td>\n");
                    if (xl_video_register[reg_num].depth == 3 )
                      {
                        fprintf(fd,"%u,",*ptr); ++ptr;
                        fprintf(fd,"%u,",*ptr); ++ptr;
                        fprintf(fd,"%u" ,*ptr); ++ptr;
                      } else
                    if (xl_video_register[reg_num].depth == 1 )
                      {
                          fprintf(fd,"%u",*ptr);
                          ++ptr;
                      }
                  fprintf(fd,"  </td>\n");
                  ++x;
              }
              fprintf(fd," </tr>\n");
                ++y;
            }
        fprintf(fd,"</table>\n");
	  fprintf(fd,"</body></html>");
	  fclose(fd);
     }
  return 1;
}

int SaveRegisterToFile(char * filename,unsigned int reg_num)
{
    FILE *fd=0;
    fd = fopen(filename,"wb");

    if (fd!=0)
	{
     unsigned int n;
     fprintf(fd, "P6\n%d %d\n255\n", video_register[reg_num].size_x, video_register[reg_num].size_y);
     n = video_register[reg_num].size_x * video_register[reg_num].size_y;
     fwrite(video_register[reg_num].pixels, 3, n, fd);
     fflush(fd);
     fclose(fd);
     return 1;
	}
  return 0;
}

int SaveRegisterPartToFile(char * filename,unsigned int reg_num,unsigned int x_start,unsigned int y_start ,unsigned int width,unsigned int height)
{
    FILE *fd=0;
    fd = fopen(filename,"wb");

    if (fd!=0)
	{
      unsigned int pixel_incrementation=video_register[reg_num].size_x * 3;
      unsigned int x,y;
      unsigned char * pixel_memory_end = video_register[reg_num].pixels + metrics[RESOLUTION_MEMORY_LIMIT_3BYTE];
      unsigned char * pixel_row;
      fprintf(fd, "P6\n%d %d\n255\n", width, height);

       //fprintf(stderr,"SaveRegister(%u) Part (%u,%u -> %u,%u ) \n",reg_num,x_start,y_start,x_start+width,y_start+height);
       x = x_start; y = y_start;
       pixel_row = (unsigned char *) video_register[reg_num].pixels  +  (( y * video_register[reg_num].size_x * 3 ) + ( x * 3 ));
       while ( (y<y_start+height) && (pixel_row+video_register[reg_num].size_x*3<pixel_memory_end) )
        {
          fwrite(pixel_row,3,width,fd);
          pixel_row += pixel_incrementation;
          ++y;
        }

     fflush(fd);
     fclose(fd);
     return 1;
	}
  return 0;
}

int LoadRegisterFromFile(char * filename,unsigned int reg_num)
{
FILE *pf=0;
    pf = fopen(filename,"rb");

    if (pf!=0 )
    {
        char buf[PPMREADBUFLEN], *t;
        unsigned int w=0, h=0, d=0;
        int r=0;

        t = fgets(buf, PPMREADBUFLEN, pf);
        if ( (t == 0) || ( strncmp(buf, "P6\n", 3) != 0 ) ) { fclose(pf); return 0; }
        do
        { /* Px formats can have # comments after first line */
           t = fgets(buf, PPMREADBUFLEN, pf);
           if ( t == 0 ) { fclose(pf); return 0; }
        } while ( strncmp(buf, "#", 1) == 0 );
        r = sscanf(buf, "%u %u", &w, &h);
        if ( r < 2 ) { fclose(pf); return 0; }
        // The program fails if the first byte of the image is equal to 32. because
        // the fscanf eats the space and the image is read with some bit less
        r = fscanf(pf, "%u\n", &d);
        if ( (r < 1) || ( d != 255 ) ) { fclose(pf); return 0; }

        if ( (w!=video_register[reg_num].size_x) || (h!=video_register[reg_num].size_y) )
           {
             fprintf(stderr,"Incorrect file size ( %s ) :P\n",filename);
             if ( w * h > video_register[reg_num].size_x * video_register[reg_num].size_y )
               {
                 fprintf(stderr,"File %s will lead to overflow stopping read..\n",filename);
                 fclose(pf);
                 return 0;
               }
           }

        if ( video_register[reg_num].pixels != 0 )
        {
            size_t rd = fread(video_register[reg_num].pixels,3, w*h, pf);
            fclose(pf);
            if ( rd < w*h )
            {
               return 0;
            }
            return 1;
        }
      fclose(pf);
    }
  return 0;
}

unsigned int GetTempRegister()
{
    int res_reg = GENERAL_1;
    while ( (video_register[res_reg].used) && (res_reg < REGISTERS_COUNT) )
     {
         ++res_reg;
     }

    if (res_reg >= REGISTERS_COUNT) { return 0; }
    video_register[res_reg].used=1;

    return res_reg;
}

unsigned int StopUsingVideoRegister(unsigned int thereg)
{
    video_register[thereg].used=0;
    return 1;
}

