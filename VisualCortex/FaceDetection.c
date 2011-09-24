#include "FaceDetection.h"
#include <stdio.h>
#include <stdlib.h>
#include "cv.h"
#include "VisionMemory.h"
#include "FeatureLists.h"
#include "StateSetting.h"

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
    cvReleaseImage( &image );
}


unsigned int RecognizeFaces(unsigned int vid_reg)
{

    ClearFeatureList(video_register[vid_reg].faces);

    if  (  video_register[vid_reg].pixels == 0 )  { return 0; }

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
            , cvSize( 40, 40 )
            , cvSize( 50, 50 ) // <--- This might have to be commented out on older OpenCV versions where there is only a minimum Window!
            );

    /* for each face found, draw a red box */
    char timestamped_filename[512]={0};
    int i;
    for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ )
    {
        CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );

        if ( settings[REMEMBER_FACES] )
         {
           timestamped_filename[0]=0; timestamped_filename[1]=0;
           GetANewSnapShotFileName(timestamped_filename,"memfs/faces/face_snap",".ppm");
           SaveRegisterPartToFile(timestamped_filename,vid_reg, r->x , r->y , r->width , r->height );
         }

        AddToFeatureList(  video_register[vid_reg].faces  ,
                           r->x , r->y , 0 ,
                           r->width , r->height , 0
                         );

    }
	return 0;
}

