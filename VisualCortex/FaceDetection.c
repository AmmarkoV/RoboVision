#include "FaceDetection.h"
#include <stdio.h>
#include <stdlib.h>
#include "cv.h"
#include "VisionMemory.h"
#include "FeatureLists.h"

IplImage  *image;

CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;

void CloseFaceRecognition() // To kanei to VisionSubsystem me to init tou ousiastika
{
    cvReleaseHaarClassifierCascade( &cascade );
    cvReleaseMemStorage( &storage );
    cvReleaseImage(&image );
}

void InitFaceRecognition(unsigned int x,unsigned int y)
{
    char      *filename = "haarcascade_frontalface_alt.xml";

    /* load the classifier
       note that I put the file in the same directory with
       this code */
    cascade = ( CvHaarClassifierCascade* ) cvLoad( filename, 0, 0, 0 );

    /* setup memory buffer; needed by the face detector */
    storage = cvCreateMemStorage( 0 );

    image = cvCreateImage( cvSize(x,y), IPL_DEPTH_8U, 3 );
}

unsigned int RecognizeFaces(unsigned int vid_reg)
{
    ClearFeatureList(video_register[vid_reg].faces);

    /* detect faces */
    memcpy( image->imageData , video_register[vid_reg].pixels , metrics[RESOLUTION_MEMORY_LIMIT_3BYTE] );
    CvSeq *faces = cvHaarDetectObjects
           (
            image,
            cascade,
            storage,
            1.1,
            3,
            0 /*CV_HAAR_DO_CANNY_PRUNNING*/
            , cvSize( 40, 40 ) ,
              cvSize( 50, 50 )
            );

    /* for each face found, draw a red box */
    int i;
    for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ )
    {
        CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );

        AddToFeatureList(  video_register[vid_reg].faces  ,
                           r->x + (r->width/2), r->y + (r->height/2) ,
                           +r->height);
        /*
        CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );

        cvRectangle( image,
                     cvPoint( r->x, r->y ),
                     cvPoint( r->x + r->width, r->y + r->height ),
                     CV_RGB( 255, 0, 0 ), 1, 8, 0 );
            */
    }
	return 0;
}


void GetFaceNumber(char num,unsigned int *pos_x,unsigned int *pos_y,unsigned int *total_size)
{


  return;
}
