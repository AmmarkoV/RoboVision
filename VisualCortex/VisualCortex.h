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


enum VisCortxPipelineLabels
{
   EXECUTE_DEPTHMAP = 0 ,
   VISCORTX_AUTONOMOUS  ,


   PIPELINE_SWITCH_COUNT
};


enum VisCortxSettings
{
   EMPTY = 0,

   INPUT_CALIBRATION,


   DISABLE_TIMER_OPERATIONS,
   USE_OPENCV,

   HYPERVISOR_STORE_PERFORMANCE_STATISTICS,


   PASS_TO_WORLD_3D,
   PASS_TO_FACE_DETECTOR,
   PASS_TO_FEATURE_DETECTOR,

   RENDER_IMAGE_WITH_MOVEMENT_COMPENSATED,

   CALCULATE_MOVEMENT_MATRIX,
   CALCULATE_MOVEMENT_FLOW ,
   USE_MOVEMENT_FLOW_FOR_CALCULATION_SKIPPING,
   ALTERNATE_DETECTORS_BETWEEN_EYES_FOR_CALCULATION_SKIPPING,

   REMEMBER_FACES,

   SETTINGS_GUARD_BYTES_0 , // --------------------------------------- //

   DEPTHMAP_DEBUG,
   DEPTHMAP_RGB_MULTIPLIER,
   DEPTHMAP_MOVEMENT_MULTIPLIER,
   DEPTHMAP_SOBEL_MULTIPLIER,
   DEPTHMAP_SECOND_DERIVATIVE_MULTIPLIER,


   DEPTHMAP_START,
   DEPTHMAP_STARTLEFT_X,

   SETTINGS_GUARD_BYTES_1 , // --------------------------------------- //

   DEPTHMAP_DETAIL,
   DEPTHMAP_USE_OPENCV,
   DEPTHMAP_OPENCV_LIKE_OUTPUT,
   DEPTHMAP_EDGE_LOW_STRICTNESS,
   DEPTHMAP_EDGE_HIGH_STRICTNESS,
   DEPTHMAP_INSTANT_DETAIL,
   DEPTHMAP_VERT_SHIFT_UP,
   DEPTHMAP_VERT_SHIFT_DOWN,
   DEPTHMAP_VERT_OFFSET_UP,
   DEPTHMAP_VERT_OFFSET_DOWN,

   DEPTHMAP_COMPARISON_DECIDES_FOR_MORE_PIXELS_RIGHT,
   DEPTHMAP_COMPARISON_DECIDES_FOR_MORE_PIXELS_DOWN,


   DEPTHMAP_COMPARISON_TOO_GOOD_THRESHOLD,
   DEPTHMAP_COMPARISON_THRESHOLD,
   DEPTHMAP_COMPARISON_THRESHOLD_ADDED,
   DEPTHMAP_COMPARISON_THRESHOLD_LARGE_PATCH,
   DEPTHMAP_COMPARISON_THRESHOLD_EXTRALARGE_PATCH,
   DEPTHMAP_COMPARISON_DO_NOT_PERFORM_FULL_COUNT,
   DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_PREVIOUS_PATCH_SIZE_DEPTH,
   DEPTHMAP_COMPARISON_ALSO_REVERSE_CHECK,
   DEPTHMAP_COMPARISON_DO_NOT_PROCESS_FURTHER_THAN_CLOSEST_DEPTH,
   DEPTHMAP_CLOSEST_DEPTH,
   DEPTHMAP_GUESSES,
   DEPTHMAP_IMPROVE_USING_MOVEMENT,
   DEPTHMAP_IMPROVE_USING_HISTOGRAM,
   DEPTHMAP_IMPROVE_FILLING_HOLES,
   DEPTHMAP_IMPROVE_USING_EDGES,
   DEPTHMAP_END,


   SETTINGS_GUARD_BYTES_2 , // --------------------------------------- //

   FEATURE_TRACKING_MAX_MOVEMENT_THRESHOLD,
   FEATURE_TRACKING_COMPARISON_THRESHOLD,

   FEATURE_DETECTION_THRESHOLD,


   PATCH_COMPARISON_START,
   PATCH_COMPARISON_SCORE_MIN,
   PATCH_COMPARISON_LEVELS,
   PATCH_COMPARISON_EDGES_PERCENT_REQUIRED,
   PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_LARGE_PATCH,
   PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_EXTRALARGE_PATCH,
   PATCH_COMPARISON_END,
   PATCH_HIST_THRESHOLD_R, PATCH_HIST_THRESHOLD_G , PATCH_HIST_THRESHOLD_B,

   PATCH_TRACKING_WIDTH,
   PATCH_TRACKING_HEIGHT,


   SETTINGS_GUARD_BYTES_3 , // --------------------------------------- //

   TIME_BETWEEN_TRACKING ,

   MAX_FACES,
   MAX_FEATURES,

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

   FRAMES_PROCESSED ,

   //DEPTH MAP
   LAST_DEPTH_MAP_COVERAGE,
   LAST_DEPTH_MAP_TOO_CLOSE_COVERAGE,



   //STATISTICS
   HISTOGRAM_DENIES,
   COMPAREPATCH_IMMEDIATE_ACCEPTS,
   COMPAREPATCH_REVERSE_ACCEPTS,
   COMPAREPATCH_ALGORITHM_DENIES,
   COMPAREPATCH_TOTAL_CALLS,

   //PERFORMANCE
   VIDEOINPUT_PROCESSING_DELAY_MICROSECONDS,
   DEPTHMAP_DELAY_MICROSECONDS,
   TOTAL_DEPTHMAP_DELAY_MICROSECONDS,
   TOTAL_DEPTHMAPS_PERFORMED,




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

   SECOND_DERIVATIVE_LEFT,
   SECOND_DERIVATIVE_RIGHT,

   LAST_LEFT_EYE,
   LAST_RIGHT_EYE,

   LAST_CALIBRATED_LEFT_EYE,
   LAST_CALIBRATED_RIGHT_EYE,

   LAST_EDGES_LEFT,
   LAST_EDGES_RIGHT,

   LAST_SECOND_DERIVATIVE_LEFT,
   LAST_SECOND_DERIVATIVE_RIGHT,

   LAST_MOVEMENT_LEFT,
   LAST_MOVEMENT_RIGHT,

  // GENERAL REGISTERS MUST BE AT THE END FOR THE GetTempRegister() FUNCTION TO WORK
   GENERAL_1,
   GENERAL_2,
   GENERAL_3,
   GENERAL_4,
   GENERAL_5,

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
    EDGES_GROUPED_LEFT,
    EDGES_GROUPED_RIGHT,
    EDGES_PRESENCE_GROUPED_LEFT,
    EDGES_PRESENCE_GROUPED_RIGHT,
    SECOND_DERIVATIVE_GROUPED_LEFT,
    SECOND_DERIVATIVE_GROUPED_RIGHT,
    EXTRA_LARGE_REGISTERS_COUNT
};


enum FeatureTypeElements
{
    TOTAL_POINTS = 0,
    FEATURE_X ,
    FEATURE_Y ,
    FEATURE_Z ,
    FEATURE_DIM_X ,
    FEATURE_DIM_Y ,
    FEATURE_DIM_Z ,
    FEATURE_LAST_X ,
    FEATURE_LAST_Y ,
    FEATURE_LAST_Z ,
    PRINT_FEATURE_LIST,
    MATCHED_WITH_REG ,
    FEATURE_IS_LOST,
    FEATURE_LAST_TRACK_TIME,

    // ---------------
    TOTAL_FEATURE_ELEMENTS
};


enum OrientationSettins
{
    DIRECTION_UP = 0,
    DIRECTION_DOWN ,
    DIRECTION_LEFT ,
    DIRECTION_RIGHT ,
    // ---------------
    TOTAL_DIRECTIONS
};

char *  VisCortx_Version();
void VisCortx_SetTime(unsigned int thetime);
unsigned int VisCortx_GetTime();

unsigned int VisCortx_Start(unsigned int res_x,unsigned int res_y,char * envpath);
unsigned int VisCortx_Stop();
unsigned int VisCortx_SetCamerasGeometry(float distance_between_cameras,float diagonal_field_of_view,float horizontal_field_of_view,float vertical_field_of_view);
unsigned int VisCortx_SelfCheck();
void VisCortx_SetSetting(unsigned int set_num,unsigned int set_val);
void VisCortx_SetDefaultSettings();
void VisCortx_GetHyperVisorStatus(unsigned int print_std,unsigned int print_file,char * output);
unsigned int VisCortx_GetSetting(unsigned int get_num);
unsigned int VisCortx_GetMetric(unsigned int get_num);
void VisCortx_SetPipelineSwitch(unsigned int set_num,unsigned int set_val);
unsigned int VisCortx_GetPipelineSwitch(unsigned int set_num);
void VisCortx_SetMetric(unsigned int set_num,unsigned int set_val);
void VisCortx_CameraParameters(int right_cam,double fx,double fy,double cx,double cy,double k1,double k2,double p1,double p2,double k3);
unsigned int VisCortx_GetVideoRegisterData(unsigned int input_img_regnum,unsigned int metric_num);
unsigned int VisCortx_VideoRegistersSynced(unsigned int img_regnum1,unsigned int img_regnum2);

unsigned int VisCortX_NewFrame(unsigned int input_img_regnum,unsigned int size_x,unsigned int size_y,unsigned int depth,unsigned char * rgbdata);
unsigned int VisCortX_ClearVideoRegister(unsigned int input_img_regnum);
unsigned int VisCortX_SwapVideoRegisters(unsigned int input_img_regnum,unsigned int output_img_regnum);
unsigned int VisCortX_CopyVideoRegister(unsigned int input_img_regnum,unsigned int output_img_regnum,unsigned int copy_features,unsigned int copy_faces);
unsigned int VisCortX_ConvertVideoRegisterToColorDepth(unsigned int input_img_regnum,unsigned int new_color_depth);
unsigned int VisCortX_BitBltVideoRegister(unsigned int input_img_regnum,unsigned int output_img_regnum,unsigned int px,unsigned int py,unsigned int tx,unsigned int ty,unsigned int size_x,unsigned int size_y);
unsigned int VisCortx_WriteToVideoRegister(unsigned int reg_num,unsigned int size_x,unsigned int size_y,unsigned int depth,unsigned char * rgbdata);
unsigned char * VisCortx_ReadFromVideoRegister(unsigned int reg_num,unsigned int size_x,unsigned int size_y,unsigned int depth);


unsigned int VisCortX_LoadVideoRegisterFromFile(unsigned int reg_num,char * filename);
unsigned int VisCortX_SaveVideoRegisterToFile(unsigned int reg_num,char * filename);
unsigned int VisCortX_SaveVideoRegisterToJPEGFile(unsigned int reg_num,char * filename);
unsigned int VisCortX_SaveVideoRegisterToJPEGMemory(unsigned int reg_num,char *mem,unsigned long * mem_size);

int VisCortx_OperationLockFramesLeftRight();
int VisCortx_OperationUnLockFramesLeftRight();
int VisCortx_CheckTheFramesLeftRightForLock();

void ExecutePipeline();

void  VisCortx_FullDepthMap(unsigned int max_milliseconds);
void VisCorteX_DisparityMapAutoCalibrate(unsigned int max_vertical_error);
unsigned int  VisCortx_Get_DepthMapData(unsigned int typeofdata,unsigned int px,unsigned int py);
unsigned int  VisCortx_GetPatchDescriptor(unsigned int vid_register,unsigned int x,unsigned int y,unsigned int patch_x,unsigned int patch_y,struct PatchSignature * result);

 int VisCortx_ConvolutionFilter(unsigned int reg_in,unsigned int reg_out,signed char * table,signed int divisor,unsigned int table_size);

int VisCortx_Movement_Detection(unsigned int left_cam,unsigned int right_cam);
int VisCortx_Movement_At_Rectangle(unsigned int reg_num , unsigned int x, unsigned int y , unsigned int width , unsigned int height );

int VisCortxGetFundamentalMatrix(float * table,int size_of_table);


// FEATURE EXTRACTION / LISTING / TRACKING
// ------------------------------------------------------------------------------------------
unsigned int  VisCortx_GetTrackedPoints(unsigned int cam);
void  VisCortx_AddTrackPoint(unsigned int cam,unsigned int x,unsigned int y,unsigned int group);
void VisCortx_ClearTrackPoints(unsigned int cam);
void  VisCortx_AutoAddTrackPoints(unsigned int cam);
void  VisCortx_RemoveTimedoutTrackPoints(unsigned int vid_reg,unsigned int timeout);
unsigned int  VisCortx_GetFeature(unsigned int vid_reg,unsigned int point_num,unsigned int data_type);
void  VisCortx_CopyTrackPoints(unsigned int from_vid_reg,unsigned int to_vid_reg);
void  VisCortx_TrackPoints(unsigned int from_vid_reg,unsigned int to_vid_reg);
void  VisCortx_DrawTrackPoints();
void  VisCortx_RenewTrackPoint(unsigned int vid_reg,unsigned int tpoint);
void  VisCortx_RenewAllTrackPoints(unsigned int vid_reg);
// ------------------------------------------------------------------------------------------
// FEATURE EXTRACTION / LISTING / TRACKING


int SobelNDerivative();
void KeepOnlyPixelsClosetoColor(unsigned int src_reg, unsigned int target_reg , unsigned char R,unsigned char G,unsigned char B,unsigned char howclose);

unsigned int VisCortx_RecognizeFaces(unsigned int cam);

unsigned int  VisCortx_GetFaces(unsigned int vid_reg,unsigned int point_num,unsigned int data_type);

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
