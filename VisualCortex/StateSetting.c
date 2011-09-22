#include "StateSetting.h"
#include "VisionMemory.h"
#include "FaceDetection.h"
#include "IntegralImageConversion.h"
#include "FeatureLists.h"
#include <time.h>
#include <math.h>


void SetGuardBytes()
{
    settings[SETTINGS_GUARD_BYTES_0]=123;
    settings[SETTINGS_GUARD_BYTES_1]=123;
    settings[SETTINGS_GUARD_BYTES_2]=123;
    settings[SETTINGS_GUARD_BYTES_3]=123;
}



int GuardBytesOk()
{
    if ( settings[SETTINGS_GUARD_BYTES_0]!=123 ) { return 0; }
    if ( settings[SETTINGS_GUARD_BYTES_1]!=123 ) { return 0; }
    if ( settings[SETTINGS_GUARD_BYTES_2]!=123 ) { return 0; }
    if ( settings[SETTINGS_GUARD_BYTES_3]!=123 ) { return 0; }

    return 1;
}













unsigned int SetCamerasGeometry(float distance_between_cameras,float diagonal_field_of_view,float horizontal_field_of_view,float vertical_field_of_view)
{
  if ( ( metrics[RESOLUTION_X]==0 ) || ( metrics[RESOLUTION_Y]==0 ) )
    {
      fprintf(stderr,"Resolution not set.. VisCortx_Start needs to be called before VisCortx_SetCamerasGeometry\n");
      return 0;
    }

  /* Cameras should be parallel.. */
  camera_distance = distance_between_cameras;
  camera_diagonal_field_of_view = diagonal_field_of_view;
  camera_horizontal_field_of_view = horizontal_field_of_view;
  camera_vertical_field_of_view = vertical_field_of_view;
  if ( (camera_diagonal_field_of_view!=0.0) && (camera_horizontal_field_of_view==0.0) && (camera_vertical_field_of_view==0.0) )
    {
      fprintf(stderr,"We need to calculate horizontal and vertical field of view from diagonal , this ( as someone could expect ) is not very precise \n");
      float abs_diagonal_resolution = sqrt (metrics[RESOLUTION_X]*metrics[RESOLUTION_X] + metrics[RESOLUTION_Y]*metrics[RESOLUTION_Y]);
      if ( abs_diagonal_resolution == 0 ) { fprintf(stderr,"Could not find horizontal/vertical field of view..\nVisCortx_SetCamerasGeometry will have to be run again\n"); }
      camera_horizontal_field_of_view = ( metrics[RESOLUTION_X] / abs_diagonal_resolution ) * diagonal_field_of_view;
      camera_vertical_field_of_view = ( metrics[RESOLUTION_Y] / abs_diagonal_resolution ) * diagonal_field_of_view;
    }


  fprintf(stderr,"Camera Geometry set to : \n");
  fprintf(stderr,"Distance between cameras : %f \n",camera_distance);
  fprintf(stderr,"Field of view ( diagonal : %f , horizontal : %f , vertical : %f)\n",camera_diagonal_field_of_view,camera_horizontal_field_of_view,camera_vertical_field_of_view);

  return 1;
}



unsigned int InitializeEverything(unsigned int res_x,unsigned int res_y)
{

   fprintf(stderr,"Visual Cortex , Things todo memo : \n");
   fprintf(stderr,"Check if precalculations are correct\n");
   fprintf(stderr,"Improve filters using a single real pointer\n");
   fprintf(stderr,"Fix Feature tracking!\n");
   fprintf(stderr,"\n");
   fprintf(stderr,"---------------------------------------\n");

   //unsigned int MAX_INT=-1;
   //fprintf(stderr,"MAX_Integer is %u",MAX_INT);

       //e(mach) calculation
    float e1=1.0;
    while (1+e1 >1)
     {
        e1 = e1/2;
     }

    double e2=1.0;
    while (1+e2 >1)
     {
        e2 = e2/2;
     }

    fprintf(stderr,"Machine numerical error , single : %e , double : %e \n",e1,e2);

   InitFaceRecognition(res_x,res_y);
    int i= InitVisionMemory(res_x,res_y);


    VisCortx_SelfCheck();
    return  i;
}

unsigned int CloseEverything()
{
   CloseFaceRecognition();
   return  CloseVisionMemory();
}

unsigned int SelfCheckEverything()
{
    fprintf(stderr,"VisCortx_SelfCheck starting!\n");
    TestIntegralImaging();


   // fprintf(stderr,"GenerateCompressHistogramOfImage Beeing called!\n");
   // GenerateCompressHistogramOfImage(video_register[LEFT_EYE].pixels,l_video_register[HISTOGRAM_COMPRESSED_LEFT].pixels,30,30);
   // fprintf(stderr,"GenerateCompressHistogramOfImage is done!\n");
/*    int x=0,y=0;


    for (y=0; y<210; y++)
    {
     for (x=0; x<290; x++)
     {
      fprintf(stderr,"X/Y %u / %u ",x,y);
      struct Histogram hist1={0},hist1_old={0};
      HistogramPatch(&hist1_old,video_register[LEFT_EYE].pixels,x,y,metrics[HORIZONTAL_BUFFER],metrics[VERTICAL_BUFFER]);
      CompressedHistogramPatch(l_video_register[HISTOGRAM_COMPRESSED_LEFT].pixels,&hist1,x,y);

     if (
        (hist1.median_r!=hist1_old.median_r)||
        (hist1.median_g!=hist1_old.median_g)||
        (hist1.median_b!=hist1_old.median_b)
        )
    {
      fprintf(stderr," Miss NEW %u %u %u / OLD %u %u %u  \n",hist1.median_r,hist1.median_g,hist1.median_b,hist1_old.median_r,hist1_old.median_g,hist1_old.median_b);
     } else
     fprintf(stderr,"Success\n ");
    }
   }
   */
   fprintf(stderr,"VisCortx_SelfCheck is done!\n");
   return 0;
}



void SetThresholdsForAllPatchSizes()
{
            unsigned int threshold=settings[DEPTHMAP_COMPARISON_THRESHOLD];
            settings[DEPTHMAP_COMPARISON_THRESHOLD_LARGE_PATCH]=  (unsigned int) ( (threshold * metrics[VERTICAL_BUFFER_LARGE] * metrics[HORIZONTAL_BUFFER_LARGE] ) / (metrics[HORIZONTAL_BUFFER]*metrics[VERTICAL_BUFFER]) );
            settings[DEPTHMAP_COMPARISON_THRESHOLD_EXTRALARGE_PATCH]=  (unsigned int) ( (threshold * metrics[VERTICAL_BUFFER_EXTRALARGE] * metrics[HORIZONTAL_BUFFER_EXTRALARGE] ) / (metrics[HORIZONTAL_BUFFER]*metrics[VERTICAL_BUFFER]) );
            fprintf(stderr,"Also adjusting settings for large and extra large patches to %u and %u \n",settings[DEPTHMAP_COMPARISON_THRESHOLD_LARGE_PATCH],settings[DEPTHMAP_COMPARISON_THRESHOLD_EXTRALARGE_PATCH]);

}


void SetSetting(unsigned int set_num,unsigned int set_val)
{
    if (set_num>=SETTINGS_COUNT) {return;}

    settings[set_num]=set_val;


    if ( set_num == DEPTHMAP_COMPARISON_THRESHOLD )
        { SetThresholdsForAllPatchSizes();  }

}

void SetDefaultSettings()
{
   DefaultSettings();
}

unsigned int GetSetting(unsigned int get_num)
{
    if (get_num>=SETTINGS_COUNT) {return 0;}
    return settings[get_num];
}



unsigned int GetMetric(unsigned int get_num)
{
    if (get_num>=METRICS_COUNT) {return 0;}
    return metrics[get_num];
}


void SetTime(unsigned int thetime)
{
    if ( thetime<TIME_INC) { fprintf(stderr,"VisCortex Clock truncated"); }
TIME_INC=thetime;
}


int GetANewSnapShotFileName(char * result,char * filename_base,char * filename_extension)
{
     strcpy(result,filename_base);
     char timestamp[256]={0};
     time_t t; struct tm *tmp;
     t = time(0); tmp = localtime(&t);
     if (tmp == 0) { fprintf(stderr,"Could not get time ( localtime() ) \n"); }
     if (strftime(timestamp, sizeof(timestamp),"_%F_%T_",tmp) == 0) { fprintf(stderr, "strftime returned 0");  }
     strcat(result,timestamp);
     strcat(result,filename_extension);

  return 1;
}


unsigned int GetTime()
{
    return TIME_INC;
}
/*
 ----------------- INITIALIZATION ----------------------
*/

