#include "state.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


char video_simulation_path[256]={0};


int total_cameras=0;
unsigned char * empty_frame=0;
unsigned int largest_feed_x=320;
unsigned int largest_feed_y=240;

struct Video * camera_feeds=0;


int StateManagement_ResetVideoFeed(int inpt,char * viddev,int width,int height,int bitdepth,char snapshots_on,struct VideoFeedSettings videosettings)
{
  fprintf(stderr,"ResetVideoFeed Not implemented, it should reset feed with new settings\n");
  return 0;
}


int StateManagement_ResetFeed(int feednum)
{
  fprintf(stderr,"ResetFeed Not implemented , it should reset feed with the same settings as the original initialization\n");
  return 0;
}

int StateManagement_PauseFeed(int feednum)
{
 camera_feeds[feednum].snap_paused=1;
 return 1;
}

int StateManagement_UnpauseFeed(int feednum)
{
 camera_feeds[feednum].snap_paused=0;
 return 1;
}










void StateManagement_SetToPlay(char * filename)
{
   /*Needs an implementation*/
   StateManagement_SetToPlayOne(filename);
}

void StateManagement_SetToPlayOne(char * filename)
{
    if ( strlen( filename ) > 250 ) return;

    /*Prepare*/
    strcpy(video_simulation_path,filename);


    int i=0;
    for (i=0; i<total_cameras; i++)  camera_feeds[i].video_simulation = PLAYBACK_ON;

}


void StateManagement_SetToRecord(char * filename,int timestamp_filename,int compress)
{
    if ( strlen( filename ) > 250 ) return;

    int i=0;
    for (i=0; i<total_cameras; i++)
     {
         StateManagement_PauseFeed(i);
           camera_feeds[i].video_simulation = RECORDING_ON;
           camera_feeds[i].keep_timestamp = timestamp_filename;
           camera_feeds[i].compress = compress;
         StateManagement_UnpauseFeed(i);
     }

    strcpy(video_simulation_path,filename);
}



void StateManagement_SetToWebcamRecordOneInMem(int i,char * filename,int timestamp_filename,int compress,char * mem,unsigned long * mem_size)
{

        StateManagement_PauseFeed(i);

          camera_feeds[i].keep_timestamp = timestamp_filename;
          camera_feeds[i].compress = compress;
          camera_feeds[i].mem_buffer_for_recording=mem;
          if (mem_size==0) { camera_feeds[i].mem_buffer_for_recording_size=0; } else
                           { camera_feeds[i].mem_buffer_for_recording_size= (unsigned long) *mem_size; }
          camera_feeds[i].jpeg_compressor_running=1;
          camera_feeds[i].video_simulation = RECORDING_ONE_ON;

        StateManagement_UnpauseFeed(i);


        unsigned int loops = 0;
        unsigned int time_per_loop = 10;
        unsigned int max_time_to_wait_for_ms = 200 /*Milliseconds*/ * 1000;

        if (compress)
         {
           fprintf(stderr,"Waiting for compressor to stop ");
           loops=0;
           while ( (camera_feeds[i].jpeg_compressor_running) &&  (loops*time_per_loop<max_time_to_wait_for_ms) )
                    {
                      fprintf(stderr,".");
                      usleep(time_per_loop);
                      ++loops;
                    }
           if (!(loops*time_per_loop<max_time_to_wait_for_ms)) { fprintf(stderr,"Timed out..\n"); }
         }

        fprintf(stderr,"Waiting for recording to be marked as complete..");
        loops=0;
        while ( (camera_feeds[i].video_simulation == RECORDING_ONE_ON)&&  (loops*time_per_loop<max_time_to_wait_for_ms) )
            {
              fprintf(stderr,".");
              usleep(time_per_loop);
              ++loops;
            }
        if (!(loops*time_per_loop<max_time_to_wait_for_ms)) { fprintf(stderr,"Timed out..\n"); }

        //Copy back result..
        //fprintf(stderr,"StateManagement_SetToWebcamRecordOneInMem copies back %u bytes\n",camera_feeds[i].mem_buffer_for_recording_size);
        if ( (mem_size!=0) && (camera_feeds[i].mem_buffer_for_recording_size!=0) )
         {
           *mem_size = camera_feeds[i].mem_buffer_for_recording_size;
         }
}


void StateManagement_SetToRecordOneInMem(char * filename,int timestamp_filename,int compress,char * mem,unsigned long * mem_size)
{
    if ( strlen( filename ) > 250 ) return;

    int i=0;
    for (i=0; i<total_cameras; i++)
      {
        StateManagement_SetToWebcamRecordOneInMem(i,filename,timestamp_filename,compress,mem,mem_size);
      }
    strcpy(video_simulation_path,filename);
}


void StateManagement_SetToRecordOne(char * filename,int timestamp_filename,int compress)
{
   return StateManagement_SetToRecordOneInMem(filename,timestamp_filename,compress,0,0);
}

void StateManagement_SetToStop()
{
     int i=0;
     for (i=0; i<total_cameras; i++)  camera_feeds[i].video_simulation = LIVE_ON;
}

unsigned int StateManagement_GetVideoSimulationState()
{
  return camera_feeds[0].video_simulation;
}
