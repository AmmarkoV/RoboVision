#ifndef VISIONMEMORY_H_INCLUDED
#define VISIONMEMORY_H_INCLUDED

#include "FeatureLists.h"
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
        unsigned short movement_count;
        unsigned short movement_difference;
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
		unsigned int width;
        unsigned int height;
};

struct FundamentalMatrix
{
    float item[9];
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
  unsigned int lock;

  unsigned int size_x;
  unsigned int size_y;
  unsigned int depth;

  unsigned int time;
  char used;

  unsigned char * pixels;

  struct FeatureList * features;
  struct FeatureList * faces;
};

struct LargeVideoRegister
{
  unsigned int lock;

  unsigned int size_x;
  unsigned int size_y;
  unsigned int depth;

  unsigned int time;
  char used;

  unsigned short * pixels;
  struct FeatureList * features;
  struct FeatureList * faces;
};

struct ExtraLargeVideoRegister
{
  unsigned int lock;

  unsigned int size_x;
  unsigned int size_y;
  unsigned int depth;

  unsigned int time;
  char used;

  unsigned int * pixels;
  struct FeatureList * features;
  struct FeatureList * faces;
};

extern unsigned int TIME_INC;
extern unsigned int COLD_START;

extern unsigned int state[STATE_COUNT];
extern unsigned int settings[SETTINGS_COUNT];
extern unsigned int metrics[METRICS_COUNT];
extern struct VideoRegister video_register[REGISTERS_COUNT];
extern struct LargeVideoRegister l_video_register[LARGE_REGISTERS_COUNT];
extern struct ExtraLargeVideoRegister xl_video_register[EXTRA_LARGE_REGISTERS_COUNT];
extern struct DepthData * depth_data_array;
extern float camera_distance;
extern float camera_diagonal_field_of_view,camera_horizontal_field_of_view,camera_vertical_field_of_view;

extern float depth_units_in_cm[256];


int SetImageRegion( struct ImageRegion * ir , unsigned int x1,unsigned int y1,unsigned int width,unsigned int height);

// INIT / DEINIT
int VideoRegisterRequestIsOk(unsigned int reg_num, unsigned int res_x,unsigned int res_y,unsigned int depth);
int LargeVideoRegisterRequestIsOk(unsigned int reg_num, unsigned int res_x,unsigned int res_y,unsigned int depth);
int ExtraLargeVideoRegisterRequestIsOk(unsigned int reg_num, unsigned int res_x,unsigned int res_y,unsigned int depth);
void DefaultSettings();

int ClearVideoRegister(unsigned int reg_num);
int ClearLargeVideoRegister(unsigned int reg_num);
int ClearExtraLargeVideoRegister(unsigned int reg_num);

int InitVisionMemory(unsigned int res_x,unsigned int res_y);
int CloseVisionMemory();



void CopyPartOfImageToImage(unsigned char * input_img,unsigned char * output_img,unsigned int px,unsigned int py,unsigned int tx,unsigned int ty,unsigned int size_x,unsigned int size_y);
int CopyRegister(unsigned int source,unsigned int target,unsigned int copy_features,unsigned int copy_faces);
int SwapRegister(unsigned int source,unsigned int target);

int ThisIsA3ByteRegister(int reg);
int ThisIsA1ByteRegister(int reg);

void ConvertRegisterFrom3ByteTo1Byte(int in_reg);
void ConvertRegisterFrom1ByteTo3Byte(int in_reg);

int PrintRegister(char * filename,unsigned int reg_num);
int PrintExtraLargeRegister(char * filename,unsigned int reg_num);
int SaveRegisterToFile(char * filename,unsigned int reg_num);
int SaveRegisterPartToFile(char * filename,unsigned int reg_num,unsigned int x_start,unsigned int y_start ,unsigned int width,unsigned int height);
int LoadRegisterFromFile(char * filename,unsigned int reg_num);

unsigned int GetTempRegister();
unsigned int StopUsingVideoRegister(unsigned int thereg);

#endif // VISIONMEMORY_H_INCLUDED
