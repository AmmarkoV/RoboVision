#include "FeatureExtraction.h"
#include "FeatureLists.h"
#include "VisCortexFilters.h"
#include "VisCortexConvolutionFilters.h"
#include "FeatureTracking.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../3dpartylibs/fast/fast.h"

struct xy_local
{
 int x, y;
};

enum LineDirections
{
   LUp = 0 ,
   LUpLeft,
   LUpRight,

   LLeft,

   LDownLeft,
   LDown,
   LDownRight,

   LRight ,

   TOTAL_LINE_DIRECTIONS
};


int PatchIsACorner(unsigned int x,unsigned int y,unsigned char * edge_mem,unsigned int edge_reg,unsigned int second_deriv_reg)
{
    /*
    unsigned char * ptr = edge_mem;
    //TODO , THIS COULD ALSO POINT TO THE FAST ALGORITHM

    unsigned int direct_response[TOTAL_LINE_DIRECTIONS]={0};

    if  ( (y>4) && ( x>4 ) && (y<metrics[RESOLUTION_Y]) && ( x<metrics[RESOLUTION_X] ))
       {

       }

*/
    return 0;
}




int ExtractFeatures_MyAlgorithm(int max_features,unsigned int edge_reg,unsigned int second_deriv_reg,unsigned int target_reg,unsigned int cam_num)
{
  int total_features_added=0;

  if (!ThisIsA1ByteRegister(edge_reg)) { fprintf(stderr,"ExtractFeatures_MyAlgorithm expects 1byte (monochrome) color info for edge_reg"); return 0; }
  if (!ThisIsA1ByteRegister(second_deriv_reg)) { fprintf(stderr,"ExtractFeatures_MyAlgorithm expects 1byte (monochrome) color info for second_deriv_reg"); return 0; }

  video_register[target_reg].depth=1;
  unsigned int image_x=video_register[edge_reg].size_x;
  unsigned int image_y=video_register[edge_reg].size_y;


  unsigned char * edges=video_register[edge_reg].pixels;
  unsigned char * target=video_register[target_reg].pixels;


  if ( (edges==0) || ( target == 0 ) ) { return 0; }

 ClearFeatureList(video_register[edge_reg].features);


 unsigned x_start = 15;
 unsigned y_start = 15;
 unsigned x_end = image_x-15;
 unsigned y_end = image_y-15;


 register BYTE * source_p; register BYTE * source_end; //Source
 register BYTE * target_p; register BYTE * target_end; //Target



 source_p =   ( (BYTE *) edges + (image_x * y_start) + (x_start) );
 source_end = ( (BYTE *) edges + (image_x * (y_end-1)  ) + (x_end-1) );

 target_p =   ( (BYTE *) target + (image_x * y_start) + (x_start) );
 target_end = ( (BYTE *) target + (image_x * (y_end-1) ) + (x_end-1) );



 memset(target,0,image_x*image_y); // CLEAR TARGET



 unsigned int INTENSITY_THRESHOLD_LOW = 16;

// unsigned int line_width = image_x;
// unsigned int skip_step_abs = 40;
// unsigned int skip_step =  skip_step_abs;
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

             //++y;   source_p+=line_width;   target_p+=line_width; // SKIP A LINE
             //++y;   source_p+=line_width;   target_p+=line_width; // SKIP A LINE
           }

         //OLD AddPointToTrackList(cam_num,x,y,0);

         if ( PatchIsACorner(x,y,source_p,edge_reg,second_deriv_reg) )
           {
             AddToFeatureList(video_register[edge_reg].features,x,y,1,0,0,0);
             ++total_features_added;
           }

	   } else
	   {
         if ( x >= x_end )
           {
             x=x_start; ++y;
             source_p+=image_x-x_end; target_p+=image_x-x_end;
             source_p+=x_start; target_p+=x_start;

             //++y;   source_p+=line_width;   target_p+=line_width; // SKIP A LINE
             //++y;   source_p+=line_width;   target_p+=line_width; // SKIP A LINE
           }


	   }

 }


  return 1;
}



int ExtractFeaturesOpenSURF()
{
    fprintf(stderr,"Not linked yet..\n");
    return 0;
}


int ExtractFeaturesMy(int rgb_reg,unsigned int edge_reg,unsigned int second_deriv_reg,unsigned int target_reg,unsigned int max_features,unsigned int cam_num)
{
    /*
    CopyRegister(rgb_reg,GENERAL_2);
    GaussianBlur(GENERAL_2,0);
    ConvertRegisterFrom3ByteTo1Byte(GENERAL_2);
    SecondDerivativeIntensitiesFromSource(GENERAL_2,GENERAL_3);
    ExtractFeatures_MyAlgorithm(max_features,GENERAL_3,GENERAL_3,target_reg,cam_num);
    ConvertRegisterFrom1ByteTo3Byte(target_reg);
    CopyFeatureList(video_register[GENERAL_3].features,video_register[rgb_reg].features);
*/

    ExtractFeatures_MyAlgorithm(max_features,edge_reg,second_deriv_reg,target_reg,cam_num);
    ConvertRegisterFrom1ByteTo3Byte(target_reg);
    CopyFeatureList(video_register[edge_reg].features,video_register[rgb_reg].features);

/*
  THIS CODE OUTPUTS SECOND DERIVATIVE TO THE RIGHT OPERATION SCREEN
  //  CopyRegister(GENERAL_3,LAST_RIGHT_OPERATION);
  //  ConvertRegisterFrom1ByteTo3Byte(LAST_RIGHT_OPERATION,image_x,image_y);
*/

    return 1;
}



int ExtractFeatures(int rgb_reg,unsigned int edge_reg,unsigned int second_deriv_reg,unsigned int cam_num)
{
  // fprintf(stderr,"ExtractFeatures called \n");
   CopyRegister(rgb_reg,GENERAL_2);
  // fprintf(stderr,"CopyRegister called \n");
   ConvertRegisterFrom3ByteTo1Byte(GENERAL_2);
  // fprintf(stderr,"ConvertRegisterFrom3ByteTo1Byte called \n");
   ClearFeatureList(video_register[rgb_reg].features);
  // fprintf(stderr,"ClearFeatureList called \n");

   int numcorners=0;
   struct xy_local * corner_list; //(struct xy * )
   corner_list = 0;
   corner_list = (struct xy_local *) fast9_detect_nonmax ( video_register[GENERAL_2].pixels ,
                                                           metrics[RESOLUTION_X] , metrics[RESOLUTION_Y] ,
                                                           metrics[RESOLUTION_X] ,
                                                           settings[FEATURE_DETECTION_THRESHOLD] ,
                                                           &numcorners );
  //fprintf(stderr,"ExtractFeatures called \n");

  if ( corner_list == 0 )
    {
        fprintf(stderr,"FAST Feature extractor returned null pointer .. assuming no memory leak and returning 0 \n");
        return 0;
    }

  int i=0 ;
  for ( i=0; i <numcorners; i++ )
    {

         AddToFeatureList(  video_register[rgb_reg].features  ,
                            corner_list[i].x , corner_list[i].y , 1 ,0,0,0);
    }
//  fprintf(stderr,"AddToFeatureList called %u times \n",numcorners);

  free(corner_list);
  //fprintf(stderr,"free called \n");

  //  fprintf(stderr,"Extract features returns %u corners \n",numcorners);
    return numcorners;
}





