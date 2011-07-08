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
#include "VisualCortex.h"
#include "VisionMemory.h"
#include "Precalculations.h"
#include "DisparityDepthMap.h"
#include "DisparityDepthMap_Heuristics.h"
#include "MovementRegistration.h"
#include "VisCortexFilters.h"
#include "VisCortexConvolutionFilters.h"
#include "FeatureExtraction.h"
#include "PatternRecognition.h"
#include "FeatureTracking.h"
#include "FaceDetection.h"
#include "StateSetting.h"
#include "LinearAlgebra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char * VISCORTEX_VER = "0.592";

/*

                 TODO LIST , BULETTIN BOARD
  TODO STUFF
   - I Have the parameters of my cameras , I must add calibration/rectification/Camera resectioning code for the frames , there is no excuse :P
   - I Currently have 2 Histogram block generators , the one specific and one generic , I should only use the generic one :P
   - I have to make an algorithm that identifies lines
   - ... etc ...

  I am currently restructuring the whole library to make it a little more organized..
  Different "logical functions" have been split to different files , basically the project is now divided
  to the following sections


     MAIN LIBRARY / MEMORY / INTERFACES
          VisualCortex.c
          VisionMemory.c
          StateSetting.c

     MATH
          Linear Algebra.c
          Precalculations.c <- this is to speed up math that are cpu intensive

     FILTERS / IMAGE PROCESSING TOOLS ( These are shared functions and are used in all other parts )
          VisCortexConvolutionFilters.c
          VisCortexFilters.c
          VisCortexHeuristics.c
          IntegralImageConversion.c

     MOVEMENT REGISTRATION
          MovementRegistration.c <- This is complete , it doesn`t track features it just outputs on memory segments of the frame that have motion to help other functions

     DISPARITY MAPPING
          DisparityDepthMap.c <- This is in a good state :)
          DisparityDepthMap_Heuristics.c <- This is in a good state , although not very useful


    ---

     FEATURE LISTS
          FeatureLists.c <- This creates the Feature containers to be used by the next 2 modules

     FEATURE EXTRACTION
          FeatureExtraction.c <- This currently is a stub , it should output second degree derivative edges or something

     FEATURE TRACKING
          FeatureTracking.c <- This used to work , but due to changes on the basic functions called , it will have to be rewritten from scratch

     PATTERN RECOGNITION
          PatternRecognition.c <- This currently is a stub , I have written some code for haar features but it needs much more

     FACE DETECTION
          FaceDetection.c <- This currently is a stub


*/






char *  VisCortx_Version()
{
	return VISCORTEX_VER;
}

/*
 -----------------
                     INITIALIZATION
                     StateSetting.c/h
                                      ----------------------
*/
unsigned int VisCortx_SetCamerasGeometry(float distance_between_cameras,float diagonal_field_of_view,float horizontal_field_of_view,float vertical_field_of_view)
{
  return SetCamerasGeometry(
                            distance_between_cameras,
                            diagonal_field_of_view,
                            horizontal_field_of_view,
                            vertical_field_of_view
                           );
}



unsigned int VisCortx_Start(unsigned int res_x,unsigned int res_y)
{
   return InitializeEverything(res_x,res_y);
}

unsigned int VisCortx_Stop()
{
   return  CloseEverything();
}

void VisCortx_SetTime(unsigned int thetime)
{
     SetTime(thetime);
}

unsigned int VisCortx_GetTime()
{
    return GetTime();
}

unsigned int VisCortx_SelfCheck()
{
    return SelfCheckEverything();
}

void VisCortx_SetSetting(unsigned int set_num,unsigned int set_val)
{
    SetSetting(set_num,set_val);
}

void VisCortx_SetDefaultSettings()
{
    SetDefaultSettings();
}

unsigned int VisCortx_GetSetting(unsigned int get_num)
{
    return GetSetting(get_num);
}


void VisCortx_SetMetric(unsigned int set_num,unsigned int set_val)
{
  metrics[set_num]=set_val;
}

unsigned int VisCortx_GetMetric(unsigned int get_num)
{
    return GetMetric(get_num);
}


unsigned int VisCortx_GetVideoRegisterStats(unsigned int metric_num)
{
   return GetVideoRegisterStats(metric_num);
}

/*
 -----------------
                     INITIALIZATION
                     StateSetting.c/h
                                      ----------------------
*/



void VisCortx_CameraParameters(int right_cam,double fx,double fy,double cx,double cy,double k1,double k2,double p1,double p2,double k3)
{
   if ( right_cam == 0 )  { PrecalcResectioning(resection_left_precalc,fx,fy,cx,cy,k1,k2,p1,p2,k3); } else
   if ( right_cam == 1 )  { PrecalcResectioning(resection_right_precalc,fx,fy,cx,cy,k1,k2,p1,p2,k3); }
}


/*
 ----------------- VIDEO INPUT/OUTPUT ----------------------
*/

/*
 THIS FUNCTION IS CALLED EVERY TIME A NEW FRAME IS READY FOR STEREO VISION
*/
unsigned int VisCortX_NewFrame(unsigned int input_img_regnum,unsigned int size_x,unsigned int size_y,unsigned int depth,unsigned char * rgbdata)
{
   if ( rgbdata == 0 ) { fprintf(stderr,"VisCortX_NewFrame given zero pointer"); return 0; }
   if ( input_img_regnum == LEFT_EYE )
    {
       if ( video_register[input_img_regnum].lock )
         {
            // fprintf(stderr,"Left Reg is locked \n ");
             return 0;
         }

       video_register[input_img_regnum].lock=1;
       // FIRST STORE OLD REGISTERS
       VisCortX_CopyVideoRegister(LEFT_EYE,LAST_LEFT_EYE);
       VisCortX_CopyVideoRegister(EDGES_LEFT,LAST_EDGES_LEFT);
       VisCortX_CopyVideoRegister(SECOND_DERIVATIVE_LEFT,LAST_SECOND_DERIVATIVE_LEFT);
       VisCortX_CopyVideoRegister(MOVEMENT_LEFT,LAST_MOVEMENT_LEFT);

       // SECOND PASS NEW IMAGE
       VisCortx_WriteToVideoRegister(LEFT_EYE,size_x,size_y,depth,rgbdata);

       // THIRD PROCESS NEW IMAGE
       CalibrateImage(LEFT_EYE,CALIBRATED_LEFT_EYE,resection_left_precalc);
       PrepareCleanSobeledGaussianAndDerivative(CALIBRATED_LEFT_EYE,EDGES_LEFT,SECOND_DERIVATIVE_LEFT,settings[DEPTHMAP_EDGE_LOW_STRICTNESS],settings[DEPTHMAP_EDGE_HIGH_STRICTNESS]);
       GenerateCompressHistogramOfImage(video_register[CALIBRATED_LEFT_EYE].pixels,l_video_register[HISTOGRAM_COMPRESSED_LEFT].pixels,metrics[HORIZONTAL_BUFFER],metrics[VERTICAL_BUFFER]);

        VisCortx_Movement_Detection(1,0);
        TrackAllPointsOnRegisters(LEFT_EYE,LAST_LEFT_EYE,8000);

       video_register[input_img_regnum].lock=0;
    } else
    if ( input_img_regnum == RIGHT_EYE )
    {

        if ( video_register[input_img_regnum].lock )
         {
            // fprintf(stderr,"Right Reg is locked \n ");
             return 0;
         }

       video_register[input_img_regnum].lock=1;
       // FIRST STORE OLD REGISTERS
       VisCortX_CopyVideoRegister(RIGHT_EYE,LAST_RIGHT_EYE);
       VisCortX_CopyVideoRegister(EDGES_RIGHT,LAST_EDGES_RIGHT);
       VisCortX_CopyVideoRegister(SECOND_DERIVATIVE_RIGHT,LAST_SECOND_DERIVATIVE_RIGHT);
       VisCortX_CopyVideoRegister(MOVEMENT_RIGHT,LAST_MOVEMENT_RIGHT);

       // SECOND PASS NEW IMAGE
       VisCortx_WriteToVideoRegister(RIGHT_EYE,size_x,size_y,depth,rgbdata);

       // THIRD PROCESS NEW IMAGE
       CalibrateImage(RIGHT_EYE,CALIBRATED_RIGHT_EYE,resection_right_precalc);
       PrepareCleanSobeledGaussianAndDerivative(CALIBRATED_RIGHT_EYE,EDGES_RIGHT,SECOND_DERIVATIVE_RIGHT,settings[DEPTHMAP_EDGE_LOW_STRICTNESS],settings[DEPTHMAP_EDGE_HIGH_STRICTNESS]);
       GenerateCompressHistogramOfImage(video_register[CALIBRATED_RIGHT_EYE].pixels,l_video_register[HISTOGRAM_COMPRESSED_RIGHT].pixels,metrics[HORIZONTAL_BUFFER],metrics[VERTICAL_BUFFER]);

        VisCortx_Movement_Detection(0,1);
        TrackAllPointsOnRegisters(RIGHT_EYE,LAST_RIGHT_EYE,8000);
        video_register[input_img_regnum].lock=0;
    }
 return 1;
}

unsigned int VisCortX_ClearVideoRegister(unsigned int input_img_regnum)
{
    return ClearVideoRegister(input_img_regnum);
}

unsigned int VisCortX_CopyVideoRegister(unsigned int input_img_regnum,unsigned int output_img_regnum)
{
    return CopyRegister(input_img_regnum,output_img_regnum);
}

unsigned int VisCortX_CopyFromVideoToVideoRegister(unsigned int input_img_regnum,unsigned int output_img_regnum)
{
  unsigned long syst_mem_end=metrics[RESOLUTION_X]*metrics[RESOLUTION_Y]*3;
  if ( video_register[output_img_regnum].depth != video_register[input_img_regnum].depth )
    {
        if ( ( video_register[input_img_regnum].depth == 1 ) && ( video_register[output_img_regnum].depth == 3) )
          {
            //fprintf(stderr,"Kind of unsafe conversion from 1bit to 3bit ;P \n");
            unsigned char * src=video_register[input_img_regnum].pixels;
            unsigned char  * target=video_register[output_img_regnum].pixels;
            int i=0;
            while ( i < syst_mem_end )
             {
                *target = *src; target++;
                *target = *src; target++;
                *target = *src; target++;
                src++;

                i++;
             }
          } else
          {
              fprintf(stderr,"VisCortX_CopyFromVideoToVideoRegister from %u bit to %u bit not implemented\n",video_register[input_img_regnum].depth,video_register[output_img_regnum].depth);
              return 0;
          }
    }  else
    {
      memcpy(video_register[output_img_regnum].pixels,video_register[input_img_regnum].pixels,syst_mem_end);
    }

  return 1;
}

unsigned int VisCortX_BitBltVideoRegister(unsigned int input_img_regnum,unsigned int output_img_regnum,unsigned int px,unsigned int py,unsigned int tx,unsigned int ty,unsigned int size_x,unsigned int size_y)
{
  CopyPartOfImageToImage(video_register[input_img_regnum].pixels,video_register[output_img_regnum].pixels,px,py,tx,ty,size_x,size_y);
  return 1;
}

unsigned int VisCortX_LoadVideoRegisterFromFile(unsigned int reg_num,char * filename)
{
   return LoadRegisterFromFile(filename,reg_num);
}

unsigned int VisCortX_SaveVideoRegisterToFile(unsigned int reg_num,char * filename)
{
   return SaveRegisterToFile(filename,reg_num);
}

unsigned int VisCortx_WriteToVideoRegister(unsigned int reg_num,unsigned int size_x,unsigned int size_y,unsigned int depth,unsigned char * rgbdata)
{
	if (  !VideoRegisterRequestIsOk(reg_num,size_x,size_y,depth) ) { fprintf(stderr,"VisCortx_WriteToVideoRegister failing.."); return 0; }
	if ( ( rgbdata == 0 ) || ( depth == 0 ) || ( size_x == 0 ) || ( size_y == 0 ) ) { fprintf(stderr,"VisCortx_WriteToVideoRegister failing..");  return 0; /*Discarding erroneous input*/ }

    unsigned long syst_mem_end=metrics[RESOLUTION_X]*metrics[RESOLUTION_Y]*3;
    unsigned long pic_mem_end=size_x*size_y*depth;

    unsigned long mem_end=syst_mem_end;
    if ( pic_mem_end > mem_end ) { fprintf(stderr,"Register is not big enough to accomodate data! \n "); return 1; } else
    if ( pic_mem_end < mem_end ) { mem_end = pic_mem_end; }


    memcpy(video_register[reg_num].pixels,rgbdata,mem_end);

/*
    switch (reg_num)
     {
        case LEFT_EYE :
         CalibrateImage(LEFT_EYE,CALIBRATED_LEFT_EYE);
        break;
        case RIGHT_EYE :
         CalibrateImage(RIGHT_EYE,CALIBRATED_RIGHT_EYE);
        break;
     };*/

	return 1;
}

unsigned char * VisCortx_ReadFromVideoRegister(unsigned int reg_num,unsigned int size_x,unsigned int size_y,unsigned int depth)
{
   if (  !VideoRegisterRequestIsOk(reg_num,size_x,size_y,depth) ) { return 0; }

   if ( (size_x!=video_register[reg_num].size_x) ||
        (size_y!=video_register[reg_num].size_y) ||
        (depth !=video_register[reg_num].depth)
      )
      { fprintf(stderr,"Wrong Register Request Size :S \n ");
        fprintf(stderr,"Register ( %ux%u:%u ) , Request ( %ux%u:%u )\n ",size_x,size_y,depth,video_register[reg_num].size_x,video_register[reg_num].size_y,video_register[reg_num].depth);

        return 0; }


   return video_register[reg_num].pixels;
}
/*
 ----------------- VIDEO INPUT/OUTPUT ----------------------
*/


int VisCortx_OperationLockFrames()
{
 int waiting_counter = 0;
 while ( ( video_register[LEFT_EYE].lock ) && ( waiting_counter < 10000 ) )    { ++waiting_counter; usleep(1); }
 if ( !video_register[LEFT_EYE].lock )
   {
    // fprintf(stderr,"LOCKING 1\n");
     video_register[LEFT_EYE].lock=1;
   }


 waiting_counter = 0;
 while ( ( video_register[RIGHT_EYE].lock ) && ( waiting_counter < 10000 ) )    { ++waiting_counter; usleep(1); }

 if ( !video_register[RIGHT_EYE].lock )
   {
    // fprintf(stderr,"LOCKING 2\n");
     video_register[RIGHT_EYE].lock=1;
   }

 return 1;
}

int VisCortx_OperationUnLockFrames()
{
     video_register[LEFT_EYE].lock=0;
     video_register[RIGHT_EYE].lock=0;
     return 1;
}


/*
 ----------------- DEPTH MAPPING FUNCTIONS ----------------------
*/
void  VisCortx_FullDepthMap()
{
  unsigned int edgepercent=settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED],patch_x=metrics[HORIZONTAL_BUFFER],patch_y=metrics[VERTICAL_BUFFER];
   unsigned int originalthreshold=settings[DEPTHMAP_COMPARISON_THRESHOLD];


 /*
    The register CALIBRATED_LEFT_EYE , CALIBRATED_RIGHT_EYE IS CONSTANTLY UPDATED , SO WE CANNOT OUTPUT A STEADY RESULT IF WHILE CALCULATING THE IMAGE CHANGES ..!
    Thats why we copy to LEFT_EYE_NOT_LIVE and RIGHT_EYE_NOT_LIVE , which are in turn passed for disparity mapping
 */
   if (!VisCortx_OperationLockFrames()) { fprintf(stderr,"Cannot Lock Video Input\n"); }

  /*
     WE COMPARE PATCHES ON 3 DIFFERENT LEVELS , EXTRA LARGE PATCHES , LARGE PATCHES , NORMAL PATCHES
   */
  /*
    CALCULATION OF EXTRA LARGE PATCHES FOLLOWS
   */

   unsigned int default_detail =  settings[DEPTHMAP_DETAIL];

   settings[DEPTHMAP_DETAIL]=settings[DEPTHMAP_DETAIL]/2;

  settings[DEPTHMAP_COMPARISON_THRESHOLD]=settings[DEPTHMAP_COMPARISON_THRESHOLD_EXTRALARGE_PATCH];
  settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED]=settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_EXTRALARGE_PATCH];
  metrics[VERTICAL_BUFFER]=metrics[VERTICAL_BUFFER_EXTRALARGE];
  metrics[HORIZONTAL_BUFFER]=metrics[HORIZONTAL_BUFFER_EXTRALARGE];

  DepthMapFull( CALIBRATED_LEFT_EYE,
                CALIBRATED_RIGHT_EYE,
                DEPTH_LEFT,
                DEPTH_RIGHT,
                1
             );

  /*
    CALCULATION OF LARGE PATCHES FOLLOWS
   */

settings[DEPTHMAP_DETAIL]=default_detail;

if ( settings[PATCH_COMPARISON_LEVELS] >= 2 )
{
  settings[DEPTHMAP_COMPARISON_THRESHOLD]=settings[DEPTHMAP_COMPARISON_THRESHOLD_LARGE_PATCH];
  settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED]=settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_LARGE_PATCH];
  metrics[VERTICAL_BUFFER]=metrics[VERTICAL_BUFFER_LARGE];
  metrics[HORIZONTAL_BUFFER]=metrics[HORIZONTAL_BUFFER_LARGE];

  DepthMapFull( CALIBRATED_LEFT_EYE,
                CALIBRATED_RIGHT_EYE,
                DEPTH_LEFT,
                DEPTH_RIGHT,
                0
             );


}

  /*
    CALCULATION OF NORMAL PATCHES FOLLOWS
   */
   settings[DEPTHMAP_COMPARISON_THRESHOLD]=originalthreshold;
   settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED]=edgepercent;
   metrics[VERTICAL_BUFFER]=patch_y;
   metrics[HORIZONTAL_BUFFER]=patch_x;
   /* THESE 3 LINES ARE DELIBERATELY OUT OF THE IF CONTROL BECAUSE THESE VALUES ARE DEFAULT*/

if ( settings[PATCH_COMPARISON_LEVELS] >= 3 )
{
  DepthMapFull( CALIBRATED_LEFT_EYE,
                CALIBRATED_RIGHT_EYE,
                DEPTH_LEFT,
                DEPTH_RIGHT,
                0
             );
}
  /*
    CONVERTING DEPTH DATA TO RGB VIDEO FORMAT ( FOR USER VIEWING )
   */
  DepthMapToVideo(DEPTH_LEFT,DEPTH_LEFT_VIDEO,1);
  if (settings[PASS_TO_WORLD_3D])
   {
       //(unsigned char *)
       SaveRegisterToFile("DEPTH0",DEPTH_LEFT_VIDEO);
       SaveRegisterToFile("COLOR0",CALIBRATED_LEFT_EYE);
   }

  VisCortx_OperationUnLockFrames();

}

unsigned int  VisCortx_Get_DepthMapData(unsigned int typeofdata,unsigned int px,unsigned int py)
{
  //if ((px>=settings[RESOLUTION_X])||(py>=settings[RESOLUTION_Y])) { return 0; }
  unsigned int mem_place=( px + py * metrics[RESOLUTION_X] );

  unsigned int retval = 0;
     switch ( typeofdata )
     {
     case 1 : retval= (unsigned int) l_video_register[DEPTH_LEFT].pixels[mem_place]; break;
	 case 2 : retval= (unsigned int) depth_data_array[mem_place].score; break;
	 case 3 : retval= (unsigned int) depth_data_array[mem_place].edge_count; break;
	 case 4 : retval= (unsigned int) depth_data_array[mem_place].x1_patch; break;
	 case 5 : retval= (unsigned int) depth_data_array[mem_place].y1_patch; break;
	 case 6 : retval= (unsigned int) depth_data_array[mem_place].x2_patch; break;
	 case 7 : retval= (unsigned int) depth_data_array[mem_place].y2_patch; break;
	 case 8 : retval= (unsigned int) depth_data_array[mem_place].patch_size_x; break;
	 case 9 : retval= (unsigned int) depth_data_array[mem_place].patch_size_y;  break;
     case 10 : retval= metrics[VERTICAL_BUFFER]; break;
	 case 11 : retval= metrics[HORIZONTAL_BUFFER]; break;
     case 12 : retval= mem_place; break;
     case 13 : retval= (unsigned int) depth_data_array[mem_place].movement_count; break;
     case 14 : retval= (unsigned int) depth_data_array[mem_place].movement_difference; break;
     };
  return retval;
}

unsigned int  VisCortx_GetPatchDescriptor(unsigned int vid_register,unsigned int x,unsigned int y,unsigned int patch_x,unsigned int patch_y,struct PatchSignature * result)
{
    return GetPatchSignature(video_register[vid_register].pixels,video_register[vid_register].size_x,video_register[vid_register].size_y,x,y,patch_x,patch_y,result);
}

/*
 ----------------- DEPTH MAPPING FUNCTIONS ----------------------
*/




 int VisCortx_ConvolutionFilter(unsigned int reg_in,unsigned int reg_out,signed char * table,signed int divisor,unsigned int table_size)
 {
    // assuming table_size 9
    //CONVOLUTION FILTER(9,6,-1,0,1,0,0,0,1,0,-1)
    //CONVOLUTION FILTER(9,1,-1,0,1,0,0,0,1,0,-1)
    //CONVOLUTION FILTER(9,1,1,1,1,1,5,1,1,1,1)
    /*Testing*/
    CopyRegister(reg_in,GENERAL_2);
    GaussianBlur(GENERAL_2,0);
    ConvertRegisterFrom3ByteTo1Byte(GENERAL_2);
    ConvolutionFilter9_1Byte(GENERAL_2,reg_out,table,divisor);
    ConvertRegisterFrom1ByteTo3Byte(reg_out);
    return 1;
 }





/*
 ----------------- MOVEMENT REGISTRATION ----------------------
*/
int VisCortx_Movement_Detection(unsigned int left_cam,unsigned int right_cam)
{
   if ( left_cam == 1 ) {
                          metrics[CHANGES_LEFT]=RegisterMovements(1,LAST_LEFT_EYE,LEFT_EYE,MOVEMENT_LEFT,MOVEMENT_GROUPED_LEFT);
                        }
   if (right_cam == 1 ) {
                          metrics[CHANGES_RIGHT]=RegisterMovements(0,LAST_RIGHT_EYE,RIGHT_EYE,MOVEMENT_RIGHT,MOVEMENT_GROUPED_RIGHT);
                        }
  return 1;
}
/*
 ----------------- MOVEMENT REGISTRATION ----------------------
*/

/*
 ----------------- FEATURE TRACKING ----------------------
*/
int VisCortxGetFundamentalMatrix(float * table,int size_of_table)
{
    return ComputeFundamentalMatrix();
}


unsigned int  VisCortx_GetTrackedPoints(unsigned int cam)
{
   if (cam==0) { GetFeatureData(video_register[LEFT_EYE].features,0,TOTAL_POINTS); } else
   if (cam==1) { GetFeatureData(video_register[RIGHT_EYE].features,0,TOTAL_POINTS); }
  return 0;
}

void  VisCortx_AddTrackPoint(unsigned int cam,unsigned int x,unsigned int y,unsigned int group)
{
  fprintf(stderr,"VisCortx_AddTrackPoint %u %u,%u : %u\n",cam,x,y,group);
   if (cam==0) { AddToFeatureList(video_register[LEFT_EYE].features,x,y,1); } else
   if (cam==1) { AddToFeatureList(video_register[RIGHT_EYE].features,x,y,1); }
}

void VisCortxClearTrackPoints(unsigned int cam)
{
   if (cam==0) { ClearFeatureList(video_register[LEFT_EYE].features); } else
   if (cam==1) { ClearFeatureList(video_register[RIGHT_EYE].features); }
}

void  VisCortx_AutoAddTrackPoints(unsigned int cam)
{
 if (cam==0)
  {
      CopyFeatureList(video_register[LEFT_EYE].features,video_register[LAST_LEFT_EYE].features);
      ExtractFeatures(LEFT_EYE,LAST_LEFT_OPERATION,200,0);
      //ExtractFeatures(100,video_register[EDGES_LEFT].pixels,video_register[GENERAL_1].pixels,metrics[RESOLUTION_X],metrics[RESOLUTION_Y],0);
  } else
  if (cam==1)
  {
      CopyFeatureList(video_register[RIGHT_EYE].features,video_register[LAST_RIGHT_EYE].features);
      ExtractFeatures(RIGHT_EYE,LAST_RIGHT_OPERATION,200,1);
  }

 fprintf(stderr,"VisCortx_AutoAddTrackPoints ok\n");
 return;
}


void  VisCortx_RemoveTimedoutTrackPoints(unsigned int vid_reg,unsigned int timeout)
{
   RemoveTrackPointsIfTimedOut(video_register[vid_reg].features,timeout);
}


unsigned int  VisCortx_GetFeature(unsigned int vid_reg,unsigned int point_num,unsigned int data_type)
{
  return GetFeatureData(video_register[vid_reg].features,point_num,data_type);
}


void  VisCortx_CopyTrackPoints(unsigned int from_vid_reg,unsigned int to_vid_reg)
{
  CopyFeatureList(video_register[from_vid_reg].features,video_register[to_vid_reg].features);
}



void  VisCortx_TrackPoints(unsigned int from_vid_reg,unsigned int to_vid_reg)
{
  unsigned int i=0;
  for (i=0; i<GetFeatureData(video_register[from_vid_reg].features,0,TOTAL_POINTS); i++)
   {
	   ExecuteTrackPoint(from_vid_reg,to_vid_reg,i);
   }
  VisCortx_CopyTrackPoints(from_vid_reg,to_vid_reg);
  video_register[to_vid_reg].features->last_track_time = TIME_INC;
}

void  VisCortx_DrawTrackPoints()
{
  fprintf(stderr,"Draw Track Points not used & implemented\n");
}

void  VisCortx_RenewTrackPoint(unsigned int vid_reg,unsigned int tpoint)
{
   RenewTrackPoints(video_register[vid_reg].features,tpoint);
}


void  VisCortx_RenewAllTrackPoints(unsigned int vid_reg)
{
  unsigned int i=0;
  for (i=0; i<GetFeatureData(video_register[vid_reg].features,0,TOTAL_POINTS); i++)
   {
	   RenewTrackPoints(video_register[vid_reg].features,i);
   }
}
/*
 ----------------- FEATURE TRACKING ----------------------
*/

/*
 ----------------- IMAGE PROCESSING ----------------------
*/

void KeepOnlyPixelsClosetoColor(unsigned char R,unsigned char G,unsigned char B,unsigned char howclose)
{
 //   VisCortX_BitBltVideoRegister(LEFT_EYE,GENERAL_1,0,0,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
    memcpy(video_register[GENERAL_1].pixels,video_register[LEFT_EYE].pixels,metrics[RESOLUTION_X]*metrics[RESOLUTION_Y]*3*sizeof(BYTE));
    KillDifferentPixels(video_register[GENERAL_1].pixels ,metrics[RESOLUTION_X],metrics[RESOLUTION_Y],R,G,B,howclose);
}

int SobelNDerivative(int n)
{
    return SobelNDegreeDerivative(n,LEFT_EYE,LAST_LEFT_OPERATION);
}

/*
 ----------------- FACE DETECTION ----------------------
*/
unsigned int VisCortx_RecognizeFaces(unsigned int cam)
{
   if ( cam == 0 ) { return RecognizeFaces(video_register[LEFT_EYE].pixels); } else
   if ( cam == 1 ) { return RecognizeFaces(video_register[RIGHT_EYE].pixels); }
   return 0;
}

void VisCortx_GetFaceNumber(char num,unsigned int *pos_x,unsigned int *pos_y,unsigned int *total_size)
{
    GetFaceNumber(num,pos_x,pos_y,total_size);
}

/*
 ----------------- LIDAR DEPTH DATA EMULATION ----------------------
*/

float VisCortx_MinCameraHorizontalAngle()
{
  return -camera_horizontal_field_of_view/2;
}

float VisCortx_MaxCameraHorizontalAngle()
{
  return camera_horizontal_field_of_view/2;
}

float VisCortx_CameraHorizontalAngleStep()
{
  if ( camera_horizontal_field_of_view == 0 ) { return 0; }
  return  metrics[RESOLUTION_X] / camera_horizontal_field_of_view;
}

unsigned short VisCortx_SetDepthScale(unsigned short depth_units,float centimeters)
{
  if ( depth_units<255 )
     {
       depth_units_in_cm[depth_units]=centimeters;
     }
  return 0;
}

float VisCortx_DepthUnitsToCM(unsigned short depth_units)
{
  if (depth_units<255)
   {
       return depth_units_in_cm[depth_units];
   }
  return 0.0;
}

unsigned short VisCortx_GetDepth_From_Angle(char num,float horizontal_angle,float vertical_angle)
{
  unsigned int uint_pixel_x, uint_pixel_y;

   if ( GetCameraCoords_From_Angle(horizontal_angle,vertical_angle,&uint_pixel_x,&uint_pixel_y) )
    {
         unsigned int Camera_Selected = DEPTH_LEFT;
         if ( num == 1 ) { Camera_Selected = DEPTH_RIGHT; }

         unsigned int ptr = uint_pixel_y * metrics[RESOLUTION_X] + uint_pixel_x;

         if ( metrics[RESOLUTION_MEMORY_LIMIT_1BYTE] <= ptr ) { return 0; }

         return l_video_register[Camera_Selected].pixels[ptr];
    }
    return 0;
}

