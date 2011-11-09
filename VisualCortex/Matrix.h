#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "VisionMemory.h"
#include "FeatureLists.h"

int ClearTransformationMatrix(struct TransformationMatrix * matrix);
int SetMatrixToIdentity(unsigned int columns,unsigned int rows,struct TransformationMatrix * matrix);

int PrintMatrix(char * name, struct TransformationMatrix * matrix);

int CopyMatrixToMatrix(struct TransformationMatrix * target_matrix,struct TransformationMatrix * matrix);
int SaveTransformationMatrixToFile(char * filename,struct TransformationMatrix * matrix);

int Multiply4x4Matrices(struct TransformationMatrix * target_matrix,struct TransformationMatrix * mult_1,struct TransformationMatrix * mult_2);
int Multiply3DPointWithMatrix(float * x, float * y , float * z ,struct TransformationMatrix * rotation_and_translation_matrix);
int Multiply3DPointsWithMatrix(struct FeatureList * list,struct TransformationMatrix * rotation_and_translation_matrix);


#endif // POINTS_3D_H_INCLUDED
