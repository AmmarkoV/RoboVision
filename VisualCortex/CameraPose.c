#include "CameraPose.h"
#include "Precalculations.h"
#include "Matrix.h"
#include "LinearAlgebra.h"
#include "VisCortexTimer.h"
#include "DisparityDepthMap_Heuristics.h"
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

void PassDepthMapToCameraSystem()
{
     //(unsigned char *)
       fprintf(stderr,"Registers DEPTH0 and COLOR0 are written to filesystem\n");
       SaveRegisterToFile("memfs/DEPTH0",DEPTH_LEFT_VIDEO);
       SaveRegisterToFile("memfs/COLOR0",CALIBRATED_LEFT_EYE);
       SaveTransformationMatrixToFile("memfs/LEFT_HOMOGRAPHY0",&left_homography);
       SaveTransformationMatrixToFile("memfs/RIGHT_HOMOGRAPHY0",&right_homography);

       SaveFeatureListContents(video_register[CALIBRATED_LEFT_EYE].features,"memfs/POINTS_FOR_LEFT_HOMOGRAPHY0");
       SaveFeatureListContents(video_register[CALIBRATED_RIGHT_EYE].features,"memfs/POINTS_FOR_RIGHT_HOMOGRAPHY0");

       SaveTransformationMatrixToFile("memfs/LEFT_ROTATION0",&left_rotation);
       SaveTransformationMatrixToFile("memfs/RIGHT_ROTATION0",&right_rotation);

       SaveTransformationMatrixToFile("memfs/TOTAL_LEFT_ROTATION0",&total_left_rotation);
       SaveTransformationMatrixToFile("memfs/TOTAL_RIGHT_ROTATION0",&total_right_rotation);

       SaveTransformationMatrixToFile("memfs/LEFT_TRANSLATION0",&left_translation);
       SaveTransformationMatrixToFile("memfs/RIGHT_TRANSLATION0",&right_translation);

       SaveTransformationMatrixToFile("memfs/LEFT_ROTATION_AND_TRANSLATION0",&left_rotation_and_translation);
       SaveTransformationMatrixToFile("memfs/RIGHT_ROTATION_AND_TRANSLATION0",&right_rotation_and_translation);

     //  SaveDepthMapToFile("memfs/DEPTH_MAP",CALIBRATED_LEFT_EYE);

     //THIS IS NOW PASSED THROUGH MasterWorld!
     PassDepthMapToMasterWorld(CALIBRATED_LEFT_EYE);
     Save3DPointCloudToFile("memfs/DEPTH_MAP");

}



int UpdateCameraPose(unsigned int reg_num)
{
 StartTimer(UPDATE_CAMERA_POSE_DELAY); // STATISTICS KEEPER FOR HYPERVISOR | START

   int PRINT_THINGS_DEBUG = 0;

              if (reg_num == CALIBRATED_LEFT_EYE )
                {
                  if ( settings[USE_OPENCV] ) { ComputeHomographyFromPointCorrespondanceOpenCV(video_register[reg_num].features,
                                                                                              &left_calibration_data,
                                                                                              &left_rotation,
                                                                                              &left_translation,
                                                                                              &left_rotation_and_translation,
                                                                                              &left_homography,
                                                                                              0 //THIS SWITCH RENDERS ( OR NOT ) THE WARPED IMAGE
                                                                                              );
                                              }


                 struct TransformationMatrix tmp_matrix;
                 CopyMatrixToMatrix(&tmp_matrix,&total_left_rotation);
                 Multiply4x4Matrices(&total_left_rotation,&left_rotation,&tmp_matrix);


                 if (PRINT_THINGS_DEBUG) { PrintMatrix("Left rotation",&left_rotation); }

                 float  x = 1.0 , y = 1.0 , z = 1.0 , heading , pitch , yaw ;
                 GetAgent(0,&x,&y,&z,&heading,&pitch,&yaw);
                 if (PRINT_THINGS_DEBUG) { fprintf(stderr,"Point was %0.2f , %0.2f , %0.2f  and now is ",x,y,z); }
                 Multiply3DPointWithMatrix(&x,&y,&z ,&left_rotation);
                 if (PRINT_THINGS_DEBUG) { fprintf(stderr," %0.2f , %0.2f , %0.2f  \n",x,y,z); }
                 SetAgent(0,x,y,z ,heading,pitch,yaw);


                } else
               if (reg_num == CALIBRATED_RIGHT_EYE )
                {
                  if ( settings[USE_OPENCV] ) { ComputeHomographyFromPointCorrespondanceOpenCV(video_register[reg_num].features,
                                                                                               &right_calibration_data,
                                                                                               &right_rotation,
                                                                                               &right_translation,
                                                                                               &right_rotation_and_translation,
                                                                                               &right_homography,
                                                                                               0 //THIS SWITCH RENDERS ( OR NOT ) THE WARPED IMAGE
                                                                                               );
                                              }

                 struct TransformationMatrix tmp_matrix;
                 CopyMatrixToMatrix(&tmp_matrix,&total_right_rotation);
                 Multiply4x4Matrices(&total_right_rotation,&right_rotation,&tmp_matrix);

                 if (PRINT_THINGS_DEBUG) { PrintMatrix("Right rotation",&right_rotation); }

                 float  x = 1.0 , y = 1.0 , z = 1.0 , heading , pitch , yaw ;
                 GetAgent(1,&x,&y,&z,&heading,&pitch,&yaw);
                 Multiply3DPointWithMatrix(&x,&y,&z ,&left_rotation);
                 SetAgent(1,x,y,z ,heading,pitch,yaw);

                }


  EndTimer(UPDATE_CAMERA_POSE_DELAY); // STATISTICS KEEPER FOR HYPERVISOR | END

  return 1;
}


int InitCameraPose()
{
 //LEFT CAMERA
 SetMatrixToIdentity(4,4,&left_homography);
 SetMatrixToIdentity(4,4,&total_left_rotation);
 SetMatrixToIdentity(4,4,&left_rotation);
 SetMatrixToIdentity(4,4,&left_translation);
 SetMatrixToIdentity(4,4,&left_rotation_and_translation);
 SetMatrixToIdentity(4,4,&total_left_rotation_and_translation);
 SetAgent(0, 1.0,1.0,1.0 ,0.0,0.0,0.0);

 //RIGHT CAMERA
 SetMatrixToIdentity(4,4,&right_homography);
 SetMatrixToIdentity(4,4,&total_right_rotation);
 SetMatrixToIdentity(4,4,&right_rotation);
 SetMatrixToIdentity(4,4,&right_translation);
 SetMatrixToIdentity(4,4,&right_rotation_and_translation);
 SetMatrixToIdentity(4,4,&total_right_rotation_and_translation);
 SetAgent(1, 1.0,1.0,1.0 ,0.0,0.0,0.0);

 return 1;
}
