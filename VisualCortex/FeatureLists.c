#include "FeatureLists.h"
#include "VisualCortex.h"
#include "VisionMemory.h"
#include <stdio.h>
#include <stdlib.h>

int ListIsOk(struct FeatureList * list)
{
    if ( list == 0 ) {  return 0; }
    return 1;
}

int ClearFeatureList(struct FeatureList * list)
{
  if  (!ListIsOk(list)) { fprintf(stderr,"ClearFeatureList called with a zero list \n");  return 0; }
  list->reg_for_correspondance=0;
  list->current_features = 0;
  return 1;
}


struct FeatureList * CreateFeatureList(unsigned int size , unsigned int def_patch_width,unsigned int def_patch_height)
{
  struct FeatureList * new_fls =  malloc( sizeof(struct FeatureList) );
  new_fls->max_features = size;
  new_fls->list = malloc ( sizeof(struct FeatureData) * size );
  if (new_fls->list==0) { fprintf(stderr,"Error allocating memory for feature list structure\n"); return 0; }
  new_fls->last_track_time = 0;


  ClearFeatureList(new_fls);
  return new_fls;
}

int DestroyFeatureList(struct FeatureList * list)
{
  if  (!ListIsOk(list)) { fprintf(stderr,"DestroyFeatureList called with a zero list \n");  return 0; }
  list->reg_for_correspondance=0;

  list->max_features = 0;
  free(list->list);

  return 1;
}


int CopyFeatureList(struct FeatureList * source,struct FeatureList * target)
{
  if  (!ListIsOk(source)) { fprintf(stderr,"CopyFeatureList arg 1 a zero list \n");  return 0; }
  if  (!ListIsOk(target)) { fprintf(stderr,"CopyFeatureList arg 2 a zero list \n");  return 0; }


  if ( source->max_features > target->max_features )
    {
       /*TODO One solution here is realloc with different size*/
    }
  if ( source->current_features >= target->max_features )
    {
      fprintf(stderr,"Some features will be lost becaus target is smaller \n");
      source->current_features = target->max_features;
      if ( source->current_features  > 0  ) { --source->current_features; }
    }


  target->current_features = source->current_features;

  if ( target->current_features == 0 ) { return 1; }

  target->last_track_time = source->last_track_time;

  int i=0;
  for (i=0; i < source->current_features; i++)
   {
      target->list[i].lost = source->list[i].lost;
      target->list[i].lost_since = source->list[i].lost_since;
      target->list[i].x = source->list[i].x;
      target->list[i].dim_x = source->list[i].dim_x;
      target->list[i].last_x = source->list[i].last_x;
      target->list[i].last_dim_x = source->list[i].last_dim_x;
      target->list[i].y = source->list[i].y;
      target->list[i].dim_y = source->list[i].dim_y;
      target->list[i].last_y = source->list[i].last_y;
      target->list[i].last_dim_y = source->list[i].last_dim_y;
      target->list[i].z = source->list[i].z;
      target->list[i].dim_z = source->list[i].dim_z;
      target->list[i].last_z = source->list[i].last_z;
      target->list[i].last_dim_z = source->list[i].last_dim_z;

      target->list[i].patch_width = source->list[i].patch_width;
      target->list[i].patch_height = source->list[i].patch_height;

      target->list[i].correspondance_score = source->list[i].correspondance_score;

      target->list[i].mem = source->list[i].mem;
      target->list[i].group = source->list[i].group;
   }

  target->reg_for_correspondance = source->reg_for_correspondance;

   return 1;
}

int RenewTrackPoints(struct FeatureList * list,int point)
{
    if  (!ListIsOk(list)) { fprintf(stderr,"RenewTrackPoints called with a zero list \n");  return 0; }
    list->list[point].lost_since = 0;
    return 1;
}


// TODO ADD HERE COPYING OF THE FEATURE IMAGE PATCH , IT IS NEEDED FOR BETTER TRACKING..!
int AddToFeatureList(struct FeatureList * list, float x, float y,float z , int dim_x, int dim_y,int dim_z)
{
   if  (!ListIsOk(list)) { fprintf(stderr,"AddToFeatureList called with a zero list \n");  return 0; }
   if ( list->current_features >= list->max_features-1 ) { fprintf(stderr,"Cannot add to feature list , feature list is full\n"); return 0; }
   unsigned int cur=list->current_features;
    list->list[cur].x=x; list->list[cur].y=y; list->list[cur].z=z;
    list->list[cur].dim_x=dim_x; list->list[cur].dim_y=dim_y; list->list[cur].dim_z=dim_z;
    list->list[cur].last_x=x; list->list[cur].last_y=y; list->list[cur].last_z=z;

    list->list[cur].lost=0;

   ++list->current_features;
 return 1;
}


int AssociatePointWithPoint(struct FeatureList * list,int point,int x, int y,int z)
{
 return 0;
}

int SwapPointsAtFeatureList(struct FeatureList * list, int pointA , int pointB )
{
   if  (!ListIsOk(list)) { fprintf(stderr,"SwapPointsAtFeatureList called with a zero list \n");  return 0; }
   if ( list->current_features == 0 ) { fprintf(stderr,"Cannot swap from empty feature list\n"); return 0; }

   if ( (pointA>=list->current_features)||(pointB>=list->current_features) )
     {  fprintf(stderr,"Points %u and %u are out of bounds ( %u ) \n",pointA,pointB,list->current_features);
        return 0; }

   struct FeatureData tmp=list->list[pointA];
   list->list[pointA]=list->list[pointB];
   list->list[pointB]=tmp;

 return 1;
}

int RemoveFromFeatureList(struct FeatureList * list, int point)
{
   if  (!ListIsOk(list)) { fprintf(stderr,"RemoveFromFeatureList called with a zero list \n");  return 0; }
   if ( list->current_features == 0 ) { fprintf(stderr,"Cannot remove from feature list , feature list is empty\n"); return 0; }
   if ( list->current_features == 1 ) { list->current_features=0; return 1; }

   if ( SwapPointsAtFeatureList(list,point,list->current_features-1) )
     {
       --list->current_features;
       return 1;
     }

 return 0;
}



void RemoveTrackPointsIfTimedOut(struct FeatureList * list,unsigned int timeout)
{
    if  (!ListIsOk(list)) { fprintf(stderr,"RemoveTrackPointsIfTimedOut called with a zero list \n");  return; }
    if ( list->current_features == 0 ) { fprintf(stderr,"Cannot RemoveTrackPointsIfTimedOut from feature list , feature list is empty\n"); return; }

	int i=list->current_features-1;
	while ( i >= 0 )
    {
     if (list->list[i].lost==1)
	 {
	  if (  list->list[i].lost_since+timeout>TIME_INC   )
	   {
	       RemoveFromFeatureList(list,i);
	   }
	 }

       if ( i==0 ) {  break; }  else
                   {  --i;   }
    }
}

unsigned int RemoveTrackPointsIfMovementMoreThan(struct FeatureList * list,unsigned int movement_max)
{
    if  (!ListIsOk(list)) { fprintf(stderr,"RemoveTrackPointsIfMovementMoreThan called with a zero list \n");  return 0; }
    if ( list->current_features == 0 ) { fprintf(stderr,"Cannot RemoveTrackPointsIfMovementMoreThan from feature list , feature list is empty\n"); return 0; }

	int total_removed=0,i=list->current_features-1;
	while ( i >= 0 )
    {
	  unsigned int total_length=0 , dim_length=0;
	  if ( list->list[i].x >= list->list[i].last_x )  { dim_length=list->list[i].x-list->list[i].last_x; } else
	  if ( list->list[i].x <  list->list[i].last_x )  { dim_length=list->list[i].last_x-list->list[i].x; }
      total_length += dim_length*dim_length;

      if ( list->list[i].y >= list->list[i].last_y )  { dim_length=list->list[i].y-list->list[i].last_y; } else
	  if ( list->list[i].y <  list->list[i].last_y )  { dim_length=list->list[i].last_y-list->list[i].y; }
      total_length += dim_length*dim_length;

      if ( list->list[i].z >= list->list[i].last_z )  { dim_length=list->list[i].z-list->list[i].last_z; } else
	  if ( list->list[i].z <  list->list[i].last_z )  { dim_length=list->list[i].last_z-list->list[i].z; }
      total_length += dim_length*dim_length;


	   if ( total_length > movement_max*movement_max )
	    {
	      fprintf(stderr,"Removing point %u that went from %f,%f to %f,%f \n",i,list->list[i].x,list->list[i].y,list->list[i].last_x,list->list[i].last_y);
	      RemoveFromFeatureList(list,i);
	      ++total_removed;
	    }

       if ( i==0 ) {  break; }  else
                   {  --i;   }
    }

  return total_removed;
}



unsigned int Remove2DTrackPointsIfOutOfBounds(struct FeatureList * list,unsigned int x,unsigned int y ,unsigned int width , unsigned int height)
{
    if  (!ListIsOk(list)) { fprintf(stderr,"Remove2DTrackPointsIfOutOfBounds called with a zero list \n");  return 0; }
    if ( list->current_features == 0 ) { fprintf(stderr,"Cannot Remove2DTrackPointsIfOutOfBounds from feature list , feature list is empty\n"); return 0; }

	unsigned int total_removed=0,drop_point =0 ;
    unsigned int x2 = x+width,y2 = y+height ;

    int i=list->current_features-1;
	while ( i >= 0 )
    {
      drop_point=0;

      if (  list->list[i].x < x )  { drop_point=1; } else
      if (  list->list[i].x > x2 )  { drop_point=1; } else
      if (  list->list[i].y < y )   { drop_point=1; } else
      if (  list->list[i].y > y2 )   { drop_point=1; }

	   if ( drop_point )
	    {
	      fprintf(stderr,"Removing point %u that went from %f,%f to %f,%f \n",i,list->list[i].x,list->list[i].y,list->list[i].last_x,list->list[i].last_y);
	      RemoveFromFeatureList(list,i);
	      ++total_removed;
	    }

       if ( i==0 ) {  break; }  else
                   {  --i;   }
    }

   return total_removed;
}



int GetFeatureData(struct FeatureList * list, unsigned int point_num,unsigned int data_type)
{
    if  (!ListIsOk(list)) { fprintf(stderr,"GetFeatureData called with a zero list \n");  return 0; }
    if  (point_num >= list->current_features ) { return 0; }
    switch (data_type)
    {
      case TOTAL_POINTS : return list->current_features; break;
      case FEATURE_X : return list->list[point_num].x; break;
      case FEATURE_Y : return list->list[point_num].y; break;
      case FEATURE_Z : return list->list[point_num].z; break;
      case FEATURE_DIM_X : return list->list[point_num].dim_x; break;
      case FEATURE_DIM_Y : return list->list[point_num].dim_y; break;
      case FEATURE_DIM_Z : return list->list[point_num].dim_z; break;
      case FEATURE_LAST_X : return list->list[point_num].last_x; break;
      case FEATURE_LAST_Y : return list->list[point_num].last_y; break;
      case FEATURE_LAST_Z : return list->list[point_num].last_z; break;
      case PRINT_FEATURE_LIST : PrintFeatureListContents(list); break;
      case FEATURE_IS_LOST : return list->list[point_num].lost; break;
      case FEATURE_LAST_TRACK_TIME: return list->last_track_time; break;
      case MATCHED_WITH_REG : return 0; break;
    };
  return 0;
}


int PrintFeatureListContents(struct FeatureList * list)
{
    if  (!ListIsOk(list)) { fprintf(stderr,"PrintFeatureListContents called with a zero list \n");  return 0; }
    fprintf(stderr,"Feature list total points : %u \n ",list->current_features);
    int i=0;
    for (i=0; i<list->current_features; i++)
     {
         // fprintf(stderr,"Point %u : x,y,z ( %u ,%u , %u ) \n ",i,list->list[i].x,list->list[i].y,list->list[i].z);
     }
   return 1;
}
