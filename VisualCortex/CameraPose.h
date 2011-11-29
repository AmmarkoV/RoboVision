#ifndef CAMERAPOSE_H_INCLUDED
#define CAMERAPOSE_H_INCLUDED

#include "VisionMemory.h"


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


int InitCameraPose();

void PassDepthMapToCameraSystem();
int UpdateCameraPose(unsigned int reg_num);

#endif // CAMERAPOSE_H_INCLUDED
