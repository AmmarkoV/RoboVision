#include "FeatureTracking.h"
#include "FeatureLists.h"
#include "Precalculations.h"
#include "MovementRegistration.h"
#include "VisCortexFilters.h"


unsigned int PATCH_DISPLACEMENT=PATCH_SIZE/3; // PATCH_SIZE div 2 ( gia PATCH_SIZE=9 -> 4 )
unsigned int PATCH_SIZE_MULT_3=PATCH_SIZE*3;
// MEMORY


void ExecuteTrackPoint(struct FeatureList * list,unsigned int point_num)
{
/*
  const int FAIL_IF_SCORE_OVER=PATCH_SIZE*PATCH_SIZE*40;//35;
  struct ImageRegion track_buf;
  track_buf.x1=0; track_buf.y1=0;
  track_buf.x2=PATCH_SIZE; track_buf.y2=PATCH_SIZE;
  track_buf.full_x=PATCH_SIZE; track_buf.full_y=PATCH_SIZE;

  struct ImageRegion this_buf;
  this_buf.full_x=metrics[RESOLUTION_X]; this_buf.full_y=metrics[RESOLUTION_Y];

  struct ImageRegion best_match;
  best_match.x1=track_mem[point_num].track_x-PATCH_DISPLACEMENT;
  best_match.y1=track_mem[point_num].track_y-PATCH_DISPLACEMENT;
  best_match.x2=track_mem[point_num].track_x+PATCH_DISPLACEMENT;
  best_match.y2=track_mem[point_num].track_y+PATCH_DISPLACEMENT;

  unsigned int score=0;
  unsigned int best_score=0;

  //NORMAL BORDERS
  signed int start_x=(signed int) track_mem[point_num].track_x-PATCH_SEEK_AREA;
  signed int start_y=(signed int) track_mem[point_num].track_y-PATCH_SEEK_AREA;
  signed int end_x=(signed int) track_mem[point_num].track_x+PATCH_SEEK_AREA;
  signed int end_y=(signed int) track_mem[point_num].track_y+PATCH_SEEK_AREA;

  //BORDER LIMITS
  if (start_x>(signed int) metrics[RESOLUTION_X]-PATCH_SIZE) { start_x=metrics[RESOLUTION_X]-PATCH_SIZE-1; }
  if (start_y>(signed int) metrics[RESOLUTION_Y]-PATCH_SIZE) { start_y=metrics[RESOLUTION_Y]-PATCH_SIZE-1; }

  if (start_x<0) {start_x=0;}
  if (start_y<0) {start_y=0;}
  if (end_x>(signed int) metrics[RESOLUTION_X]) {end_x=metrics[RESOLUTION_X];}
  if (end_y>(signed int) metrics[RESOLUTION_Y]) {end_y=metrics[RESOLUTION_Y];}


  int res=0;
  unsigned long edges_counted;
  int x,y;
  for (y=start_y; y<end_y; y++)
  {
	this_buf.y1=y;
	this_buf.y2=y+PATCH_SIZE;
	for (x=start_x; x<end_x; x++)
	{
      this_buf.x1=x;
	  this_buf.x2=x+PATCH_SIZE;

	 // if (PatchHasMovement(&this_buf,video_register[MOVEMENT_LEFT].pixels,video_register[GROUP_MOVEMENT_LEFT].pixels))
	  { //PATCH MOVEMENT ara pithanon na metakinithikame ekei!
        score=PatchDistanceCompare_RGB(&track_buf,&this_buf,track_mem[point_num].now_block,video_register[LEFT_EYE].pixels,FAIL_IF_SCORE_OVER,&edges_counted);
	    if (score>=FAIL_IF_SCORE_OVER) { // DEN KANOUME TPT
	                                   } else
	    if ((score<best_score)|| (res==0) )
	     {
		   best_score=score;
		   best_match.x1=this_buf.x1;
		   best_match.y1=this_buf.y1;
		   res=1;
		 }
	  } //PATCH MOVEMENT ara pithanon na metakinithikame ekei!

	}
  }

  track_mem[point_num].score=best_score;
  if (res==1)
   {
     track_mem[point_num].track_x=best_match.x1+PATCH_DISPLACEMENT;
     track_mem[point_num].track_y=best_match.y1+PATCH_DISPLACEMENT;
	 track_mem[point_num].lost=0;
     track_mem[point_num].lost_since=0;
	 LoadNowVideoMemToTrackMem(point_num);
   } else
  {
    //AN DEN EINAI SET TO RES FLAG SIMAINEI #1 OTI DEN ALLAKSAME POINT GIATI DN YPIRKSE KINSII KONTA MAS
	// #2 OTI XASAME TO TRACKING POINT! :(

	  // TODO NA VALW KWDIKA GIA NA SYGKRINEI TO DEFAULT KOMMATI ME AYTO STIN MNIMI KAI AN EINAI POLY DIAFORETIKO NA EPISTREFEI LOST
	this_buf.x1=track_mem[point_num].track_x-PATCH_DISPLACEMENT;
	this_buf.x2=track_mem[point_num].track_x+PATCH_DISPLACEMENT;
    this_buf.y1=track_mem[point_num].track_y-PATCH_DISPLACEMENT;
	this_buf.y2=track_mem[point_num].track_y+PATCH_DISPLACEMENT;
	score=PatchDistanceCompare_RGB(&track_buf,&this_buf,track_mem[point_num].now_block,video_register[LEFT_EYE].pixels,FAIL_IF_SCORE_OVER,&edges_counted);
	if (score>=FAIL_IF_SCORE_OVER) { //AN EXOUME XASE ENTELWS TIN SXESI ME TO PRWTOTYPO PATCH KANOUME REVERT SE AYTO!
		                            score=PatchDistanceCompare_RGB(&track_buf,&track_buf,track_mem[point_num].now_block,track_mem[point_num].original_block,FAIL_IF_SCORE_OVER,&edges_counted);
	                                if (score>=FAIL_IF_SCORE_OVER)
									         { RevertToOriginalTrackMem(point_num); }

		                            if (track_mem[point_num].lost==0)
									         { track_mem[point_num].lost_since=TIME_INC; } // <- AN TO SIMEIO MOLIS XATHIKE MARKAROUME TON XRONO POU EKSAFANISTIKE!

									track_mem[point_num].lost=1;
	                              } else
	                              { //PROFANWS APLA DEN YPARXEI KINISI KAI EIMASTE STATIKA! :) GOOD!
									track_mem[point_num].lost=0;
								    track_mem[point_num].lost_since=0;
								  }

  }
*/
}
