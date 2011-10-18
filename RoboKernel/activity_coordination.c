#include "activity_coordination.h"
#include "visual_system.h"
#include "../MotorFoundation/MotorHAL.h"

int current_activity=0;




int TrackFace()
{
  unsigned int frame_to_use = 0;
  if  (VisCortx_GetFaces(CALIBRATED_LEFT_EYE,0,TOTAL_POINTS) > 0)  {  frame_to_use = CALIBRATED_LEFT_EYE;} else
  if  (VisCortx_GetFaces(CALIBRATED_RIGHT_EYE,0,TOTAL_POINTS) > 0) {  frame_to_use = CALIBRATED_RIGHT_EYE;}

  if  (  frame_to_use!=0 )
    {
         unsigned int x = VisCortx_GetFaces(frame_to_use,0,FEATURE_X);
         unsigned int y = VisCortx_GetFaces(frame_to_use,0,FEATURE_Y);


         unsigned int  cam_rotation,cam_pitch;
         RobotGetHeadPose(&cam_rotation,&cam_pitch);

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
