#ifndef FEATURE_TRACKING_H
#define FEATURE_TRACKING_H

#include "VisionMemory.h"


#define  MAX_TRACK_MEM 10000
#define PATCH_SEEK_AREA 30 // Ousiastika tha einai PATCH_SEEK_AREA*2
#define PATCH_SIZE 15//PREPEI NA EINAI PERITOS KAI NA YPARXEI KENTRO SYMMETRIAS
extern unsigned int PATCH_DISPLACEMENT; // PATCH_SIZE div 2 ( gia PATCH_SIZE=9 -> 4 )
extern unsigned int PATCH_SIZE_MULT_3;


void ExecuteTrackPoint(unsigned int from,unsigned int to,unsigned int point_num);
int TrackAllPointsOnRegisters(unsigned int reg_new , unsigned int reg_old , unsigned int timeout);

#endif
