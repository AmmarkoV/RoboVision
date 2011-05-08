#include "FeatureExtraction.h"
#include "VisCortexFilters.h"
#include "VisCortexConvolutionFilters.h"
#include "FeatureTracking.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int ExtractFeatures_MyAlgorithm(int max_features,unsigned int edge_reg,unsigned int target_reg)
{
  int total_features_added=0;

  if (!ThisIsA1ByteRegister(edge_reg)) { fprintf(stderr,"ExtractFeatures_MyAlgorithm expects 1byte (monochrome) color info"); return 0; }
  video_register[target_reg].depth=1;
  unsigned int image_x=video_register[edge_reg].size_x;
  unsigned int image_y=video_register[edge_reg].size_y;


  unsigned char * edges=video_register[edge_reg].pixels;
  unsigned char * target=video_register[target_reg].pixels;


  if ( (edges==0) || ( target == 0 ) ) { return 0; }

 //Source
 register BYTE * source_p; register BYTE * source_end;
 //Target
 register BYTE * target_p; register BYTE * target_end;


 source_p =  (BYTE *)  edges;
 source_end = ( (BYTE *)    edges + (image_x * (image_y-1)  ) + ((image_x-1)) );

 target_p =  (BYTE *)  target;
 target_end = ( (BYTE *)    target + (image_x * (image_y-1) ) + ((image_x-1)) );


 memset(target,0,image_x*image_y); // CLEAR TARGET


 unsigned x_start =10;
 unsigned y_start =10;
 unsigned x_end = image_x-10;
 unsigned y_end = image_y-10;


 unsigned int line_width = image_x;
 unsigned int skip_step_abs = 40;
 unsigned int skip_step =  skip_step_abs;
 unsigned int x=0,y=1;


 while (source_p<source_end)
 {
	  source_p++;
      target_p++;
      ++x;

	  if (*source_p>20)
	   {

	     *target_p=255;

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


int ExtractFeatures(int rgb_reg,unsigned int target_reg,unsigned int max_features)
{
        //CONVOLUTION FILTER(9,1,-1,0,1,0,0,0,1,0,-1)
    //CONVOLUTION FILTER(9,1,1,1,1,1,5,1,1,1,1)
    /*Testing*/
    int image_x=video_register[rgb_reg].size_x;
    int image_y=video_register[rgb_reg].size_y;
    signed char table[9]={-1,0,1,0,0,0,1,0,-1};
    signed int divisor = 3;

    CopyRegister(rgb_reg,GENERAL_2);
    GaussianBlur(GENERAL_2,image_x,image_y,0);
    ConvertRegisterFrom3ByteTo1Byte(GENERAL_2,image_x,image_y);
    ConvolutionFilter9_1Byte(GENERAL_2,GENERAL_3,table,divisor);
    ExtractFeatures_MyAlgorithm(max_features,GENERAL_3,target_reg);
//<- This is not needed
    CopyRegister(GENERAL_3,LAST_RIGHT_OPERATION);
    ConvertRegisterFrom1ByteTo3Byte(LAST_RIGHT_OPERATION,image_x,image_y);
// <- This is not needed
    ConvertRegisterFrom1ByteTo3Byte(target_reg,image_x,image_y);

    return 1;
}




