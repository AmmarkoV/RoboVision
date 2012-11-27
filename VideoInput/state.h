#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED



#include "V4L2_c.h"
#include "VideoInput.h"
#include "image_storage.h"
#include "PixelFormats.h"
#include "PixelFormatConversions.h"


#define DO_NOT_RETURN_NULL_POINTERS 1
#define VIDEOINPUT_DEBUG 0
#define VIDEOINPUT_INCREASEPRIORITY 0

struct Video
{
  /* DEVICE NAME */
  char * videoinp;
  unsigned int height;
  unsigned int width;
  unsigned int frame_rate;
  unsigned int sleep_time_per_frame_microseconds;
  unsigned int frame_already_passed;

  /* VIDEO 4 LINUX DATA */
  struct v4l2_format fmt;
  void *frame;
  unsigned int size_of_frame;
  //V4L2 *v4l2_intf;
  struct V4L2_c_interface v4l2_interface;

  /* CAMERA INTRINSIC PARAMETERS */
  double fx,fy,cx,cy;
  double k1,k2,p1,p2,k3;

  /* DATA NEEDED FOR DECODERS TO WORK */
  unsigned int input_pixel_format;
  unsigned int input_pixel_format_bitdepth;
  char * decoded_pixels;
  int frame_decoded;

  /*VIDEO SIMULATION DATA*/
  struct Image rec_video;
  int video_simulation;
  int keep_timestamp;
  int compress;

  int jpeg_compressor_running;
  char * mem_buffer_for_recording;
  unsigned long mem_buffer_for_recording_size;

  /* THREADING DATA */
  int thread_alive_flag;
  int snap_paused; /* If set to 1 Will continue to snap frames but not save the reference ( that way video loop wont die out ) */
  int snap_lock; /* If set to 1 Will not snap frames at all ( video loop will die out after a while ) */
  int stop_snap_loop;
  pthread_t loop_thread;
};


extern char video_simulation_path[256];

extern int total_cameras;
extern unsigned char * empty_frame;
extern unsigned int largest_feed_x;
extern unsigned int largest_feed_y;

extern struct Video * camera_feeds;


int StateManagement_ResetVideoFeed(int inpt,char * viddev,int width,int height,int bitdepth,char snapshots_on,struct VideoFeedSettings videosettings);
int StateManagement_ResetFeed(int feednum);
int StateManagement_PauseFeed(int feednum);
int StateManagement_UnpauseFeed(int feednum);


void StateManagement_SetToWebcamRecordOneInMem(int i,char * filename,int timestamp_filename,int compress,char * mem,unsigned long * mem_size);

void StateManagement_SetToPlay(char * filename);
void StateManagement_SetToPlayOne(char * filename);
void StateManagement_SetToRecord(char * filename,int timestamp_filename,int compress);
void StateManagement_SetToRecordOneInMem(char * filename,int timestamp_filename,int compress,char * mem,unsigned long * mem_size);
void StateManagement_SetToRecordOne(char * filename,int timestamp_filename,int compress);
void StateManagement_SetToStop();

unsigned int StateManagement_GetVideoSimulationState();

#endif // STATE_H_INCLUDED
