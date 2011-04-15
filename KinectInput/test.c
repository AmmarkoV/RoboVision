  #include <stdlib.h>
  #include <stdio.h>
  #include <opencv/cv.h>
  #include <opencv/highgui.h>
  #include "libfreenect_sync.h"
  int main()
  {   
      //freenect_sync_set_tilt_degs(20,0);

      IplImage *image = cvCreateImageHeader(cvSize(640,480), 8, 3);
      while (cvWaitKey(10) < 0) 
      {
        char *data;
        unsigned int timestamp;
        freenect_sync_get_video((void**)(&data), &timestamp, 0, FREENECT_VIDEO_RGB);
        cvSetData(image, data, 640*3);
        cvCvtColor(image, image, CV_RGB2BGR);
        cvShowImage("RGB", image);
      }
      freenect_sync_stop();       
      cvFree(&image);
      return EXIT_SUCCESS;
  }

