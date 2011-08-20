#include "FeatureTracking.h"
#include "FeatureLists.h"
#include "Precalculations.h"
#include "MovementRegistration.h"
#include "VisCortexFilters.h"
#include "VisionMemory.h"
#include "PatchComparison.h"


unsigned int PATCH_DISPLACEMENT=PATCH_SIZE/3; // PATCH_SIZE div 2 ( gia PATCH_SIZE=9 -> 4 )
unsigned int PATCH_SIZE_MULT_3=PATCH_SIZE*3;
// MEMORY


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



void ExecuteTrackPointBrute(unsigned int from,unsigned int to,unsigned int point_num)
{
  unsigned int from_edges,from_derivatives,from_movement;
  unsigned int to_edges,to_derivatives,to_movement;

  GetSecondaryRegisterNamesFromLastToNew
                             ( from,to ,
                               &from_edges,&from_derivatives,&from_movement,
                               &to_edges,&to_derivatives,&to_movement
                              );
/*
  if ((from==LEFT_EYE)||(to==LEFT_EYE))
       {
         from_edges=LAST_EDGES_LEFT; from_derivatives=LAST_SECOND_DERIVATIVE_LEFT; from_movement=LAST_MOVEMENT_LEFT;
         to_edges=EDGES_LEFT; to_derivatives=SECOND_DERIVATIVE_LEFT; to_movement=MOVEMENT_LEFT;
       }
  if ((from==RIGHT_EYE)||(to==RIGHT_EYE))
       {
         from_edges=LAST_EDGES_RIGHT; from_derivatives=LAST_SECOND_DERIVATIVE_RIGHT; from_movement=LAST_MOVEMENT_RIGHT;
         to_edges=EDGES_RIGHT; to_derivatives=SECOND_DERIVATIVE_RIGHT; to_movement=MOVEMENT_RIGHT;
       }
*/


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

      unsigned int patch_displacement = PATCH_SEEK_AREA /2;

      unsigned int x_search_area_start=video_register[from].features->list[point_num].x;
      unsigned int y_search_area_start=video_register[from].features->list[point_num].y;
      PointGoUpLeft(&x_search_area_start,&y_search_area_start,&patch_displacement,&patch_displacement,&min,&min);

      unsigned int x_search_area_end=video_register[from].features->list[point_num].x;
      unsigned int y_search_area_end=video_register[from].features->list[point_num].y;
      PointGoDownRight(&x_search_area_end,&y_search_area_end,&patch_displacement,&patch_displacement,&size_x,&size_y);

      fprintf(stderr," Search area from %u,%u to %u,%u\n",x_search_area_start,y_search_area_start,x_search_area_end,y_search_area_end);


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

int SetImageRegionFromFeatureNumber( struct ImageRegion * last_ir, struct ImageRegion * ir ,unsigned int reg,unsigned int feature_num)
{
   if ( ir == 0 ) { return 0; }
   last_ir->x1=video_register[reg].features->list[feature_num].last_x;
   last_ir->y1=video_register[reg].features->list[feature_num].last_y;
   ir->x1=video_register[reg].features->list[feature_num].x;
   ir->y1=video_register[reg].features->list[feature_num].y;

/*
   last_ir->width = width;
   last_ir->height = height;
   ir->width = width;
   ir->height = height;*/
   return 1;
}


int MatchFeaturesPoints(unsigned int reg_1,unsigned int feature_num_1,unsigned int reg_2,unsigned int feature_num_2)
{
  unsigned int size_x=metrics[RESOLUTION_X],size_y=metrics[RESOLUTION_Y];
  unsigned int from_edges,from_derivatives,from_movement;
  unsigned int to_edges,to_derivatives,to_movement;
    GetSecondaryRegisterNamesFromLastToNew
                             ( reg_1,reg_2 ,
                              &from_edges,&from_derivatives,&from_movement,
                              &to_edges,&to_derivatives,&to_movement
                              );
  unsigned int best_score = video_register[reg_1].features->list[feature_num_1].correspondance_score;
  unsigned int score;
  struct ImageRegion source_rgn={0},target_rgn={0};
  SetImageRegionFromFeatureNumber(&source_rgn,&target_rgn,reg_1,feature_num_1);
  SetImageRegionFromFeatureNumber(&source_rgn,&target_rgn,reg_2,feature_num_2);

  score = ComparePatches( &source_rgn , &target_rgn, video_register[reg_1].pixels,video_register[reg_2].pixels,
                           video_register[from_edges].pixels , video_register[to_edges].pixels ,
                           video_register[from_derivatives].pixels  , video_register[to_derivatives].pixels ,
                           video_register[from_movement].pixels  , video_register[to_movement].pixels ,
                           metrics[HORIZONTAL_BUFFER] , metrics[VERTICAL_BUFFER],
                           size_x , size_y ,
                           best_score);

  if ( (score < video_register[reg_1].features->list[feature_num_1].correspondance_score) &&
       (score < video_register[reg_2].features->list[feature_num_2].correspondance_score)
      )
     {
          video_register[reg_1].features->list[feature_num_1].correspondance_score = score;
          video_register[reg_2].features->list[feature_num_2].correspondance_score = score;
     }


  return 1;
}


int TrackAllPointsOnRegisters(unsigned int reg_new , unsigned int reg_old , unsigned int timeout)
{
    if (   video_register[reg_old].features->last_track_time < settings[TIME_BETWEEN_TRACKING] + TIME_INC )
     {
        RemoveTrackPointsIfTimedOut(video_register[reg_new].features,timeout);


        int old_feature_iterator = 0;
        int new_feature_iterator = 0;

        while ( old_feature_iterator < video_register[reg_old].features->current_features)
          {
             ++old_feature_iterator;
             new_feature_iterator = 0;

             while ( new_feature_iterator < video_register[reg_new].features->current_features)
             {
               ++new_feature_iterator;
               MatchFeaturesPoints(reg_new,new_feature_iterator,reg_old,old_feature_iterator);
             }
          }
     }

    return 1;
}



