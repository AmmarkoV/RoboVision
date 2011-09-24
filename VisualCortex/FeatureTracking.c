#include "FeatureTracking.h"
#include "FeatureLists.h"
#include "Precalculations.h"
#include "MovementRegistration.h"
#include "VisCortexFilters.h"
#include "VisionMemory.h"
#include "PatchComparison.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

unsigned int PATCH_DISPLACEMENT=PATCH_SIZE/3; // PATCH_SIZE div 2 ( gia PATCH_SIZE=9 -> 4 )
unsigned int PATCH_SIZE_MULT_3=PATCH_SIZE*3;
// MEMORY

#define MAX_CORNERS 500
IplImage  *eig_image;
IplImage  *tmp_image;

CvPoint2D32f* cornersA;
CvPoint2D32f* cornersB;

IplImage  *image_1;
char * opencv_pointer_retainer_1; // This is a kind of an ugly hack ( see lines noted with UGLY HACK ) to minimize memcpying between my VisCortex and OpenCV , without disturbing OpenCV
IplImage  *image_2;
char * opencv_pointer_retainer_2; // This is a kind of an ugly hack ( see lines noted with UGLY HACK ) to minimize memcpying between my VisCortex and OpenCV , without disturbing OpenCV



inline void PointGoUpLeft(unsigned int *x,unsigned int *y,unsigned int *x_subtract,unsigned int *y_subtract,unsigned int *x_min,unsigned int *y_min)
{
   if ( *x > *x_min + *x_subtract) { *x=*x - *x_subtract; } else
                                   { *x=*x_min; }
   if ( *y > *y_min + *y_subtract) { *y=*y - *y_subtract; } else
                                   { *y=*y_min; }
}

inline void PointGoDownRight(unsigned int *x,unsigned int *y,unsigned int *x_add,unsigned int *y_add,unsigned int *x_max,unsigned int *y_max)
{
   if ( *x + *x_add < *x_max ) { *x=*x + *x_add; } else
                               { *x=*x_max; }
   if ( *y + *y_add < *y_max ) { *y=*y + *y_add; } else
                               { *y=*y_max; }
}


void GetSecondaryRegisterNamesFromLastToNew
                             (unsigned int from,unsigned int to ,
                              unsigned int * from_edges , unsigned int * from_derivatives , unsigned int * from_movement,
                              unsigned int * to_edges , unsigned int * to_derivatives , unsigned int * to_movement
                              )
{
  /*
     This is a small function to reduce code clutter in the functions bellow
     If we are looking at left_eye we want to go from LAST_EDGES_LEFT ( etc ) TO  EDGES_LEFT
  */
  if ((from==LEFT_EYE)||(to==LEFT_EYE))
       {
         *from_edges=LAST_EDGES_LEFT; *from_derivatives=LAST_SECOND_DERIVATIVE_LEFT; *from_movement=LAST_MOVEMENT_LEFT;
         *to_edges=EDGES_LEFT; *to_derivatives=SECOND_DERIVATIVE_LEFT; *to_movement=MOVEMENT_LEFT;
       }
  if ((from==RIGHT_EYE)||(to==RIGHT_EYE))
       {
         *from_edges=LAST_EDGES_RIGHT; *from_derivatives=LAST_SECOND_DERIVATIVE_RIGHT; *from_movement=LAST_MOVEMENT_RIGHT;
         *to_edges=EDGES_RIGHT; *to_derivatives=SECOND_DERIVATIVE_RIGHT; *to_movement=MOVEMENT_RIGHT;
       }
}


int FastDistanceBetween2Points(struct ImageRegion * ir1,struct ImageRegion * ir2)
{
  int total_length=0;
   if ( ir1->x1 > ir2->x1 ) { total_length=ir1->x1 - ir2->x1; } else
                            { total_length=ir2->x1 - ir1->x1; }

   if ( ir1->y1 > ir2->y1 ) { total_length+=ir1->y1 - ir2->y1; } else
                            { total_length+=ir2->y1 - ir1->y1; }


   return total_length;
}

void ExecuteTrackPointBrute(unsigned int from,unsigned int to,unsigned int point_num)
{
  unsigned int from_edges,from_derivatives,from_movement;
  unsigned int to_edges,to_derivatives,to_movement;

  GetSecondaryRegisterNamesFromLastToNew
                             ( from,to ,
                               &from_edges,&from_derivatives,&from_movement,
                               &to_edges,&to_derivatives,&to_movement
                              );


  unsigned int min=0,size_x=metrics[RESOLUTION_X],size_y=metrics[RESOLUTION_Y];
  unsigned int x1=0,y1=0,x2=size_x,y2=size_y , width =  PATCH_SIZE , height =  PATCH_SIZE;
  unsigned int center_distance_x = PATCH_SIZE / 2;
  unsigned int center_distance_y = PATCH_SIZE / 2;

      x1=video_register[from].features->list[point_num].x;
      y1=video_register[from].features->list[point_num].y;
      PointGoUpLeft(&x1,&y1,&center_distance_x,&center_distance_y,&min,&min);

      x2=video_register[from].features->list[point_num].x;
      y2=video_register[from].features->list[point_num].y;
      PointGoDownRight(&x2,&y2,&center_distance_x,&center_distance_y,&size_x,&size_y);
      /*We now have the starting X1,Y1 - X2,Y2 rectangle and will now try to compare it to its neighborhood on the target image*/

      unsigned int patch_displacement = PATCH_SEEK_AREA / 3;

      unsigned int x_search_area_start=video_register[from].features->list[point_num].x;
      unsigned int y_search_area_start=video_register[from].features->list[point_num].y;
      PointGoUpLeft(&x_search_area_start,&y_search_area_start,&patch_displacement,&patch_displacement,&min,&min);

      unsigned int x_search_area_end=video_register[from].features->list[point_num].x;
      unsigned int y_search_area_end=video_register[from].features->list[point_num].y;
      PointGoDownRight(&x_search_area_end,&y_search_area_end,&patch_displacement,&patch_displacement,&size_x,&size_y);

     // fprintf(stderr," Search area from %u,%u to %u,%u\n",x_search_area_start,y_search_area_start,x_search_area_end,y_search_area_end);


      struct ImageRegion source_rgn={0},target_rgn={0};
      SetImageRegion(&source_rgn,x1,y1,x2-x1,y2-y1);

      unsigned int  result_exists=0, best_score=settings[DEPTHMAP_COMPARISON_THRESHOLD]+1 , best_x=0,best_y=0;
      unsigned int  prox=0;

      best_x=video_register[from].features->list[point_num].x;
      best_y=video_register[from].features->list[point_num].y;
      SetImageRegion(&target_rgn,best_x,best_y,width,height);
      best_score = ComparePatches( &source_rgn , &target_rgn, video_register[from].pixels,video_register[to].pixels, video_register[from_edges].pixels , video_register[to_edges].pixels , video_register[from_derivatives].pixels  , video_register[to_derivatives].pixels ,
                                   video_register[from_movement].pixels  , video_register[to_movement].pixels , metrics[HORIZONTAL_BUFFER] , metrics[VERTICAL_BUFFER], size_x , size_y , best_score);



      unsigned int x,y;
      for (x=x_search_area_start; x<x_search_area_end; x++)
       {
         for (y=y_search_area_start; y<y_search_area_end; y++)
         {
            SetImageRegion(&target_rgn,x,y,width,height);
            prox = ComparePatches( &source_rgn , &target_rgn,
                                   video_register[from].pixels,video_register[to].pixels,
                                   video_register[from_edges].pixels , video_register[to_edges].pixels ,
                                   video_register[from_derivatives].pixels  , video_register[to_derivatives].pixels ,
                                   video_register[from_movement].pixels  , video_register[to_movement].pixels ,
                                   metrics[HORIZONTAL_BUFFER] , metrics[VERTICAL_BUFFER],
                                   size_x , size_y ,
                                   best_score);


           if ( best_score > prox )
            {
                //NEW BEST RESULT
                best_x = target_rgn.x1;
                best_y = target_rgn.y1;
                best_score = prox;
                result_exists=1;
            }
         }
       }

 if (result_exists)
 {
  video_register[from].features->list[point_num].last_x=video_register[from].features->list[point_num].x;
  video_register[from].features->list[point_num].last_y=video_register[from].features->list[point_num].y;
  video_register[from].features->list[point_num].x=best_x+center_distance_x;
  video_register[from].features->list[point_num].y=best_y+center_distance_y;
 }


}

int TrackAllPointsOnRegistersBrute(unsigned int reg_new , unsigned int reg_old , unsigned int timeout)
{

    if (   video_register[reg_old].features->last_track_time < settings[TIME_BETWEEN_TRACKING] + TIME_INC )
     {
        RemoveTrackPointsIfTimedOut(video_register[reg_new].features,timeout);
        CopyFeatureList(video_register[reg_new].features,video_register[reg_old].features);
        VisCortx_TrackPoints(reg_old,reg_new);
        VisCortx_RenewAllTrackPoints(reg_new);
     }

    return 1;
}



void ExecuteTrackPoint(unsigned int from,unsigned int to,unsigned int point_num)
{
  unsigned int from_edges,from_derivatives,from_movement;
  unsigned int to_edges,to_derivatives,to_movement;
    GetSecondaryRegisterNamesFromLastToNew
                             ( from,to ,
                              &from_edges,&from_derivatives,&from_movement,
                              &to_edges,&to_derivatives,&to_movement
                              );
}

int SetImageRegionFromFeatureNumber(struct ImageRegion * ir ,unsigned int reg,unsigned int feature_num)
{
   if  ( ir == 0 )  { return 0; }
   ir->x1=video_register[reg].features->list[feature_num].x;
   ir->y1=video_register[reg].features->list[feature_num].y;


   ir->width = metrics[HORIZONTAL_BUFFER];
   ir->height = metrics[VERTICAL_BUFFER];

   return 1;
}


int LinkFeatures(unsigned int reg_new,unsigned int feature_num_new,unsigned int reg_old,unsigned int feature_num_old)
{
   video_register[reg_new].features->list[feature_num_new].last_x = video_register[reg_old].features->list[feature_num_old].x;
   video_register[reg_new].features->list[feature_num_new].last_y = video_register[reg_old].features->list[feature_num_old].y;
   video_register[reg_new].features->list[feature_num_new].last_z = video_register[reg_old].features->list[feature_num_old].z;
   video_register[reg_new].features->list[feature_num_new].lost = 0;

   return 1;
}



int MatchFeaturesPoints(unsigned int reg_new,unsigned int feature_num_new,unsigned int reg_old,unsigned int feature_num_old)
{
  unsigned int size_x=metrics[RESOLUTION_X],size_y=metrics[RESOLUTION_Y];
  unsigned int from_edges,from_derivatives,from_movement;
  unsigned int to_edges,to_derivatives,to_movement;
    GetSecondaryRegisterNamesFromLastToNew
                             ( reg_new,reg_old ,
                              &from_edges,&from_derivatives,&from_movement,
                              &to_edges,&to_derivatives,&to_movement
                              );
  unsigned int best_score = video_register[reg_new].features->list[feature_num_new].correspondance_score;
  unsigned int score;
  struct ImageRegion source_rgn={0},target_rgn={0};
  SetImageRegionFromFeatureNumber(&target_rgn,reg_new,feature_num_new);
  SetImageRegionFromFeatureNumber(&source_rgn,reg_old,feature_num_old);

  if (FastDistanceBetween2Points(&target_rgn,&source_rgn)>10 ) { /* Points to far away filtering */ return 0; }

  score = ComparePatches( &target_rgn,&source_rgn ,  video_register[reg_new].pixels,video_register[reg_old].pixels,
                           video_register[from_edges].pixels , video_register[to_edges].pixels ,
                           video_register[from_derivatives].pixels  , video_register[to_derivatives].pixels ,
                           video_register[from_movement].pixels  , video_register[to_movement].pixels ,
                           metrics[HORIZONTAL_BUFFER] , metrics[VERTICAL_BUFFER],
                           size_x , size_y ,
                           best_score);

  if (
        (score < settings[FEATURE_TRACKING_COMPARISON_THRESHOLD] ) &&
        (score < video_register[reg_new].features->list[feature_num_new].correspondance_score)
     )
     {
          video_register[reg_new].features->list[feature_num_new].correspondance_score = score;
          LinkFeatures(reg_new,feature_num_new,reg_old,feature_num_old);
          return 1;
     }


  return 0;
}


int TrackAllPointsOnRegisters(unsigned int reg_new , unsigned int reg_old , unsigned int timeout)
{
    if (   video_register[reg_old].features->last_track_time < settings[TIME_BETWEEN_TRACKING] + TIME_INC )
     {
       // RemoveTrackPointsIfTimedOut(video_register[reg_new].features,timeout);
        int old_feature_iterator = 0;
        int new_feature_iterator = 0;

        while ( new_feature_iterator < video_register[reg_new].features->current_features)
             { // RESET SCORE AND MARK ALL NODES AS LOST
                video_register[reg_new].features->list[new_feature_iterator].correspondance_score = settings[FEATURE_TRACKING_COMPARISON_THRESHOLD]+1;
                video_register[reg_new].features->list[new_feature_iterator].lost = 1;
                ++new_feature_iterator;
             }


        old_feature_iterator = 0;
        while ( old_feature_iterator < video_register[reg_old].features->current_features)
          {
             new_feature_iterator = 0;
             while ( new_feature_iterator < video_register[reg_new].features->current_features)
             {
               MatchFeaturesPoints(reg_new,new_feature_iterator,reg_old,old_feature_iterator);
               ++new_feature_iterator;
             }

             ++old_feature_iterator;
          }
     }

    return 1;
}


int InitFeatureTracking()
{
    image_1 = cvCreateImage( cvSize(metrics[RESOLUTION_X],metrics[RESOLUTION_Y]), IPL_DEPTH_8U, 1 );
    opencv_pointer_retainer_1 = image_1->imageData; // UGLY HACK

    image_2 = cvCreateImage( cvSize(metrics[RESOLUTION_X],metrics[RESOLUTION_Y]), IPL_DEPTH_8U, 1 );
    opencv_pointer_retainer_2 = image_2->imageData; // UGLY HACK


	CvSize img_sz = cvGetSize( image_1 );
    eig_image = cvCreateImage( img_sz, IPL_DEPTH_32F, 1 );
	tmp_image = cvCreateImage( img_sz, IPL_DEPTH_32F, 1 );

	cornersA = malloc ( sizeof(CvPoint2D32f) *  (MAX_CORNERS+1)  ) ;
	cornersB = malloc ( sizeof(CvPoint2D32f) *  (MAX_CORNERS+1)  ) ;

    return 1;
}

int CloseFeatureTracking()
{
    image_1->imageData = opencv_pointer_retainer_1; // UGLY HACK
    cvReleaseImage(&image_1);

    image_2->imageData = opencv_pointer_retainer_2; // UGLY HACK
    cvReleaseImage(&image_2);


   cvReleaseImage(&eig_image);
   cvReleaseImage(&tmp_image);


   free(cornersA);
   free(cornersB);

    return 1;
}





int FindAndTrackAllPointsOnRegistersOpenCV(unsigned int reg_new , unsigned int reg_old , unsigned int timeout)
{

    if  ( ( video_register[reg_new].pixels == 0 ) || ( video_register[reg_old].pixels == 0 ) ) { return 0; }

    // Load two images and allocate other structures
    unsigned int MONOCHROME_TMP_REGISTER_OLD = GetTempRegister();
    if (MONOCHROME_TMP_REGISTER_OLD == 0 ) { fprintf(stderr," Error Getting the first temporary Video Register ( TrackAllPointsOnRegistersOpenCV ) \n"); }

    unsigned int MONOCHROME_TMP_REGISTER_NEW = GetTempRegister();
    if (MONOCHROME_TMP_REGISTER_NEW == 0 ) { fprintf(stderr," Error Getting the second temporary Video Register ( TrackAllPointsOnRegistersOpenCV ) \n"); }

    CopyRegister(reg_new,MONOCHROME_TMP_REGISTER_NEW,0,0);
    ConvertRegisterFrom3ByteTo1Byte(MONOCHROME_TMP_REGISTER_NEW);

    CopyRegister(reg_old,MONOCHROME_TMP_REGISTER_OLD,0,0);
    ConvertRegisterFrom3ByteTo1Byte(MONOCHROME_TMP_REGISTER_OLD);

    image_1->imageData=(char*) video_register[MONOCHROME_TMP_REGISTER_OLD].pixels; // UGLY HACK
    image_2->imageData=(char*) video_register[MONOCHROME_TMP_REGISTER_NEW].pixels; // UGLY HACK



	int win_size = 15;


	// Get the features for tracking

	int corner_count = MAX_CORNERS;


	cvGoodFeaturesToTrack( image_1, eig_image, tmp_image, cornersA, &corner_count, 0.05, 5.0, 0, 3, 0, 0.04 );

	cvFindCornerSubPix( image_1, cornersA, corner_count, cvSize( win_size, win_size ),
		cvSize( -1, -1 ), cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03 ) );

	// Call Lucas Kanade algorithm
	char features_found[ MAX_CORNERS ];
	float feature_errors[ MAX_CORNERS ];

	CvSize pyr_sz = cvSize( image_1->width+8, image_2->height/3 );

	IplImage* pyrA = cvCreateImage( pyr_sz, IPL_DEPTH_32F, 1 );
	IplImage* pyrB = cvCreateImage( pyr_sz, IPL_DEPTH_32F, 1 );


	cvCalcOpticalFlowPyrLK( image_1, image_2, pyrA, pyrB, cornersA, cornersB, corner_count,
		cvSize( win_size, win_size ), 5, features_found, feature_errors,
		 cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3 ), 0 );





   ClearFeatureList(video_register[reg_new].features);
   video_register[reg_new].features->last_track_time  = video_register[reg_new].time; // AFTER the procedure , the feature list is up to date

   int i=0 ;
   for ( i=0; i <corner_count; i++ )
    {

         AddToFeatureList(  video_register[reg_new].features  ,
                            cornersB[i].x , cornersB[i].y , 1 ,0,0,0);

         video_register[reg_new].features->list[i].last_x = cornersA[i].x;
         video_register[reg_new].features->list[i].last_y = cornersA[i].y;
    }




   cvReleaseImage(&pyrA);
   cvReleaseImage(&pyrB);


   StopUsingVideoRegister(MONOCHROME_TMP_REGISTER_NEW);
   StopUsingVideoRegister(MONOCHROME_TMP_REGISTER_OLD);


   	    return 0;
}



