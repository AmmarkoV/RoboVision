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
#include "FaceDetection.h"
#include "StateSetting.h"
#include "VisCortexTimer.h"
#include "IntegralImageConversion.h"
#include "VisCortexTimer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 THIS FUNCTION IS CALLED EVERY TIME A NEW FRAME IS READY FOR STEREO VISION
*/



inline unsigned int FrameProcessing
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

     unsigned int REG_GROUP_MOVEMENT,
     unsigned int REG_GROUP_EDGES,
     unsigned int REG_GROUP_EDGES_PRESENCE,
     unsigned int REG_GROUP_SECOND_DERIVATIVE,

     // Switches ..
     unsigned int ADD_EDGES_PRESENCE_SWITCH ,
     unsigned int left_right_switch,

     // RAW IMAGE
     unsigned int size_x,unsigned int size_y,unsigned int depth,
     unsigned char * rgbdata

   )
{
       video_register[REG_EYE].lock=1;

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
        VisCortx_Movement_Detection((left_right_switch==1),(left_right_switch==1));

        if ( settings[PASS_TO_FACE_DETECTOR] ) { RecognizeFaces(REG_CALIBRATED_EYE); }

        if ( settings[PASS_TO_FEATURE_DETECTOR] )
        {

            FindAndTrackAllPointsOnRegistersOpenCV(REG_CALIBRATED_EYE,REG_LAST_CALIBRATED_EYE,1000);
            //VisCortx_AutoAddTrackPoints(left_right_switch);
            //TrackAllPointsOnRegistersBrute(REG_CALIBRATED_EYE,REG_LAST_CALIBRATED_EYE,8000);
        }


        video_register[REG_EYE].time=TIME_INC;
        video_register[REG_EYE].lock=0;

        return 1;
}




unsigned int Pipeline_Stereo_Frames_Collected_Actions()
{
  if ( pipeline_switches[EXECUTE_DEPTHMAP]==1  )
   {
      pipeline_switches[EXECUTE_DEPTHMAP]=2;
      ExecuteDisparityMappingPyramid();
      pipeline_switches[EXECUTE_DEPTHMAP]=0;
   }


  return 1;
}


unsigned int PassNewFrameFromVideoInput(unsigned int input_img_regnum,unsigned int size_x,unsigned int size_y,unsigned int depth,unsigned char * rgbdata)
{
   StartTimer(TIMER_PROCESSING_DELAY);

   if ( rgbdata == 0 ) { fprintf(stderr,"VisCortX_NewFrame given zero pointer"); return 0; }

   int retres = 0;
   if ( input_img_regnum == LEFT_EYE )
    {
      retres = FrameProcessing
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

                      MOVEMENT_GROUPED_LEFT,
                      EDGES_GROUPED_LEFT,
                      EDGES_PRESENCE_GROUPED_LEFT,
                      SECOND_DERIVATIVE_GROUPED_LEFT,

                      1 , // ACTIVATE PRSENCE GROUPED
                      0 ,

                      size_x,size_y,depth,
                      rgbdata
                    );

    } else
    if ( input_img_regnum == RIGHT_EYE )
    {
      retres = FrameProcessing
                    ( RIGHT_EYE,
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

                      MOVEMENT_GROUPED_RIGHT,
                      EDGES_GROUPED_RIGHT,
                      EDGES_PRESENCE_GROUPED_RIGHT,
                      SECOND_DERIVATIVE_GROUPED_RIGHT,

                      0 , // ACTIVATE PRSENCE GROUPED
                      1 ,

                      size_x,size_y,depth,
                      rgbdata
                    );
    }

  if ( video_register[CALIBRATED_LEFT_EYE].time == video_register[CALIBRATED_RIGHT_EYE].time )
   {
     Pipeline_Stereo_Frames_Collected_Actions(); // <- Activate things in pipe line that need to be done when both frames are collected!
   }

  metrics[VIDEOINPUT_PROCESSING_DELAY_MICROSECONDS] = EndTimer(TIMER_PROCESSING_DELAY);


  if (!GuardBytesOk())
    {
        fprintf(stderr,"\n\n\n\n !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        fprintf(stderr,"GUARD BYTES CORRUPTED , SOMETHING IS OVERWRITING MEMORY .. \n");
        fprintf(stderr,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n\n\n ");
    }


 return retres ;
}


