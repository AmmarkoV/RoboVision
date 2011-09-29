#ifndef  _HOMOGRAPHY_POSE_H
#define  _HOMOGRAPHY_POSE_H

#include <cv.h>
#include <highgui.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <GL/glut.h>

using namespace std;

class homography_pose
{
private:
   vector<double> v_opt_param;
   CvCapture* capture;
   IplImage *image;//抓取的每帧图像(彩色)
   IplImage *imageGray;//将抓取的每帧图像转为灰度图像
   IplImage* correspond;
   CvMemStorage* storage;
   IplImage* object;
   CvSeq *objectKeypoints, *objectDescriptors ;
   CvSeq *imageKeypoints, *imageDescriptors;
   CvSURFParams params;
   
   double intri[9];
   CvMat intriMat;

   CvMat* homography;
   CvMat* transformMat;
   //==========methods===========//
   double compareSURFDescriptors( const float* d1, const float* d2, double best, int length );
   int naiveNearestNeighbor( const float* vec, int laplacian,
      const CvSeq* model_keypoints,
      const CvSeq* model_descriptors );
   void findPairs( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
      const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, vector<int>& ptpairs );
   void PrintMat(CvMat* A);
   CvMat * inverseCalibrationMatrix( CvMat *in );
   void ComposeRotationTranslationTo3x4Matrix( CvMat* RotTrans , CvMat* Rot, CvMat*Trans );
   CvMat* CreateHomographyRotationTranslationMatrix( CvMat* m_homography,CvMat* m_intric );   
public:
   homography_pose();
   ~homography_pose();
   void cvForLoop();
   void init();

   float tranMat[16];

};
#endif
