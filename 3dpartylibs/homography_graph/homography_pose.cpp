#include "homography_pose.h"

homography_pose::homography_pose(){}

double homography_pose::compareSURFDescriptors( const float* d1, const float* d2, double best, int length )
{
   double total_cost = 0;
   assert( length % 4 == 0 );
   for( int i = 0; i < length; i += 4 )
   {
      double t0 = d1[i] - d2[i];
      double t1 = d1[i+1] - d2[i+1];
      double t2 = d1[i+2] - d2[i+2];
      double t3 = d1[i+3] - d2[i+3];
      total_cost += t0*t0 + t1*t1 + t2*t2 + t3*t3;
      if( total_cost > best )
         break;
   }
   return total_cost;
}

int homography_pose::naiveNearestNeighbor( const float* vec, int laplacian,
                   const CvSeq* model_keypoints,
                   const CvSeq* model_descriptors )
{
   int length = (int)(model_descriptors->elem_size/sizeof(float));
   int i, neighbor = -1;
   double d, dist1 = 1e6, dist2 = 1e6;
   CvSeqReader reader, kreader;
   cvStartReadSeq( model_keypoints, &kreader, 0 );
   cvStartReadSeq( model_descriptors, &reader, 0 );

   for( i = 0; i < model_descriptors->total; i++ )
   {
      const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
      const float* mvec = (const float*)reader.ptr;
      CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
      CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
      if( laplacian != kp->laplacian )
         continue;
      d = compareSURFDescriptors( vec, mvec, dist2, length );
      if( d < dist1 )
      {
         dist2 = dist1;
         dist1 = d;
         neighbor = i;
      }
      else if ( d < dist2 )
         dist2 = d;
   }
   if ( dist1 < 0.6*dist2 )
      return neighbor;
   return -1;
}

void homography_pose::findPairs( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
            const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, vector<int>& ptpairs )
{
   int i;
   CvSeqReader reader, kreader;
   cvStartReadSeq( objectKeypoints, &kreader );
   cvStartReadSeq( objectDescriptors, &reader );
   ptpairs.clear();

   for( i = 0; i < objectDescriptors->total; i++ )
   {
      const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
      const float* descriptor = (const float*)reader.ptr;
      CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
      CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
      int nearest_neighbor = naiveNearestNeighbor( descriptor, kp->laplacian, imageKeypoints, imageDescriptors );
      if( nearest_neighbor >= 0 )
      {
         ptpairs.push_back(i);
         ptpairs.push_back(nearest_neighbor);
      }
   }
}
// 显示矩阵
void homography_pose::PrintMat(CvMat* A)
{
   int i,j;
   //printf("\nMatrix = :");
   for(i=0;i<A->rows;i++)
   {
      printf("\n");

      switch( CV_MAT_DEPTH(A->type) )
      {
      case CV_32F:
      case CV_64F:
         for(j=0;j<A->cols;j++)
            printf("%9.3f ", (float) cvGetReal2D( A, i, j ));
         break;
      case CV_8U:
      case CV_16U:
         for(j=0;j<A->cols;j++)
            printf("m",(int)cvGetReal2D( A, i, j ));
         break;
      default:
         break;
      }
   }
   printf("\n");
}
CvMat * homography_pose::inverseCalibrationMatrix( CvMat *in ){
   if( cvmGet( in, 1, 1 ) != 0 ){
      CvMat* out   = cvCreateMat( 3, 3, CV_64FC1 );
      double tau   = cvmGet( in, 0, 0 )/cvmGet( in, 1, 1 );
      double f   = cvmGet( in, 1, 1 );
      double u0   = cvmGet( in, 0, 2 );
      double v0   = cvmGet( in, 1, 2 );
      cvmSet( out, 0, 0, 1.00/(tau*f)      );
      cvmSet( out, 0, 1, 0.00            );
      cvmSet( out, 0, 2, -(u0)/(tau*f)   );
      cvmSet( out, 1, 0, 0.00            );
      cvmSet( out, 1, 1, 1.00/f         );
      cvmSet( out, 1, 2, -(v0)/f         );
      cvmSet( out, 2, 0, 0.00          );
      cvmSet( out, 2, 1, 0.00          );
      cvmSet( out, 2, 2, 1.00          );
      return out;
   } else
      return NULL;
}
void homography_pose::ComposeRotationTranslationTo3x4Matrix( CvMat* RotTrans , CvMat* Rot, CvMat*Trans ){
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

CvMat* homography_pose::CreateHomographyRotationTranslationMatrix( CvMat* m_homography,CvMat* m_intric ){

   int i;
   // Vectors holding columns of H and R:
   //得到Homography的三列,其实可以直接用cvGetCol().
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
   CvMat* m_Cinv = inverseCalibrationMatrix( m_intric);//应该是内参数阵求逆.

   // Create norming factor lambda:
   cvGEMM( m_Cinv, &m_H1, 1, NULL, 0, &m_CinvH1, 0 );

   // Search next orthonormal matrix:
   if( cvNorm( &m_CinvH1, NULL, CV_L2, NULL ) != 0 ){
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
      double a_W[9];   CvMat  m_W  = cvMat( 3, 3, CV_64FC1, a_W  );
      double a_U[9];   CvMat  m_U  = cvMat( 3, 3, CV_64FC1, a_U  );
      double a_Vt[9];   CvMat  m_Vt = cvMat( 3, 3, CV_64FC1, a_Vt );
      cvSVD( &m_R, &m_W, &m_U, &m_Vt, CV_SVD_MODIFY_A | CV_SVD_V_T );
      cvMatMul( &m_U, &m_Vt, &m_R );
      // Put the rotation matrix and the translation vector together:
      CvMat*  m_view_to_cam = cvCreateMat( 3, 4, CV_64FC1 );
      ComposeRotationTranslationTo3x4Matrix( m_view_to_cam, &m_R, &m_T );
      return m_view_to_cam;
   }
}

void homography_pose::cvForLoop()
{
   vector<int> ptpairs;

   cvGrabFrame( capture );
   image = cvQueryFrame( capture );
   if( !image )
   {
      printf("capture image failed!");
      return;
   }
   cvCvtColor(image,imageGray,CV_BGR2GRAY);

   cvExtractSURF( imageGray, 0, &imageKeypoints, &imageDescriptors, storage, params );
   //printf("Image Descriptors: %d\n", imageDescriptors->total);
   cvSetImageROI( correspond, cvRect( 0, 0, object->width, object->height ) );
   cvCopy( object, correspond );
   //cvResetImageROI( correspond );
   cvSetImageROI( correspond, cvRect( 0, object->height, correspond->width, correspond->height ) );
   cvCopy( imageGray, correspond );
   cvResetImageROI( correspond );
   //vector<int> ptpairs;
   //下面这段if用来在correspond image与物体image上画对应特征点的连线
   findPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
   int ptNum=(int)ptpairs.size();
   if(ptNum>10)
   {
      CvMat *srcPts=cvCreateMat(2,ptNum/2,CV_64F);
      CvMat *dstPts=cvCreateMat(2,ptNum/2,CV_64F);
      for( int i = 0; i < ptNum; i += 2 )
      {
         CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem( objectKeypoints, ptpairs[i] );
         CvSURFPoint* r2 = (CvSURFPoint*)cvGetSeqElem( imageKeypoints, ptpairs[i+1] );
         cvLine( correspond, cvPointFrom32f(r1->pt),   cvPoint(cvRound(r2->pt.x), cvRound(r2->pt.y+object->height)), cvScalar(255,255,255) );
         cvmSet(srcPts,0,i/2,r1->pt.x);
         cvmSet(srcPts,1,i/2,r1->pt.y);
         cvmSet(dstPts,0,i/2,r2->pt.x);
         cvmSet(dstPts,1,i/2,r2->pt.y);
      }
      cvFindHomography( srcPts,dstPts,homography );
      cvReleaseMat(&srcPts);
      cvReleaseMat(&dstPts);
      transformMat=CreateHomographyRotationTranslationMatrix( homography,&intriMat );

      //PrintMat(transformMat);
      
      //cvWaitKey(0);
      tranMat[0]=cvmGet(transformMat,0,0);
      tranMat[1]=cvmGet(transformMat,1,0);
      tranMat[2]=cvmGet(transformMat,2,0);
      tranMat[3]=0;
      tranMat[4]=cvmGet(transformMat,0,1);
      tranMat[5]=cvmGet(transformMat,1,1);
      tranMat[6]=cvmGet(transformMat,2,1);
      tranMat[7]=0;
      tranMat[8]=cvmGet(transformMat,0,2);
      tranMat[9]=cvmGet(transformMat,1,2);
      tranMat[10]=cvmGet(transformMat,2,2);
      tranMat[11]=0;
      tranMat[12]=cvmGet(transformMat,0,3)*0.01;
      tranMat[13]=cvmGet(transformMat,1,3)*0.01;
      tranMat[14]=cvmGet(transformMat,2,3)*0.01;
      tranMat[15]=1;
      //glutPostRedisplay();
   }
   
   cvShowImage( "Object Correspond", correspond );

}
void homography_pose::init()
{
   const char* object_filename = "1.jpg";
   storage = cvCreateMemStorage(0);
   object = cvLoadImage( object_filename, CV_LOAD_IMAGE_GRAYSCALE );
   //CvSeq *objectKeypoints = 0, *objectDescriptors = 0;
   params = cvSURFParams(500, 1);

   cvNamedWindow("Object", 1);
   cvNamedWindow("Object Correspond", 1);
   cvExtractSURF( object, 0, &objectKeypoints, &objectDescriptors, storage, params );
   cvShowImage( "Object", object );
   capture =cvCaptureFromCAM(CV_CAP_ANY);
   if( !capture )
   {
      fprintf(stderr,"Could not initialize capturing...\n");
      return;
   }

   image =cvQueryFrame(capture);//抓取的每帧图像
   correspond = cvCreateImage( cvSize(image->width, object->height+image->height), 8, 1 );
   imageGray = cvCreateImage(cvSize(image->width,image->height), image->depth, 1);
   imageKeypoints = 0;
   imageDescriptors = 0;

   intri[0]=772.5483;
   intri[1]=0;
   intri[2]=320;
   intri[3]=0;
   intri[4]=772.5483;
   intri[5]=240;
   intri[6]=0;
   intri[7]=0;
   intri[8]=1;
   intriMat=cvMat(3,3,CV_64F,intri);
   tranMat[0]=1;tranMat[1]=0;tranMat[2]=0;tranMat[3]=0;
   tranMat[4]=0;tranMat[5]=1;tranMat[6]=0;tranMat[7]=0;
   tranMat[8]=0;tranMat[9]=0;tranMat[10]=1;tranMat[11]=0;
   tranMat[12]=0;tranMat[13]=0;tranMat[14]=0;tranMat[15]=1;
   homography=cvCreateMat(3,3,CV_64F);
}
