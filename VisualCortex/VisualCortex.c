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
#include "DisparityDepthMap.h"
#include "MovementRegistration.h"
#include "VisCortexFilters.h"
#include "FeatureExtraction.h"
#include "PatternRecognition.h"
#include "FeatureTracking.h"
#include "FaceDetection.h"
#include "StateSetting.h"
#include "LinearAlgebra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * VISCORTEX_VER = "0.53";


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



unsigned int VisCortx_GetMetric(unsigned int get_num)
{
    return GetMetric(get_num);
}


unsigned int VisCortx_GetVideoRegisterStats(unsigned int metric_num)
{
   return GetVideoRegisterStats(metric_num);
}


void VisCortx_SetTime(unsigned int thetime)
{
     SetTime(thetime);
}

unsigned int VisCortx_GetTime()
{
    return GetTime();
}

/*
 -----------------
                     INITIALIZATION
                     StateSetting.c/h
                                      ----------------------
*/



/*
 ----------------- VIDEO INPUT/OUTPUT ----------------------
*/
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
	if (  !VideoRegisterRequestIsOk(reg_num,size_x,size_y,depth) ) { return 1; }

    unsigned long syst_mem_end=metrics[RESOLUTION_X]*metrics[RESOLUTION_Y]*3;
    unsigned long pic_mem_end=size_x*size_y*depth;

    unsigned long mem_end=syst_mem_end;
    if ( pic_mem_end > mem_end ) { fprintf(stderr,"Register is not big enough to accomodate data! \n "); return 1; } else
    if ( pic_mem_end < mem_end ) { mem_end = pic_mem_end; }


    memcpy(video_register[reg_num].pixels,rgbdata,mem_end);

	return 0;
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


/*
 ----------------- DEPTH MAPPING FUNCTIONS ----------------------
*/
void  VisCortx_FullDepthMap()
{
  unsigned int edgepercent=settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED],patch_x=metrics[HORIZONTAL_BUFFER],patch_y=metrics[VERTICAL_BUFFER];
  unsigned int threshold=settings[DEPTHMAP_COMPARISON_THRESHOLD];
  unsigned int threshold_added=settings[DEPTHMAP_COMPARISON_THRESHOLD_ADDED];

  if ( threshold != 0 )
   {
    settings[DEPTHMAP_COMPARISON_THRESHOLD_LARGE_PATCH]=  (unsigned int) ( (threshold * metrics[VERTICAL_BUFFER_LARGE] * metrics[HORIZONTAL_BUFFER_LARGE] ) / (metrics[HORIZONTAL_BUFFER]*metrics[VERTICAL_BUFFER]) ); //16000;
    settings[DEPTHMAP_COMPARISON_THRESHOLD_EXTRALARGE_PATCH]=  (unsigned int) ( (threshold * metrics[VERTICAL_BUFFER_EXTRALARGE] * metrics[HORIZONTAL_BUFFER_EXTRALARGE] ) / (metrics[HORIZONTAL_BUFFER]*metrics[VERTICAL_BUFFER]) ); //16000;
   }
  /*
     WE COMPARE PATCHES ON 3 DIFFERENT LEVELS , EXTRA LARGE PATCHES , LARGE PATCHES , NORMAL PATCHES
   */

  /*
    CALCULATION OF EXTRA LARGE PATCHES FOLLOWS
   */

  settings[DEPTHMAP_COMPARISON_THRESHOLD]=settings[DEPTHMAP_COMPARISON_THRESHOLD_EXTRALARGE_PATCH];
  settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED]=settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_EXTRALARGE_PATCH];
  metrics[VERTICAL_BUFFER]=metrics[VERTICAL_BUFFER_EXTRALARGE];
  metrics[HORIZONTAL_BUFFER]=metrics[HORIZONTAL_BUFFER_EXTRALARGE];

  DepthMapFull( LEFT_EYE,
                RIGHT_EYE,
                DEPTH_LEFT,
                DEPTH_RIGHT,
                metrics[RESOLUTION_X],
                metrics[RESOLUTION_Y],
                1
             );

  /*
    CALCULATION OF LARGE PATCHES FOLLOWS
   */

if ( settings[PATCH_COMPARISON_LEVELS] >= 2 )
{
  settings[DEPTHMAP_COMPARISON_THRESHOLD]=settings[DEPTHMAP_COMPARISON_THRESHOLD_LARGE_PATCH];
  settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED]=settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_LARGE_PATCH];
  metrics[VERTICAL_BUFFER]=metrics[VERTICAL_BUFFER_LARGE];
  metrics[HORIZONTAL_BUFFER]=metrics[HORIZONTAL_BUFFER_LARGE];

  DepthMapFull( LEFT_EYE,
                RIGHT_EYE,
                DEPTH_LEFT,
                DEPTH_RIGHT,
                metrics[RESOLUTION_X],
                metrics[RESOLUTION_Y],
                0
             );


}


  /*
    CALCULATION OF NORMAL PATCHES FOLLOWS
   */
   settings[DEPTHMAP_COMPARISON_THRESHOLD_ADDED]=threshold_added;
   settings[DEPTHMAP_COMPARISON_THRESHOLD]=threshold;
   settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED]=edgepercent;
   metrics[VERTICAL_BUFFER]=patch_y;
   metrics[HORIZONTAL_BUFFER]=patch_x;
   /* THESE 3 LINES ARE DELIBERATELY OUT OF THE IF CONTROL BECAUSE THESE VALUES ARE DEFAULT*/
if ( settings[PATCH_COMPARISON_LEVELS] >= 3 )
{
  DepthMapFull( LEFT_EYE,
                RIGHT_EYE,
                DEPTH_LEFT,
                DEPTH_RIGHT,
                metrics[RESOLUTION_X],
                metrics[RESOLUTION_Y],
                0
             );
}
  /*
    CONVERTING DEPTH DATA TO RGB VIDEO FORMAT ( FOR USER VIEWING )
   */
  DepthMapToVideo(DEPTH_LEFT,DEPTH_LEFT_VIDEO);

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
unsigned int  VisCortx_GetTrackedPoints()
{
  return GetPointTrackList();
}

void  VisCortx_AddTrackPoint(unsigned int cam,unsigned int x,unsigned int y,unsigned int group)
{
  fprintf(stderr,"VisCortx_AddTrackPoint %u %u,%u : %u\n",cam,x,y,group);
  AddPointToTrackList(cam,x-1,y-1,group);
}

void  VisCortx_AutoAddTrackPoints(unsigned int cam)
{
 if (cam==0)
  {
      PrepareCleanSobeledGaussian(video_register[LEFT_EYE].pixels,video_register[EDGES_LEFT].pixels,settings[DEPTHMAP_EDGE_STRICTNESS]);
      ClearVideoRegister(GENERAL_1);
      ClearTrackPoints();
      ExtractFeatures(100,video_register[EDGES_LEFT].pixels,video_register[GENERAL_1].pixels,metrics[RESOLUTION_X],metrics[RESOLUTION_Y],0);
  }
  //fprintf(stderr,"VisCortx_AutoAddingTrackPoint %u %u,%u : %u\n",cam,x,y,group);
 //
 fprintf(stderr,"VisCortx_AutoAddTrackPoints ok\n");
 return;
}


void  VisCortx_RemoveTimedoutTrackPoints(unsigned int timeout)
{
   RemoveTrackPointIfTimedOut(timeout);
}

unsigned int  VisCortx_GetTrackPoint(unsigned int dat,unsigned int trackpoint)
{

  char cam;
  unsigned int x,y;
  if (dat<4) { GetTrackPoint(trackpoint,1,&cam,&x,&y); } else
             { GetTrackPoint(trackpoint,2,&cam,&x,&y); }
  switch (dat)
  {
   case 1: { return x; break; }
   case 2: { return y; break; }
   case 3: { return cam; break; }
   case 4: { return x; break; }
   case 5: { return y; break; }
   case 6: { return cam; break; }
   case 7: { return GetTrackData(trackpoint,1); break; }
   case 8: { return GetTrackData(trackpoint,2); break; }
   case 9: { return GetTrackData(trackpoint,3); break; }
   default : return 0;
  };
 return 0;
}


void  VisCortx_TrackPoints()
{
  unsigned int i=0;
  for (i=0; i<GetPointTrackList(); i++)
   {
	   ExecuteTrackPoint(i);
   }
}

void  VisCortx_DrawTrackPoints()
{ fprintf(stderr,"Draw Track Points not implemented ok\n");

  unsigned int i=0;
  for (i=0; i<GetPointTrackList(); i++)
   {
	 BitBltTrackPointMemToRegister(i,LAST_LEFT_OPERATION,i*30,0);
   }
}

void  VisCortx_RenewTrackPoint(unsigned int tpoint)
{
  ApplyTrackPositionAsOriginal(tpoint);
}


void  VisCortx_RenewAllTrackPoints()
{
  if ( GetPointTrackList() == 0 ) { return; }

  fprintf(stderr,"TODO : Add different levels of gaussian blur to track points! :P\n");
  unsigned int i=0;
  for (i=0; i<GetPointTrackList(); i++)
   {
	   ApplyTrackPositionAsOriginal(i);
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
    return SobelNDegreeDerivative(n,video_register[LEFT_EYE].pixels,video_register[LAST_LEFT_OPERATION].pixels,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
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

