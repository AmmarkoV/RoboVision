
#include "V4L2.h"
#include "PixelFormats.h"

#include <iostream>
using namespace std;

int main(int argc,char **argv)
{
  if (argc>1 && strcmp(argv[1],"-h")==0)
  {
    fprintf(stderr,"usage: V4L2Test [device] [IO_METHOD_READ/IO_METHOD_MMAP/IO_METHOD_USERPTR]\n");
    exit(0);
  }

  char *dev="/dev/video0";
  if (argc>1)
    dev=argv[1];

  io_method io=IO_METHOD_MMAP;
  if (argc>2)
  {
    if (strcmp(argv[2],"IO_METHOD_READ")==0)
      io=IO_METHOD_READ;
    if (strcmp(argv[2],"IO_METHOD_MMAP")==0)
      io=IO_METHOD_MMAP;
    if (strcmp(argv[2],"IO_METHOD_USERPTR")==0)
      io=IO_METHOD_USERPTR;
  }

  V4L2 v4l2(dev,io);

  v4l2.printSettings();

  // set format
  struct v4l2_format fmt;
  CLEAR (fmt);
  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width       = 320;
  fmt.fmt.pix.height      = 240;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
  fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
  if (!v4l2.set(fmt))
  {
    fprintf(stderr,"device does not support settings:\n");
    print(fmt);
  }

  v4l2.initBuffers();
  v4l2.startCapture();
  for (int i=0;i<1;i++)
  {
    cout<<"grep frame"<<endl;
    void *frame=v4l2.getFrame();

    cout<<"save as test.yuv420 file"<<endl;
    FILE *file=fopen("test.yuv420", "wb");
    if (file!=NULL)
    {
      int width=fmt.fmt.pix.width;
      int height=fmt.fmt.pix.height;
      fprintf(file,"yuv420\n%u %u\n",width,height);
      fwrite((unsigned char *)(frame),1,(int)(width*height*1.5),file);
      fclose(file);
    }

    cout<<"save as test.ppm file"<<endl;
    void *rgb24=NULL;    
    rgb24=yuv420p_to_rgb24(fmt.fmt.pix.width,
			   fmt.fmt.pix.height,
			   (unsigned char *)frame,
			   (unsigned char *)rgb24);
    file=fopen("test.ppm", "wb");
    if (file!=NULL)
    {
      int width=fmt.fmt.pix.width;
      int height=fmt.fmt.pix.height;
      fprintf(file,"P6\n%u %u\n255\n",width,height);
      fwrite((unsigned char *)(rgb24),3,width*height,file);
      fclose(file);
    }
    

  }
  v4l2.stopCapture();
  v4l2.freeBuffers();

  return 1;
}
