#ifndef POINTS3D_H_INCLUDED
#define POINTS3D_H_INCLUDED

#include "VisionMemory.h"
#include "FeatureLists.h"


int CopyMatrixToMatrix(struct TransformationMatrix * target_matrix,struct TransformationMatrix * matrix);
int SaveTransformationMatrixToFile(char * filename,struct TransformationMatrix * matrix);
int ClearTransformationMatrix(struct TransformationMatrix * matrix);

int Multiply4x4Matrices(struct TransformationMatrix * target_matrix,struct TransformationMatrix * mult_1,struct TransformationMatrix * mult_2);
int Multiply3DPointsWithMatrix(struct FeatureList * list,struct TransformationMatrix * rotation_and_translation_matrix);

int InitCameraPose();


#endif // POINTS_3D_H_INCLUDED
