#include "visual_system.h"
#include "configuration.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "webinterface.h"

// TO SET VIDEO MODE V4L2_PIX_FMT_RGB24
#include <linux/videodev2.h>

int acquire_width=320,acquire_height=240;
int width=320,height=240;
char * right_frame_temporary = 0;
int has_init = 0;
int has_left = 0 , has_right = 0;


int wait_for_cameras_to_init()
{
        int waittime=0;
        while ( !VideoInput_FeedReceiveLoopAlive(0) ) { printf(".1."); ++waittime; }
        waittime=0;
        while ( !VideoInput_FeedReceiveLoopAlive(1) ) { printf(".2."); ++waittime; }
        fprintf(stderr,"Video loops running!\n");
   return 1;
}

int InitVisualSystem()
{

    //LoadConfiguration(); // Pull Devices :P

    VideoInput_InitializeLibrary(2);
    int camerasok=0;

    struct VideoFeedSettings feedsettings={0};
    feedsettings.PixelFormat=V4L2_PIX_FMT_RGB24;


    camerasok+=VideoInput_OpenFeed(0,video_device_1,acquire_width,acquire_height,24,25,1,feedsettings);
    camerasok+=VideoInput_OpenFeed(1,video_device_2,acquire_width,acquire_height,24,25,1,feedsettings);
    usleep(200);

    if ( (camerasok==2) )
      {
          if ( !wait_for_cameras_to_init() ) { fprintf(stderr,"CamerasTimed out!!\n"); /*return 0; The rest of the libs must be inited regardless of the failure of the visual system , or else the whole thing will segfault */
                                                  }
      } else
      {
       fprintf(stderr,"Failed to init Video Feeds!\n");
       /*return 0; The rest of the libs must be inited regardless of the failure of the visual system , or else the whole thing will segfault */
      }


    VisCortx_Start(width,height,ENVDIR);

    /*
         THE FOLLOWING CODE IS KIND OF BAD :P  , I WILL GRADUALLY MOVE RESECTIONING TO VIDEO INPUT SO
         IT MAKES SENSE , BUT IT IS BAD FOR NOW
     */
    VideoInput_SetCameraParameter(0,2,&camera_params_1[2]); VideoInput_SetCameraParameter(0,3,&camera_params_1[3]); VideoInput_SetCameraParameter(0,4,&camera_params_1[4]); VideoInput_SetCameraParameter(0,5,&camera_params_1[5]);
    VideoInput_SetCameraParameter(0,6,&camera_params_1[6]); VideoInput_SetCameraParameter(0,7,&camera_params_1[7]); VideoInput_SetCameraParameter(0,8,&camera_params_1[8]); VideoInput_SetCameraParameter(0,9,&camera_params_1[9]);
    VideoInput_SetCameraParameter(0,10,&camera_params_1[10]);

    double fx=VideoInput_GetCameraParameter(0,2) , fy=VideoInput_GetCameraParameter(0,3) , cx=VideoInput_GetCameraParameter(0,4) , cy=VideoInput_GetCameraParameter(0,5);
    double k1=VideoInput_GetCameraParameter(0,6) , k2=VideoInput_GetCameraParameter(0,7) , p1=VideoInput_GetCameraParameter(0,9) , p2=VideoInput_GetCameraParameter(0,10) , k3=VideoInput_GetCameraParameter(0,8);
    VisCortx_CameraParameters(0,fx,fy,cx,cy,k1,k2,p1,p2,k3);


    VideoInput_SetCameraParameter(1,2,&camera_params_2[2]); VideoInput_SetCameraParameter(1,3,&camera_params_2[3]); VideoInput_SetCameraParameter(1,4,&camera_params_2[4]); VideoInput_SetCameraParameter(1,5,&camera_params_2[5]);
    VideoInput_SetCameraParameter(1,6,&camera_params_2[6]); VideoInput_SetCameraParameter(1,7,&camera_params_2[7]); VideoInput_SetCameraParameter(1,8,&camera_params_2[8]); VideoInput_SetCameraParameter(1,9,&camera_params_2[9]);
    VideoInput_SetCameraParameter(1,10,&camera_params_2[10]);

    fx=VideoInput_GetCameraParameter(1,2) , fy=VideoInput_GetCameraParameter(1,3) , cx=VideoInput_GetCameraParameter(1,4) , cy=VideoInput_GetCameraParameter(1,5);
    k1=VideoInput_GetCameraParameter(1,6) , k2=VideoInput_GetCameraParameter(1,7) , p1=VideoInput_GetCameraParameter(1,9) , p2=VideoInput_GetCameraParameter(1,10) , k3=VideoInput_GetCameraParameter(1,8);
    VisCortx_CameraParameters(1,fx,fy,cx,cy,k1,k2,p1,p2,k3);
    /*
        END OF BAD CODE -----------------------------------------------------------------------------
     */


    VisCortx_SetCamerasGeometry(6.5,72.0,0.0,0.0);

    right_frame_temporary  = (char *) malloc( width * height * 3 * sizeof(char));

    has_init=1;
    return 1;
}

int PassVideoInputToCortex(unsigned int clock_time)
{
  void *frame1=0 ,*frame2 = 0;


 // GET INPUT  ( SWAPED OR NOT! )
 if ( swap_inputs == 0 ) { frame1=VideoInput_GetFrame(0); frame2=VideoInput_GetFrame(1); } else
                         { frame1=VideoInput_GetFrame(1); frame2=VideoInput_GetFrame(0); }

 VisCortx_SetTime(clock_time);

  if ( VideoInput_NewFrameAvailiable(0) )
   {
      if ( frame1 != 0 )
       {
         has_left = 1;
       } else
       {
          /* FRAME1 IS DEAD*/
       }
   }

  if ( VideoInput_NewFrameAvailiable(1) )
  {
       if ( frame2 != 0 )
       {
         has_right = 1;
       } else
       {
          /* FRAME2 IS DEAD*/
       }
  }

 /*
  * Frames should be signaled processed AFTER they have been passed to Visual Cortex :P
  */

  if ( ( has_left ) && ( has_right ) )
   {
     memcpy(right_frame_temporary,frame2,width*height*3);
     if ( VisCortX_NewFrame(LEFT_EYE,width,height,3,(unsigned char *)frame1) )  { VideoInput_SignalFrameProcessed(0); VideoInput_SignalFrameProcessed(1); }
     if ( VisCortX_NewFrame(RIGHT_EYE,width,height,3,(unsigned char *)right_frame_temporary) ) {  }
     has_right = 0;
     has_left = 0;
   }


 return 1;
}


int CloseVisualSystem()
{
  free(right_frame_temporary);
  right_frame_temporary =0;

    VideoInput_DeinitializeLibrary();
    VisCortx_Stop();
  return 1;
}
