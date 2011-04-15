// Be sure to link with -lfreenect_sync
#include "KinectInput.h"
#include "../VisualCortex/VisualCortex.h"
#include "libfreenect_sync.h"

freenect_context *f_ctx;
freenect_device *f_dev;

      //char *data=0;
      //unsigned int timestamp=0;

int Kinect_Init()
{
    return 1;
}

int Kinect_Close()
{
   // if ( data != 0 ) { free(data); }
    freenect_sync_stop();
    return 1;
}

int Kinect_GetDepth(void **depth , uint32_t * timestamp,int index , freenect_depth_format fmt )
{
    return freenect_sync_get_depth(depth,timestamp,index,fmt);
}

int Kinect_GetVideo(void **video , uint32_t * timestamp,int index , freenect_video_format fmt )
{
    return freenect_sync_get_video(video,timestamp,index,fmt);
}

