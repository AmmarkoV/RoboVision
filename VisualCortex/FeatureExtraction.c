#include "FeatureExtraction.h"
#include "VisCortexFilters.h"
#include "VisCortexConvolutionFilters.h"
#include "FeatureTracking.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int ExtractFeatures_MyAlgorithm(int max_features,unsigned int edge_reg,unsigned int target_reg,unsigned int cam_num)
{
  int total_features_added=0;

  if (!ThisIsA1ByteRegister(edge_reg)) { fprintf(stderr,"ExtractFeatures_MyAlgorithm expects 1byte (monochrome) color info"); return 0; }
  video_register[target_reg].depth=1;
  unsigned int image_x=video_register[edge_reg].size_x;
  unsigned int image_y=video_register[edge_reg].size_y;


  unsigned char * edges=video_register[edge_reg].pixels;
  unsigned char * target=video_register[target_reg].pixels;


  if ( (edges==0) || ( target == 0 ) ) { return 0; }



 unsigned x_start = 10;
 unsigned y_start = 10;
 unsigned x_end = image_x-10;
 unsigned y_end = image_y-10;


 register BYTE * source_p; register BYTE * source_end; //Source
 register BYTE * target_p; register BYTE * target_end; //Target



 source_p =   ( (BYTE *) edges + (image_x * y_start) + (x_start) );
 source_end = ( (BYTE *) edges + (image_x * (y_end-1)  ) + (x_end-1) );

 target_p =   ( (BYTE *) target + (image_x * y_start) + (x_start) );
 target_end = ( (BYTE *) target + (image_x * (y_end-1) ) + (x_end-1) );



 memset(target,0,image_x*image_y); // CLEAR TARGET



 unsigned int INTENSITY_THRESHOLD_LOW = 17;

 unsigned int line_width = image_x;
 unsigned int skip_step_abs = 40;
 unsigned int skip_step =  skip_step_abs;
 unsigned int x=x_start,y=y_start;


 while ( (source_p<source_end) && ( y < y_end ) )
 {
	  ++x; source_p++; target_p++;


	  if ( *source_p>=INTENSITY_THRESHOLD_LOW )
	   {
	     *target_p=255;

         if ( x >= x_end )
           {
             x=x_start; ++y;
             source_p+=image_x-x_end; target_p+=image_x-x_end;
             source_p+=x_start; target_p+=x_start;

             ++y;   source_p+=line_width;   target_p+=line_width; // SKIP A LINE
             ++y;   source_p+=line_width;   target_p+=line_width; // SKIP A LINE
           }

         AddPointToTrackList(cam_num,x,y,0);
         ++total_features_added;
         //if ( total_features_added >= max_features ) { fprintf(stderr,"Cannot add more features .. :) \n"); return 1; }
	   } else
	   {
         if ( x >= x_end )
           {
             x=x_start; ++y;
             source_p+=image_x-x_end; target_p+=image_x-x_end;
             source_p+=x_start; target_p+=x_start;

             ++y;   source_p+=line_width;   target_p+=line_width; // SKIP A LINE
             ++y;   source_p+=line_width;   target_p+=line_width; // SKIP A LINE
           }


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


int ExtractFeatures(int rgb_reg,unsigned int target_reg,unsigned int max_features,unsigned int cam_num)
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
    ExtractFeatures_MyAlgorithm(max_features,GENERAL_3,target_reg,cam_num);
    ConvertRegisterFrom1ByteTo3Byte(target_reg,image_x,image_y);

/*
  THIS CODE OUTPUTS SECOND DERIVATIVE TO THE RIGHT OPERATION SCREEN
  //  CopyRegister(GENERAL_3,LAST_RIGHT_OPERATION);
  //  ConvertRegisterFrom1ByteTo3Byte(LAST_RIGHT_OPERATION,image_x,image_y);
*/

    return 1;
}




