/***************************************************************************
* Copyright (C) 2010 by Ammar Qammaz *
* ammarkov@gmail.com *
* *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or *
* (at your option) any later version. *
* *
* This program is distributed in the hope that it will be useful, *
* but WITHOUT ANY WARRANTY; without even the implied warranty of *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the *
* GNU General Public License for more details. *
* *
* You should have received a copy of the GNU General Public License *
* along with this program; if not, write to the *
* Free Software Foundation, Inc., *
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. *
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../VideoInput.h"
#include <linux/videodev2.h>

int main()
{
    struct timespec prog_start,prog_end;
    clock_gettime(CLOCK_REALTIME,&prog_start);

    printf("Testing VideoInput v%s !\n",VideoInput_Version());
    printf("This should compile ok provided that you link libVideoInput.a and pthreads\n");
    printf("For now the whole library is linux only , I have a windows version of the library\n");
    printf("I will have to merge them when I have the time..\n");
    printf("-----------------------------------------------------------\n");


    printf("I Will now attempt to start Video Devices with a total maximum number of 3 video devices! :) ..");
    if ( InitVideoInputs(3)==1 ) { printf(" .. done \n"); } else
                                 { printf(" .. failed \n"); return 0; }

    printf("I Will now attempt to start sampling on VideoDevice slot 0  :) \n");
    printf("The function called will be : InitVideoFeed(0,\"/dev/video0\",320,240,1);\n");
    printf("0 is the first device , /dev/video0 the linux location of the device , 320x240 the size of the picture and 1 means enable snapshots!\n");

    printf("Creating setting structure \n");
    struct VideoFeedSettings feedsettings={0};

    char BITRATE=32;

    feedsettings.PixelFormat=V4L2_PIX_FMT_YUYV; BITRATE=16;// <- Common setting for UVC webcams
    //feedsettings.PixelFormat=V4L2_PIX_FMT_RGB24; BITRATE=24;//<- NO COMPRESION Common setting for V4L devices


    char SNAPSHOTS_ON=1;

    struct timespec feed_start,feed_end;
    clock_gettime(CLOCK_REALTIME,&feed_start);
    if ( InitVideoFeed(0,(char *) "/dev/video0",320,240,BITRATE,SNAPSHOTS_ON,feedsettings)==1  ) { printf(" .. done \n"); } else
                                                                                            { printf(" .. failed \n"); return 0; }

    printf("Waiting for loop to begin receiving video ");

    int MAX_waittime=10000;
    int waittime=0;
    while ( ( !FeedReceiveLoopAlive(0) )&& (waittime<MAX_waittime) )
      {
        printf(".");
        ++waittime;
        usleep(50);
      }

    if (waittime>=MAX_waittime) {  printf(" failed! \n"); return 0;} else
                                {  printf(" ok! \n");}

    clock_gettime(CLOCK_REALTIME,&feed_end);
    printf("Bringing Video0 up took %u milliseconds ..!\n",(unsigned int) ((feed_end.tv_nsec-feed_start.tv_nsec)/1000000));


    printf("The Video feed is now beeing read from a secondary thread so each time we need a new snap we \"GetFrame\"\n");
    printf("unsigned char * pixels  is the pointer to the image data , this minimizes copying through memory\n");
    unsigned char * pixels = 0;



    int i , ilim=160;
    printf("I Will now attempt to catch %u frames \n",ilim);
    struct timespec start,end;
    clock_gettime(CLOCK_REALTIME,&start);
    for ( i=0; i<ilim; i++)
     {
       pixels = (unsigned char *) GetFrame(0);
     }
    clock_gettime(CLOCK_REALTIME,&end);
    unsigned int achieved_nsecs = end.tv_nsec-start.tv_nsec;
    unsigned int achieved_fps = (unsigned int) ( ( ilim * 1000 * 1000 ) / achieved_nsecs ) * 1000;

    if ( achieved_nsecs == 0 ) {} else
    printf(" %u frames took %u nanoseconds , achieved %u fps \n",ilim,achieved_nsecs,achieved_fps);
 /*   160   achieved_nsecs / 1000 * 1000 * 1000
      x     1 */

    printf("The numbers above are not actual frames grabbed but how many times we were able to grab a frame from the stream.");
    printf(" Most cameras operate at 25-30 fps we could by average access the frame %u times for each frame actually taken from the device\n\n\n",achieved_fps/30);



    if (pixels == 0 ) { printf("Something was not right and we got back a zero frame , test failed\n");
                        CloseVideoInputs();
                        return 1;}


    printf("I Will now extract the RGB value from pixel 0,0 and pixel 10,40 and 11,40 \n");
    int x=0,y=0;

    unsigned char *px=0;
    unsigned char *r=0;
    unsigned char *g=0;
    unsigned char *b=0;

    px=pixels+ (y*320*3 + x*3);
    r=px++; g=px++; b=px;
    printf("The RGB value of pixel %u,%u is %u %u %u \n",x,y,*r,*g,*b);

    x=10; y=40;
    px=pixels+ (y*320*3 + x*3);
    r=px++; g=px++; b=px++;
    printf("The RGB value of pixel %u,%u is %u %u %u \n",x,y,*r,*g,*b);

    r=px++; g=px++; b=px;
    printf("The RGB value of pixel %u,%u is %u %u %u \n",x+1,y,*r,*g,*b);


    printf ("I Will now try to write what the camera is seeing in a file called raw.ppm  ... ");
    fflush(0);

    RecordOne((char*) "raw",0,0);
    sleep(1);
    printf("Done\n");

    printf ("I Will now try to write what the camera is seeing in a file again in a file called raw.ppm  ... ");
    fflush(0);

    RecordOne((char*) "raw",0,0);
    sleep(1);
    printf("Done\n");

    printf ("I Will now try to emulate camera input using the written file called raw.ppm  ... ");
    Play((char*) "raw");
    sleep(1);
    printf("Done\n");

    printf ("I Will now try to write the file again in a file called piperaw.raw  ... ");
    RecordOne((char*) "piperaw",0,0);
    sleep(1);
    printf("Done\n");

    printf ("I Will now try to stop emulation and resume the live feed  ... ");
    Stop();
    sleep(1);
    printf("Done\n");


    printf("All tests are complete , closing video inputs!\n");
    CloseVideoInputs();
    printf("Done..!\n");

    clock_gettime(CLOCK_REALTIME,&prog_end);
    printf("Tests took a total of %u milliseconds ..!\n",(unsigned int) ( (prog_end.tv_nsec-prog_start.tv_nsec) /(1000 * 1000) ) );

    return 0;
}
