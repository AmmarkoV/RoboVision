#include "FeatureLists.h"
#include "VisualCortex.h"
#include <stdio.h>
#include <stdlib.h>


int ClearFeatureList(struct FeatureList * list)
{

  return 1;
}


struct FeatureList * CreateFeatureList(unsigned int size , unsigned int def_patch_width,unsigned int def_patch_height)
{
 // struct FeatureList * new_fls =  malloc( sizeof(struct FeatureList) );
//  new_fls->points = malloc ( sizeof(struct FeatureList) * size );
// new_fls->total_points = size;
  return 0;
}

int DestroyFeatureList(struct FeatureList * list)
{


  return 1;
}


int AddToFeatureList(struct FeatureList * list, int x, int y,int z)
{
 return 0;
}


int AssociatePointWithPoint(struct FeatureList * list,int point,int x, int y,int z)
{
 return 0;
}


int RemoveFromFeatureList(struct FeatureList * list, int point)
{
 return 0;
}


int GetFeatureList(struct FeatureList * list, int point)
{
 return 0;
}

