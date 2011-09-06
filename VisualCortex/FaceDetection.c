#include "FaceDetection.h"
#include <stdio.h>
#include <stdlib.h>
#include "cv.h"
#include "VisionMemory.h"
#include "FeatureLists.h"

IplImage  *image;
char * opencv_pointer_retainer; // This is a kind of an ugly hack ( see lines noted with UGLY HACK ) to minimize memcpying between my VisCortex and OpenCV , without disturbing OpenCV

CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;


void InitFaceRecognition(unsigned int x,unsigned int y)
{
    char *filename = "haarcascade_frontalface_alt.xml";

    /* load the classifier
       note that I put the file in the same directory with
       this code */
    cascade = ( CvHaarClassifierCascade* ) cvLoad( filename, 0, 0, 0 );

    /* setup memory buffer; needed by the face detector */
    storage = cvCreateMemStorage( 0 );

    image = cvCreateImage( cvSize(x,y), IPL_DEPTH_8U, 3 );
    opencv_pointer_retainer = image->imageData; // UGLY HACK
}

void CloseFaceRecognition()
{
    cvReleaseHaarClassifierCascade( &cascade );
    cvReleaseMemStorage( &storage );

    image->imageData = opencv_pointer_retainer; // UGLY HACK
    cvReleaseImage(&image );
}


unsigned int RecognizeFaces(unsigned int vid_reg)
{
    ClearFeatureList(video_register[vid_reg].faces);

    /* detect faces */
    image->imageData=(char*) video_register[vid_reg].pixels; // UGLY HACK

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
    int circle_size;
    int i;
    for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ )
    {
        CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );


        circle_size = r->width;
        if ( r->width > r->height ) {  circle_size = r->height; }

        char timestamped_filename[512]={0};

       // GetANewSnapShotFileName(timestamped_filename,"memfs/faces/face_snap");
        SaveRegisterPartToFile("memfs/faces/face_snap",vid_reg,r->x,r->y,r->width,r->height);

        AddToFeatureList(  video_register[vid_reg].faces  ,
                           r->x + (r->width/2), r->y + (r->height/2) ,
                           circle_size );

    }
	return 0;
}

