#ifndef LINEARALGEBRA_H_INCLUDED
#define LINEARALGEBRA_H_INCLUDED


#include "VisionMemory.h"
#include "Precalculations.h"

int GetCameraCoords_From_Angle(float horizontal_angle,float vertical_angle,unsigned int * uint_pixel_x,unsigned int * uint_pixel_y);

int ComputeHomographyFromPointCorrespondanceOpenCV(struct FeatureList * source,struct CameraCalibrationData * calibration,struct TransformationMatrix * E);

int ComputeFundamentalMatrixFromPointCorrespondance(struct FeatureList * list,struct TransformationMatrix * E);

#endif // LINEARALGEBRA_H_INCLUDED
