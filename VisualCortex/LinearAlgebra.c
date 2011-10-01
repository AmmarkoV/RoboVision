#include "LinearAlgebra.h"
#include "VisionMemory.h"
#include <stdio.h>
#include "cv.h"
#include <gsl/gsl_linalg.h>
#include <cv.h>
#include <highgui.h>
#include <ctype.h>

int GetCameraCoords_From_Angle(float horizontal_angle,float vertical_angle,unsigned int * uint_pixel_x,unsigned int * uint_pixel_y)
{
 /*
    horizontal_angle == 0 AND vertical_angle == 0 ---> The center of the Image

    horizontal_angle ( left is less , right is more )
    vertical angle ( down is less , up is more )

     When X is 0 horizontal -> angle -field_of_view
     When X is RES_X/2 -> horizontal angle 0
     When X is RES_X -> horizontal angle +field_of_view
  */
  float degree_step_x = 0 , degree_step_y = 0;
  if (camera_horizontal_field_of_view !=0) { degree_step_x = metrics[RESOLUTION_X] / camera_horizontal_field_of_view; } else
                                           { fprintf(stderr,"Camera horizontal field of view NOT set .. , this will probably keep popping up , until you call VisCortx_SetCamerasGeometry\n"); return 0; }
  if ( degree_step_x == 0 ) { fprintf(stderr,"Camera resolution probably not set , cannot continue to get depth \n"); return 0; }


  if (camera_vertical_field_of_view !=0) { degree_step_y = metrics[RESOLUTION_Y] / camera_vertical_field_of_view; } else
                                         { fprintf(stderr,"Camera vertical field of view NOT set .. , this will probably keep popping up , until you call VisCortx_SetCamerasGeometry\n"); return 0; }
  if ( degree_step_y == 0 ) { fprintf(stderr,"Camera resolution probably not set , cannot continue to get depth \n"); return 0; }

  *uint_pixel_x=metrics[RESOLUTION_X] / 2;
  *uint_pixel_y=metrics[RESOLUTION_Y] / 2;

  float abs_horizontal_angle = horizontal_angle; if ( abs_horizontal_angle < 0 ) { abs_horizontal_angle = abs_horizontal_angle  * (-1); }
  float abs_vertical_angle = vertical_angle;     if ( abs_vertical_angle < 0 ) { abs_vertical_angle = abs_vertical_angle  * (-1); }

  float pixel_x = abs_horizontal_angle / degree_step_x  , pixel_y =abs_vertical_angle / degree_step_y;
  if ( horizontal_angle < 0 ) { pixel_x  = (metrics[RESOLUTION_X] / 2) - abs_horizontal_angle; }
  if ( vertical_angle < 0   ) { pixel_y  = (metrics[RESOLUTION_Y] / 2) - abs_vertical_angle;   }

  if ( ( pixel_x >= 0.0 ) && (pixel_x < metrics[RESOLUTION_X] ) ) { *uint_pixel_x = (unsigned int) pixel_x; }
  if ( ( pixel_y >= 0.0 ) && (pixel_y < metrics[RESOLUTION_Y] ) ) { *uint_pixel_y = (unsigned int) pixel_y; }

  return 1;
}








 CvMat * inverseCalibrationMatrix( CvMat *in )
 {
    //http://forum.openframeworks.cc/index.php?topic=2313.0

    if( cvmGet( in, 1, 1 ) != 0 ){
        CvMat* out  = cvCreateMat( 3, 3, CV_64FC1 );
        double tau  = cvmGet( in, 0, 0 )/cvmGet( in, 1, 1 );
        double f    = cvmGet( in, 1, 1 );
        double u0   = cvmGet( in, 0, 2 );
        double v0   = cvmGet( in, 1, 2 );
        cvmSet( out, 0, 0, 1.00/(tau*f)     );
        cvmSet( out, 0, 1, 0.00             );
        cvmSet( out, 0, 2, -(u0)/(tau*f)    );
        cvmSet( out, 1, 0, 0.00             );
        cvmSet( out, 1, 1, 1.00/f           );
        cvmSet( out, 1, 2, -(v0)/f          );
        cvmSet( out, 2, 0, 0.00             );
        cvmSet( out, 2, 1, 0.00             );
        cvmSet( out, 2, 2, 1.00             );
        return out;
    } else
        return NULL;
}

void ComposeRotationTranslationTo3x4Matrix( CvMat* RotTrans , CvMat* Rot, CvMat*Trans )
{
    cvmSet( RotTrans, 0, 0, cvmGet( Rot, 0, 0 ) );
    cvmSet( RotTrans, 0, 1, cvmGet( Rot, 0, 1 ) );
    cvmSet( RotTrans, 0, 2, cvmGet( Rot, 0, 2 ) );
    cvmSet( RotTrans, 1, 0, cvmGet( Rot, 1, 0 ) );
    cvmSet( RotTrans, 1, 1, cvmGet( Rot, 1, 1 ) );
    cvmSet( RotTrans, 1, 2, cvmGet( Rot, 1, 2 ) );
    cvmSet( RotTrans, 2, 0, cvmGet( Rot, 2, 0 ) );
    cvmSet( RotTrans, 2, 1, cvmGet( Rot, 2, 1 ) );
    cvmSet( RotTrans, 2, 2, cvmGet( Rot, 2, 2 ) );
    cvmSet( RotTrans, 0, 3, -cvmGet( Trans, 0, 0 ) );
    cvmSet( RotTrans, 1, 3, -cvmGet( Trans, 1, 0 ) );
    cvmSet( RotTrans, 2, 3, -cvmGet( Trans, 2, 0 ) );
}

CvMat* CreateHomographyRotationTranslationMatrix( CvMat* m_homography,CvMat* m_intric )
{
   //http://forum.openframeworks.cc/index.php?topic=2313.0

    //This produces Relative Rotation
    // Suppose you are at a some position P(X,Y,Z), and you are looking off in some direction D. Your position is represented by the translation matrix T, and the direction of your view is represented by the rotation matrix R. The combined information is held in the translation matrix Tr. We saw this at the beginning of the presentation:

    int i;
    // Vectors holding columns of H and R:
    //Homography cvGetCol().
    double a_H1[3];
    CvMat  m_H1 = cvMat( 3, 1, CV_64FC1, a_H1 );
    for( i = 0; i < 3; i++ ) cvmSet( &m_H1, i, 0, cvmGet( m_homography, i, 0 ) );

    double a_H2[3];
    CvMat  m_H2 = cvMat( 3, 1, CV_64FC1, a_H2 );
    for( i = 0; i < 3; i++ ) cvmSet( &m_H2, i, 0, cvmGet( m_homography, i, 1 ) );

    double a_H3[3];
    CvMat  m_H3 = cvMat( 3, 1, CV_64FC1, a_H3 );
    for( i = 0; i < 3; i++ ) cvmSet( &m_H3, i, 0, cvmGet( m_homography, i, 2 ) );

    double a_CinvH1[3];
    CvMat  m_CinvH1 = cvMat( 3, 1, CV_64FC1, a_CinvH1 );

    double a_R1[3];
    CvMat  m_R1 = cvMat( 3, 1, CV_64FC1, a_R1 );

    double a_R2[3];
    CvMat  m_R2 = cvMat( 3, 1, CV_64FC1, a_R2 );

    double a_R3[3];
    CvMat  m_R3 = cvMat( 3, 1, CV_64FC1, a_R3 );

    // The rotation matrix:
    double a_R[9];
    CvMat  m_R = cvMat( 3, 3, CV_64FC1, a_R );

    // The translation vector:
    double a_T[3];
    CvMat  m_T = cvMat( 3, 1, CV_64FC1, a_T );

    ////////////////////////////////////////////////////////
    // Create inverse calibration matrix:
    CvMat* m_Cinv = inverseCalibrationMatrix( m_intric);// .

    // Create norming factor lambda:
    cvGEMM( m_Cinv, &m_H1, 1, NULL, 0, &m_CinvH1, 0 );

    // Search next orthonormal matrix:
    if( cvNorm( &m_CinvH1, NULL, CV_L2, NULL ) != 0 )
    {
        double lambda = 1.00/cvNorm( &m_CinvH1, NULL, CV_L2, NULL );

        // Create normalized R1 & R2:
        cvGEMM( m_Cinv, &m_H1, lambda, NULL, 0, &m_R1, 0 );
        cvGEMM( m_Cinv, &m_H2, lambda, NULL, 0, &m_R2, 0 );

        // Get R3 orthonormal to R1 and R2:
        cvCrossProduct( &m_R1, &m_R2, &m_R3 );

        // Put the rotation column vectors in the rotation matrix:
        for( i = 0; i < 3; i++ ){
            cvmSet( &m_R, i, 0,  cvmGet( &m_R1, i, 0 ) );
            cvmSet( &m_R, i, 1,  cvmGet( &m_R2, i, 0 ) );
            cvmSet( &m_R, i, 2,  cvmGet( &m_R3, i, 0 ) );
        }

        // Calculate Translation Vector T (- because of its definition):
        cvGEMM( m_Cinv, &m_H3, -lambda, NULL, 0, &m_T, 0 );

        // Transformation of R into - in Frobenius sense - next orthonormal matrix:
        double a_W[9];  CvMat  m_W  = cvMat( 3, 3, CV_64FC1, a_W  );
        double a_U[9];  CvMat  m_U  = cvMat( 3, 3, CV_64FC1, a_U  );
        double a_Vt[9]; CvMat  m_Vt = cvMat( 3, 3, CV_64FC1, a_Vt );
        cvSVD( &m_R, &m_W, &m_U, &m_Vt, CV_SVD_MODIFY_A | CV_SVD_V_T );
        cvMatMul( &m_U, &m_Vt, &m_R );
        // Put the rotation matrix and the translation vector together:
        CvMat*  m_view_to_cam = cvCreateMat( 3, 4, CV_64FC1 );
        ComposeRotationTranslationTo3x4Matrix( m_view_to_cam, &m_R, &m_T );
        return m_view_to_cam;
    }
   return 0;
}



int ComputeHomographyFromPointCorrespondanceOpenCV ( struct FeatureList * source,
                                                     struct CameraCalibrationData * calibration,
                                                     struct TransformationMatrix * rotation_matrix,
                                                     struct TransformationMatrix * translation_matrix,
                                                     struct TransformationMatrix * rotation_and_translation_matrix,
                                                     struct TransformationMatrix * homography_matrix)
{
   if ( source->current_features == 0 ) { return 0; }
   if ( source->current_features < 4) { return 0; }

   int i=0 , res = 1;
   unsigned int points_limit = source->current_features;
   //points_limit  = 4; // If we want just a perspective Transform and not a Homography

   CvMat* srcPoints = cvCreateMat(2,points_limit,CV_32FC1);
   if ( srcPoints != 0 )
   {
    for ( i=0; i<points_limit;  i++ )
     {   cvmSet(srcPoints,0,i,(float) source->list[i].last_x);
         cvmSet(srcPoints,1,i,(float) source->list[i].last_y); }
   }

   CvMat* dstPoints = cvCreateMat(2,points_limit,CV_32FC1);
   if ( dstPoints != 0 )
   {
    for ( i=0; i<points_limit; i++ )
     {   cvmSet(srcPoints,0,i,(float) source->list[i].x);
         cvmSet(srcPoints,1,i,(float) source->list[i].y); }
   }


   CvMat* status=0;
   CvMat* H =  cvCreateMat(3,3,CV_64F);
   cvZero(H);

    res = cvFindHomography(srcPoints,dstPoints,H,CV_RANSAC,2.5,status);
    //cvPerspectiveTransform(srcPoints,dstPoints, H);


   i=0;
   int mem=0,j=0;
   homography_matrix->rows=3;
   homography_matrix->columns=3;
    for(i=0; i<3; i++)
     { for(j=0; j<3; j++)
       {
         homography_matrix->item[mem++]=cvmGet(H,i,j);
       }
     }

   IplImage  * image = cvCreateImage( cvSize(320,240), IPL_DEPTH_8U, 3 );
   memcpy(image->imageData , video_register[CALIBRATED_LEFT_EYE].pixels , metrics[RESOLUTION_MEMORY_LIMIT_3BYTE]);
   IplImage  * dstImg = cvCloneImage(image);

   cvWarpPerspective(image, dstImg, H , CV_INTER_CUBIC | CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );

   memcpy( video_register[LAST_LEFT_OPERATION].pixels , dstImg->imageData , metrics[RESOLUTION_MEMORY_LIMIT_3BYTE]);
   video_register[LAST_LEFT_OPERATION].time = video_register[CALIBRATED_LEFT_EYE].time;
   cvReleaseImage( &image );
   cvReleaseImage( &dstImg );



   // transformed output image
   CvMat*  intriMat=cvCreateMat(3,3,CV_64F); //cvMat(3,3,CV_64F,calibration->intrinsic_parameters_array);
           cvmSet(intriMat,0,0,calibration->intrinsic_parameters_array[0]); cvmSet(intriMat,0,1,calibration->intrinsic_parameters_array[1]); cvmSet(intriMat,0,2,calibration->intrinsic_parameters_array[2]);
           cvmSet(intriMat,1,0,calibration->intrinsic_parameters_array[3]); cvmSet(intriMat,1,1,calibration->intrinsic_parameters_array[4]); cvmSet(intriMat,1,2,calibration->intrinsic_parameters_array[5]);
           cvmSet(intriMat,2,0,calibration->intrinsic_parameters_array[6]); cvmSet(intriMat,2,1,calibration->intrinsic_parameters_array[7]); cvmSet(intriMat,2,2,calibration->intrinsic_parameters_array[8]);



   CvMat*  homography_decomposition_to_translation_and_rotation = CreateHomographyRotationTranslationMatrix(H,intriMat);


   if ( homography_decomposition_to_translation_and_rotation != 0 )
   {
    ClearTransformationMatrix(rotation_matrix);
    rotation_matrix->rows=3;
    rotation_matrix->columns=3;
    rotation_matrix->item[0]=cvmGet(homography_decomposition_to_translation_and_rotation,0,0);
    rotation_matrix->item[1]=cvmGet(homography_decomposition_to_translation_and_rotation,0,1);
    rotation_matrix->item[2]=cvmGet(homography_decomposition_to_translation_and_rotation,0,2);
    rotation_matrix->item[3]=cvmGet(homography_decomposition_to_translation_and_rotation,1,0);
    rotation_matrix->item[4]=cvmGet(homography_decomposition_to_translation_and_rotation,1,1);
    rotation_matrix->item[5]=cvmGet(homography_decomposition_to_translation_and_rotation,1,2);
    rotation_matrix->item[6]=cvmGet(homography_decomposition_to_translation_and_rotation,2,0);
    rotation_matrix->item[7]=cvmGet(homography_decomposition_to_translation_and_rotation,2,1);
    rotation_matrix->item[8]=cvmGet(homography_decomposition_to_translation_and_rotation,2,2);


    ClearTransformationMatrix(translation_matrix);
    translation_matrix->rows=1;
    translation_matrix->columns=4;
    translation_matrix->item[0]=cvmGet(homography_decomposition_to_translation_and_rotation,0,3);
    translation_matrix->item[1]=cvmGet(homography_decomposition_to_translation_and_rotation,1,3);
    translation_matrix->item[2]=cvmGet(homography_decomposition_to_translation_and_rotation,2,3);
    translation_matrix->item[3]=1;


    ClearTransformationMatrix(rotation_and_translation_matrix);
    rotation_and_translation_matrix->rows=4;
    rotation_and_translation_matrix->columns=4;
    rotation_and_translation_matrix->item[0]=cvmGet(homography_decomposition_to_translation_and_rotation,0,0);
    rotation_and_translation_matrix->item[1]=cvmGet(homography_decomposition_to_translation_and_rotation,0,1);
    rotation_and_translation_matrix->item[2]=cvmGet(homography_decomposition_to_translation_and_rotation,0,2);

    rotation_and_translation_matrix->item[4]=cvmGet(homography_decomposition_to_translation_and_rotation,1,0);
    rotation_and_translation_matrix->item[5]=cvmGet(homography_decomposition_to_translation_and_rotation,1,1);
    rotation_and_translation_matrix->item[6]=cvmGet(homography_decomposition_to_translation_and_rotation,1,2);

    rotation_and_translation_matrix->item[8]=cvmGet(homography_decomposition_to_translation_and_rotation,2,0);
    rotation_and_translation_matrix->item[9]=cvmGet(homography_decomposition_to_translation_and_rotation,2,1);
    rotation_and_translation_matrix->item[10]=cvmGet(homography_decomposition_to_translation_and_rotation,2,2);

    rotation_and_translation_matrix->item[3]=cvmGet(homography_decomposition_to_translation_and_rotation,0,3);
    rotation_and_translation_matrix->item[7]=cvmGet(homography_decomposition_to_translation_and_rotation,1,3);
    rotation_and_translation_matrix->item[11]=cvmGet(homography_decomposition_to_translation_and_rotation,2,3);

    rotation_and_translation_matrix->item[12]=0.0;
    rotation_and_translation_matrix->item[13]=0.0;
    rotation_and_translation_matrix->item[14]=0.0;
    rotation_and_translation_matrix->item[15]=1.0;
   }

   if ( srcPoints != 0 ) { cvReleaseMat(&srcPoints); }
   if ( dstPoints != 0 ) { cvReleaseMat(&dstPoints); }
   if ( H != 0 ) { cvReleaseMat(&H); }
   if ( homography_decomposition_to_translation_and_rotation != 0 ) { cvReleaseMat(&homography_decomposition_to_translation_and_rotation); }
   if ( intriMat != 0 ) { cvReleaseMat(&intriMat); }

   return res;
}



int ComputeFundamentalMatrixFromPointCorrespondance(struct FeatureList * list,struct TransformationMatrix * E)
{
    return 0;
/*
    // TODO TODO TODO TODO
    //The following program solves the linear system A x = b. The system to be solved is, and the solution is found using LU decomposition of the matrix A.
       double a_data[] = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,
                           1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,
                           1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,
                           1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,
                           1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,
                           1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,
                           1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,
                           1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,
                           1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9
                         };

       double b_data[] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  };
       unsigned int array_offset=0;
       unsigned int array_row=0;
       unsigned int feature_num=0;

   int i=0;
     while ( i < 9 )
      {

       array_offset =  9 * array_row;
       a_data[array_offset + 0] = list->list[feature_num].x * list->list[feature_num].last_x;
       a_data[array_offset + 1] = list->list[feature_num].x * list->list[feature_num].last_x;
       a_data[array_offset + 2] = list->list[feature_num].x;
       a_data[array_offset + 3] = list->list[feature_num].y * list->list[feature_num].last_x;
       a_data[array_offset + 4] = list->list[feature_num].y * list->list[feature_num].last_y;
       a_data[array_offset + 5] = list->list[feature_num].y;
       a_data[array_offset + 6] = list->list[feature_num].last_x;
       a_data[array_offset + 7] = list->list[feature_num].last_y;
       a_data[array_offset + 8] = 1;

       ++i;
       ++array_row;
       ++feature_num;
      }


       gsl_matrix_view m
         = gsl_matrix_view_array (a_data, 9, 9);

       gsl_vector_view b
         = gsl_vector_view_array (b_data, 9);

       gsl_vector *x = gsl_vector_alloc (9);

       int s;

       gsl_permutation * p = gsl_permutation_alloc (9);

       if ( gsl_linalg_LU_decomp (&m.matrix, p, &s) )
       {
         gsl_linalg_LU_solve (&m.matrix, p, &b.vector, x);



         E->item[0] = gsl_vector_get ( x , 0); // e11
         E->item[1] = gsl_vector_get ( x , 1); // e12
         E->item[2] = gsl_vector_get ( x , 2); // e13
         E->item[3] = gsl_vector_get ( x , 3); // e21
         E->item[4] = gsl_vector_get ( x , 4); // e22
         E->item[5] = gsl_vector_get ( x , 5); // e23
         E->item[6] = gsl_vector_get ( x , 6); // e31
         E->item[7] = gsl_vector_get ( x , 7); // e32
         E->item[8] = gsl_vector_get ( x , 8); // e33

         printf ("x = \n");
         gsl_vector_fprintf (stdout, x, "%g");
       }

       gsl_permutation_free (p);
       gsl_vector_free (x);
       */
       return 1;
}



