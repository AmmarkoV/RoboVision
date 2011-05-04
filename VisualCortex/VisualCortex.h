#ifndef VISUALCORTEX_H_INCLUDED
#define VISUALCORTEX_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SEGMENTS_SIGNATURE 20
struct PatchSignature
{
   unsigned int total_segments;
   unsigned int segment[MAX_SEGMENTS_SIGNATURE];
};

enum VisCortxSettings
{
   NOTHING = 0,

   DEPTHMAP_START,
   DEPTHMAP_STARTLEFT_X,
   DEPTHMAP_DETAIL,
   DEPTHMAP_EDGE_LOW_STRICTNESS,
   DEPTHMAP_EDGE_HIGH_STRICTNESS,
   DEPTHMAP_INSTANT_DETAIL,
   DEPTHMAP_VERT_OFFSET_UP,
   DEPTHMAP_VERT_OFFSET_DOWN,
   DEPTHMAP_COMPARISON_THRESHOLD,
   DEPTHMAP_COMPARISON_THRESHOLD_ADDED,
   DEPTHMAP_COMPARISON_THRESHOLD_LARGE_PATCH,
   DEPTHMAP_COMPARISON_THRESHOLD_EXTRALARGE_PATCH,
   DEPTHMAP_COMPARISON_DO_NOT_PERFORM_FULL_COUNT,
   DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_CLOSEST_DEPTH,
   DEPTHMAP_CLOSEST_DEPTH,
   DEPTHMAP_GUESSES,
   DEPTHMAP_IMPROVE_USING_MOVEMENT,
   DEPTHMAP_IMPROVE_USING_HISTOGRAM,
   DEPTHMAP_IMPROVE_FILLING_HOLES,
   DEPTHMAP_IMPROVE_USING_EDGES,
   DEPTHMAP_END,

   PATCH_COMPARISON_START,
   PATCH_COMPARISON_SCORE_MIN,
   PATCH_COMPARISON_LEVELS,
   PATCH_COMPARISON_EDGES_PERCENT_REQUIRED,
   PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_LARGE_PATCH,
   PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_EXTRALARGE_PATCH,
   PATCH_COMPARISON_END,
   PATCH_HIST_THRESHOLD_R, PATCH_HIST_THRESHOLD_G , PATCH_HIST_THRESHOLD_B,

   MOVEMENT_PATCH_SENSITIVITY,

   MOVEMENT_R_THRESHOLD, MOVEMENT_G_THRESHOLD , MOVEMENT_B_THRESHOLD ,
   MOVEMENT_MIN_R_THRESHOLD, MOVEMENT_MIN_G_THRESHOLD , MOVEMENT_MIN_B_THRESHOLD ,

   MOVEMENT_ALARM,

   SETTINGS_COUNT
};

enum VisCortxMetrics
{
   RESOLUTION_X = 0,
   RESOLUTION_Y,
   RESOLUTION_X_3_BYTE,
   RESOLUTION_DEPTH,
   RESOLUTION_MEMORY_LIMIT_1BYTE,
   RESOLUTION_MEMORY_LIMIT_3BYTE,
   CHANGES_LEFT,
   CHANGES_RIGHT,

   VERTICAL_BUFFER,
   HORIZONTAL_BUFFER,

   VERTICAL_BUFFER_LARGE,
   HORIZONTAL_BUFFER_LARGE,

   VERTICAL_BUFFER_EXTRALARGE,
   HORIZONTAL_BUFFER_EXTRALARGE,

   GROUP_MOVEMENT_ARRAY_SIZE,

   //STATISTICS
   HISTOGRAM_DENIES,
   COMPAREPATCH_ALGORITHM_DENIES,

   METRICS_COUNT

};

enum VisCortxRegisters
{
   LEFT_EYE = 0,
   RIGHT_EYE,

   LAST_LEFT_OPERATION,
   LAST_RIGHT_OPERATION,

   CALIBRATED_LEFT_EYE,
   CALIBRATED_RIGHT_EYE,

   LAST_LEFT_EYE,
   LAST_RIGHT_EYE,

   BACKGROUND_LEFT,
   BACKGROUND_RIGHT,

   MOVEMENT_LEFT,
   MOVEMENT_RIGHT,

   GROUP_MOVEMENT_LEFT,
   GROUP_MOVEMENT_RIGHT,

   DIFFERENCE_LEFT,
   DIFFERENCE_RIGHT,

   DEPTH_LEFT_VIDEO ,
   DEPTH_RIGHT_VIDEO ,

   EDGES_LEFT,
   EDGES_RIGHT,

   GENERAL_1,
   GENERAL_2,
   GENERAL_3,
   GENERAL_4,

   REGISTERS_COUNT
};

enum VisCortxLargeRegisters
{
    DEPTH_LEFT=0 ,
    DEPTH_RIGHT ,
    HISTOGRAM_COMPRESSED_LEFT,
    HISTOGRAM_COMPRESSED_RIGHT,
    EDGES_COMPRESSED_LEFT,
    EDGES_COMPRESSED_RIGHT,
    GENERAL_LARGE_1,
    GENERAL_LARGE_2,
    LARGE_REGISTERS_COUNT
};

enum VisCortxExtraLargeRegisters
{
    GENERAL_XLARGE_1 = 0,
    GENERAL_XLARGE_2,
    MOVEMENT_GROUPED_LEFT,
    MOVEMENT_GROUPED_RIGHT,
    EXTRA_LARGE_REGISTERS_COUNT
};

char *  VisCortx_Version();
void VisCortx_SetTime(unsigned int thetime);
unsigned int VisCortx_GetTime();

unsigned int VisCortx_Start(unsigned int res_x,unsigned int res_y);
unsigned int VisCortx_Stop();
unsigned int VisCortx_SetCamerasGeometry(float distance_between_cameras,float diagonal_field_of_view,float horizontal_field_of_view,float vertical_field_of_view);
unsigned int VisCortx_SelfCheck();
void VisCortx_SetSetting(unsigned int set_num,unsigned int set_val);
void VisCortx_SetDefaultSettings();
unsigned int VisCortx_GetSetting(unsigned int get_num);
unsigned int VisCortx_GetMetric(unsigned int get_num);
unsigned int VisCortx_GetVideoRegisterStats(unsigned int metric_num);


unsigned int VisCortX_CopyVideoRegister(unsigned int input_img_regnum,unsigned int output_img_regnum);
unsigned int VisCortX_CopyFromVideoToVideoRegister(unsigned int input_img_regnum,unsigned int output_img_regnum);
unsigned int VisCortX_BitBltVideoRegister(unsigned int input_img_regnum,unsigned int output_img_regnum,unsigned int px,unsigned int py,unsigned int tx,unsigned int ty,unsigned int size_x,unsigned int size_y);
unsigned int VisCortx_WriteToVideoRegister(unsigned int reg_num,unsigned int size_x,unsigned int size_y,unsigned int depth,unsigned char * rgbdata);
unsigned char * VisCortx_ReadFromVideoRegister(unsigned int reg_num,unsigned int size_x,unsigned int size_y,unsigned int depth);


unsigned int VisCortX_LoadVideoRegisterFromFile(unsigned int reg_num,char * filename);
unsigned int VisCortX_SaveVideoRegisterToFile(unsigned int reg_num,char * filename);

void  VisCortx_FullDepthMap();
unsigned int  VisCortx_Get_DepthMapData(unsigned int typeofdata,unsigned int px,unsigned int py);
unsigned int  VisCortx_GetPatchDescriptor(unsigned int vid_register,unsigned int x,unsigned int y,unsigned int patch_x,unsigned int patch_y,struct PatchSignature * result);

 int VisCortx_ConvolutionFilter(unsigned int reg_in,unsigned int reg_out,signed char * table,unsigned int table_size);

int VisCortx_Movement_Detection(unsigned int left_cam,unsigned int right_cam);

unsigned int  VisCortx_GetTrackedPoints();
void  VisCortx_AddTrackPoint(unsigned int cam,unsigned int x,unsigned int y,unsigned int group);
void  VisCortx_AutoAddTrackPoints(unsigned int cam);
void  VisCortx_RemoveTimedoutTrackPoints(unsigned int timeout);
unsigned int  VisCortx_GetTrackPoint(unsigned int dat,unsigned int trackpoint);
void  VisCortx_TrackPoints();
void  VisCortx_DrawTrackPoints();
void  VisCortx_RenewTrackPoint(unsigned int tpoint);
void  VisCortx_RenewAllTrackPoints();

int SobelNDerivative();
void KeepOnlyPixelsClosetoColor(unsigned char R,unsigned char G,unsigned char B,unsigned char howclose);

unsigned int VisCortx_RecognizeFaces(unsigned int cam);
void VisCortx_GetFaceNumber(char num,unsigned int *pos_x,unsigned int *pos_y,unsigned int *total_size);


float VisCortx_MinCameraHorizontalAngle();
float VisCortx_MaxCameraHorizontalAngle();
float VisCortx_CameraHorizontalAngleStep();

unsigned short VisCortx_GetDepth_From_Angle(char num,float horizontal_angle,float vertical_angle);
unsigned short VisCortx_SetDepthScale(unsigned short depth_units,float centimeters);
float VisCortx_DepthUnitsToCM(unsigned short depth_units);

#ifdef __cplusplus
}
#endif

#endif // VISUALCORTEX_H_INCLUDED
