#include "CameraPose.h"
#include "Precalculations.h"
#include "Matrix.h"
#include "LinearAlgebra.h"

struct TransformationMatrix left_homography;
struct TransformationMatrix right_homography;

struct TransformationMatrix total_left_rotation;
struct TransformationMatrix total_right_rotation;

struct TransformationMatrix left_rotation;
struct TransformationMatrix right_rotation;


struct TransformationMatrix left_translation;
struct TransformationMatrix right_translation;

struct TransformationMatrix left_rotation_and_translation;
struct TransformationMatrix right_rotation_and_translation;

struct TransformationMatrix total_left_rotation_and_translation;
struct TransformationMatrix total_right_rotation_and_translation;


/*

             THIS IS A BIG ENOUGH PART THAT HAS TO BE WRITTEN

             THAT WILL TRACK THE CAMERA MOVEMENT AND ALSO  KEEP SNAPSHOTS AS "KEYFRAMES"

             IN ORDER FOR THE SYSTEM TO BE ABLE TO WITHSTAND LOSS OF TRACKING..!

*/


int UpdateCameraPose(unsigned int reg_num)
{

              if (reg_num == CALIBRATED_LEFT_EYE )
                {
                  if ( settings[USE_OPENCV] ) { ComputeHomographyFromPointCorrespondanceOpenCV(video_register[reg_num].features,
                                                                                              &left_calibration_data,
                                                                                              &left_rotation,
                                                                                              &left_translation,
                                                                                              &left_rotation_and_translation,
                                                                                              &left_homography);
                                              }


                 struct TransformationMatrix tmp_matrix;
                 CopyMatrixToMatrix(&tmp_matrix,&total_left_rotation);
                 Multiply4x4Matrices(&total_left_rotation,&left_rotation,&tmp_matrix);


                } else
               if (reg_num == CALIBRATED_RIGHT_EYE )
                {
                  if ( settings[USE_OPENCV] ) { ComputeHomographyFromPointCorrespondanceOpenCV(video_register[reg_num].features,
                                                                                               &right_calibration_data,
                                                                                               &right_rotation,
                                                                                               &right_translation,
                                                                                               &right_rotation_and_translation,
                                                                                               &right_homography);
                                              }

                 struct TransformationMatrix tmp_matrix;
                 CopyMatrixToMatrix(&tmp_matrix,&total_right_rotation);
                 Multiply4x4Matrices(&total_right_rotation,&right_rotation,&tmp_matrix);


                }
  return 1;
}
