#ifndef KINECTINPUT_H_INCLUDED
#define KINECTINPUT_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


// Be sure to link with -lfreenect_sync
#include "libfreenect_sync.h"
 
int Kinect_Init();
int Kinect_Close();
int Kinect_GetDepth(void **depth , uint32_t * timestamp,int index , freenect_depth_format fmt );
int Kinect_GetVideo(void **video , uint32_t * timestamp,int index , freenect_video_format fmt );


#ifdef __cplusplus
}
#endif


#endif // VIDEOINPUT_H_INCLUDED
