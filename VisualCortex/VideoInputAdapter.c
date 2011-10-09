#include "VideoInputAdapter.h"
#include "VisualCortex.h"
#include "VisionMemory.h"
#include "DisparityDepthMap.h"
#include "Precalculations.h"
#include "MovementRegistration.h"
#include "VisCortexFilters.h"
#include "VisCortexConvolutionFilters.h"
#include "FeatureExtraction.h"
#include "PatternRecognition.h"
#include "FeatureTracking.h"
#include "CameraPose.h"
#include "Matrix.h"
#include "FaceDetection.h"
#include "StateSetting.h"
#include "VisCortexTimer.h"
#include "IntegralImageConversion.h"
#include "VisCortexTimer.h"
#include "LinearAlgebra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 THIS FUNCTION IS CALLED EVERY TIME A NEW FRAME IS READY FOR STEREO VISION
*/
inline unsigned int FrameAcquisition
   (

     unsigned int REG_EYE,
     unsigned int REG_CALIBRATED_EYE,
     unsigned int REG_EDGES,
     unsigned int REG_SECOND_DERIVATIVE,
     unsigned int REG_MOVEMENT,
     unsigned int REG_HISTOGRAM,

     unsigned int * REG_CALIBRATION_PRECALCULATION,

     unsigned int REG_LAST_EYE,
     unsigned int REG_LAST_CALIBRATED_EYE,
     unsigned int REG_LAST_EDGES,
     unsigned int REG_LAST_SECOND_DERIVATIVE,
     unsigned int REG_LAST_MOVEMENT,

          // RAW IMAGE
     unsigned int size_x,unsigned int size_y,unsigned int depth,
     unsigned char * rgbdata
   )

 {
       // FIRST STORE OLD REGISTERS
       SwapRegister(REG_EYE,REG_LAST_EYE);
       SwapRegister(REG_CALIBRATED_EYE,REG_LAST_CALIBRATED_EYE);
       SwapRegister(REG_EDGES,REG_LAST_EDGES);
       SwapRegister(REG_SECOND_DERIVATIVE,REG_LAST_SECOND_DERIVATIVE);
       SwapRegister(REG_MOVEMENT,REG_LAST_MOVEMENT);

       // SECOND PASS NEW IMAGE
       VisCortx_WriteToVideoRegister(REG_EYE,size_x,size_y,depth,rgbdata);
       video_register[REG_EYE].time=TIME_INC;
       CalibrateImage(REG_EYE,REG_CALIBRATED_EYE,REG_CALIBRATION_PRECALCULATION);

   return 1;
 }

inline unsigned int FrameProcessing
   (

     unsigned int REG_EYE,
     unsigned int REG_CALIBRATED_EYE,
     unsigned int REG_EDGES,
     unsigned int REG_SECOND_DERIVATIVE,
     unsigned int REG_MOVEMENT,
     unsigned int REG_HISTOGRAM,

     unsigned int REG_GROUP_MOVEMENT,
     unsigned int REG_GROUP_EDGES,
     unsigned int REG_GROUP_EDGES_PRESENCE,
     unsigned int REG_GROUP_SECOND_DERIVATIVE,

     unsigned int REG_LAST_CALIBRATED_EYE,

     // Switches ..
     unsigned int ADD_EDGES_PRESENCE_SWITCH ,
     unsigned int left_right_switch

     // RAW IMAGE
    // unsigned int size_x,unsigned int size_y,unsigned int depth,
    // unsigned char * rgbdata

   )
{
       // THIRD PROCESS NEW IMAGE  ( COMPRESSION )
       PrepareCleanSobeledGaussianAndDerivative
         (   REG_CALIBRATED_EYE,
             REG_EDGES,
             REG_SECOND_DERIVATIVE,
             settings[DEPTHMAP_EDGE_LOW_STRICTNESS],
             settings[DEPTHMAP_EDGE_HIGH_STRICTNESS]
         );

       GenerateCompressHistogramOfImage
         ( video_register[REG_CALIBRATED_EYE].pixels,
           l_video_register[REG_HISTOGRAM].pixels,
           metrics[HORIZONTAL_BUFFER],
           metrics[VERTICAL_BUFFER]
         );

      if ( ADD_EDGES_PRESENCE_SWITCH )
       {
           unsigned int TMP_REGISTER = GetTempRegister();
           if (TMP_REGISTER == 0 ) { fprintf(stderr," Error Getting a temporary Video Register ( PassNewFrameFromVideoInput ) \n"); }
           CopyRegister(REG_EDGES,TMP_REGISTER,0,0);
           PixelsOverThresholdSetAsOne(TMP_REGISTER,1);
           CompressRegister(TMP_REGISTER,REG_GROUP_EDGES_PRESENCE);
           StopUsingVideoRegister(TMP_REGISTER);
       }

        CompressRegister(REG_MOVEMENT,REG_GROUP_MOVEMENT);
        CompressRegister(REG_EDGES,REG_GROUP_EDGES);
        CompressRegister(REG_SECOND_DERIVATIVE,REG_GROUP_SECOND_DERIVATIVE);

        // FOURTH TRACK ALL POINTS ON NEW FRAME FROM OLD FRAME
        if ( settings[CALCULATE_MOVEMENT_FLOW ] )
         {
           VisCortx_Movement_Detection((left_right_switch==1),(left_right_switch==1));
         }

        if ( settings[PASS_TO_FACE_DETECTOR] ) { RecognizeFaces(REG_CALIBRATED_EYE); }

        if ( settings[PASS_TO_FEATURE_DETECTOR] )
        {

            if ( settings[USE_OPENCV] )
              {
                  FindAndTrackAllPointsOnRegistersOpenCV(REG_CALIBRATED_EYE,REG_LAST_CALIBRATED_EYE,1000);
              } else
              {
                  VisCortx_AutoAddTrackPoints(left_right_switch);
                  TrackAllPointsOnRegistersBrute(REG_CALIBRATED_EYE,REG_LAST_CALIBRATED_EYE,8000);
              }


            if ( settings[CALCULATE_MOVEMENT_MATRIX] )
             {
                UpdateCameraPose(REG_CALIBRATED_EYE);
             }

        }



        return 1;
}




unsigned int Pipeline_Stereo_Frames_Collected_Actions()
{
  // THESE GET EXECUTED WHEN WE HAVE A PROCESSED STEREO PAIR :P
  if ( pipeline_switches[EXECUTE_DEPTHMAP]==1  )
   {
      pipeline_switches[EXECUTE_DEPTHMAP]=2;
      ExecuteDisparityMappingPyramid();
       //ExecuteDisparityMappingOpenCV();
      VisCortxMillisecondsSleep(1);
      pipeline_switches[EXECUTE_DEPTHMAP]=0;
   }


  return 1;
}


unsigned int Auto_Set_Pipeline_Switches()
{
  if ( pipeline_switches[EXECUTE_DEPTHMAP]==0  )
   {
      // FOR NOW THIS JUST STUPIDLY CALLS FOR A DEPTH MAP EVERY 12 FRAMES
      // THIS WILL HAVE TO MONITOR THE WHOLE CONDITION OF THE VISUAL CORTEX
      // AND HOW MUCH EACH CALL TAKES , FOR NOW ONLY A STUPID THROTTLER IS APPLIED WHEN THERE IS NO MOVEMENT :P
     int should_execute_depth_map = 0;

     if ( ( !settings[CALCULATE_MOVEMENT_FLOW] ) && (metrics[FRAMES_PROCESSED]%6==0) )
          {
           should_execute_depth_map = 1;
          } else
     if  ( (settings[CALCULATE_MOVEMENT_FLOW]) && ( ( metrics[CHANGES_LEFT]>100)||( metrics[CHANGES_RIGHT]>100) ) &&
            (metrics[FRAMES_PROCESSED]%6==0) )
           {
           should_execute_depth_map = 1;
          }

      if (should_execute_depth_map)
            {
              pipeline_switches[EXECUTE_DEPTHMAP]=1;
            }
   }
 return 1;
}



unsigned int PassNewFrameFromVideoInput(unsigned int input_img_regnum,unsigned int size_x,unsigned int size_y,unsigned int depth,unsigned char * rgbdata)
{
   StartTimer(TIMER_PROCESSING_DELAY);

   if ( rgbdata == 0 ) { fprintf(stderr,"VisCortX_NewFrame given zero pointer"); return 0; }


    video_register[input_img_regnum].lock=1;

   int retres = 0;
   if ( input_img_regnum == LEFT_EYE )
    {
          retres += FrameAcquisition
                    ( LEFT_EYE,
                      CALIBRATED_LEFT_EYE,
                      EDGES_LEFT,
                      SECOND_DERIVATIVE_LEFT,
                      MOVEMENT_LEFT,
                      HISTOGRAM_COMPRESSED_LEFT,

                      resection_left_precalc,

                      LAST_LEFT_EYE,
                      LAST_CALIBRATED_LEFT_EYE,
                      LAST_EDGES_LEFT,
                      LAST_SECOND_DERIVATIVE_LEFT,
                      LAST_MOVEMENT_LEFT,

                      size_x,size_y,depth,
                      rgbdata);

           retres += FrameProcessing
                    ( LEFT_EYE,
                      CALIBRATED_LEFT_EYE,
                      EDGES_LEFT,
                      SECOND_DERIVATIVE_LEFT,
                      MOVEMENT_LEFT,
                      HISTOGRAM_COMPRESSED_LEFT,


                      MOVEMENT_GROUPED_LEFT,
                      EDGES_GROUPED_LEFT,
                      EDGES_PRESENCE_GROUPED_LEFT,
                      SECOND_DERIVATIVE_GROUPED_LEFT,

                      LAST_CALIBRATED_LEFT_EYE,

                      1 , // ACTIVATE PRSENCE GROUPED
                      0
                    );


    } else
    if ( input_img_regnum == RIGHT_EYE )
    {
         retres += FrameAcquisition
                    (  RIGHT_EYE,
                      CALIBRATED_RIGHT_EYE,
                      EDGES_RIGHT,
                      SECOND_DERIVATIVE_RIGHT,
                      MOVEMENT_RIGHT,
                      HISTOGRAM_COMPRESSED_RIGHT,

                      resection_right_precalc,

                      LAST_RIGHT_EYE,
                      LAST_CALIBRATED_RIGHT_EYE,
                      LAST_EDGES_RIGHT,
                      LAST_SECOND_DERIVATIVE_RIGHT,
                      LAST_MOVEMENT_RIGHT,

                      size_x,size_y,depth,
                      rgbdata);



      retres += FrameProcessing
                    ( RIGHT_EYE,
                      CALIBRATED_RIGHT_EYE,
                      EDGES_RIGHT,
                      SECOND_DERIVATIVE_RIGHT,
                      MOVEMENT_RIGHT,
                      HISTOGRAM_COMPRESSED_RIGHT,


                      MOVEMENT_GROUPED_RIGHT,
                      EDGES_GROUPED_RIGHT,
                      EDGES_PRESENCE_GROUPED_RIGHT,
                      SECOND_DERIVATIVE_GROUPED_RIGHT,

                      LAST_CALIBRATED_RIGHT_EYE,

                      0 , // ACTIVATE PRSENCE GROUPED
                      1
                    );


    }

   ++metrics[FRAMES_PROCESSED];


   if ( pipeline_switches[VISCORTX_AUTONOMOUS] )
    {
        /*This function auto allocates time for depth maps in order to achieve best possible tracking without delays*/
        Auto_Set_Pipeline_Switches();
    }


  // if ( video_register[LEFT_EYE].time == video_register[RIGHT_EYE].time ) /*This should be the criteria to mark a stereo pair collected */
   if (metrics[FRAMES_PROCESSED]%2==0)
   {
     Pipeline_Stereo_Frames_Collected_Actions(); // <- Activate things in pipe line that need to be done when both frames are collected!
   } else
   {
       //THIS SHOWS THE TIMES OF FRAMES CAPTURED
       //fprintf(stderr,"LEFT_EYE=%u RIGHT_EYE=%u\n",video_register[LEFT_EYE].time,video_register[RIGHT_EYE].time);
   }



  video_register[input_img_regnum].lock=0;
  metrics[VIDEOINPUT_PROCESSING_DELAY_MICROSECONDS] = EndTimer(TIMER_PROCESSING_DELAY);






  if (!GuardBytesOk())
    {
        fprintf(stderr,"\n\n\n\n !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        fprintf(stderr,"GUARD BYTES CORRUPTED , SOMETHING IS OVERWRITING MEMORY .. \n");
        fprintf(stderr,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n\n\n ");
        return 0;
    }

 if ( retres == 2 ) { retres = 1; } else
                    { retres =0;  }
 return retres ;
}


