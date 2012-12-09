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


//These are some calculations that are very frequently used
//They are defined here as macros to make the code easier to read..
#define MEMPLACE3(x,y,width)  ( y * ( width * 3 ) + x*3 )
#define MEMPLACE1(x,y,width)  ( y * ( width ) + x )
#define RGB(r,g,b)  B + (G * 256) + (R * 65536) )
#define ABSDIFF(num1,num2)  ( (num1-num2) >=0 ? (num1-num2) : (num2 - num1) )

extern unsigned int resection_left_precalc[321*241*3];
extern unsigned int resection_right_precalc[321*241*3];
extern unsigned int CameraDistanceInMM;
extern float CameraDistanceMultipliedByFocalLength;

extern unsigned int precalc_group_block_belong[ABSOLUTE_MAX_WIDTH+1][ABSOLUTE_MAX_HEIGHT+1];



//unsigned int RGB(unsigned char R,unsigned char G,unsigned char B);
unsigned int PrecalcResectioning(unsigned int * frame ,  double fx,double fy , double cx,double cy ,
                                                         double k1,double k2 , double p1,double p2 , double k3   );



extern struct CameraCalibrationData left_calibration_data;
extern struct CameraCalibrationData right_calibration_data;

void Precalculations();

#endif // PRECALCULATIONS_H_INCLUDED
