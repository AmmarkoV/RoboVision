#ifndef VISCORTEXTIMER_H_INCLUDED
#define VISCORTEXTIMER_H_INCLUDED



enum VisCortTimerList
{
   TIMER_PROCESSING_DELAY = 0 ,
   TIMER_DEPTH_MAP_DELAY ,


   // INTERNAL FUNCTIONS
   WRITE_REGISTER_DELAY ,

   // INTERNAL FUNCTIONS
   COPY_REGISTER_DELAY ,

   // CALIBRATION STAGE
   CALIBRATION_DELAY ,

   // GAUSSIAN CONVOLUTION FILTER
   GAUSSIAN_DELAY ,

   // SOBEL CONVOLUTION FILTER
   FIRST_DERIVATIVE_DELAY ,

   // SECOND DERIVATIVE CONVOLUTION FILTER
   SECOND_DERIVATIVE_DELAY ,

   // PIXEL OVER THRESHOLD FILTER
   PIXEL_OVER_THRESHOLD_DELAY ,

   // MOVEMENT STAGE
   MOVEMENT_RAW_DELAY ,

   // GAUSSIAN BLUR + SOBEL + SECOND DERIV + CONVERSIONS
   PREPARE_GRADIENTS_DELAY ,

   // PIXEL OVER THRESHOLD FILTER
   COMPRESS_IMAGE_DELAY ,

   // RECOGNIZE FACES FILTER
   RECOGNIZE_FACES_DELAY ,

   // FIND CORNERS
   FIND_CORNERS_DELAY ,

   // TRACK CORNERS
   TRACK_CORNERS_DELAY ,

   // UPDATE CAMERA POSE
   UPDATE_CAMERA_POSE_DELAY ,

   // PASS CAMERA TO 2D MAP
   PASS_CAMERA_TO_2D_MAP ,


   TOTAL_TIMERS
};


void StartTimer( unsigned int timer_num );
unsigned int EndTimer( unsigned int timer_num );
unsigned int GetLastTimer( unsigned int timer_num );
unsigned int GetAverageTimer( unsigned int timer_num );
unsigned int GetTimesTimerTimed( unsigned int timer_num );

void VisCortxMillisecondsSleep(unsigned int milliseconds);
void VisCortxMicrosecondsSleep(unsigned int microseconds);
#endif // VISCORTEXTIMER_H_INCLUDED
