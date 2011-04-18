#include "FeatureTracking.h"
#include "Precalculations.h"
#include "MovementRegistration.h"
#include "VisCortexFilters.h"

// MEMORY
unsigned int total_track_points=0;
struct TrackPoint track_mem[MAX_TRACK_MEM]={{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}};
unsigned int PATCH_DISPLACEMENT=PATCH_SIZE/3; // PATCH_SIZE div 2 ( gia PATCH_SIZE=9 -> 4 )
unsigned int PATCH_SIZE_MULT_3=PATCH_SIZE*3;
// MEMORY

void inline RevertToOriginalTrackMem(unsigned int tpoint)
{
	memcpy(track_mem[tpoint].now_block,track_mem[tpoint].original_block,PATCH_SIZE*PATCH_SIZE*3*sizeof(unsigned char));
}

void inline LoadVideoMemToTrackMem(unsigned int tpoint)
{
 unsigned int pt_x=track_mem[tpoint].original_x-PATCH_DISPLACEMENT;
 unsigned int pt_y=track_mem[tpoint].original_y-PATCH_DISPLACEMENT;
 unsigned int start_mem=0,end_mem=PATCH_SIZE_MULT_3,start_armem=0;

 unsigned int MAX_RES_MULT_3 = metrics[RESOLUTION_X] * 3;
 int y;
 for (y=0; y<PATCH_SIZE; y++)
  {
	start_armem=( (pt_y+y) * MAX_RES_MULT_3 )+( pt_x *3 );
    while(start_mem<end_mem)
	 {
	  if (track_mem[tpoint].cam==0) { track_mem[tpoint].original_block[start_mem]=video_register[LEFT_EYE].pixels[start_armem];
	                                  track_mem[tpoint].now_block[start_mem]=video_register[LEFT_EYE].pixels[start_armem];
	                                } else
	  if (track_mem[tpoint].cam==1) { track_mem[tpoint].original_block[start_mem]=video_register[RIGHT_EYE].pixels[start_armem];
	                                  track_mem[tpoint].now_block[start_mem]=video_register[RIGHT_EYE].pixels[start_armem];
	                                }


      ++start_mem;
	  ++start_armem;
	}

    start_mem=(y+1) * PATCH_SIZE_MULT_3 ;
	end_mem=start_mem + PATCH_SIZE_MULT_3;

  }
}


void inline LoadNowVideoMemToTrackMem(unsigned int tpoint)
{
 unsigned int pt_x=track_mem[tpoint].track_x-PATCH_DISPLACEMENT;
 unsigned int pt_y=track_mem[tpoint].track_y-PATCH_DISPLACEMENT;
 unsigned int start_mem=0,end_mem=PATCH_SIZE_MULT_3,start_armem=0;

 unsigned int MAX_RES_MULT_3 = metrics[RESOLUTION_X] * 3;
 int y;
 for (y=0; y<PATCH_SIZE; y++)
  {
	start_armem=( (pt_y+y) * MAX_RES_MULT_3 )+( pt_x *3 );
    while(start_mem<end_mem)
	 {
	  if (track_mem[tpoint].cam==0) { track_mem[tpoint].now_block[start_mem]=video_register[LEFT_EYE].pixels[start_armem];
	                                } else
	  if (track_mem[tpoint].cam==1) { track_mem[tpoint].now_block[start_mem]=video_register[RIGHT_EYE].pixels[start_armem];
	                                }


      ++start_mem;
	  ++start_armem;
	}

    start_mem=(y+1) * PATCH_SIZE_MULT_3 ;
	end_mem=start_mem + PATCH_SIZE_MULT_3 ;

  }
}

void BitBltTrackPointMemToRegister(unsigned int tpoint,unsigned int videoregister,unsigned int x , unsigned int y)
{
unsigned int scanline=0;
 for ( scanline=0; scanline<PATCH_SIZE; scanline++)
  {
   /*We bitblt each line of the memory due to different size calculations on CopyPartOfImageToImage ( it is assumed that source will have the
     same size as target which is the size of every register*/
   CopyPartOfImageToImage(track_mem[tpoint].original_block+(PATCH_SIZE*scanline*3),video_register[videoregister].pixels,0,0,x,y+scanline,PATCH_SIZE,1);
   CopyPartOfImageToImage(track_mem[tpoint].now_block+(PATCH_SIZE*scanline*3),video_register[videoregister].pixels,0,0,x,y+PATCH_SIZE+scanline,PATCH_SIZE,1);
  }
}


void ApplyTrackPositionAsOriginal(unsigned int tpoint)
{
     //SETAROUME TO ORIGINAL_X/y SAN TO SIMEIO STO OPOIO EIMASTE TWRA!
	 track_mem[tpoint].original_x=track_mem[tpoint].track_x;
     track_mem[tpoint].original_y=track_mem[tpoint].track_y;
	 //SETAROUME TO ORIGINAL_X/y SAN TO SIMEIO STO OPOIO EIMASTE TWRA!
}



unsigned int GetTrackData(unsigned int tpoint,unsigned int dat)
{
 switch (dat)
  {
    case 1: {return track_mem[tpoint].score; }
	case 2: {return PATCH_SIZE; }
	case 3: {return track_mem[tpoint].lost; }
	default : return 0;
  };
 return 0;
}

unsigned int GetTrackColor(unsigned int tpoint,unsigned int pt_x,unsigned int pt_y)
{
  unsigned int start_mem=( pt_y * PATCH_SIZE_MULT_3 )+( pt_x * 3 );
  return RGB(track_mem[tpoint].now_block[start_mem],track_mem[tpoint].now_block[start_mem+1],track_mem[tpoint].now_block[start_mem+2]);
}


unsigned int GetOriginalTrackColor(unsigned int tpoint,unsigned int pt_x,unsigned int pt_y)
{
  unsigned int start_mem=( pt_y * PATCH_SIZE_MULT_3 )+( pt_x * 3 );
  return RGB(track_mem[tpoint].original_block[start_mem],track_mem[tpoint].original_block[start_mem+1],track_mem[tpoint].original_block[start_mem+2]);
}

unsigned int AddPointToTrackList(char cam,unsigned int pt_x,unsigned int pt_y,unsigned int group_belong)
{
  if (MAX_TRACK_MEM<=total_track_points+1) { return 0; }
  if ( ( pt_x<=PATCH_DISPLACEMENT ) || (pt_y<=PATCH_DISPLACEMENT ) ) { return 0; }
  if ( ( pt_x>=metrics[RESOLUTION_X] ) || (pt_y>=metrics[RESOLUTION_Y] ) ) { return 0; }


  fprintf(stderr,"AddPointToTrackList %u %u .. ",pt_x,pt_y);
  track_mem[total_track_points].cam=cam;
  track_mem[total_track_points].original_x=pt_x;
  track_mem[total_track_points].original_y=pt_y;
  track_mem[total_track_points].track_x=pt_x; //Tracking position is initial position
  track_mem[total_track_points].track_y=pt_y; //Tracking position is initial position
  track_mem[total_track_points].group=group_belong;
  track_mem[total_track_points].lost=0;
  track_mem[total_track_points].lost_since=0;
  fprintf(stderr,"loading video mem to track (TODO FIX SEGFAULTS!) ");
  LoadVideoMemToTrackMem(total_track_points);
  fprintf(stderr,"ok\n");

  ++total_track_points;
  return total_track_points;
}


unsigned int LastNonTimedOutPoint(unsigned int timeout)
{
  signed int i=GetPointTrackList()-1;
  while ( (i>0) && ( track_mem[i].lost_since+timeout>TIME_INC ) )
   {
	   --i;
   }

   return i;
}

void RemoveTrackPointIfTimedOut(unsigned int timeout)
{
	int i=0;
	int last_non_timeout=GetPointTrackList()-1;
	while (i<GetPointTrackList() )
    {
     if (track_mem[i].lost==1)
	 {
	  if (  track_mem[i].lost_since+timeout>TIME_INC   )
	   {
	     last_non_timeout = LastNonTimedOutPoint(timeout);


	   if (last_non_timeout!=0)
	    {
	      track_mem[i]=track_mem[last_non_timeout];
		}

        --total_track_points;
	   }
	 }
	   ++i;
    }
}



void ClearTrackPoints()
{
   total_track_points=0;
}

unsigned int GetTrackPoint(unsigned int point_num,char choice,char *pt_cam,unsigned int *pt_x,unsigned int *pt_y)
{
 if (point_num>=total_track_points) { return 0; }
 switch (choice)
 {
     case 1:
		   {
		     *pt_x=track_mem[point_num].original_x;
             *pt_y=track_mem[point_num].original_y;
             *pt_cam=track_mem[point_num].cam;
             break;
		   }
    case 2:
		   {
		     *pt_x=track_mem[point_num].track_x;
             *pt_y=track_mem[point_num].track_y;
             *pt_cam=track_mem[point_num].cam;
             break;
		   }
 };
  return 1;
}

unsigned int GetPointTrackList()
{
 return total_track_points;
}



void ExecuteTrackPoint(unsigned int point_num)
{
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

	  if (PatchHasMovement(&this_buf,video_register[MOVEMENT_LEFT].pixels,video_register[GROUP_MOVEMENT_LEFT].pixels))
	  { //PATCH MOVEMENT ara pithanon na metakinithikame ekei!
        score=PatchDistanceCompare_RGB(&track_buf,&this_buf,track_mem[point_num].now_block,video_register[LEFT_EYE].pixels,FAIL_IF_SCORE_OVER,&edges_counted);
	    if (score>=FAIL_IF_SCORE_OVER) { /* DEN KANOUME TPT */ } else
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

	  /* TODO NA VALW KWDIKA GIA NA SYGKRINEI TO DEFAULT KOMMATI ME AYTO STIN MNIMI KAI AN EINAI POLY DIAFORETIKO
	    NA EPISTREFEI LOST */
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
}
