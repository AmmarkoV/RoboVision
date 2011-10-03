#include "CameraPose.h"

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
