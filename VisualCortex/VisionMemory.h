#ifndef VISIONMEMORY_H_INCLUDED
#define VISIONMEMORY_H_INCLUDED

#include "VisualCortex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char BYTE;
typedef unsigned char BOOLEAN;

struct DepthData{   // Declare PERSON struct type
        unsigned int score;
        unsigned short depth;
        unsigned short edge_count;
        unsigned short x1_patch;
        unsigned short y1_patch;
        unsigned short x2_patch;
        unsigned short y2_patch;
		unsigned short patch_size_x;
        unsigned short patch_size_y;
        unsigned short second_pass_improvement;
};

struct ImageRegion{
        unsigned int x1;
        unsigned int y1;
        unsigned int x2;
        unsigned int y2;
		unsigned int full_x;
        unsigned int full_y;
};

struct Histogram
{
   unsigned char median_r;
   unsigned char median_g;
   unsigned char median_b;
   unsigned char median;
   unsigned char std_deviation;
};

struct VideoRegister
{
  unsigned int size_x;
  unsigned int size_y;
  unsigned int depth;

  unsigned int time;
  char used;

  unsigned char * pixels;
};

struct LargeVideoRegister
{
  unsigned int size_x;
  unsigned int size_y;
  unsigned int depth;

  unsigned int time;
  char used;

  unsigned short * pixels;
};

extern unsigned int TIME_INC;
extern unsigned int COLD_START;

extern unsigned int settings[SETTINGS_COUNT];
extern unsigned int metrics[METRICS_COUNT];
extern struct VideoRegister video_register[REGISTERS_COUNT];
extern struct LargeVideoRegister l_video_register[LARGE_REGISTERS_COUNT];
extern struct DepthData * depth_data_array;

// INIT / DEINIT
int VideoRegisterRequestIsOk(unsigned int reg_num, unsigned int res_x,unsigned int res_y,unsigned int depth);
void DefaultSettings();
void ClearVideoRegister(unsigned int reg_num );

int InitVisionMemory(unsigned int res_x,unsigned int res_y);
int CloseVisionMemory();

int SaveRegisterToFile(char * filename,unsigned int reg_num);
int LoadRegisterFromFile(char * filename,unsigned int reg_num);

#endif // VISIONMEMORY_H_INCLUDED