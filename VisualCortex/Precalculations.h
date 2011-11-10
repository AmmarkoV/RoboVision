#ifndef PRECALCULATIONS_H_INCLUDED
#define PRECALCULATIONS_H_INCLUDED


struct CameraCalibrationData
{
  double fx;
  double fy;
  double cx;
  double cy;

  double k1, k2 , p1 , p2 , k3;


  double CameraDistanceMultipliedByFocalLength;

  double intrinsic_parameters_array[9];
};

#define ABSOLUTE_MAX_WIDTH 320
#define ABSOLUTE_MAX_HEIGHT 240

extern unsigned int resection_left_precalc[321*241*3];
extern unsigned int resection_right_precalc[321*241*3];
extern unsigned int CameraDistanceInMM;
extern float CameraDistanceMultipliedByFocalLength;

extern unsigned int precalc_group_block_belong[ABSOLUTE_MAX_WIDTH+1][ABSOLUTE_MAX_HEIGHT+1];
extern unsigned long precalc_memplace_3byte[ABSOLUTE_MAX_WIDTH+1][ABSOLUTE_MAX_HEIGHT+1];
extern unsigned long precalc_memplace_1byte[ABSOLUTE_MAX_WIDTH+1][ABSOLUTE_MAX_HEIGHT+1];

inline unsigned char AbsUCharVDiff(unsigned char param1,unsigned char param2);
inline unsigned char AbsUCharDiff(unsigned char * param1,unsigned char * param2);
inline unsigned int AbsUIntDiff(unsigned int num1,unsigned int num2);

unsigned int RGB(unsigned char R,unsigned char G,unsigned char B);
unsigned int PrecalcResectioning(unsigned int * frame ,  double fx,double fy , double cx,double cy ,
                                                         double k1,double k2 , double p1,double p2 , double k3   );



extern struct CameraCalibrationData left_calibration_data;
extern struct CameraCalibrationData right_calibration_data;

void Precalculations();

#endif // PRECALCULATIONS_H_INCLUDED
