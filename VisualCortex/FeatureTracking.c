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
   if ( *x - *x_subtract >= *x_min )
          { *x=*x - *x_subtract; } else
          { *x=0; }
   if ( *y - *y_subtract >= *y_min )
          { *y=*y - *y_subtract; } else
          { *y=0; }
}

inline void PointGoDownRight(unsigned int *x,unsigned int *y,unsigned int *x_add,unsigned int *y_add,unsigned int *x_max,unsigned int *y_max)
{
   if ( *x + *x_add < *x_max )
          { *x=*x + *x_add; } else
          { *x=*x_max; }
   if ( *y + *y_add < *y_max )
          { *y=*y + *y_add; } else
          { *y=*y_max; }
}

void ExecuteTrackPoint(unsigned int from,unsigned int to,unsigned int point_num)
{ return;
  unsigned int from_edges,from_derivatives,from_movement;
  unsigned int to_edges,to_derivatives,to_movement;
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



  unsigned int min=0,size_x=metrics[RESOLUTION_X],size_y=metrics[RESOLUTION_Y];
  unsigned int x1=0,y1=0,x2=size_x,y2=size_y , width =  PATCH_SIZE /2 , height =  PATCH_SIZE /2;

      x1=video_register[from].features->list[point_num].x;
      y1=video_register[from].features->list[point_num].y;
      PointGoUpLeft(&x1,&y1,&width,&height,&min,&min);

      x2=video_register[from].features->list[point_num].x;
      y2=video_register[from].features->list[point_num].y;
      PointGoDownRight(&x2,&y2,&width,&height,&size_x,&size_y);
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

      unsigned int best_score=settings[DEPTHMAP_COMPARISON_THRESHOLD]+1 , best_x=0,best_y=0;
      unsigned int  prox=0;

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
            }
         }
       }

}
