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

#include "DisparityDepthMap.h"
#include "DisparityDepthMap_Heuristics.h"
#include "VisCortexFilters.h"
#include "IntegralImageConversion.h"
#include "MovementRegistration.h"
#include "Precalculations.h"
#include "PatchComparison.h"
#include "VisCortexTimer.h"
#include "cv.h"

#include <stdio.h>
#include <string.h>

typedef unsigned int uint;


int PrepareForDepthMapping(
                           uint left_depth_reg,
                           uint right_depth_reg,
                           unsigned char clear_and_calculate)
{
   if ( clear_and_calculate == 1 )
    {
       memset(depth_data_array,0,sizeof(struct DepthData) * metrics[RESOLUTION_X] * metrics[RESOLUTION_Y] );// CLEAR DEPTH MAP FROM ARTIFACTS
       ClearLargeVideoRegister(left_depth_reg);
       ClearLargeVideoRegister(right_depth_reg);

      // THIS IS DONE FOR EVERY FRAME NOW PrepareCleanSobeledGaussianAndDerivative(LEFT_EYE,EDGES_LEFT,SECOND_DERIVATIVE_LEFT,settings[DEPTHMAP_EDGE_LOW_STRICTNESS],settings[DEPTHMAP_EDGE_HIGH_STRICTNESS]);
      // THIS IS DONE FOR EVERY FRAME NOW PrepareCleanSobeledGaussianAndDerivative(RIGHT_EYE,EDGES_RIGHT,SECOND_DERIVATIVE_RIGHT,settings[DEPTHMAP_EDGE_LOW_STRICTNESS],settings[DEPTHMAP_EDGE_HIGH_STRICTNESS]);
    }

   if (  settings[DEPTHMAP_IMPROVE_USING_HISTOGRAM] == 1 )
   {
       /*
       GenerateCompressHistogramOfImage(video_register[CALIBRATED_LEFT_EYE].pixels,l_video_register[HISTOGRAM_COMPRESSED_LEFT].pixels,metrics[HORIZONTAL_BUFFER],metrics[VERTICAL_BUFFER]);
       GenerateCompressHistogramOfImage(video_register[CALIBRATED_RIGHT_EYE].pixels,l_video_register[HISTOGRAM_COMPRESSED_RIGHT].pixels,metrics[HORIZONTAL_BUFFER],metrics[VERTICAL_BUFFER]);
       */
       /* TODO ADD GENERIC HISTOGRAM COMPRESSION  HERE .. ( 3 byte )*/
   }


    if ( clear_and_calculate == 1 )
     {
       //CompressPresenceRegister(EDGES_LEFT,GENERAL_XLARGE_1,5); // This must be executed
        // I dont like the following , I should really change the sobel function to give a one byte response..!

        /* This is now calculated in every frame
        CopyRegister(EDGES_LEFT,GENERAL_3);
        PixelsOverThresholdSetAsOne(GENERAL_3,1);
        CompressRegister(GENERAL_3,GENERAL_XLARGE_1);

        CompressRegister(MOVEMENT_LEFT,MOVEMENT_GROUPED_LEFT);
        CompressRegister(MOVEMENT_RIGHT,MOVEMENT_GROUPED_RIGHT);

        CompressRegister(EDGES_LEFT,EDGES_GROUPED_LEFT);
        CompressRegister(EDGES_RIGHT,EDGES_GROUPED_RIGHT);

        CompressRegister(SECOND_DERIVATIVE_LEFT,SECOND_DERIVATIVE_GROUPED_LEFT);
        CompressRegister(SECOND_DERIVATIVE_RIGHT,SECOND_DERIVATIVE_GROUPED_RIGHT);
        */

     }
  return 1;
}



void inline FillDepthMemWithData(unsigned short * depth_data_raw_left,unsigned short * depth_data_raw_right,struct DepthData * depth_data_full,struct DepthData *depth_data,unsigned int image_x,unsigned int image_y)
{

	 unsigned short far_away=0;
     if (depth_data->x1_patch-depth_data->x2_patch>0) { far_away=depth_data->x1_patch-depth_data->x2_patch; } else
                                                      { far_away=depth_data->x2_patch-depth_data->x1_patch; }

	 unsigned int x=0;
	 unsigned int y=0;
	 unsigned int xlim=0 , ylim=0 , full_lim=image_x*image_y;


	y=depth_data->y1_patch;
	ylim=y+depth_data->patch_size_y;
	while (y<ylim)
	{
		x = ( (y) * image_x ) + ( depth_data->x1_patch );
	    xlim=x+depth_data->patch_size_x;

		while ( ( x<xlim ) && ( x < full_lim) )
		{
		    depth_data_raw_left[x]=far_away;
		    // TODO depth_data_raw_right
			depth_data_full[x].depth=depth_data->depth;
            depth_data_full[x].score=depth_data->score;
            depth_data_full[x].edge_count=depth_data->edge_count;
            depth_data_full[x].movement_count=depth_data->movement_count;
            depth_data_full[x].movement_difference=depth_data->movement_difference;
            depth_data_full[x].x1_patch=depth_data->x1_patch;
            depth_data_full[x].y1_patch=depth_data->y1_patch;
            depth_data_full[x].x2_patch=depth_data->x2_patch;
            depth_data_full[x].y2_patch=depth_data->y2_patch;
		    depth_data_full[x].patch_size_x=depth_data->patch_size_x;
            depth_data_full[x].patch_size_y=depth_data->patch_size_y;
			++x;
		}
	  ++y;
	}


}


void PassGuessNextDepthMem(unsigned int prox,unsigned int patch_x,unsigned int patch_y,unsigned short * depth_data_raw_left,unsigned short * depth_data_raw_right,struct DepthData * depth_data_full,struct DepthData *depth_data,unsigned int image_x,unsigned int image_y)
{

		//if (depth_data.x1_patch>image_x) { depth_data.x1_patch=image_x; }
	    //if (depth_data.x2_patch>image_x) { depth_data.x2_patch=image_x; }
		//if (depth_data.x2_patch<=depth_data.x1_patch) {depth_data.x1_patch=depth_data.x2_patch-1;}
        depth_data->score=prox;
        FillDepthMemWithData(depth_data_raw_left,depth_data_raw_right,depth_data_full,depth_data,image_x,image_y);
}





inline void MatchInHorizontalScanline(unsigned char *rgb1,unsigned char *rgb2,
                                      struct ImageRegion * left_rgn,
                                      struct DepthData *best_match,
                                      unsigned char * has_match
                                     )
{

  *has_match=0;
  uint max_prox_score = settings[DEPTHMAP_COMPARISON_THRESHOLD]+settings[DEPTHMAP_COMPARISON_THRESHOLD_ADDED];
  best_match->score = settings[DEPTHMAP_COMPARISON_THRESHOLD]+1;

  struct ImageRegion right_rgn={0};

  uint xr_start = 0;
  uint xr_lim=left_rgn->x1;

  if ( ( left_rgn->x1 > settings[DEPTHMAP_CLOSEST_DEPTH] ) &&
       (settings[DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_CLOSEST_DEPTH])
     )
      { xr_start = left_rgn->x1 - settings[DEPTHMAP_CLOSEST_DEPTH]; }


                                                                 /*SHIFTING LEFT IMAGE UP OR DOWN  TO MAKE UP FOR WRONG PHYSICAL CALIBRATION*/
  uint yr_lim=left_rgn->y1+settings[DEPTHMAP_VERT_OFFSET_DOWN] + settings[DEPTHMAP_VERT_SHIFT_DOWN];
  uint prox=0;
                                                                /*SHIFTING LEFT IMAGE UP OR DOWN  TO MAKE UP FOR WRONG PHYSICAL CALIBRATION*/
  right_rgn.y1=left_rgn->y1-settings[DEPTHMAP_VERT_OFFSET_UP] - settings[DEPTHMAP_VERT_SHIFT_UP];
  right_rgn.width=metrics[HORIZONTAL_BUFFER];
  right_rgn.height=metrics[VERTICAL_BUFFER];




  uint PREVIOUS_PATCH_SIZE_DISTANCE = 10;
  if ( settings[DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_PREVIOUS_PATCH_SIZE_DEPTH] )
  {
      //TODO : THIS COULD BE IMPROVED
     uint depth_at_point = VisCortx_Get_DepthMapData(1,left_rgn->x1,left_rgn->y1);
      if ( depth_at_point > 1 )
       {
         if (left_rgn->x1-depth_at_point-PREVIOUS_PATCH_SIZE_DISTANCE>0)
         {
            xr_start=left_rgn->x1-depth_at_point-PREVIOUS_PATCH_SIZE_DISTANCE;
         }
       }
  }


      while (right_rgn.y1 <= yr_lim)
       {
         right_rgn.x1=xr_lim;
         while (right_rgn.x1 > xr_start)
         {

								prox = ComparePatches( left_rgn , &right_rgn,
                                                       rgb1,rgb2,
                                                       video_register[EDGES_LEFT].pixels , video_register[EDGES_RIGHT].pixels ,
                                                       video_register[SECOND_DERIVATIVE_LEFT].pixels  , video_register[SECOND_DERIVATIVE_RIGHT].pixels ,
                                                       video_register[MOVEMENT_LEFT].pixels  , video_register[MOVEMENT_RIGHT].pixels ,
                                                       metrics[HORIZONTAL_BUFFER] , metrics[VERTICAL_BUFFER],
                                                       metrics[RESOLUTION_X] , metrics[RESOLUTION_Y] ,
                                                       best_match->score);

                                // DEPTH ESTIMATION ---------------------------------------
								 uint depth_act;
								 if ( left_rgn->x1 > right_rgn.x1 ) { depth_act=left_rgn->x1 - right_rgn.x1; } else
                                                                    { depth_act=right_rgn.x1 - left_rgn->x1; }
								// DEPTH ESTIMATION ---------------------------------------


                                if  (
                                         (prox < best_match->score) // kanoume qualify san kalytero apotelesma
                                      && (prox < max_prox_score) // to threshold mas
									  && (depth_act<settings[DEPTHMAP_CLOSEST_DEPTH])  // CULLING -> Praktika dedomena deixnoun oti synithws apotelesmata panw apo 100 einai thoryvos!
								    )
								{
                                    /* THIS IS THE BEST PATCH SO FAR! */
								    best_match->score=prox;
                                    best_match->depth=depth_act;
                                    best_match->x1_patch=left_rgn->x1;   // SYNTETAGMENES ARXIS PATCH X STIN EIKONA #1
                                    best_match->y1_patch=left_rgn->y1;   // SYNTETAGMENES ARXIS PATCH Y STIN EIKONA #1
                                    best_match->x2_patch=right_rgn.x1; // SYNTETAGMENES ARXIS PATCH X STIN EIKONA #2
                                    best_match->y2_patch=right_rgn.y1;   // SYNTETAGMENES ARXIS PATCH Y STIN EIKONA #2
                                    best_match->movement_count=0;
                                    best_match->movement_difference = 0 ;
                                    /* WE FOUND A MATCH */

                                    *has_match=1;

                                   if ( settings[DEPTHMAP_COMPARISON_TOO_GOOD_THRESHOLD] >= prox )
                                   {
                                     ++metrics[COMPAREPATCH_IMMEDIATE_ACCEPTS];
                                     *has_match=1;
                                     return ;
                                   }
                                }



           --right_rgn.x1;
         }
          ++right_rgn.y1;
       }


  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
  if ( settings[DEPTHMAP_COMPARISON_ALSO_REVERSE_CHECK] )
   {
       /*Check from left to right for optimal result , if the two results contradict each other fail matching..!*/
       struct ImageRegion new_left_rgn={0};

       new_left_rgn.height=left_rgn->height;
       new_left_rgn.width=left_rgn->width;
       new_left_rgn.x1=left_rgn->x1;
       new_left_rgn.y1=left_rgn->y1;
       right_rgn.x1 = best_match->x2_patch;
       right_rgn.y1 = best_match->y2_patch;

       //while ( reverse_check_y )
        while ( ( new_left_rgn.x1 > right_rgn.x1 ) && (new_left_rgn.x1>1) )
         {
              // TODO ADD OCCLUSION HANDLING HERE ! AND RIGHT -> LEFT CHECKING HERE !
             prox = ComparePatches( &new_left_rgn , &right_rgn,
                                     rgb1,rgb2,
                                     video_register[EDGES_LEFT].pixels , video_register[EDGES_RIGHT].pixels ,
                                     video_register[SECOND_DERIVATIVE_LEFT].pixels  , video_register[SECOND_DERIVATIVE_RIGHT].pixels ,
                                     video_register[MOVEMENT_LEFT].pixels  , video_register[MOVEMENT_RIGHT].pixels ,
                                     metrics[HORIZONTAL_BUFFER] , metrics[VERTICAL_BUFFER],
                                     metrics[RESOLUTION_X] , metrics[RESOLUTION_Y] ,
                                     best_match->score);
            // DEPTH ESTIMATION ---------------------------------------
			 uint depth_act;
			 if ( new_left_rgn.x1 > right_rgn.x1 ) { depth_act=new_left_rgn.x1 - right_rgn.x1; } else
                                                   { depth_act=right_rgn.x1 - new_left_rgn.x1; }
            // DEPTH ESTIMATION ---------------------------------------
            if  (
                      (prox < best_match->score) // kanoume qualify san kalytero apotelesma
                   && (prox < max_prox_score) // to threshold mas
                   && (depth_act<settings[DEPTHMAP_CLOSEST_DEPTH])  // CULLING -> Praktika dedomena deixnoun oti synithws apotelesmata panw apo 100 einai thoryvos!
                )
	            	{
                                    /* THIS IS THE BEST PATCH SO FAR! */
								    best_match->score=prox;
                                    best_match->depth=depth_act;
                                    best_match->x1_patch=new_left_rgn.x1;   // SYNTETAGMENES ARXIS PATCH X STIN EIKONA #1
                                    best_match->y1_patch=new_left_rgn.y1;   // SYNTETAGMENES ARXIS PATCH Y STIN EIKONA #1
                                    best_match->x2_patch=right_rgn.x1; // SYNTETAGMENES ARXIS PATCH X STIN EIKONA #2
                                    best_match->y2_patch=right_rgn.y1;   // SYNTETAGMENES ARXIS PATCH Y STIN EIKONA #2
                                    best_match->movement_count=0;
                                    best_match->movement_difference = 0 ;
                                    /* WE FOUND A MATCH */

                                    ++metrics[COMPAREPATCH_REVERSE_ACCEPTS];
                                    *has_match=1;
                    }

           --new_left_rgn.x1;
         }
    }
  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */


  return;
}



void DepthMapFull  ( unsigned int left_view_reg,
                     unsigned int right_view_reg,
                     unsigned int left_depth_reg,
                     unsigned int right_depth_reg,
                     unsigned char clear_and_calculate /*Cleaning the depth arrays takes a little longer :) */
                    )
{
    metrics[COMPAREPATCH_TOTAL_CALLS]=0;
    metrics[COMPAREPATCH_IMMEDIATE_ACCEPTS]=0;
    metrics[COMPAREPATCH_REVERSE_ACCEPTS]=0;

    PrepareForDepthMapping( left_depth_reg , right_depth_reg , clear_and_calculate);



  if ( settings[DEPTHMAP_DEBUG] )
    {  //DEBUG THINGS ON HDD
         // WILL BE REMOVED ENTIRELY WHEN LOCKING PROBLEMS ARE SOLVED..
        SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_LEFT.ppm",CALIBRATED_LEFT_EYE,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
        SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_RIGHT.ppm",CALIBRATED_RIGHT_EYE,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
        SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_EDGES_LEFT.ppm",EDGES_LEFT,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
        SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_EDGES_RIGHT.ppm",EDGES_RIGHT,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
        SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_SECOND_DERIVATIVE_LEFT.ppm",SECOND_DERIVATIVE_LEFT,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
        SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_SECOND_DERIVATIVE_RIGHT.ppm",SECOND_DERIVATIVE_RIGHT,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
        SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_MOVEMENT_LEFT.ppm",MOVEMENT_LEFT,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
        SaveRegisterPartToFile("memfs/DEBUG_Dpthmap_MOVEMENT_RIGHT.ppm",MOVEMENT_RIGHT,0,0,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);

    }

    metrics[HISTOGRAM_DENIES]=0;
    metrics[COMPAREPATCH_ALGORITHM_DENIES]=0;

    uint edges_required_to_process=( (uint) ( metrics[VERTICAL_BUFFER] * metrics[HORIZONTAL_BUFFER] * settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED] )  / 100 );

    struct DepthData best_match={0};
    best_match.patch_size_x=(unsigned short) metrics[HORIZONTAL_BUFFER];
	best_match.patch_size_y=(unsigned short) metrics[VERTICAL_BUFFER];

	unsigned int x_vima=metrics[HORIZONTAL_BUFFER] , y_vima=metrics[VERTICAL_BUFFER];
	if ( settings[DEPTHMAP_DETAIL] <= 0 ) { settings[DEPTHMAP_DETAIL]=1; } // :D , swstos programmatismos!
    x_vima= (unsigned int) (metrics[HORIZONTAL_BUFFER] / settings[DEPTHMAP_DETAIL]);
    y_vima= (unsigned int) (metrics[VERTICAL_BUFFER] / settings[DEPTHMAP_DETAIL]);
    if ( metrics[HORIZONTAL_BUFFER]<metrics[VERTICAL_BUFFER]  ) { y_vima = y_vima / 2; } else
    if ( metrics[HORIZONTAL_BUFFER]>metrics[VERTICAL_BUFFER]  ) { x_vima = x_vima / 2; }
    if ( y_vima < 1 ) { y_vima = 1;}
    if ( x_vima < 1 ) { x_vima = 1;}

    struct ImageRegion left_rgn={0};
    left_rgn.width=metrics[HORIZONTAL_BUFFER];
    left_rgn.height=metrics[VERTICAL_BUFFER]; // These are standard

    left_rgn.y1=settings[DEPTHMAP_VERT_OFFSET_DOWN]; // Coords on LeftFrame
    uint xl_lim=metrics[RESOLUTION_X]-metrics[HORIZONTAL_BUFFER];
    uint yl_lim=metrics[RESOLUTION_Y]-metrics[VERTICAL_BUFFER]-settings[DEPTHMAP_VERT_OFFSET_UP]; // Added 5/3/2010 :) SPEED++ Quality ++


    unsigned char patch_has_match=0;

     while ( left_rgn.y1 < yl_lim )
      {
        left_rgn.x1 = settings[DEPTHMAP_STARTLEFT_X];
        while ( left_rgn.x1 < xl_lim )
         {
           best_match.edge_count = GetCompressedRegisterPatchSum1Byte(EDGES_PRESENCE_GROUPED_LEFT,left_rgn.x1,left_rgn.y1,metrics[HORIZONTAL_BUFFER],metrics[VERTICAL_BUFFER]);
           if ( best_match.edge_count > edges_required_to_process)
           {
             //IF THIS PATCH ( xl,yl with size metrics[HORIZONTAL_BUFFER],metrics[VERTICAL_BUFFER] has enough edges process it..
             MatchInHorizontalScanline( video_register[left_view_reg].pixels , video_register[right_view_reg].pixels,
                                             &left_rgn,
                                             &best_match,
                                             &patch_has_match );

             if ( patch_has_match )
               {
                 /* WE FOUND A MATCH */
                 FillDepthMemWithData(l_video_register[left_depth_reg].pixels,
                                      l_video_register[right_depth_reg].pixels,
                                      depth_data_array,
                                      &best_match,
                                      metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
               } else
               {
                 /* AREA IS NOT MATCHED :P */
               }
           }

            left_rgn.x1+=x_vima;
         }
         left_rgn.y1+=y_vima;
       }

   video_register[left_depth_reg].time = video_register[left_view_reg].time;
   video_register[right_depth_reg].time = video_register[right_view_reg].time;
  fprintf(stderr,"Depth Map did a total of %u Patch Comparisons , %u reverse , %u immediate \n",metrics[COMPAREPATCH_TOTAL_CALLS] , metrics[COMPAREPATCH_REVERSE_ACCEPTS] , metrics[COMPAREPATCH_IMMEDIATE_ACCEPTS]);

  if ( settings[DEPTHMAP_IMPROVE_FILLING_HOLES]!=0 )  EnhanceDepthMapFillHoles(video_register[LEFT_EYE].pixels, l_video_register[left_depth_reg].pixels,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
  if ( settings[DEPTHMAP_IMPROVE_USING_EDGES]!=0 )  EnhanceDepthMapWithEdges(video_register[LEFT_EYE].pixels, l_video_register[left_depth_reg].pixels,video_register[EDGES_LEFT].pixels,metrics[RESOLUTION_X],metrics[RESOLUTION_Y]);
  return;
}



int ExecuteDisparityMappingPyramid()
{
  if ( ( video_register[DEPTH_LEFT].lock ) || ( video_register[DEPTH_RIGHT].lock ) )
   {
     fprintf(stderr,"ExecuteDisparityMappingPyramid while another disparity map is pending..! \n"); //return 0;
   }
  video_register[DEPTH_RIGHT].lock=1; video_register[DEPTH_LEFT].lock=1;


  StartTimer(TIMER_DEPTH_MAP_DELAY);

  unsigned int edgepercent=settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED],patch_x=metrics[HORIZONTAL_BUFFER],patch_y=metrics[VERTICAL_BUFFER];
   unsigned int originalthreshold=settings[DEPTHMAP_COMPARISON_THRESHOLD];
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
 // THIS IS SET HERE IN ORDER TO BE PASSED TO Depth_Left_Video without problems
  video_register[DEPTH_LEFT].time = video_register[CALIBRATED_LEFT_EYE].time;
  video_register[DEPTH_RIGHT].time = video_register[CALIBRATED_RIGHT_EYE].time;

  /*
    CONVERTING DEPTH DATA TO RGB VIDEO FORMAT ( FOR USER VIEWING )
   */
  DepthMapToVideo(DEPTH_LEFT,DEPTH_LEFT_VIDEO,1);
  DepthMapToVideo(DEPTH_RIGHT,DEPTH_RIGHT_VIDEO,1);
  if (settings[PASS_TO_WORLD_3D])
   {
       //(unsigned char *)
       fprintf(stderr,"Registers DEPTH0 and COLOR0 are written to filesystem\n");
       SaveRegisterToFile("memfs/DEPTH0",DEPTH_LEFT_VIDEO);
       SaveRegisterToFile("memfs/COLOR0",CALIBRATED_LEFT_EYE);
       SaveTransformationMatrixToFile("memfs/LEFT_HOMOGRAPHY0",&left_homography);
       SaveTransformationMatrixToFile("memfs/RIGHT_HOMOGRAPHY0",&right_homography);

       SaveFeatureListContents(video_register[CALIBRATED_LEFT_EYE].features,"memfs/POINTS_FOR_LEFT_HOMOGRAPHY0");
       SaveFeatureListContents(video_register[CALIBRATED_RIGHT_EYE].features,"memfs/POINTS_FOR_RIGHT_HOMOGRAPHY0");
     /*
       SaveTransformationMatrixToFile("memfs/LEFT_ROTATION0",&left_rotation_transformation);
       SaveTransformationMatrixToFile("memfs/RIGHT_ROTATION0",&right_rotation_transformation);

       SaveTransformationMatrixToFile("memfs/LEFT_TRANSLATION0",&left_translation_transformation);
       SaveTransformationMatrixToFile("memfs/RIGHT_TRANSLATION0",&right_translation_transformation);

       SaveTransformationMatrixToFile("memfs/LEFT_ROTATION_AND_TRANSLATION0",&left_rotation_and_translation_matrix);
       SaveTransformationMatrixToFile("memfs/RIGHT_ROTATION_AND_TRANSLATION0",&right_rotation_and_translation_matrix);
       */
   }


  metrics[DEPTHMAP_DELAY_MICROSECONDS] = EndTimer(TIMER_DEPTH_MAP_DELAY);
  metrics[TOTAL_DEPTHMAP_DELAY_MICROSECONDS]+=metrics[DEPTHMAP_DELAY_MICROSECONDS] ;
  ++metrics[TOTAL_DEPTHMAPS_PERFORMED];

  video_register[DEPTH_RIGHT].lock=0;
  video_register[DEPTH_LEFT].lock=0;
return 1;
}


unsigned int ExecuteDisparityMappingOpenCV()
{
  IplImage* srcLeft = cvCreateImage(cvGetSize(srcLeft), IPL_DEPTH_8U, 3);
  IplImage* srcRight = cvCreateImage(cvGetSize(srcRight), IPL_DEPTH_8U, 3);
  IplImage* leftImage = cvCreateImage(cvGetSize(leftImage), IPL_DEPTH_8U, 1);
  IplImage* rightImage = cvCreateImage(cvGetSize(rightImage), IPL_DEPTH_8U, 1);
  IplImage* depthImage = cvCreateImage(cvGetSize(srcRight), IPL_DEPTH_8U, 1);


  char * srcLeftPointer=0;
  char * srcRightPointer=0;

  srcLeftPointer = srcLeft->imageData; // UGLY HACK
  srcRightPointer = srcRight->imageData; // UGLY HACK

  srcLeft->imageData = (char *) video_register[DEPTH_LEFT_VIDEO].pixels;
  srcRight->imageData = (char *) video_register[DEPTH_RIGHT_VIDEO].pixels;

  cvCvtColor(srcLeft, leftImage, CV_BGR2GRAY);
  cvCvtColor(srcRight, rightImage, CV_BGR2GRAY);

   //cvFindStereoCorrespondence( leftImage, rightImage, 0, depthImage, 50, 15, 3, 6, 8, 15 );

  memcpy(video_register[DEPTH_LEFT_VIDEO].pixels,depthImage->imageData,metrics[RESOLUTION_MEMORY_LIMIT_1BYTE]);

  srcLeft->imageData = (char *)  srcLeftPointer; // UGLY HACK
  srcRight->imageData = (char *)  srcRightPointer; // UGLY HACK

  cvReleaseImage(&srcLeft);
  cvReleaseImage(&srcRight);

  cvReleaseImage(&leftImage);
  cvReleaseImage(&rightImage);
  cvReleaseImage(&depthImage);

  return 1;
}

