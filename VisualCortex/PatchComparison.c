#include "PatchComparison.h"
#include "IntegralImageConversion.h"
#include "Precalculations.h"


/*
   ComparePatchesUsingHistogram uses compares compressed histograms ( Integral Imaging ) to get a fast reaction
   smaller return values means closer patches
*/
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


/*
   ComparePatches uses compares rgb , sobel , secondderivatives , movement data , from 2 patches to
   compare them , smaller return values mean best matches
*/
inline unsigned int ComparePatches(       struct ImageRegion * source_block,    struct ImageRegion * target_block,
                                   unsigned char *rgb1,                  unsigned char *rgb2,
                                   unsigned char *sobel1,                unsigned char *sobel2,
                                   unsigned char *secondderiv1,          unsigned char *secondderiv2,
                                   unsigned char *movement1,             unsigned char *movement2,
                                   unsigned int width,                   unsigned int height,

                                   unsigned int image_x,unsigned int image_y,
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


    ++metrics[COMPAREPATCH_TOTAL_CALLS];

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
      rgb_score=0,move_score=0,sobel_score=0,second_deriv_score=0;

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
		 sobel_score+=precalc_sub[*sobel_px1][*sobel_px2]; //This holds the sobel difference value
         if ( sobel_score > 30 ) { sobel_mismatch=1; }
         ++sobel_px1; ++sobel_px2;
         //USE SSD instead of SAD :P sobel_score = sobel_score * sobel_score;
         // BIGER SCORE -> MORE PATCH DIFFERENCE  !
         // ************** SOBEL COMPARISON **************


         // ************** SOBEL SECOND DERIVATIVE **************
		 // BIGER SCORE -> MORE PATCH DIFFERENCE  !
		 second_deriv_score+=precalc_sub[*secondderiv_px1][*secondderiv_px2]; //This holds the sobel difference value
         ++secondderiv_px1; ++secondderiv_px2;
         //USE SSD instead of SAD :P second_deriv_score = second_deriv_score * second_deriv_score;
         // BIGER SCORE -> MORE PATCH DIFFERENCE  !
         // ************** SOBEL SECOND DERIVATIVE **************


        // ************** RGB COMPARISON **************
        // BIGER SCORE -> MORE PATCH DIFFERENCE  !
        rgb_score+= ( precalc_sub[*image_px1] [*image_px2]  );
		image_px1++; image_px2++;
		rgb_score+= ( precalc_sub[*image_px1] [*image_px2]  );
		image_px1++; image_px2++;
		rgb_score+= ( precalc_sub[*image_px1] [*image_px2]  );
		image_px1++; image_px2++;
		//rgb_score = rgb_score / 3;
		// BIGER SCORE -> MORE PATCH DIFFERENCE  !
        // ************** RGB COMPARISON **************


         // ************** MOVEMENT COMPARISON **************
		 // BIGER SCORE -> MORE PATCH DIFFERENCE  !
		 move_score+=precalc_sub[*sobel_px1][*sobel_px2]; //This holds the move difference value
         if ( move_score > 30 ) { move_mismatch=1; }
         ++move_px1; ++move_px2;
         // BIGER SCORE -> MORE PATCH DIFFERENCE  !
         // ************** MOVEMENT COMPARISON **************
	  }


	  total_score+= rgb_score*settings[DEPTHMAP_RGB_MULTIPLIER];
	  total_score+= move_score*settings[DEPTHMAP_MOVEMENT_MULTIPLIER];
	  total_score+= sobel_score*settings[DEPTHMAP_SOBEL_MULTIPLIER];
	  total_score+= second_deriv_score*settings[DEPTHMAP_SECOND_DERIVATIVE_MULTIPLIER];

        if ( score_threshold<total_score )
          {
             ++metrics[COMPAREPATCH_ALGORITHM_DENIES];
             return total_score; // SPEED CUT :) ++PERFOMANCE  --DEBUGGING :P
          }

	  ++y;
	}
 return total_score;
}
