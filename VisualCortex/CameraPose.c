#include "CameraPose.h"
#include "Precalculations.h"
#include "Matrix.h"
#include "LinearAlgebra.h"
#include "../WorldMapping/MasterWorld/MasterWorld.h"

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


                 PrintMatrix("Left rotation",&left_rotation);

                 float  x , y , z , heading , pitch , yaw ;
                 GetAgent(0,&x,&y,&z,&heading,&pitch,&yaw);
                 fprintf(stderr,"Point was %0.2f , %0.2f , %0.2f  and now is ",x,y,z);
                 Multiply3DPointWithMatrix(&x,&y,&z ,&left_rotation);
                 fprintf(stderr," %0.2f , %0.2f , %0.2f  \n",x,y,z);
                 SetAgent(0,x,y,z ,heading,pitch,yaw);


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

                 PrintMatrix("Right rotation",&right_rotation);

                 float  x , y , z , heading , pitch , yaw ;
                 GetAgent(1,&x,&y,&z,&heading,&pitch,&yaw);
                 Multiply3DPointWithMatrix(&x,&y,&z ,&left_rotation);
                 SetAgent(1,x,y,z ,heading,pitch,yaw);

                }
  return 1;
}


int InitCameraPose()
{
/*
 ClearTransformationMatrix(&total_left_rotation);
 total_left_rotation.rows=4 , total_left_rotation.columns=4;
 total_left_rotation.item[0]=1.0;
 total_left_rotation.item[5]=1.0;
 total_left_rotation.item[10]=1.0;
 total_left_rotation.item[15]=1.0;
*/
 SetMatrixToIdentity(4,4,&left_homography);
 SetMatrixToIdentity(4,4,&total_left_rotation);
 SetMatrixToIdentity(4,4,&left_rotation);
 SetMatrixToIdentity(4,4,&left_translation);
 SetMatrixToIdentity(4,4,&left_rotation_and_translation);
 SetMatrixToIdentity(4,4,&total_left_rotation_and_translation);

 SetAgent(0, 1.0,1.0,1.0 ,0.0,0.0,0.0);

/*
 ClearTransformationMatrix(&total_right_rotation);
 total_right_rotation.rows=4 , total_right_rotation.columns=4;
 total_right_rotation.item[0]=1.0;
 total_right_rotation.item[5]=1.0;
 total_right_rotation.item[10]=1.0;
 total_right_rotation.item[15]=1.0;*/
 SetMatrixToIdentity(4,4,&right_homography);
 SetMatrixToIdentity(4,4,&total_right_rotation);
 SetMatrixToIdentity(4,4,&right_rotation);
 SetMatrixToIdentity(4,4,&right_translation);
 SetMatrixToIdentity(4,4,&right_rotation_and_translation);
 SetMatrixToIdentity(4,4,&total_right_rotation_and_translation);



 SetAgent(1, 1.0,1.0,1.0 ,0.0,0.0,0.0);



 return 1;
}
