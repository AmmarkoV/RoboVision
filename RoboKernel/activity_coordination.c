#include "activity_coordination.h"
#include "visual_system.h"
#include "../MotorFoundation/MotorHAL.h"

int current_activity=0;


#define RESX 320
#define RESY 240

int center_of_attention_lost = 0 ;

int TrackFace()
{
  unsigned int frame_to_use = 0;
  if  (VisCortx_GetFaces(CALIBRATED_LEFT_EYE,0,TOTAL_POINTS) > 0)  {  frame_to_use = CALIBRATED_LEFT_EYE;} else
  if  (VisCortx_GetFaces(CALIBRATED_RIGHT_EYE,0,TOTAL_POINTS) > 0) {  frame_to_use = CALIBRATED_RIGHT_EYE;}

  if  (  frame_to_use!=0 )
    {
         center_of_attention_lost=0;
         unsigned int x = VisCortx_GetFaces(frame_to_use,0,FEATURE_X) +  VisCortx_GetFaces(frame_to_use,0,FEATURE_DIM_X)  / 2;
         unsigned int y = VisCortx_GetFaces(frame_to_use,0,FEATURE_Y) +  VisCortx_GetFaces(frame_to_use,0,FEATURE_DIM_Y) / 2;


         unsigned int  cam_rotation,cam_pitch;
         RobotGetHeadPose(&cam_rotation,&cam_pitch);

         if ( y < (RESY/2)-8 )
           {
              --cam_pitch;
              RobotSetHeadPose(cam_rotation,cam_pitch);
              //fprintf(stderr,"FACE UP ");
           } else
         if ( y > (RESY/2)+8 )
           {
              ++cam_pitch;
              RobotSetHeadPose(cam_rotation,cam_pitch);
              //fprintf(stderr,"FACE DOWN");
           } else
            {
                /* DEAD ZONE THAT IS CONSIDERED CENTER ENOUGHT :P */
            }


         RobotSetLightsState(0,1);
        /*
           when rotation servo will be bought there will be rotation code too here

        */


    } else
    {
        ++center_of_attention_lost;
        if ( center_of_attention_lost >1200 )
          {
            RobotSetLightsState(0,0);
            RobotSetHeadPose(90,90);
            center_of_attention_lost=0;
          }
    }

  return 1;
}






























// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// OUTSIDE INTERFACES FOLLOW
int EngageActivity(int activity_num)
{
  current_activity = activity_num;
  return 1;
}


int StopCurrentActivity()
{
  current_activity = IDLE;
  return 1;
}


int ActivityLoop()
{
  switch (current_activity)

  {
      case IDLE :
      break;

      case FACE_TRACKING:
         return TrackFace();
      break;
  }
 return 1;
}
