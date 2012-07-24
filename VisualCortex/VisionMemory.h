#ifndef VISIONMEMORY_H_INCLUDED
#define VISIONMEMORY_H_INCLUDED

#include "FeatureLists.h"
#include "VisualCortex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char BYTE;
typedef unsigned char BOOLEAN;

struct DepthData
{   // Declare PERSON struct type
        unsigned int score;

        unsigned short depth;
        unsigned short depth_raw;
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
        unsigned int   choices_rejected;

        float X;
        float Y;
        float Z;

};

struct ImageRegion
{
        unsigned int x1;
        unsigned int y1;
		unsigned int width;
        unsigned int height;
};

struct TransformationMatrix
{
    unsigned int rows;
    unsigned int columns;
    float item[17];
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
  unsigned int GUARD_BYTE1;

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
  unsigned int GUARD_BYTE1;

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
  unsigned int GUARD_BYTE1;

  struct FeatureList * features;
  struct FeatureList * faces;
};

extern unsigned int TIME_INC;
extern unsigned int TIME_START;
extern unsigned int COLD_START;

extern unsigned int pipeline_switches[PIPELINE_SWITCH_COUNT];
extern unsigned int settings[SETTINGS_COUNT];
extern unsigned int metrics[METRICS_COUNT];
extern struct VideoRegister video_register[REGISTERS_COUNT];
extern struct LargeVideoRegister l_video_register[LARGE_REGISTERS_COUNT];
extern struct ExtraLargeVideoRegister xl_video_register[EXTRA_LARGE_REGISTERS_COUNT];
extern struct DepthData * depth_data_array;
extern float camera_distance;
extern float camera_diagonal_field_of_view,camera_horizontal_field_of_view,camera_vertical_field_of_view;

extern float depth_units_in_cm[256];

extern char VisCortxENV[512];


int SetImageRegion( struct ImageRegion * ir , unsigned int x1,unsigned int y1,unsigned int width,unsigned int height);

// INIT / DEINIT
int VideoRegisterRequestIsOk(unsigned int reg_num, unsigned int res_x,unsigned int res_y,unsigned int depth);
int LargeVideoRegisterRequestIsOk(unsigned int reg_num, unsigned int res_x,unsigned int res_y,unsigned int depth);
int ExtraLargeVideoRegisterRequestIsOk(unsigned int reg_num, unsigned int res_x,unsigned int res_y,unsigned int depth);
void DefaultSettings();


int ClearVideoRegister(struct VideoRegister * reg_num);
int ClearLargeVideoRegister(struct LargeVideoRegister * reg_num);
int ClearExtraLargeVideoRegister(struct ExtraLargeVideoRegister *  reg_num);

int InitVisionMemory(unsigned int res_x,unsigned int res_y);
int CloseVisionMemory();

unsigned char CheckRegistersForSynchronization(struct VideoRegister * source,struct VideoRegister * target);
void MarkRegistersAsSynced(struct VideoRegister * source,struct VideoRegister * target);

void CopyPartOfImageToImage(unsigned char * input_img,unsigned char * output_img,unsigned int px,unsigned int py,unsigned int tx,unsigned int ty,unsigned int size_x,unsigned int size_y);
int CopyRegister(struct VideoRegister * source,struct VideoRegister * target,unsigned int copy_features,unsigned int copy_faces);
int SwapRegister(unsigned int source,unsigned int target);

int ThisIsA3ByteRegister(struct VideoRegister * reg);
int ThisIsA1ByteRegister(struct VideoRegister * reg);

void ConvertRegisterFrom3ByteTo1Byte(struct VideoRegister * in_reg);
void ConvertRegisterFrom1ByteTo3Byte(struct VideoRegister * in_reg);

int PrintRegister(char * filename,unsigned int reg_num);
int PrintExtraLargeRegister(char * filename,unsigned int reg_num);
int SaveRegisterToFile(char * filename,unsigned int reg_num);
int SaveRegisterPartToFile(char * filename,unsigned int reg_num,unsigned int x_start,unsigned int y_start ,unsigned int width,unsigned int height);
int LoadRegisterFromFile(char * filename,unsigned int reg_num);


struct VideoRegister * GetTempRegister();
unsigned int StopUsingVideoRegister(struct VideoRegister * thereg);

unsigned int MarkVideoRegistersAsUnsynced(struct VideoRegister * unsync_reg , struct VideoRegister * other_reg);

#endif // VISIONMEMORY_H_INCLUDED
