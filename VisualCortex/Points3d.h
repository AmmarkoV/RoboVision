#ifndef POINTS3D_H_INCLUDED
#define POINTS3D_H_INCLUDED

#include "VisionMemory.h"
#include "FeatureLists.h"

extern struct TransformationMatrix left_homography;
extern struct TransformationMatrix right_homography;

extern struct TransformationMatrix total_left_rotation;
extern struct TransformationMatrix total_right_rotation;

extern struct TransformationMatrix left_rotation;
extern struct TransformationMatrix right_rotation;


extern struct TransformationMatrix left_translation;
extern struct TransformationMatrix right_translation;

extern struct TransformationMatrix left_rotation_and_translation;
extern struct TransformationMatrix right_rotation_and_translation;

extern struct TransformationMatrix total_left_rotation_and_translation;
extern struct TransformationMatrix total_right_rotation_and_translation;

int CopyMatrixToMatrix(struct TransformationMatrix * target_matrix,struct TransformationMatrix * matrix);
int SaveTransformationMatrixToFile(char * filename,struct TransformationMatrix * matrix);
int ClearTransformationMatrix(struct TransformationMatrix * matrix);

int Multiply4x4Matrices(struct TransformationMatrix * target_matrix,struct TransformationMatrix * mult_1,struct TransformationMatrix * mult_2);
int Multiply3DPointsWithMatrix(struct FeatureList * list,struct TransformationMatrix * rotation_and_translation_matrix);

int InitCameraPose();


#endif // POINTS_3D_H_INCLUDED
