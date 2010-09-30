#ifndef PATCH_TRACKING_H
#define PATCH_TRACKING_H

#include "VisionMemory.h"


#define  MAX_TRACK_MEM 10000
#define PATCH_SEEK_AREA 40 // Ousiastika einai PATCH_SEEK_AREA*2
#define PATCH_SIZE 15//PREPEI NA EINAI PERITOS KAI NA YPARXEI KENTRO SYMMETRIAS
extern unsigned int PATCH_DISPLACEMENT; // PATCH_SIZE div 2 ( gia PATCH_SIZE=9 -> 4 )
extern unsigned int PATCH_SIZE_MULT_3;

struct TrackPoint
{
   char cam;
   char lost;
   unsigned int lost_since;
   unsigned int score;
   unsigned int original_x;
   unsigned int original_y;
   unsigned int track_x;
   unsigned int track_y;
   unsigned int group;
   unsigned char original_block[PATCH_SIZE*PATCH_SIZE*3]; // 9x9 x RGB , center pixel 41
   unsigned char now_block[PATCH_SIZE*PATCH_SIZE*3]; // 9x9 x RGB , center pixel 41
};


unsigned int AddPointToTrackList(char cam,unsigned int pt_x,unsigned int pt_y,unsigned int group_belong);
void RemoveTrackPointIfTimedOut(unsigned int timeout);
unsigned int GetTrackPoint(unsigned int point_num,char choice,char *pt_cam,unsigned int *pt_x,unsigned int *pt_y);
unsigned int GetTrackData(unsigned int tpoint,unsigned int dat);
void BitBltTrackPointMemToRegister(unsigned int tpoint,unsigned int videoregister,unsigned int x , unsigned int y);
void ApplyTrackPositionAsOriginal(unsigned int tpoint);
unsigned int GetTrackColor(unsigned int tpoint,unsigned int pt_x,unsigned int pt_y);
unsigned int GetOriginalTrackColor(unsigned int tpoint,unsigned int pt_x,unsigned int pt_y);
unsigned int GetPointTrackList();
void ExecuteTrackPoint(unsigned int point_num);

#endif
