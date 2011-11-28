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
#include "VisCortexOptimizedConvolutionFilters.h"
#include "FeatureExtraction.h"
#include "PatternRecognition.h"
#include "FeatureTracking.h"
#include "FaceDetection.h"
#include "StateSetting.h"
#include "IntegralImageConversion.h"
#include "VisCortexTimer.h"
#include "LinearAlgebra.h"
#include "VideoInputAdapter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char * VISCORTEX_VER = "0.621";

/*

                 TODO LIST , BULETTIN BOARD
  TODO STUFF
   - Fix Syncing code between 2 frames coming via USB
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


void VisCortx_SetPipelineSwitch(unsigned int set_num,unsigned int set_val)
{
  pipeline_switches[set_num]=set_val;
}

unsigned int VisCortx_GetPipelineSwitch(unsigned int set_num)
{
  return pipeline_switches[set_num];
}


void VisCortx_GetHyperVisorStatus(unsigned int print_std,unsigned int print_file)
{
 if (print_std)
 {
  fprintf(stderr,"Visual Cortex HyperVisor status -=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
  fprintf(stderr,"PERFORMANCE , ALL TIMES ARE IN ! ! MICROSECONDS ! !\n");
  fprintf(stderr," MEMCPY TO REGISTER , AVERAGE %u , LAST %u , SAMPLES %u \n",GetAverageTimer(WRITE_REGISTER_DELAY),GetLastTimer(WRITE_REGISTER_DELAY),GetTimesTimerTimed(WRITE_REGISTER_DELAY));
  fprintf(stderr," CALIBRATION , AVERAGE %u , LAST %u , SAMPLES %u \n",GetAverageTimer(CALIBRATION_DELAY),GetLastTimer(CALIBRATION_DELAY),GetTimesTimerTimed(CALIBRATION_DELAY));
  fprintf(stderr," GAUSSIAN , AVERAGE %u , LAST %u , SAMPLES %u \n",GetAverageTimer(GAUSSIAN_DELAY),GetLastTimer(GAUSSIAN_DELAY),GetTimesTimerTimed(GAUSSIAN_DELAY));
  fprintf(stderr," SOBEL , AVERAGE %u , LAST %u , SAMPLES %u \n",GetAverageTimer(SOBEL_DELAY),GetLastTimer(SOBEL_DELAY),GetTimesTimerTimed(SOBEL_DELAY));
  fprintf(stderr," SECOND DERIV. , AVERAGE %u , LAST %u , SAMPLES %u \n",GetAverageTimer(SECOND_DERIVATIVE_DELAY),GetLastTimer(SECOND_DERIVATIVE_DELAY),GetTimesTimerTimed(SECOND_DERIVATIVE_DELAY));
  fprintf(stderr," PIXEL OV THR , AVERAGE %u , LAST %u , SAMPLES %u \n",GetAverageTimer(PIXEL_OVER_THRESHOLD_DELAY),GetLastTimer(PIXEL_OVER_THRESHOLD_DELAY),GetTimesTimerTimed(PIXEL_OVER_THRESHOLD_DELAY));
  fprintf(stderr," MOVEMENT RAW , AVERAGE %u , LAST %u , SAMPLES %u \n",GetAverageTimer(MOVEMENT_RAW_DELAY),GetLastTimer(MOVEMENT_RAW_DELAY),GetTimesTimerTimed(MOVEMENT_RAW_DELAY));
  fprintf(stderr," COMPRESS IMAGE , AVERAGE %u , LAST %u , SAMPLES %u \n",GetAverageTimer(COMPRESS_IMAGE_DELAY),GetLastTimer(COMPRESS_IMAGE_DELAY),GetTimesTimerTimed(COMPRESS_IMAGE_DELAY));
  fprintf(stderr," RECOGNIZE FACES , AVERAGE %u , LAST %u , SAMPLES %u \n",GetAverageTimer(RECOGNIZE_FACES_DELAY ),GetLastTimer(RECOGNIZE_FACES_DELAY ),GetTimesTimerTimed(RECOGNIZE_FACES_DELAY ));
  fprintf(stderr," FIND CORNERS , AVERAGE %u , LAST %u , SAMPLES %u \n",GetAverageTimer(FIND_CORNERS_DELAY),GetLastTimer(FIND_CORNERS_DELAY),GetTimesTimerTimed(FIND_CORNERS_DELAY));
  fprintf(stderr," TRACK CORNERS , AVERAGE %u , LAST %u , SAMPLES %u \n",GetAverageTimer(TRACK_CORNERS_DELAY),GetLastTimer(TRACK_CORNERS_DELAY),GetTimesTimerTimed(TRACK_CORNERS_DELAY));
  fprintf(stderr," DEPTH MAP , AVERAGE %u , LAST %u , SAMPLES %u \n",GetAverageTimer(TIMER_DEPTH_MAP_DELAY),GetLastTimer(TIMER_DEPTH_MAP_DELAY),GetTimesTimerTimed(TIMER_DEPTH_MAP_DELAY));
  fprintf(stderr," CAMERA POSE TR , AVERAGE %u , LAST %u , SAMPLES %u \n",GetAverageTimer(UPDATE_CAMERA_POSE_DELAY),GetLastTimer(UPDATE_CAMERA_POSE_DELAY),GetTimesTimerTimed(UPDATE_CAMERA_POSE_DELAY));
  fprintf(stderr,"     -=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
  fprintf(stderr,"PER FRAME , ALL TIMES ARE IN ! ! MILLISECONDS ! !\n");
 }

  float fps_average = GetAverageTimer(TIMER_PROCESSING_DELAY)/1000 , fps_last = GetLastTimer(TIMER_PROCESSING_DELAY)/1000;
  if ( fps_average != 0.0 ) { fps_average = 1000 / fps_average; }
  if ( fps_last != 0.0 )    { fps_last = 1000 / fps_last; }




 if (print_std)
 {
  fprintf(stderr," AVERAGE PROCESSING TIME PER FRAME %u ms - framerate %0.2f fps\n",  ( unsigned int ) GetAverageTimer(TIMER_PROCESSING_DELAY)/1000 , fps_average );
  fprintf(stderr," PROCESSING TIME FOR LAST FRAME %u ms - framerate %0.2f fps\n",  ( unsigned int ) GetLastTimer(TIMER_PROCESSING_DELAY)/1000 , fps_last );
  fprintf(stderr,"     -=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
 }

if ( (print_file) && (settings[HYPERVISOR_STORE_PERFORMANCE_STATISTICS]) )
  {
    UpdateStatistics(GetLastTimer(TIMER_PROCESSING_DELAY),fps_last);
  }
}

void VisCortx_SetMetric(unsigned int set_num,unsigned int set_val)
{
  metrics[set_num]=set_val;
}

unsigned int VisCortx_GetMetric(unsigned int get_num)
{
    return GetMetric(get_num);
}


unsigned int VisCortx_GetVideoRegisterData(unsigned int input_img_regnum,unsigned int metric_num)
{
   return video_register[input_img_regnum].time;
}



unsigned int VisCortx_VideoRegistersSynced(unsigned int img_regnum1,unsigned int img_regnum2)
{
   return ( video_register[img_regnum1].time == video_register[img_regnum2].time );
}

/*
 -----------------
                     INITIALIZATION
                     StateSetting.c/h
                                      ----------------------
*/
void VisCortx_CameraParameters(int right_cam,double fx,double fy,double cx,double cy,double k1,double k2,double p1,double p2,double k3)
{
   if ( right_cam == 0 )
        {

           // SEE http://en.wikipedia.org/wiki/Camera_resectioning#Intrinsic_parameters
           // and http://opencv.willowgarage.com/documentation/camera_calibration_and_3d_reconstruction.html

           if ( (CameraDistanceInMM != 0 ) && (fx != 0) )
           {
            left_calibration_data.CameraDistanceMultipliedByFocalLength = (double) CameraDistanceInMM/10;
            left_calibration_data.CameraDistanceMultipliedByFocalLength *= fx;
           }

           left_calibration_data.fx = fx ;
           left_calibration_data.fy = fy ;

           left_calibration_data.cx = cx ;
           left_calibration_data.cy = cy ;

           left_calibration_data.intrinsic_parameters_array[0]=fx;
           left_calibration_data.intrinsic_parameters_array[1]=0;
           left_calibration_data.intrinsic_parameters_array[2]=cx;

           left_calibration_data.intrinsic_parameters_array[3]=0;
           left_calibration_data.intrinsic_parameters_array[4]=fy;
           left_calibration_data.intrinsic_parameters_array[5]=cy;

           left_calibration_data.intrinsic_parameters_array[6]=0;
           left_calibration_data.intrinsic_parameters_array[7]=0;
           left_calibration_data.intrinsic_parameters_array[8]=1;

           left_calibration_data.k1 = k1 ;
           left_calibration_data.k2 = k2 ;
           left_calibration_data.p1 = p1 ;
           left_calibration_data.p2 = p2 ;
           left_calibration_data.k3 = k3 ;

           PrecalcResectioning(resection_left_precalc,fx,fy,cx,cy,k1,k2,p1,p2,k3);
        } else
   if ( right_cam == 1 )
        {
           // SEE http://en.wikipedia.org/wiki/Camera_resectioning#Intrinsic_parameters
           // and http://opencv.willowgarage.com/documentation/camera_calibration_and_3d_reconstruction.html

           if ( (CameraDistanceInMM != 0 ) && (fx != 0) )
           {
            right_calibration_data.CameraDistanceMultipliedByFocalLength = (double) CameraDistanceInMM/10;
            right_calibration_data.CameraDistanceMultipliedByFocalLength *= fx;
           }

           right_calibration_data.fx = fx ;
           right_calibration_data.fy = fy ;

           right_calibration_data.cx = cx ;
           right_calibration_data.cy = cy ;

           right_calibration_data.intrinsic_parameters_array[0]=fx;
           right_calibration_data.intrinsic_parameters_array[1]=0;
           right_calibration_data.intrinsic_parameters_array[2]=cx;

           right_calibration_data.intrinsic_parameters_array[3]=0;
           right_calibration_data.intrinsic_parameters_array[4]=fy;
           right_calibration_data.intrinsic_parameters_array[5]=cy;

           right_calibration_data.intrinsic_parameters_array[6]=0;
           right_calibration_data.intrinsic_parameters_array[7]=0;
           right_calibration_data.intrinsic_parameters_array[8]=1;

           right_calibration_data.k1 = k1 ;
           right_calibration_data.k2 = k2 ;
           right_calibration_data.p1 = p1 ;
           right_calibration_data.p2 = p2 ;
           right_calibration_data.k3 = k3 ;

           PrecalcResectioning(resection_right_precalc,fx,fy,cx,cy,k1,k2,p1,p2,k3);
        }
}



/*
 ----------------- VIDEO INPUT/OUTPUT ----------------------
*/

/*
 THIS FUNCTION IS CALLED EVERY TIME A NEW FRAME IS READY FOR STEREO VISION
*/
unsigned int VisCortX_NewFrame(unsigned int input_img_regnum,unsigned int size_x,unsigned int size_y,unsigned int depth,unsigned char * rgbdata)
{

    while ( !VisCortx_OperationLockFramesLeftRight() ) { fprintf(stderr,"*"); }

    //if ( wait_time>=max_wait_time  ) {  fprintf(stderr,"VisCortX_NewFrame timed out because of a locked frame\n"); return 0; }

    unsigned int retres = PassNewFrameFromVideoInput(input_img_regnum,size_x,size_y,depth,rgbdata);


    VisCortx_OperationUnLockFramesLeftRight();
    return retres;
}


unsigned int VisCortX_ClearVideoRegister(unsigned int input_img_regnum)
{
    return ClearVideoRegister(input_img_regnum);
}

unsigned int VisCortX_CopyVideoRegister(unsigned int input_img_regnum,unsigned int output_img_regnum,unsigned int copy_features,unsigned int copy_faces)
{
    return CopyRegister(input_img_regnum,output_img_regnum,copy_features,copy_faces);
}

unsigned int VisCortX_SwapVideoRegisters(unsigned int input_img_regnum,unsigned int output_img_regnum)
{
    return SwapRegister(input_img_regnum,output_img_regnum);
}

unsigned int VisCortX_ConvertVideoRegisterToColorDepth(unsigned int input_img_regnum,unsigned int new_color_depth)
{
  if ( video_register[input_img_regnum].depth == new_color_depth ) { return 1;}
  if ( ( video_register[input_img_regnum].depth == 1 )&& ( new_color_depth == 3 ) ) { ConvertRegisterFrom1ByteTo3Byte(input_img_regnum); return 1; } else
  if ( ( video_register[input_img_regnum].depth == 3 )&& ( new_color_depth == 1 ) ) { ConvertRegisterFrom3ByteTo1Byte(input_img_regnum); return 1; } else
                                                                          {  fprintf(stderr,"Don`t know what to do for conversion of register %u from color depth %u to %u\n",input_img_regnum,video_register[input_img_regnum].depth,new_color_depth); }

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

    StartTimer( WRITE_REGISTER_DELAY );
     memcpy(video_register[reg_num].pixels,rgbdata,mem_end);
	EndTimer( WRITE_REGISTER_DELAY );

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


int VisCortx_OperationLockFramesLeftRight()
{
 int waiting_counter = 0 , max_waiting = 10000;
 while ( ( video_register[LEFT_EYE].lock ) && ( waiting_counter < max_waiting ) )    { ++waiting_counter; usleep(1); }
 if ( !video_register[LEFT_EYE].lock )
   {
     video_register[LEFT_EYE].lock=1;
   }


 waiting_counter = 0;
 while ( ( video_register[RIGHT_EYE].lock ) && ( waiting_counter < max_waiting ) )    { ++waiting_counter; usleep(1); }

 if ( !video_register[RIGHT_EYE].lock )
   {
     video_register[RIGHT_EYE].lock=1;
   }

 return 1;
}

int VisCortx_OperationUnLockFramesLeftRight()
{
     video_register[LEFT_EYE].lock=0;
     video_register[RIGHT_EYE].lock=0;
     return 1;
}

int VisCortx_CheckTheFramesLeftRightForLock()
{
  if ( (video_register[LEFT_EYE].lock ) || ( video_register[RIGHT_EYE].lock ) )
    {
         return 1;
    }
  return 0;
}
/*
 ----------------- DEPTH MAPPING FUNCTIONS ----------------------
*/

void ExecutePipeline()
{
    Pipeline_Stereo_Frames_Collected_Actions();
}


void  VisCortx_FullDepthMap(unsigned int max_milliseconds)
{

   VisCortx_SetPipelineSwitch(EXECUTE_DEPTHMAP,1);

  unsigned int sleepy_time = 10 , wait_time = 0, max_wait_time = max_milliseconds /*milliseconds*/  / sleepy_time; // 2 sec max wait time , after this we declare the effort failed :P
  while
     ( ( VisCortx_GetPipelineSwitch(EXECUTE_DEPTHMAP) != 0  ) && ( wait_time<max_wait_time ) )
    {
              //Here we wait for the InputAdapter to process request and then set the switch back to 0..
              VisCortxMillisecondsSleep(sleepy_time);
              ++wait_time;
    }
   if ( wait_time >= max_wait_time ) { fprintf(stderr,"Timed out waiting for VisCortx_FullDepthMap() to complete..\n"); }
   return;
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
     case 15 : retval= (unsigned int) depth_data_array[mem_place].choices_rejected; break;
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



void VisCorteX_DisparityMapAutoCalibrate(unsigned int max_vertical_error)
{
   //TEST FUNCTION ######################################################################
   fprintf(stderr,"For now this only works as an heuristic to vertically calibrate LEFT_EYE and RIGHT_EYE\n");
   fprintf(stderr,"it basically scans for the vertical shift that takes the fewest comparisons to compute..\n");

   if ( max_vertical_error == 0 ) { return ; }
   unsigned int up_deviation=max_vertical_error , down_deviation=0;
   unsigned int up_deviation_preset=settings[DEPTHMAP_VERT_SHIFT_UP];
   unsigned int down_deviation_preset=settings[DEPTHMAP_VERT_SHIFT_DOWN];

   fprintf(stderr,"Vertical Calibration was %u-%u ",settings[DEPTHMAP_VERT_SHIFT_UP],settings[DEPTHMAP_VERT_SHIFT_DOWN]);


   unsigned int best_result = metrics[RESOLUTION_X]*metrics[RESOLUTION_X]*metrics[RESOLUTION_Y]+1;
   signed int vertical_shift = 0;

   settings[DEPTHMAP_VERT_SHIFT_DOWN]=0;
   while ( up_deviation>0 )
     {
         settings[DEPTHMAP_VERT_SHIFT_UP]=up_deviation;
         VisCortx_FullDepthMap(3000);
         if ( metrics[COMPAREPATCH_TOTAL_CALLS]<best_result )
            {
                best_result=metrics[COMPAREPATCH_TOTAL_CALLS];
                vertical_shift = up_deviation;
            }
         --up_deviation;
     }


   settings[DEPTHMAP_VERT_SHIFT_UP]=0;
   down_deviation=0;
   while ( down_deviation<max_vertical_error )
     {
         settings[DEPTHMAP_VERT_SHIFT_DOWN]=down_deviation;
         VisCortx_FullDepthMap(3000);
         if ( metrics[COMPAREPATCH_TOTAL_CALLS]<best_result )
            {
                best_result=metrics[COMPAREPATCH_TOTAL_CALLS];
                vertical_shift = (-1) * down_deviation;
            }
         ++down_deviation;
     }


   if ( best_result == 0 )    {
                                settings[DEPTHMAP_VERT_SHIFT_UP]=up_deviation_preset;
                                settings[DEPTHMAP_VERT_SHIFT_DOWN]=down_deviation_preset;
                              } else
   if ( vertical_shift >= 0 ) {
                                settings[DEPTHMAP_VERT_SHIFT_UP] = vertical_shift;
                                settings[DEPTHMAP_VERT_SHIFT_DOWN] = 0;
                              } else
   if ( vertical_shift < 0 )  {
                                settings[DEPTHMAP_VERT_SHIFT_DOWN] = (-1)*vertical_shift;
                                settings[DEPTHMAP_VERT_SHIFT_UP] = 0;
                              }
   fprintf(stderr," and is now fixed to %u-%u \n",settings[DEPTHMAP_VERT_SHIFT_UP],settings[DEPTHMAP_VERT_SHIFT_DOWN]);
   //TEST FUNCTION ######################################################################
}

 int VisCortx_ConvolutionFilter(unsigned int reg_in,unsigned int reg_out,signed char * table,signed int divisor,unsigned int table_size)
 {
    // assuming table_size 9
    //CONVOLUTION FILTER(9,6,-1,0,1,0,0,0,1,0,-1)
    //CONVOLUTION FILTER(9,1,-1,0,1,0,0,0,1,0,-1)
    //CONVOLUTION FILTER(9,1,1,1,1,1,5,1,1,1,1)
    /*Testing*/
     unsigned int TMP_REGISTER = GetTempRegister();
     if (TMP_REGISTER == 0 ) { fprintf(stderr," Error Getting a temporary Video Register ( VisCortx_ConvolutionFilter )\n"); }

    CopyRegister(reg_in,TMP_REGISTER,0,0);
    GaussianBlur(TMP_REGISTER,0);
    ConvertRegisterFrom3ByteTo1Byte(TMP_REGISTER);
    ConvolutionFilter9_1ByteOptimized(TMP_REGISTER,reg_out,table,divisor);

    StopUsingVideoRegister(TMP_REGISTER);

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

int VisCortx_Movement_At_Rectangle(unsigned int reg_num , unsigned int x, unsigned int y , unsigned int width , unsigned int height )
{
    if (  (reg_num==LEFT_EYE) || (reg_num==CALIBRATED_LEFT_EYE) ) {  return GetCompressedRegisterPatchSum(MOVEMENT_GROUPED_LEFT,x,y,width,height);  } else
    if (  (reg_num==RIGHT_EYE) || (reg_num==CALIBRATED_RIGHT_EYE) ) {  return GetCompressedRegisterPatchSum(MOVEMENT_GROUPED_RIGHT,x,y,width,height);  } else
    {
        fprintf(stderr,"VisCortx_Movement_At_Rectangle called with unknown register ( returning zero ) \n");
    }

    return 0;
}


int VisCortx_Movement_At_Edge(unsigned int reg_num , unsigned int edge_enum)
{
    unsigned int dim_length = 0;

    switch ( edge_enum )
     {
         case DIRECTION_UP  :
             dim_length =  metrics[RESOLUTION_Y] / 4 ;
             return VisCortx_Movement_At_Rectangle(reg_num,0,0,metrics[RESOLUTION_X],dim_length);
         break;
         case DIRECTION_DOWN  :
             dim_length =  metrics[RESOLUTION_Y] / 4 ;
             return VisCortx_Movement_At_Rectangle(reg_num,0,metrics[RESOLUTION_Y]-dim_length-1,metrics[RESOLUTION_X],dim_length);
         break;
         case DIRECTION_LEFT  :
             dim_length = metrics[RESOLUTION_X] / 6 ;
             return VisCortx_Movement_At_Rectangle(reg_num,0,0,dim_length,metrics[RESOLUTION_Y]);
         break;
         case DIRECTION_RIGHT  :
             dim_length = metrics[RESOLUTION_X] / 6 ;
             return VisCortx_Movement_At_Rectangle(reg_num,metrics[RESOLUTION_X]-dim_length-1,0,dim_length,metrics[RESOLUTION_Y]);
         break;

         default :
         break;
     };


    return 0;
}

/*
 ----------------- MOVEMENT REGISTRATION ----------------------
*/

/*
 ----------------- FEATURE TRACKING ----------------------
*/
int VisCortxGetFundamentalMatrix(float * table,int size_of_table)
{
   return 0;
   // return ComputeFundamentalMatrix();
}


unsigned int  VisCortx_GetTrackedPoints(unsigned int cam)
{
   if (cam==0) { GetFeatureData(video_register[CALIBRATED_LEFT_EYE].features,0,TOTAL_POINTS); } else
   if (cam==1) { GetFeatureData(video_register[CALIBRATED_RIGHT_EYE].features,0,TOTAL_POINTS); }
  return 0;
}

void  VisCortx_AddTrackPoint(unsigned int cam,unsigned int x,unsigned int y,unsigned int group)
{
  fprintf(stderr,"VisCortx_AddTrackPoint %u %u,%u : %u\n",cam,x,y,group);
   if (cam==0) { AddToFeatureList(video_register[CALIBRATED_LEFT_EYE].features,x,y,1,0,0,0); } else
   if (cam==1) { AddToFeatureList(video_register[CALIBRATED_RIGHT_EYE].features,x,y,1,0,0,0); }
}

void VisCortx_ClearTrackPoints(unsigned int cam)
{
   if (cam==0)
     {
       ClearFeatureList(video_register[CALIBRATED_LEFT_EYE].features); // This is the main carrier of features
       ClearFeatureList(video_register[LEFT_EYE].features);
     } else
   if (cam==1)
     {
       ClearFeatureList(video_register[CALIBRATED_RIGHT_EYE].features); // This is the main carrier of features
       ClearFeatureList(video_register[RIGHT_EYE].features);
     }
}

void  VisCortx_AutoAddTrackPoints(unsigned int cam)
{
 if (cam==0)
  {
      CopyFeatureList(video_register[CALIBRATED_LEFT_EYE].features,video_register[LAST_LEFT_EYE].features);
      ExtractFeatures(CALIBRATED_LEFT_EYE,EDGES_LEFT,SECOND_DERIVATIVE_LEFT,0);
  } else
  if (cam==1)
  {
      CopyFeatureList(video_register[CALIBRATED_RIGHT_EYE].features,video_register[LAST_RIGHT_EYE].features);
      ExtractFeatures(CALIBRATED_RIGHT_EYE,EDGES_RIGHT,SECOND_DERIVATIVE_RIGHT,1);
  }
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
  //fprintf(stderr,"VisCortx_TrackPoints apparently segfaults , so it is disabled (for now)\n");
  //TODO


  FindAndTrackAllPointsOnRegistersOpenCV(to_vid_reg,from_vid_reg,1200);
  return;

  unsigned int i=0;
  for (i=0; i<GetFeatureData(video_register[from_vid_reg].features,0,TOTAL_POINTS); i++)
   {
	   ExecuteTrackPointBrute(from_vid_reg,to_vid_reg,i);
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
void KeepOnlyPixelsClosetoColor(unsigned int src_reg, unsigned int target_reg , unsigned char R,unsigned char G,unsigned char B,unsigned char howclose)
{
    memcpy(video_register[target_reg].pixels,video_register[src_reg].pixels,metrics[RESOLUTION_X]*metrics[RESOLUTION_Y]*3*sizeof(BYTE));
    KillDifferentPixels(video_register[target_reg].pixels ,metrics[RESOLUTION_X],metrics[RESOLUTION_Y],R,G,B,howclose);
}

int SobelNDerivative(int n)
{
    if ( n == 1 ) {
                    SobelFromSource(CALIBRATED_LEFT_EYE,LAST_LEFT_OPERATION);
                    ConvertRegisterFrom1ByteTo3Byte(LAST_LEFT_OPERATION);
                    SobelFromSource(CALIBRATED_RIGHT_EYE,LAST_RIGHT_OPERATION);
                    ConvertRegisterFrom1ByteTo3Byte(LAST_RIGHT_OPERATION);
                  } else
    if ( n == 2 ) {
                    CopyRegister(CALIBRATED_LEFT_EYE,GENERAL_3,0,0);
                    ConvertRegisterFrom3ByteTo1Byte(GENERAL_3);
                    SecondDerivativeIntensitiesFromSource(GENERAL_3,LAST_LEFT_OPERATION);
                    ConvertRegisterFrom1ByteTo3Byte(LAST_LEFT_OPERATION);

                    CopyRegister(CALIBRATED_RIGHT_EYE,GENERAL_3,0,0);
                    ConvertRegisterFrom3ByteTo1Byte(GENERAL_3);
                    SecondDerivativeIntensitiesFromSource(GENERAL_3,LAST_RIGHT_OPERATION);
                    ConvertRegisterFrom1ByteTo3Byte(LAST_RIGHT_OPERATION);
                  } else
                  {
                      fprintf(stderr,"Higher order derivative not implemented\n");
                      return 0;
                  }
    return 1;
}

/*
 ----------------- FACE DETECTION ----------------------
*/
unsigned int VisCortx_RecognizeFaces(unsigned int cam)
{
   if ( cam == 0 ) { return RecognizeFaces(CALIBRATED_LEFT_EYE); } else
   if ( cam == 1 ) { return RecognizeFaces(CALIBRATED_RIGHT_EYE); }
   return 0;
}


unsigned int  VisCortx_GetFaces(unsigned int vid_reg,unsigned int point_num,unsigned int data_type)
{
  return GetFeatureData(video_register[vid_reg].faces,point_num,data_type);
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
  if ( depth_units<255 ) { depth_units_in_cm[depth_units]=centimeters; }
  return 0;
}

float VisCortx_DepthUnitsToCM(unsigned short depth_units)
{
  if (depth_units<255) { return depth_units_in_cm[depth_units]; }
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

