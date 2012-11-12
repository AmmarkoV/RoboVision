#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../VideoInput.h"
#include <linux/videodev2.h>



int WebIntNeedsNewSnapshot()
{
  return 1;
  FILE * pFile=0;
  pFile = fopen ("/home/ammar/public_html/viewers.dat","r");
  if (pFile!=NULL )
    {
     fclose(pFile);
      if( remove( "/home/ammar/public_html/viewers.dat" ) != 0 )
       {
        fprintf(stderr,"Could not clear viewers data\n");
        return 0;
       }
       fprintf(stderr,"Stream to viewers\n");

       return 1;
    }
 return 0;
}


int main()
{
    printf("Sample program that snaps an image every 10 seconds if a viewers.dat file exists and outputs it at /home/ammar/public_html/snap0.jpg \n");
    printf("Needs imagemagick for conversions :)\n\n\n");
    printf("Starting Video Input\n");
    if ( VideoInput_InitializeLibrary(1)==1 ) { printf(" .. done \n"); } else
                                              { printf(" .. failed \n"); return 0; }

    char BITRATE=32;
    struct VideoFeedSettings feedsettings={0};
    feedsettings.PixelFormat=V4L2_PIX_FMT_RGB24; BITRATE=24;

    printf("Starting Video Feed\n");
    if ( VideoInput_OpenFeed(0,(char *) "/dev/video0",320,240,BITRATE,25,1,feedsettings)==1  ) { printf(" .. done \n"); } else
                                                                                                 { printf(" .. failed \n"); return 0; }

    unsigned int milliseconds=0;
    unsigned int snapit=0;
    while (1)
     {
       VideoInput_GetFrame(0);
       usleep(1000);
       ++milliseconds;

       if ( milliseconds%10000 == 0 )
       { snapit=1; }

       if ((WebIntNeedsNewSnapshot()==1)&&( snapit ==1 ))
       {
        RecordOne((char*) "snap",1,1);
        //i=system((const char *)"convert snap0.ppm /home/ammar/public_html/snap0.jpg");
        milliseconds=0;
        snapit=0;
       }
     }



    printf("Closing Video Feeds\n");
    VideoInput_DeinitializeLibrary();
    return 0;
}
