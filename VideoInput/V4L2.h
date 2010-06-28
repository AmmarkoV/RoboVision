#include <stdio.h>
#include <stdlib.h>

//#include <asm-i386/types.h>
#include <linux/types.h>
#include <linux/videodev2.h>
#include "PrintV4L2.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))

typedef enum
{
  IO_METHOD_READ,
  IO_METHOD_MMAP,
  IO_METHOD_USERPTR,
} io_method;

struct buffer {
  void * start;
  size_t length;
};

/**
 * Class for getting frames from a camera via the Video For Linux library. This
 * is just an object oriented wrapper class. See
 * http://staff.science.uva.nl/~bterwijn/Projects/V4L2/v4l2_website/v4l2spec.bytesex.org/spec-single/v4l2.html for Video For Linux documentation.
 */

#define TIMEOUT_SEC 10
#define TIMEOUT_USEC 0

class V4L2
{
 public:
  V4L2(char *device="/dev/video",io_method io_meth=IO_METHOD_MMAP);
  ~V4L2();

  void printSettings();

  int getFileDescriptor();
  int get(struct v4l2_capability *cap);
  int set(struct v4l2_format  fmt);
  int get(struct v4l2_format *fmt);
  int get(struct v4l2_queryctrl *ctrl);
  int set(struct v4l2_control  control);
  int get(struct v4l2_control *control);

  void initBuffers();
  void startCapture();
  void *getFrame();
  void stopCapture();
  void freeBuffers();

 private:
  char device[100];
  io_method io;
  int fd;
  buffer *buffers;
  unsigned int n_buffers;

  void *readFrame();
  void init_read(unsigned int buffer_size);
  void init_mmap(void);
  void init_userp(unsigned int buffer_size);
};
