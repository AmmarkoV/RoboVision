#ifndef VIDEOINPUTADAPTER_H_INCLUDED
#define VIDEOINPUTADAPTER_H_INCLUDED

unsigned int PassNewFrameFromVideoInput(unsigned int input_img_regnum,unsigned int size_x,unsigned int size_y,unsigned int depth,unsigned char * rgbdata);
unsigned int Pipeline_Stereo_Frames_Collected_Actions();
int UpdateStatistics(unsigned int last_frame_microseconds, float last_fps);

#endif // VIDEOINPUTADAPTER_H_INCLUDED
