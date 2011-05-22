#include "FeatureLists.h"
#include "VisualCortex.h"
#include "VisionMemory.h"
#include <stdio.h>
#include <stdlib.h>


int ClearFeatureList(struct FeatureList * list)
{
  list->reg_for_correspondance_1=0;
  list->reg_for_correspondance_2=0;
  list->reg_for_correspondance_3=0;
  list->current_features = 0;

  return 1;
}


struct FeatureList * CreateFeatureList(unsigned int size , unsigned int def_patch_width,unsigned int def_patch_height)
{
  struct FeatureList * new_fls =  malloc( sizeof(struct FeatureList) );
  new_fls->max_features = size;
  new_fls->list = malloc ( sizeof(struct FeatureData) * size );

  new_fls->correspondance_1 = malloc ( sizeof(struct PointCorrespondence) * size );

  new_fls->correspondance_2 = malloc ( sizeof(struct PointCorrespondence) * size );

  new_fls->correspondance_3 = malloc ( sizeof(struct PointCorrespondence) * size );

  ClearFeatureList(new_fls);
  return new_fls;
}

int DestroyFeatureList(struct FeatureList * list)
{

  list->reg_for_correspondance_1=0;
  free(list->correspondance_1);

  list->reg_for_correspondance_2=0;
  free(list->correspondance_2);

  list->reg_for_correspondance_3=0;
  free(list->correspondance_3);

  list->max_features = 0;
  free(list->list);

  return 1;
}


int CopyFeatureList(struct FeatureList * source,struct FeatureList * target)
{
  if ( source->max_features > target->max_features ) { fprintf(stderr,"Cannot copy to smaller target \n"); }

  target->current_features = source->current_features;

  if ( target->current_features == 0 ) { return 1; }

  int i=0;
  for (i=0; i < target->current_features; i++)
   {
      target->list[i].lost = source->list[i].lost;
      target->list[i].lost_since = source->list[i].lost_since;
      target->list[i].x = source->list[i].x;
      target->list[i].y = source->list[i].y;
      target->list[i].z = source->list[i].z;
      target->list[i].mem = source->list[i].mem;
      target->list[i].group = source->list[i].group;
   }

  target->reg_for_correspondance_1 = source->reg_for_correspondance_1;
  for (i=0; i < target->current_features; i++)
   {
      target->correspondance_1[i].mapped_to = source->correspondance_1[i].mapped_to;
      target->correspondance_1[i].patch_width = source->correspondance_1[i].patch_width;
      target->correspondance_1[i].patch_height = source->correspondance_1[i].patch_height;
      target->correspondance_1[i].score = source->correspondance_1[i].score;
   }

   target->reg_for_correspondance_2 = source->reg_for_correspondance_2;
  for (i=0; i < target->current_features; i++)
   {
      target->correspondance_2[i].mapped_to = source->correspondance_2[i].mapped_to;
      target->correspondance_2[i].patch_width = source->correspondance_2[i].patch_width;
      target->correspondance_2[i].patch_height = source->correspondance_2[i].patch_height;
      target->correspondance_2[i].score = source->correspondance_2[i].score;
   }

  target->reg_for_correspondance_3 = source->reg_for_correspondance_3;
  for (i=0; i < target->current_features; i++)
   {
      target->correspondance_3[i].mapped_to = source->correspondance_3[i].mapped_to;
      target->correspondance_3[i].patch_width = source->correspondance_3[i].patch_width;
      target->correspondance_3[i].patch_height = source->correspondance_3[i].patch_height;
      target->correspondance_3[i].score = source->correspondance_3[i].score;
   }

   return 1;
}

int RenewTrackPoints(struct FeatureList * list,int point)
{
    list->list[point].lost_since = 0;
    return 1;
}


int AddToFeatureList(struct FeatureList * list, int x, int y,int z)
{
   if ( list->current_features >= list->max_features-1 ) { fprintf(stderr,"Cannot add to feature list , feature list is full\n"); return 0; }
   unsigned int cur=list->current_features;
    list->list[cur].x=x;
    list->list[cur].y=y;
    list->list[cur].z=z;

   ++list->current_features;
 return 1;
}


int AssociatePointWithPoint(struct FeatureList * list,int point,int x, int y,int z)
{
 return 0;
}


int RemoveFromFeatureList(struct FeatureList * list, int point)
{
 return 0;
}



void RemoveTrackPointsIfTimedOut(struct FeatureList * list,unsigned int timeout)
{
	int i=0;
	while ( i < list->current_features )
    {
     if (list->list[i].lost==1)
	 {
	  if (  list->list[i].lost_since+timeout>TIME_INC   )
	   {
	       RemoveFromFeatureList(list,i);
	   }
	 }
	   ++i;
    }
}




int GetFeatureData(struct FeatureList * list, unsigned int point_num,unsigned int data_type)
{
    switch (data_type)
    {
      case TOTAL_POINTS : return list->current_features; break;
      case FEATURE_X : return list->list[point_num].x; break;
      case FEATURE_Y : return list->list[point_num].y; break;
      case FEATURE_Z : return list->list[point_num].z; break;
      case PRINT_FEATURE_LIST : PrintFeatureListContents(list); break;
      case MATCHED_WITH_REG : return 0; break;
    };
  return 0;
}


int PrintFeatureListContents(struct FeatureList * list)
{
    fprintf(stderr,"Feature list total points : %u \n ",list->current_features);
    int i=0;
    for (i=0; i<list->current_features; i++)
     {
          fprintf(stderr,"Point %u : x,y,z ( %u ,%u , %u ) \n ",i,list->list[i].x,list->list[i].y,list->list[i].z);
     }
   return 1;
}
