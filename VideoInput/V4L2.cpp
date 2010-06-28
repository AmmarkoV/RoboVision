#include "V4L2.h"

#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>          /* for videodev2.h */

static void errno_exit  (const char *   s)
{
        fprintf (stderr, "%s error %d, %s\n",
                 s, errno, strerror (errno));
        exit (EXIT_FAILURE);
}

static int xioctl  (int   fd, int   request,  void *  arg)
{
        int r;

        do r = ioctl (fd, request, arg);
        while (-1 == r && EINTR == errno);

        return r;
}

/**
 * Constructor
 * \pre -
 * \post -
 * \param [in] device filename of device corresponding to camera (default: "/dev/video")
 * \param [in] io_meth the io_methode to use (default: IO_METHOD_MMAP)
 * \param [out] -
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
V4L2::V4L2(char *device,io_method io_meth)
{
  io=io_meth;
  fd=-1;
  buffers= NULL;
  n_buffers= 0;

  strcpy(this->device,device);
  fprintf(stderr,"opening device: %s\n",device);
  struct stat st;

  if (-1 == stat (device, &st))
    {
        fprintf (stderr, "Cannot identify '%s': %d, %s\n",device, errno, strerror (errno));
        exit (EXIT_FAILURE);
    }

  if (!S_ISCHR (st.st_mode))
    {
        fprintf (stderr, "%s is no device\n", device);
        exit (EXIT_FAILURE);
    }

  fd = open (device, O_RDWR /* required */ | O_NONBLOCK, 0);

  if (-1 == fd)
   {
        fprintf (stderr, "Cannot open '%s': %d, %s\n",device, errno, strerror (errno));
        exit (EXIT_FAILURE);
   }

   fprintf(stderr,"device opening ok \n");

}

/**
 * Destructor
 * \pre -
 * \post -
 * \param [in] -
 * \param [out] -
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
V4L2::~V4L2()
{
  if (-1 == close (fd))
    errno_exit ("close");

  fd = -1;
}

/**
 * Prints camera setting.
 * \pre -
 * \post -
 * \param [in] -
 * \param [out] -
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
void V4L2::printSettings()
{ /*
  struct v4l2_capability cap;
  if (get(&cap))
  {
    fprintf(stderr,"### capabilities:\n");print(cap);
  }
  else
    fprintf(stderr,"no capability info\n");

  struct v4l2_format fmt;
  CLEAR (fmt);
  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (get(&fmt))
  {
    fprintf(stderr,"### format:\n");print(fmt);
  }
  else
    fprintf(stderr,"no format info\n");

  fprintf(stderr,"### user controls:\n");
  for (int i=V4L2_CID_BASE;i<V4L2_CID_LASTP1;i++)
  {
    struct v4l2_queryctrl ctrl;
    ctrl.id=i;
    if (get(&ctrl))
    {
      fprintf(stderr,"### ");print(ctrl);
      struct v4l2_control  control;
      control.id=i;
      if (get(&control))
      {
	print(control);
      }
    }
  }*/
}

/**
 * Get the file descriptor of the device representing the camera.
 * \pre -
 * \post -
 * \param [in] -
 * \param [out] -
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
int V4L2::getFileDescriptor()
{
  return fd;
}

/**
 * Get further camera capabilities.
 * \pre -
 * \post -
 * \param [in] -
 * \param [out] cap the capabilities
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
int V4L2::get(struct v4l2_capability *cap)
{
  if (-1 == xioctl (fd, VIDIOC_QUERYCAP,cap))
    return 0;
  else
    return 1;

}

/**
 * Set frame format.
 * see: http://staff.science.uva.nl/~bterwijn/Projects/V4L2/v4l2_website/v4l2spec.bytesex.org/spec-single/v4l2.html#V4L2-FORMAT
 * \pre -
 * \post -
 * \param [in] fmt the format
 * \param [out] -
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
int V4L2::set(struct v4l2_format fmt)
{
  fprintf(stderr,"setting mode format.. \n");
  if (-1 == xioctl (fd, VIDIOC_S_FMT, &fmt))
    return 0;
  else
    return 1;
}

/**
 * Get frame format.
 * \pre -
 * \post -
 * \param [in] -
 * \param [out] fmt the format
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
int V4L2::get(struct v4l2_format *fmt)
{
  if (-1 == xioctl (fd, VIDIOC_G_FMT, fmt))
    return 0;
  else
    return 1;
}

/**
 * Query control information on brightness, contrast, saturation, etc.
 * see: http://staff.science.uva.nl/~bterwijn/Projects/V4L2/v4l2_website/v4l2spec.bytesex.org/spec-single/v4l2.html#V4L2-QUERYCTRL
 * \pre -
 * \post -
 * \param [in] -
 * \param [out] ctrl the querried control information
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
int V4L2::get(struct v4l2_queryctrl *ctrl)
{
  if (-1 == xioctl (fd, VIDIOC_QUERYCTRL, ctrl))
    return 0;
  else
    return 1;
}

/**
 * Set control information on brightness, contrast, saturation, etc.
 * see: http://staff.science.uva.nl/~bterwijn/Projects/V4L2/v4l2_website/v4l2spec.bytesex.org/spec-single/v4l2.html#V4L2-VIDIOC_S_CTRL
 * \pre -
 * \post -
 * \param [in] -
 * \param [out] control the control information
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
int V4L2::set(struct v4l2_control  control)
{
  if (-1 == xioctl (fd, VIDIOC_S_CTRL, &control))
    return 0;
  else
    return 1;
}

/**
 * Get control information on brightness, contrast, saturation, etc.
 * see: http://staff.science.uva.nl/~bterwijn/Projects/V4L2/v4l2_website/v4l2spec.bytesex.org/spec-single/v4l2.html#V4L2-VIDIOC_G_CTRL
 * \pre -
 * \post -
 * \param [in] -
 * \param [out] control the control information
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
int V4L2::get(struct v4l2_control *control)
{
  if (-1 == xioctl (fd, VIDIOC_G_CTRL, control))
    return 0;
  else
    return 1;
}

/**
 * Initializes the buffers.
 * \pre -
 * \post -
 * \param [in] -
 * \param [out] -
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
void V4L2::initBuffers()
{
  struct v4l2_format fmt;
  CLEAR (fmt);
  fmt.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  get(&fmt);

  switch (io)
  {
    case IO_METHOD_READ: init_read (fmt.fmt.pix.sizeimage);
                         break;

    case IO_METHOD_MMAP: init_mmap ();
                         break;

    case IO_METHOD_USERPTR: init_userp (fmt.fmt.pix.sizeimage);
                            break;
  }
}

/**
 * Frees the buffers.
 * \pre -
 * \post -
 * \param [in] -
 * \param [out] -
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
void V4L2::freeBuffers()
{
  switch (io)
  {
  case IO_METHOD_READ:
    free (buffers[0].start);
    break;
  case IO_METHOD_MMAP:
    for (int i = 0; i < (int)n_buffers; ++i)
      if (-1 == munmap (buffers[i].start, buffers[i].length))
	errno_exit ("munmap");
    break;
  case IO_METHOD_USERPTR:
    for (int i = 0; i < (int)n_buffers; ++i)
      free (buffers[i].start);
    break;
  }
  free (buffers);
}

/**
 * Start the capture process
 * \pre buffers must be initialized
 * \post -
 * \param [in] -
 * \param [out] -
 * \return -
 * \retval -
 * \exception -
 * \sa initBuffers()
 */
void V4L2::startCapture()
{
  unsigned int i;
  enum v4l2_buf_type type;
  switch (io) {
  case IO_METHOD_READ:
    /* Nothing to do. */
    break;
  case IO_METHOD_MMAP:
    for (i = 0; i < n_buffers; ++i)
    {
      struct v4l2_buffer buf;
      CLEAR (buf);
      buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory      = V4L2_MEMORY_MMAP;
      buf.index       = i;
      if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
	errno_exit ("VIDIOC_QBUF");
    }
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl (fd, VIDIOC_STREAMON, &type))
                        errno_exit ("VIDIOC_STREAMON");
    break;
  case IO_METHOD_USERPTR:
    for (i = 0; i < n_buffers; ++i)
    {
      struct v4l2_buffer buf;
      CLEAR (buf);
      buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory      = V4L2_MEMORY_USERPTR;
      buf.index       = i;
      buf.m.userptr   = (unsigned long) buffers[i].start;
      buf.length      = buffers[i].length;
      if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
	errno_exit ("VIDIOC_QBUF");
    }
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl (fd, VIDIOC_STREAMON, &type))
      errno_exit ("VIDIOC_STREAMON");
    break;
  }
}

/**
 * Stopt the capture process
 * \pre -
 * \post -
 * \param [in] -
 * \param [out] -
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
void V4L2::stopCapture()
{
  enum v4l2_buf_type type;
  switch (io)
  {
  case IO_METHOD_READ: /* Nothing to do. */ break;
  case IO_METHOD_MMAP:
  case IO_METHOD_USERPTR:
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl (fd, VIDIOC_STREAMOFF, &type))
      errno_exit ("VIDIOC_STREAMOFF");
    break;
  }
}

/**
 * Gets a frame
 * \pre the capture should have been started
 * \post -
 * \param [in] -
 * \param [out] -
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
void *V4L2::getFrame()
{
  for (;;)
  {
    fd_set fds;
    struct timeval tv;
    int r;
    FD_ZERO (&fds);
    FD_SET (fd, &fds);
    /* Timeout. */
    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = TIMEOUT_USEC;

    r = select (fd + 1, &fds, NULL, NULL, &tv);

    if (-1 == r)
    {
      if (EINTR == errno)
	  continue;
      errno_exit ("select");
    }

    if (0 == r)
    {
      fprintf (stderr, "select timeout\n");
      exit (EXIT_FAILURE);
    }


    void *p=readFrame();
    if (p!=NULL) return p;
    /* EAGAIN - continue select loop. */
  }
}

/**
 * Reads a frame from the buffer
 * \pre the capture should have been started
 * \post -
 * \param [in] -
 * \param [out] -
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
void *V4L2::readFrame()
{
  struct v4l2_buffer buf;
  unsigned int i;

  switch (io) {
  case IO_METHOD_READ:
    if (-1 == read (fd, buffers[0].start, buffers[0].length))
    {
      switch (errno) {
                        case EAGAIN:
	                     return NULL;
                        case EIO:
	                       /* Could ignore EIO, see spec. */
	                              /* fall through */
                        default:
	                     errno_exit ("read");
                     }
    }
    return buffers[0].start;
    break;

  case IO_METHOD_MMAP:
    CLEAR (buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if (-1 == xioctl (fd, VIDIOC_DQBUF, &buf))
    {
      switch (errno) {
                        case EAGAIN:
	                     return NULL;
                        case EIO:
	                        /* Could ignore EIO, see spec. */
	                               /* fall through */
                         default:
	                      errno_exit ("VIDIOC_DQBUF");
                     }
    }
    assert (buf.index < n_buffers);
    if (-1 == xioctl (fd, VIDIOC_QBUF, &buf)) errno_exit ("VIDIOC_QBUF");
    return buffers[buf.index].start;
    break;

  case IO_METHOD_USERPTR:
    CLEAR (buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_USERPTR;
    if (-1 == xioctl (fd, VIDIOC_DQBUF, &buf)) {
      switch (errno) {
      case EAGAIN:
	return NULL;
      case EIO:
	/* Could ignore EIO, see spec. */
	/* fall through */
      default:
	errno_exit ("VIDIOC_DQBUF");
      }
    }
    for (i = 0; i < n_buffers; ++i)
      if (buf.m.userptr == (unsigned long) buffers[i].start
	  && buf.length == buffers[i].length)
	break;
    assert (i < n_buffers);
    if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
      errno_exit ("VIDIOC_QBUF");
    return (void *)(buf.m.userptr);
    break;

  }
  return NULL;
}

/**
 * Does IO_METHOD_READ buffer allocation
 * \pre -
 * \post -
 * \param [in] -
 * \param [out] -
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
void V4L2::init_read(unsigned int buffer_size)
{
  buffers = (buffer *)(calloc (1, sizeof (*buffers)));
  if (!buffers)
  {
    fprintf (stderr, "Out of memory\n");
    exit (EXIT_FAILURE);
  }

  buffers[0].length = buffer_size;
  buffers[0].start = malloc (buffer_size);
  if (!buffers[0].start)
  {
    fprintf (stderr, "Out of memory\n");
    exit (EXIT_FAILURE);
  }
}

/**
 * Does IO_METHOD_MMAP buffer allocation
 * \pre -
 * \post -
 * \param [in] -
 * \param [out] -
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
void V4L2::init_mmap(void)
{
  struct v4l2_requestbuffers req;
  CLEAR (req);
  req.count               = 4;
  req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory              = V4L2_MEMORY_MMAP;
  if (-1 == xioctl (fd, VIDIOC_REQBUFS, &req))
  {
    if (EINVAL == errno)
     {
        fprintf (stderr, "%s does not support memory mapping\n", device);
        exit (EXIT_FAILURE);
     } else
     {
        errno_exit ("VIDIOC_REQBUFS");
     }
  }
  if (req.count < 2)
  {
    fprintf (stderr, "Insufficient buffer memory on %s\n",device);
    exit (EXIT_FAILURE);
  }

  buffers = (buffer *)(calloc (req.count, sizeof (*buffers)));
  if (!buffers)
  {
    fprintf (stderr, "Out of memory\n");
    exit (EXIT_FAILURE);
  }

  for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
  {
    struct v4l2_buffer buf;
    CLEAR (buf);
    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = n_buffers;
    if (-1 == xioctl (fd, VIDIOC_QUERYBUF, &buf))
      errno_exit ("VIDIOC_QUERYBUF");
    buffers[n_buffers].length = buf.length;
    buffers[n_buffers].start =
      mmap (NULL /* start anywhere */,
	        buf.length,
	        PROT_READ | PROT_WRITE /* required */,
	        MAP_SHARED /* recommended */,
	        fd,
	        buf.m.offset);

    if (MAP_FAILED == buffers[n_buffers].start) errno_exit ("mmap");
  }
}

/**
 * Does IO_METHOD_USERPTR buffer allocation
 * \pre -
 * \post -
 * \param [in] -
 * \param [out] -
 * \return -
 * \retval -
 * \exception -
 * \sa -
 */
void V4L2::init_userp(unsigned int buffer_size)
{
  struct v4l2_requestbuffers req;
  CLEAR (req);
  req.count               = 4;
  req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory              = V4L2_MEMORY_USERPTR;
  if (-1 == xioctl (fd, VIDIOC_REQBUFS, &req))
  {
    if (EINVAL == errno)
    {
      fprintf (stderr, "%s does not support user pointer i/o\n", device);
      exit (EXIT_FAILURE);
    } else
    {
      errno_exit ("VIDIOC_REQBUFS");
    }
  }

  buffers = (buffer *)(calloc (4, sizeof (*buffers)));
  if (!buffers)
  {
    fprintf (stderr, "Out of memory\n");
    exit (EXIT_FAILURE);
  }

  for (n_buffers = 0; n_buffers < 4; ++n_buffers)
  {
    buffers[n_buffers].length = buffer_size;
    buffers[n_buffers].start = malloc (buffer_size);

    if (!buffers[n_buffers].start)
    {
      fprintf (stderr, "Out of memory\n");
      exit (EXIT_FAILURE);
    }
  }
}
