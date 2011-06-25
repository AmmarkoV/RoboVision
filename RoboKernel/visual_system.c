#include "visual_system.h"
#include "configuration.h"
#include <stdio.h>
#include <stdlib.h>
#include "webinterface.h"

// TO SET VIDEO MODE V4L2_PIX_FMT_RGB24
#include <linux/videodev2.h>

int acquire_width=320,acquire_height=240;
int width=320,height=240;
int has_init = 0;

int wait_for_cameras_to_init()
{
        int waittime=0;
        while ( !FeedReceiveLoopAlive(0) ) { printf(".1."); ++waittime; }
        waittime=0;
        while ( !FeedReceiveLoopAlive(1) ) { printf(".2."); ++waittime; }
        fprintf(stderr,"Video loops running!\n");
   return 1;
}

int InitVisualSystem()
{

    LoadConfiguration(); // Pull Devices :P

    InitVideoInputs(2);
    int camerasok=0;

    struct VideoFeedSettings feedsettings={0};
    feedsettings.PixelFormat=V4L2_PIX_FMT_RGB24;


    camerasok+=InitVideoFeed(0,video_device_1,acquire_width,acquire_height,24,1,feedsettings);
    camerasok+=InitVideoFeed(1,video_device_2,acquire_width,acquire_height,24,1,feedsettings);
    if ( (camerasok==2) )
      {
          if ( !wait_for_cameras_to_init() ) { fprintf(stderr,"CamerasTimed out!!\n"); /*return 0; The rest of the libs must be inited regardless of the failure of the visual system , or else the whole thing will segfault */
                                                  }
      } else
      {
       fprintf(stderr,"Failed to init Video Feeds!\n");
       /*return 0; The rest of the libs must be inited regardless of the failure of the visual system , or else the whole thing will segfault */
      }


    VisCortx_Start(width,height);
    VisCortx_SetCamerasGeometry(6.0,72.0,0.0,0.0);

    OpenWebInterface();

    has_init=1;
    return 1;
}

int PassVideoInputToCortex(unsigned int clock_time)
{
  void *frame1=0 ,*frame2 = 0;


 // GET INPUT  ( SWAPED OR NOT! )
 if ( swap_inputs == 0 )
 {
    frame1=GetFrame(0); frame2=GetFrame(1);
 } else
 {
    frame1=GetFrame(1); frame2=GetFrame(0);
 }

 VisCortx_SetTime(clock_time);

  if ( NewFrameAvailiable(0) )
   {
      if ( frame1 != 0 )
       {
         if ( VisCortX_NewFrame(LEFT_EYE,width,height,3,(unsigned char *)frame1) ) { SignalFrameProcessed(0); }
       } else
       {
          /* FRAME1 IS DEAD*/
       }
   }

  if ( NewFrameAvailiable(1) )
  {
       if ( frame2 != 0 )
       {
        if ( VisCortX_NewFrame(RIGHT_EYE,width,height,3,(unsigned char *)frame2) ) { SignalFrameProcessed(1); }
       } else
       {
          /* FRAME2 IS DEAD*/
       }
  }

 /*
  * Frames should be signaled processed AFTER they have been passed to Visual Cortex :P
  */


 return 1;
}


int CloseVisualSystem()
{
    CloseWebInterface();

    CloseVideoInputs();
    VisCortx_Stop();
  return 1;
}
