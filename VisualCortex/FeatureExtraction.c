#include "FeatureExtraction.h"
#include "VisCortexFilters.h"
#include "FeatureTracking.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int ExtractFeatures_MyAlgorithm(int max_features,unsigned char * edge_source,unsigned char * target,int image_x,int image_y,int monochrome)
{
  int total_features_added=0;

  if ( (edge_source==0) || ( target == 0 ) ) { return 0; }
  fprintf(stderr,"ExtractFeatures_MyAlgorithm , beta function hope it doesn`t crash..\n");

 //Source
 register BYTE * source_p;
 register BYTE * source_end;
 register BYTE *source_r; register BYTE *source_g; register BYTE *source_b;

 //Target
 register BYTE * target_p;
 register BYTE * target_end;
 register BYTE *target_r; register BYTE *target_g; register BYTE *target_b;

 source_p =  (BYTE *)  edge_source;
 source_end = ( (BYTE *)    edge_source + (image_x * (image_y-1) *3 ) + (3*(image_x-1)) );

 target_p =  (BYTE *)  target;
 target_end = ( (BYTE *)    target + (image_x * (image_y-1) *3 ) + (3*(image_x-1)) );


 memset(target,0,image_x*image_y*3); // CLEAR TARGET

 unsigned int line_width = image_x*3;
 unsigned int skip_step_abs = 40;
 unsigned int skip_step = 3 * skip_step_abs;
 unsigned int x=0,y=1;

 while (source_p<source_end)
 {
	  source_r = source_p++; source_g = source_p++; source_b = source_p++;
      target_r = target_p++; target_g = target_p++; target_b = target_p++;
      ++x;

	  if ( (*source_r>20)||(*source_g>20)||(*source_b>20) )
	   {
	     *target_r=255; *target_g=255; *target_b=255;

         source_p+=skip_step;
         target_p+=skip_step;
         x=x+skip_step_abs;

         if ( x >= image_x )
           { x=x-image_x;
             ++y;

           ++y;   source_p+=line_width;   target_p+=line_width;
           ++y;   source_p+=line_width;   target_p+=line_width;
           }

         AddPointToTrackList(0,x,y,0);
         ++total_features_added;
         if ( total_features_added >= max_features ) { /*Cannot add more features .. :) */ return 1; }
	   }

     if ( x >= image_x )
           { x=x-image_x;
             ++y;

             ++y;   source_p+=line_width;   target_p+=line_width;
             ++y;   source_p+=line_width;   target_p+=line_width;
           }
 }

  fprintf(stderr,"FindGoodTrackingPoints , survived , total of %u points ..\n",GetPointTrackList());
  return 1;
}

int ExtractFeaturesOpenSURF()
{
    fprintf(stderr,"Not linked yet..\n");
    return 0;
}



int ExtractFeatures(int max_features,unsigned char * edge_source,unsigned char * target,int image_x,int image_y,int monochrome)
{
     return ExtractFeatures_MyAlgorithm(max_features,edge_source,target,image_x,image_y,monochrome);
}
