#ifndef POINTS3D_H_INCLUDED
#define POINTS3D_H_INCLUDED

#include "VisionMemory.h"
#include "FeatureLists.h"


int Multiply3DPointsWithMatrix(struct FeatureList * list,struct TransformationMatrix * rotation_and_translation_matrix);

#endif // POINTS_3D_H_INCLUDED
