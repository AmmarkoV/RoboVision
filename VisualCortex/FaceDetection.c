#include "FaceDetection.h"
#include <stdio.h>
#include <stdlib.h>

/*
	#ifndef USE_FD_LIB_FOR_FACEDETECTION
      #include "../3dpartylibs/fdlib_linux/fdlib.h"
    #elif defined(USE_OPENCV_FOR_FACEDETECTION)
    #endif
*/


unsigned char * graydata=0;
unsigned int i=0,w=0,h=0;
int x[256], y[256], size[256];
int threshold=-1;

void CloseFaceRecognition()
{
	if (graydata!=0) { free(graydata); }
	graydata=0;
	w=0;
	h=0;
}

void InitFaceRecognition(unsigned int x,unsigned int y)
{ //CloseFaceRecognition(); To kanei to VisionSubsystem me to init tou ousiastika
  graydata = (unsigned char*) malloc(x*y);
  w=x;
  h=y;
}


unsigned int RecognizeFaces(unsigned char * rgbdata)
{
/*
	#ifndef USE_FD_LIB_FOR_FACEDETECTION
    for (i=0; i<w*h; i++)
	{
	    graydata[i] = (unsigned char) ((.11*rgbdata[3*i+2] + .59*rgbdata[3*i+1] + .3*rgbdata[3*i+0]));
	}

	fdlib_detectfaces(graydata, w, h, threshold);

	int n = fdlib_getndetections();

	for ( i=0; i<n; i++)
	{
	    fdlib_getdetection(i, x+i, y+i, size+i);
	    //printf("x:%d y:%d size:%d\n", x[i], y[i], size[i]);
	}

	return n;
    #elif defined(USE_OPENCV_FOR_FACEDETECTION)

     fprintf(stderr,"No Face detection used..");

    #endif
*/

	return 0;
}


void GetFaceNumber(char num,unsigned int *pos_x,unsigned int *pos_y,unsigned int *total_size)
{
  unsigned int number=num;
  *pos_x=x[number];
  *pos_y=y[number];
  *total_size=size[number];
  return;
}
