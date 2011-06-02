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
#include <stdio.h>
#include <string.h>

inline int ComparePatchesUsingHistogram(int hist_reg_left,int hist_reg_right,unsigned int *source_x1,unsigned int *source_y1,unsigned int *target_x1,unsigned int *target_y1)
{
  if (  settings[DEPTHMAP_IMPROVE_USING_HISTOGRAM] ==0 ) { return 0; }

    // NEW HISTOGRAM TESTING :) ++SPEED ++ACCURACY
    struct Histogram hist1;
    struct Histogram hist2;

    CompressedHistogramPatch(l_video_register[HISTOGRAM_COMPRESSED_LEFT].pixels,&hist1,*source_x1,*source_y1);
    CompressedHistogramPatch(l_video_register[HISTOGRAM_COMPRESSED_RIGHT].pixels,&hist2,*target_x1,*target_y1);

    if (
        (precalc_sub[hist1.median_r][hist2.median_r]>settings[PATCH_HIST_THRESHOLD_R])||
        (precalc_sub[hist1.median_g][hist2.median_g]>settings[PATCH_HIST_THRESHOLD_G])||
        (precalc_sub[hist1.median_b][hist2.median_b]>settings[PATCH_HIST_THRESHOLD_B])
       )
          {
            ++metrics[HISTOGRAM_DENIES];
            return 1;
          } /*If histograms greatly different fail the test immediately*/

    // NEW HISTOGRAM TESTING :) ++SPEED ++ACCURACY
    return 0;
}


unsigned int inline ComparePatches(struct ImageRegion * source_block,
                                   struct ImageRegion * target_block,
                                   unsigned char *rgb1,
                                   unsigned char *rgb2,
                                   unsigned char *sobel1,
                                   unsigned char *sobel2,
                                   unsigned char *secondderiv1,
                                   unsigned char *secondderiv2,
                                   unsigned char *movement1,
                                   unsigned char *movement2,
                                   unsigned int width,
                                   unsigned int height,
                                   unsigned int image_x,
                                   unsigned int image_y,
                                   unsigned int best_result_yet
                                  )
{
    unsigned int score_threshold = settings[DEPTHMAP_COMPARISON_THRESHOLD];
    unsigned int failing_score = score_threshold+1;
    	//Kovoume ta source ektos eikonas
	if (source_block->x1+width>=image_x)  { return failing_score; }
    if (source_block->y1+height>=image_y) { return failing_score; }
	if (target_block->x1+width>=image_x)  { return failing_score; }
    if (target_block->y1+height>=image_y) { return failing_score; }


    if (settings[DEPTHMAP_COMPARISON_DO_NOT_PERFORM_FULL_COUNT])
       { /*In case we dont care for a full count of the total Comparison the threshold is the best result yet :)
           (The only reason we would like a full count is for improving this filter via statistics)*/
           if ( best_result_yet > 0 ) score_threshold = best_result_yet;
       }


    if (ComparePatchesUsingHistogram(HISTOGRAM_COMPRESSED_LEFT,HISTOGRAM_COMPRESSED_RIGHT,&source_block->x1,&source_block->y1,&target_block->x1,&target_block->y1))
          {
            ++metrics[HISTOGRAM_DENIES];
            return failing_score;
          } //If histograms greatly different fail the test immediately ++SPEED ++ACCURACY



    unsigned int total_score=0,rgb_score=0,move_score=0,sobel_score=0;
	register unsigned int y=0;
	char sobel_mismatch=0,move_mismatch=0,second_deriv_score=0;

	register BYTE *secondderiv_px1,*secondderiv_px2;
	register BYTE *sobel_px1,*sobel_px2;
	register BYTE *image_px1,*image_px2;
	register BYTE *move_px1, *move_px2;
	register BYTE *stopx1;

    unsigned int source_start_memory_point_3byte=(unsigned int) precalc_memplace_3byte[source_block->x1][source_block->y1];
	unsigned int target_start_memory_point_3byte=(unsigned int) precalc_memplace_3byte[target_block->x1][target_block->y1];
	unsigned int source_start_memory_point_1byte=(unsigned int) precalc_memplace_1byte[source_block->x1][source_block->y1];
	unsigned int target_start_memory_point_1byte=(unsigned int) precalc_memplace_1byte[target_block->x1][target_block->y1];
	unsigned int incrementation_3byte_step = 3 * ( image_x-width );
    unsigned int incrementation_1byte_step = 1 * ( image_x-width );

	  image_px1= (BYTE *) rgb1+source_start_memory_point_3byte;
	  image_px2= (BYTE *) rgb2+target_start_memory_point_3byte;
  	  sobel_px1= (BYTE *) sobel1+source_start_memory_point_1byte;
      sobel_px2= (BYTE *) sobel2+target_start_memory_point_1byte;
      secondderiv_px1=(BYTE *) secondderiv1 + source_start_memory_point_1byte;
      secondderiv_px2=(BYTE *) secondderiv2 + target_start_memory_point_1byte;
      move_px1=  (BYTE *) movement1+source_start_memory_point_1byte;
      move_px2=  (BYTE *) movement2+target_start_memory_point_1byte;
      //Pointers are ready ..!

	while (y<height)
	{
      //Prepare the pointers of the left/right patch image and the left/right patch sobel
      image_px1+=incrementation_3byte_step;
      image_px2+=incrementation_3byte_step;
      sobel_px1+=incrementation_1byte_step;
      sobel_px2+=incrementation_1byte_step;
      secondderiv_px1+=incrementation_1byte_step;
      secondderiv_px2+=incrementation_1byte_step;
      move_px1+=incrementation_1byte_step;
      move_px2+=incrementation_1byte_step;

      stopx1=image_px1+width;
      while (image_px1<stopx1)
	  {

	    sobel_mismatch=0;

         // ************** SOBEL COMPARISON **************
		 // BIGER SCORE -> MORE PATCH DIFFERENCE  !
		 sobel_score=precalc_sub[*sobel_px1][*sobel_px2]; //This holds the sobel difference value
         if ( sobel_score > 30 ) { sobel_mismatch=1; }
         ++sobel_px1; ++sobel_px2;
         //USE SSD instead of SAD :P sobel_score = sobel_score * sobel_score;
         // BIGER SCORE -> MORE PATCH DIFFERENCE  !
         // ************** SOBEL COMPARISON **************


         // ************** SOBEL SECOND DERIVATIVE **************
		 // BIGER SCORE -> MORE PATCH DIFFERENCE  !
		 second_deriv_score=precalc_sub[*secondderiv_px1][*secondderiv_px2]; //This holds the sobel difference value
		 second_deriv_score = second_deriv_score * 20;
         ++secondderiv_px1; ++secondderiv_px2;
         //USE SSD instead of SAD :P second_deriv_score = second_deriv_score * second_deriv_score;
         // BIGER SCORE -> MORE PATCH DIFFERENCE  !
         // ************** SOBEL SECOND DERIVATIVE **************


        // ************** RGB COMPARISON **************
        // BIGER SCORE -> MORE PATCH DIFFERENCE  !
        rgb_score= ( precalc_sub[*image_px1] [*image_px2]  );
		image_px1++; image_px2++;
		rgb_score+= ( precalc_sub[*image_px1] [*image_px2]  );
		image_px1++; image_px2++;
		rgb_score+= ( precalc_sub[*image_px1] [*image_px2]  );
		image_px1++; image_px2++;
		rgb_score = rgb_score / 3;
		// BIGER SCORE -> MORE PATCH DIFFERENCE  !
        // ************** RGB COMPARISON **************


         // ************** MOVEMENT COMPARISON **************
		 // BIGER SCORE -> MORE PATCH DIFFERENCE  !
		 move_score=precalc_sub[*sobel_px1][*sobel_px2]; //This holds the move difference value
         if ( move_score > 30 ) { move_mismatch=1; }
         ++move_px1; ++move_px2;
         // BIGER SCORE -> MORE PATCH DIFFERENCE  !
         // ************** MOVEMENT COMPARISON **************


		total_score+= rgb_score +  move_score + sobel_score + second_deriv_score;

        if ( score_threshold<total_score )
          {
             ++metrics[COMPAREPATCH_ALGORITHM_DENIES];
             return total_score; // SPEED CUT :) ++PERFOMANCE  --DEBUGGING :P
          }
	  }
	  ++y;
	}
 return total_score;
}

void inline FillDepthMemWithData(unsigned short * depth_data_raw,struct DepthData * depth_data_full,struct DepthData *depth_data,unsigned int image_x,unsigned int image_y)
{

	 unsigned int far_away=0;
     if (depth_data->x1_patch-depth_data->x2_patch>0) { far_away=depth_data->x1_patch-depth_data->x2_patch; } else
     if (depth_data->x2_patch-depth_data->x1_patch>0) { far_away=depth_data->x2_patch-depth_data->x1_patch; }

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
		    depth_data_raw[x]=far_away;
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


void PassGuessNextDepthMem(unsigned int prox,unsigned int patch_x,unsigned int patch_y,unsigned short * depth_data_raw,struct DepthData * depth_data_full,struct DepthData *depth_data,unsigned int image_x,unsigned int image_y)
{

		//if (depth_data.x1_patch>image_x) { depth_data.x1_patch=image_x; }
	    //if (depth_data.x2_patch>image_x) { depth_data.x2_patch=image_x; }
		//if (depth_data.x2_patch<=depth_data.x1_patch) {depth_data.x1_patch=depth_data.x2_patch-1;}
        depth_data->score=prox;
        FillDepthMemWithData(depth_data_raw,depth_data_full,depth_data,image_x,image_y);
}



void DepthMapFull  ( unsigned int left_view_reg,
                     unsigned int right_view_reg,
                     unsigned int left_depth_reg,
                     unsigned int right_depth_reg,
                     unsigned int image_x,
                     unsigned int image_y,
                     unsigned char clear_and_calculate /*Cleaning the depth arrays takes a little longer :) */
                    )
{
    unsigned char *left_view=video_register[left_view_reg].pixels;
    unsigned char *right_view=video_register[right_view_reg].pixels;
    unsigned short *left_depth=l_video_register[left_depth_reg].pixels;
    unsigned short *right_depth=l_video_register[right_depth_reg].pixels;

	if ( (left_view==0)||(right_view==0)||(left_depth==0)||(right_depth==0)) { fprintf(stderr,"DepthMap Video `registers` not ready..!"); return; }


	unsigned int edges_required_to_process=( (unsigned int) ( metrics[VERTICAL_BUFFER] * metrics[HORIZONTAL_BUFFER] * settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED] )  / 100 );
    fprintf(stderr,"Edges required to process %u (%u %%) \n",edges_required_to_process,settings[PATCH_COMPARISON_EDGES_PERCENT_REQUIRED]);

    if ( clear_and_calculate == 1 )
    {
       memset(depth_data_array,0,sizeof(struct DepthData) * image_x*image_y );// CLEAR DEPTH MAP FROM ARTIFACTS
       ClearLargeVideoRegister(left_depth_reg);
       ClearLargeVideoRegister(right_depth_reg);

       PrepareCleanSobeledGaussianAndDerivative(LEFT_EYE,EDGES_LEFT,SECOND_DERIVATIVE_LEFT,settings[DEPTHMAP_EDGE_LOW_STRICTNESS],settings[DEPTHMAP_EDGE_HIGH_STRICTNESS]);
       PrepareCleanSobeledGaussianAndDerivative(RIGHT_EYE,EDGES_RIGHT,SECOND_DERIVATIVE_RIGHT,settings[DEPTHMAP_EDGE_LOW_STRICTNESS],settings[DEPTHMAP_EDGE_HIGH_STRICTNESS]);
    }

   if (  settings[DEPTHMAP_IMPROVE_USING_HISTOGRAM] == 1 )
   {
       GenerateCompressHistogramOfImage(video_register[CALIBRATED_LEFT_EYE].pixels,l_video_register[HISTOGRAM_COMPRESSED_LEFT].pixels,metrics[HORIZONTAL_BUFFER],metrics[VERTICAL_BUFFER]);
       GenerateCompressHistogramOfImage(video_register[CALIBRATED_RIGHT_EYE].pixels,l_video_register[HISTOGRAM_COMPRESSED_RIGHT].pixels,metrics[HORIZONTAL_BUFFER],metrics[VERTICAL_BUFFER]);
   }

    if ( clear_and_calculate == 1 )
     {
        // CompressPresenceRegister(EDGES_LEFT,GENERAL_XLARGE_1,5);
        // I dont like the following , I should really change the sobel function to give a one byte response..!
        CopyRegister(EDGES_LEFT,GENERAL_3);
        PixelsOverThresholdSetAsOne(GENERAL_3,1);
        CompressRegister(GENERAL_3,GENERAL_XLARGE_1);
        CompressRegister(MOVEMENT_LEFT,MOVEMENT_GROUPED_LEFT);
        CompressRegister(MOVEMENT_RIGHT,MOVEMENT_GROUPED_RIGHT);
     }

    metrics[HISTOGRAM_DENIES]=0;
    metrics[COMPAREPATCH_ALGORITHM_DENIES]=0;
	struct DepthData best_match={0};
	best_match.patch_size_x=(unsigned short) metrics[HORIZONTAL_BUFFER];
	best_match.patch_size_y=(unsigned short) metrics[VERTICAL_BUFFER];

    unsigned int xlim=image_x-metrics[HORIZONTAL_BUFFER];
    unsigned int ylim=image_y-metrics[VERTICAL_BUFFER]; // Added 5/3/2010 :) SPEED++ Quality ++
    unsigned int x=settings[DEPTHMAP_STARTLEFT_X],y=1;

	unsigned int x_vima=metrics[HORIZONTAL_BUFFER] , y_vima=metrics[VERTICAL_BUFFER];

	if ( settings[DEPTHMAP_DETAIL] <= 0 ) { settings[DEPTHMAP_DETAIL]=1; } // :D , swstos programmatismos!
    x_vima= (unsigned int) (metrics[HORIZONTAL_BUFFER] / settings[DEPTHMAP_DETAIL]);
    y_vima= (unsigned int) (metrics[VERTICAL_BUFFER] / settings[DEPTHMAP_DETAIL]);
    y_vima = y_vima / 2; if ( y_vima < 1 ) { y_vima = 1;}

	unsigned int xblock=0 , yblock=0 ;
    unsigned int  prox=0,movement_left=0 , movement_right=0 ,movement_difference =0;
    unsigned int  max_prox_score = settings[DEPTHMAP_COMPARISON_THRESHOLD]+settings[DEPTHMAP_COMPARISON_THRESHOLD_ADDED];


    struct ImageRegion source_rgn={0},target_rgn={0};
    source_rgn.width=metrics[HORIZONTAL_BUFFER]; source_rgn.height=metrics[VERTICAL_BUFFER]; // These are standard
    target_rgn.width=metrics[HORIZONTAL_BUFFER]; target_rgn.height=metrics[VERTICAL_BUFFER]; // These are standard


    while (y<ylim)
	   {
	     x=settings[DEPTHMAP_STARTLEFT_X];
         while (x<xlim)
		 {
		    // ARXI EKSWTERIKIS EPILOGIS BLOCK
            best_match.score=settings[DEPTHMAP_COMPARISON_THRESHOLD]+1;
            best_match.x1_patch=0; best_match.y1_patch=0;
            best_match.x2_patch=0; best_match.y2_patch=0;
            best_match.edge_count=0;
            best_match.movement_difference =0; best_match.movement_count=0;

             //THA PREPEI TO SOURCE KOMMATI NA EXEI EDGES GIATI ALLIWS DEN EINAI K POLY AKRIVES TO ANTISTOIXO POU THA VRETHEI

             //unsigned int  counted_edges=CountEdges(edges_required_to_process,x,y,metrics[HORIZONTAL_BUFFER],metrics[VERTICAL_BUFFER],video_register[EDGES_LEFT].pixels);
             unsigned int  counted_edges=GetCompressedRegisterPatchSum1Byte(GENERAL_XLARGE_1,x,y,metrics[HORIZONTAL_BUFFER],metrics[VERTICAL_BUFFER]);


             //DEBUG COMMAND TO FIND WTF IS WRONG if (counted_edges>edges_required_to_process) { counted_edges=edges_required_to_process+1; }
		     //THA PREPEI TO SOURCE KOMMATI NA EXEI EDGES GIATI ALLIWS DEN EINAI K POLY AKRIVES TO ANTISTOIXO POU THA VRETHEI

				if (counted_edges>edges_required_to_process)
				      {
				         best_match.edge_count=counted_edges;  //APOTHIKEVOUME TA EDGES!

			             // ARXIZEI H SYGKRISI!
                          unsigned int x_start_right=0;
                          unsigned int x_end_right=x;

                          if ( (x > settings[DEPTHMAP_CLOSEST_DEPTH] ) && (settings[DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_CLOSEST_DEPTH]) )
                            {
                              /* If the x point is very right , we should only process the patches , near at least
                              settings[DEPTHMAP_CLOSEST_DEPTH] pixels to the patch on the right*/
                              x_start_right = x-settings[DEPTHMAP_CLOSEST_DEPTH];
                            }

						 // ESWTERIKO LOOP I DEKSIA EIKONA
						 xblock=x_start_right;  //THELOUME NA KSEKINISOUME APO TIN ARXI TIS DEKSIAS EIKONAS
                         yblock=y-settings[DEPTHMAP_VERT_OFFSET_UP]; //THELOUME NA KSEKINISOUME STIN IDIA SCAN LINE ME ENA PITHANO OFFSET LOGW KAKOU CALIBRATION


						 source_rgn.x1=x; source_rgn.y1=y; // To source ( tou aristerou matiou einai x,y )

                         movement_left=GetMovementAtBlock(MOVEMENT_GROUPED_LEFT,&source_rgn);

                         while ( yblock <= y+settings[DEPTHMAP_VERT_OFFSET_DOWN] )
						 { //KATHETI METATWPISI TOU BUFFER DEKSIAS EIKONAS
						     while ( xblock <= x_end_right )
							 {
							   //ORIZONTIA METATWPISI TOU BUFFER DEKSIAS EIKONAS

                                target_rgn.x1=xblock; target_rgn.y1=yblock;
                                target_rgn.x2=xblock+metrics[HORIZONTAL_BUFFER]; target_rgn.y2=yblock+metrics[VERTICAL_BUFFER];

                                movement_right=GetMovementAtBlock(MOVEMENT_GROUPED_RIGHT,&target_rgn);
                                movement_difference=abs(movement_right-movement_left);

								prox = ComparePatches( &source_rgn , &target_rgn,
                                                       left_view  , right_view,
                                                       video_register[EDGES_LEFT].pixels , video_register[EDGES_RIGHT].pixels ,
                                                       video_register[SECOND_DERIVATIVE_LEFT].pixels  , video_register[SECOND_DERIVATIVE_RIGHT].pixels ,
                                                       video_register[MOVEMENT_LEFT].pixels  , video_register[MOVEMENT_RIGHT].pixels ,
                                                       metrics[HORIZONTAL_BUFFER] , metrics[VERTICAL_BUFFER],
                                                       image_x , image_y ,
                                                       best_match.score);

								// TEST/DOKIMASTIKO DEPTH GIA TEST CULLING
								// H IDEA EINAI OTI ANTIKEIMENA POU EINAI PAAAAAAARA POLY KONTA EINIA MALLON ERRORS
								 unsigned int depth_act;
								 if ( x > xblock ) { depth_act=x-xblock; } else
								                   { depth_act=xblock-x; }
								// TEST/DOKIMASTIKO ALLA VGAZEI KALO APOTELESMA

                                if (
                                         (prox<best_match.score) // kanoume qualify san kalytero apotelesma
                                      && (prox<max_prox_score) // to threshold mas
									  && (depth_act<settings[DEPTHMAP_CLOSEST_DEPTH])  // TEST -> Praktika dedomena deixnoun oti synithws apotelesmata panw apo 100 einai thoryvos!
								    )
								{
                                    /* THIS IS THE BEST PATCH SO FAR! */
								    best_match.score=prox;
                                    best_match.x1_patch=x;   // SYNTETAGMENES ARXIS PATCH X STIN EIKONA #1
                                    best_match.y1_patch=y;   // SYNTETAGMENES ARXIS PATCH Y STIN EIKONA #1
                                    best_match.x2_patch=xblock; // SYNTETAGMENES ARXIS PATCH X STIN EIKONA #2
                                    best_match.y2_patch=yblock;   // SYNTETAGMENES ARXIS PATCH Y STIN EIKONA #2
                                    best_match.movement_count=movement_right;
                                    best_match.movement_difference = movement_difference ;

									FillDepthMemWithData(left_depth,depth_data_array,&best_match,image_x,image_y);


						           /* CODE GUESS CODE PATCH GOES HERE $1 */
								}

                               // ORIZONTIA METATWPISI TOU BUFFER DEKSIAS EIKONAS
                               ++xblock;
							 }
                          // KATHETI METATWPISI TOU BUFFER DEKSIAS EIKONAS
                           ++yblock;
						 }

					  }

            // TELOS EKSWTERIKIS EPILOGIS BLOCK
		   x+=x_vima;
		 }
		 y+=y_vima;
	   }

  if ( settings[DEPTHMAP_IMPROVE_FILLING_HOLES]!=0 )  EnhanceDepthMapFillHoles(video_register[LEFT_EYE].pixels,left_depth,image_x,image_y);
  if ( settings[DEPTHMAP_IMPROVE_USING_EDGES]!=0 )  EnhanceDepthMapWithEdges(video_register[LEFT_EYE].pixels,left_depth,video_register[EDGES_LEFT].pixels,image_x,image_y);
  //fprintf(stderr,"Histogram denied %u bad patches / ComparePatches Algorithm denied %u patches \n",metrics[HISTOGRAM_DENIES],metrics[COMPAREPATCH_ALGORITHM_DENIES]);

  return;
}


/*  $1
	  if (settings[DEPTHMAP_GUESSES]==1)
							    { // CODE TO GUESS NEXT BLOCK!
									 source_rgn.x1=best_match.x1_patch+metrics[HORIZONTAL_BUFFER]; source_rgn.x2=source_rgn.x1+metrics[HORIZONTAL_BUFFER];
									 target_rgn.x1=best_match.x2_patch+metrics[HORIZONTAL_BUFFER]; target_rgn.x2=target_rgn.x1+metrics[HORIZONTAL_BUFFER];
									 if ( (source_rgn.x1>image_x) || (source_rgn.x2>image_x) || (target_rgn.x1>image_x) || (target_rgn.x2>image_x) )
									 {    } // OUT OF MEMORY SPACE NOT GUESSING
								     else
								     {
									  prox = ComparePatches( &source_rgn,&target_rgn,
                                                             left_view, right_view,
                                                             video_register[EDGES_LEFT].pixels , video_register[EDGES_RIGHT].pixels,
                                                             0,
                                                             metrics[HORIZONTAL_BUFFER] , metrics[VERTICAL_BUFFER],
                                                             image_x , image_y , best_match.score);
									  // HEURISTIC GIA NA PETAME KAKES PROVLEPSEIS

									  if ( (best_match.edge_count>edges_required_to_process) && (prox<settings[PATCH_COMPARISON_SCORE_MIN]) && (best_match.score>=prox) )
                                      // HEURISTIC GIA NA PETAME KAKES PROVLEPSEIS
									   {
									     PassGuessNextDepthMem(prox,metrics[HORIZONTAL_BUFFER],metrics[VERTICAL_BUFFER],left_depth,depth_data_array,&best_match,image_x,image_y);
									   }
									 }
								  }	 // CODE TO GUESS NEXT BLOCK!

*/
