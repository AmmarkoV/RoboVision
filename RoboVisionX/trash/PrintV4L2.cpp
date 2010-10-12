#include "PrintV4L2.h"

void print(struct v4l2_capability cap)
{
  fprintf(stderr,"driver: %s\n",cap.driver);
  fprintf(stderr,"card: %s\n",cap.card);
  fprintf(stderr,"bus_info: %s\n",cap.bus_info);
  fprintf(stderr,"version: %u.%u.%u\n",
	  (cap.version >> 16) & 0xFF,
	  (cap.version >> 8) & 0xFF,
	  cap.version & 0xFF);
  fprintf(stderr,"capabilities:\n");
  fprintf(stderr,"  V4L2_CAP_VIDEO_CAPTURE      %d\n",(cap.capabilities & 0x00000001) !=0);
  fprintf(stderr,"  V4L2_CAP_VIDEO_OUTPUT       %d\n",(cap.capabilities & 0x00000002) !=0);
  fprintf(stderr,"  V4L2_CAP_VIDEO_OVERLAY      %d\n",(cap.capabilities & 0x00000004) !=0);
  fprintf(stderr,"  V4L2_CAP_VBI_CAPTURE        %d\n",(cap.capabilities & 0x00000010) !=0);
  fprintf(stderr,"  V4L2_CAP_VBI_OUTPUT         %d\n",(cap.capabilities & 0x00000020) !=0);
  fprintf(stderr,"  V4L2_CAP_SLICED_VBI_CAPTURE %d\n",(cap.capabilities & 0x00000040) !=0);
  fprintf(stderr,"  V4L2_CAP_SLICED_VBI_OUTPUT  %d\n",(cap.capabilities & 0x00000080) !=0);
  fprintf(stderr,"  V4L2_CAP_RDS_CAPTURE        %d\n",(cap.capabilities & 0x00000100) !=0);
  fprintf(stderr,"  V4L2_CAP_TUNER              %d\n",(cap.capabilities & 0x00010000) !=0);
  fprintf(stderr,"  V4L2_CAP_AUDIO              %d\n",(cap.capabilities & 0x00020000) !=0);
  fprintf(stderr,"  V4L2_CAP_RADIO              %d\n",(cap.capabilities & 0x00040000) !=0);
  fprintf(stderr,"  V4L2_CAP_READWRITE          %d\n",(cap.capabilities & 0x01000000) !=0);
  fprintf(stderr,"  V4L2_CAP_ASYNCIO            %d\n",(cap.capabilities & 0x02000000) !=0);
  fprintf(stderr,"  V4L2_CAP_STREAMING          %d\n",(cap.capabilities & 0x04000000) !=0);
}

void print(struct v4l2_format fmt)
{
  print(fmt.type);
  print(fmt.fmt.pix);
  print(fmt.fmt.win);
  print(fmt.fmt.vbi);
  //print(fmt.fmt.sliced);
}

void print(struct v4l2_pix_format pix)
{
  fprintf(stderr,"width: %d\n",pix.width);
  fprintf(stderr,"height: %d\n",pix.height);
  printPixelFormat(pix.pixelformat);
  print(pix.field);
  fprintf(stderr,"bytesperline: %d\n",pix.bytesperline);
  fprintf(stderr,"sizeimage: %d\n",pix.sizeimage);
  print(pix.colorspace);
  fprintf(stderr,"priv: %d\n",pix.priv);
}

void print(struct v4l2_window window)
{
  print(window.w);
  print(window.field);
  fprintf(stderr,"chromakey: %d\n",window.chromakey);
  fprintf(stderr,"clipcount: %d\n",window.clipcount);
  print(window.clips);
}


void print(struct v4l2_vbi_format vbi)
{
  fprintf(stderr,"sampling_rate: %d\n",vbi.sampling_rate);
  fprintf(stderr,"offset: %d\n",vbi.offset);
  fprintf(stderr,"samples_per_line: %d\n",vbi.samples_per_line);
  fprintf(stderr,"sample_format: %d\n",vbi.sample_format);
  fprintf(stderr,"start[0]: %d\n",vbi.start[0]);
  fprintf(stderr,"start[1]: %d\n",vbi.start[1]);
  fprintf(stderr,"count[0]: %d\n",vbi.count[0]);
  fprintf(stderr,"count[1]: %d\n",vbi.count[1]);
  fprintf(stderr,"flags: %d\n",vbi.flags);
}

void print(struct v4l2_queryctrl ctrl)
{
  fprintf(stderr,"name: %s\n",ctrl.name);
  fprintf(stderr,"id: %d\n",ctrl.id);
  fprintf(stderr,"type: %d\n",ctrl.type);
  fprintf(stderr,"minimum: %d\n",ctrl.minimum);
  fprintf(stderr,"maximum: %d\n",ctrl.maximum);
  fprintf(stderr,"step: %d\n",ctrl.step);
  fprintf(stderr,"default_value: %d\n",ctrl.default_value);
  fprintf(stderr,"flags: %d\n",ctrl.flags);
}

void print(struct v4l2_control  control)
{
  fprintf(stderr,"id: %d\n",control.id);
  fprintf(stderr,"value: %d\n",control.value);
}

//void print(struct v4l2_sliced_vbi_format sliced)
//{
//}

/*
void print(struct v4l2_cropcap cropcap)
{
 fprintf(stderr,"#VIDIOC_CROPCAP\n");
 fprintf(stderr,"type : ");
 if (cropcap.type==V4L2_BUF_TYPE_VIDEO_CAPTURE)
   fprintf(stderr,"V4L2_BUF_TYPE_VIDEO_CAPTURE\n");
 else if (cropcap.type==V4L2_BUF_TYPE_VIDEO_OUTPUT)
   fprintf(stderr,"V4L2_BUF_TYPE_VIDEO_OUTPUT\n");
 else if (cropcap.type==V4L2_BUF_TYPE_VIDEO_OVERLAY)
   fprintf(stderr,"V4L2_BUF_TYPE_VIDEO_OVERLAY\n");
 else if (cropcap.type==V4L2_BUF_TYPE_VBI_CAPTURE)
   fprintf(stderr,"V4L2_BUF_TYPE_VBI_CAPTURE\n");
 else if (cropcap.type==V4L2_BUF_TYPE_VBI_OUTPUT)
   fprintf(stderr,"V4L2_BUF_TYPE_VBI_OUTPUT\n");
 //else if (cropcap.type==V4L2_BUF_TYPE_SLICED_VBI_CAPTURE)
 //  fprintf(stderr,"V4L2_BUF_TYPE_SLICED_VBI_CAPTURE\n");
 //else if (cropcap.type==V4L2_BUF_TYPE_SLICED_VBI_OUTPUT)
 //  fprintf(stderr,"V4L2_BUF_TYPE_SLICED_VBI_OUTPUT\n");
 else if (cropcap.type==V4L2_BUF_TYPE_PRIVATE)
   fprintf(stderr,"V4L2_BUF_TYPE_PRIVATE\n");
 fprintf(stderr,"bounds : ");
}
*/

void print(struct v4l2_clip *clip)
{
  if (clip!=NULL)
  {
    fprintf(stderr,"clip:\n");
    print(clip->c);
    print(clip->next);
  }
}

void print(struct v4l2_rect rect)
{
  fprintf(stderr,"left: %d\n",rect.left);
  fprintf(stderr,"top: %d\n",rect.top);
  fprintf(stderr,"width: %d\n",rect.width);
  fprintf(stderr,"height: %d\n",rect.height);
}

void print(v4l2_buf_type type)
{
  fprintf(stderr,"type: ");
  if (type==V4L2_BUF_TYPE_VIDEO_CAPTURE)
   fprintf(stderr,"V4L2_BUF_TYPE_VIDEO_CAPTURE\n");
 else if (type==V4L2_BUF_TYPE_VIDEO_OUTPUT)
   fprintf(stderr,"V4L2_BUF_TYPE_VIDEO_OUTPUT\n");
 else if (type==V4L2_BUF_TYPE_VIDEO_OVERLAY)
   fprintf(stderr,"V4L2_BUF_TYPE_VIDEO_OVERLAY\n");
 else if (type==V4L2_BUF_TYPE_VBI_CAPTURE)
   fprintf(stderr,"V4L2_BUF_TYPE_VBI_CAPTURE\n");
 else if (type==V4L2_BUF_TYPE_VBI_OUTPUT)
   fprintf(stderr,"V4L2_BUF_TYPE_VBI_OUTPUT\n");
 //else if (type==V4L2_BUF_TYPE_SLICED_VBI_CAPTURE)
 //  fprintf(stderr,"V4L2_BUF_TYPE_SLICED_VBI_CAPTURE\n");
 //else if (type==V4L2_BUF_TYPE_SLICED_VBI_OUTPUT)
 //  fprintf(stderr,"V4L2_BUF_TYPE_SLICED_VBI_OUTPUT\n");
 else if (type==V4L2_BUF_TYPE_PRIVATE)
   fprintf(stderr,"V4L2_BUF_TYPE_PRIVATE\n");
 else fprintf(stderr,"UNKNOWN\n");
}

void print(v4l2_field field)
{
  fprintf(stderr,"field: ");
  if (field==V4L2_FIELD_ANY)
    fprintf(stderr,"V4L2_FIELD_ANY\n");
  else if (field==V4L2_FIELD_NONE)
    fprintf(stderr,"V4L2_FIELD_NONE\n");
  else if (field==V4L2_FIELD_TOP)
    fprintf(stderr,"V4L2_FIELD_TOP\n");
  else if (field==V4L2_FIELD_BOTTOM)
    fprintf(stderr,"V4L2_FIELD_BOTTOM\n");
  else if (field==V4L2_FIELD_INTERLACED)
    fprintf(stderr,"V4L2_FIELD_INTERLACED\n");
  else if (field==V4L2_FIELD_SEQ_TB)
    fprintf(stderr,"V4L2_FIELD_SEQ_TB\n");
  else if (field==V4L2_FIELD_SEQ_BT)
    fprintf(stderr,"V4L2_FIELD_SEQ_BT\n");
  else if (field==V4L2_FIELD_ALTERNATE)
    fprintf(stderr,"V4L2_FIELD_ALTERNATE\n");
  else fprintf(stderr,"UNKNOWN\n");
}

void print(v4l2_colorspace colorspace)
{
  fprintf(stderr,"colorspace: ");
  if (colorspace==V4L2_COLORSPACE_SMPTE170M)
    fprintf(stderr,"V4L2_COLORSPACE_SMPTE170M\n");
  else if (colorspace==V4L2_COLORSPACE_SMPTE240M)
    fprintf(stderr,"V4L2_COLORSPACE_SMPTE240M\n");
  else if (colorspace==V4L2_COLORSPACE_REC709)
    fprintf(stderr,"V4L2_COLORSPACE_REC709\n");
  else if (colorspace==V4L2_COLORSPACE_BT878)
    fprintf(stderr,"V4L2_COLORSPACE_BT878\n");
  else if (colorspace==V4L2_COLORSPACE_470_SYSTEM_M)
    fprintf(stderr,"V4L2_COLORSPACE_470_SYSTEM_M\n");
  else if (colorspace==V4L2_COLORSPACE_470_SYSTEM_BG)
    fprintf(stderr,"V4L2_COLORSPACE_470_SYSTEM_BG\n");
  else if (colorspace==V4L2_COLORSPACE_JPEG)
    fprintf(stderr,"V4L2_COLORSPACE_JPEG\n");
  else if (colorspace==V4L2_COLORSPACE_SRGB)
    fprintf(stderr,"V4L2_COLORSPACE_SRGB\n");
  else fprintf(stderr,"UNKNOWN\n");
}

void printPixelFormat(__u32 pixelformat)
{
  fprintf(stderr,"pixelformat: ");
  if (pixelformat==V4L2_PIX_FMT_RGB332)
    fprintf(stderr,"V4L2_COLORSPACE_470_SYSTEM_BG\n");
  else if (pixelformat==V4L2_PIX_FMT_RGB555)
    fprintf(stderr,"V4L2_PIX_FMT_RGB555\n");
  else if (pixelformat==V4L2_PIX_FMT_RGB565)
    fprintf(stderr,"V4L2_PIX_FMT_RGB565\n");
  else if (pixelformat==V4L2_PIX_FMT_RGB555X)
    fprintf(stderr,"V4L2_PIX_FMT_RGB555X\n");
  else if (pixelformat==V4L2_PIX_FMT_RGB565X)
    fprintf(stderr,"V4L2_PIX_FMT_RGB565X\n");
  else if (pixelformat==V4L2_PIX_FMT_BGR24)
    fprintf(stderr,"V4L2_PIX_FMT_BGR24\n");
  else if (pixelformat==V4L2_PIX_FMT_RGB24)
    fprintf(stderr,"V4L2_PIX_FMT_RGB24\n");
  else if (pixelformat==V4L2_PIX_FMT_BGR32)
    fprintf(stderr,"V4L2_PIX_FMT_BGR32\n");
  else if (pixelformat==V4L2_PIX_FMT_RGB32)
    fprintf(stderr,"V4L2_PIX_FMT_RGB32\n");
  else if (pixelformat==V4L2_PIX_FMT_GREY)
    fprintf(stderr,"V4L2_PIX_FMT_GREY\n");
  else if (pixelformat==V4L2_PIX_FMT_YVU410)
    fprintf(stderr,"V4L2_PIX_FMT_YVU410\n");
  else if (pixelformat==V4L2_PIX_FMT_YVU420)
    fprintf(stderr,"V4L2_PIX_FMT_YVU420\n");
  else if (pixelformat==V4L2_PIX_FMT_YUYV)
    fprintf(stderr,"V4L2_PIX_FMT_YUYV\n");
  else if (pixelformat==V4L2_PIX_FMT_UYVY)
    fprintf(stderr,"V4L2_PIX_FMT_UYVY\n");
  else if (pixelformat==V4L2_PIX_FMT_YUV422P)
    fprintf(stderr,"V4L2_PIX_FMT_YUV422P\n");
  else if (pixelformat==V4L2_PIX_FMT_YUV411P)
    fprintf(stderr,"V4L2_PIX_FMT_YUV411P\n");
  else if (pixelformat==V4L2_PIX_FMT_Y41P)
    fprintf(stderr,"V4L2_PIX_FMT_Y41P\n");

  else if (pixelformat==V4L2_PIX_FMT_NV12)
    fprintf(stderr,"V4L2_PIX_FMT_NV12\n");
  else if (pixelformat==V4L2_PIX_FMT_NV21)
    fprintf(stderr,"V4L2_PIX_FMT_NV21\n");

  else if (pixelformat==V4L2_PIX_FMT_YUV410)
    fprintf(stderr,"V4L2_PIX_FMT_YUV410\n");
  else if (pixelformat==V4L2_PIX_FMT_YUV420)
    fprintf(stderr,"V4L2_PIX_FMT_YUV420\n");
  else if (pixelformat==V4L2_PIX_FMT_YYUV)
    fprintf(stderr,"V4L2_PIX_FMT_YYUV\n");
  else if (pixelformat==V4L2_PIX_FMT_HI240)
    fprintf(stderr,"V4L2_PIX_FMT_HI240\n");

  else if (pixelformat==V4L2_PIX_FMT_SBGGR8)
    fprintf(stderr,"V4L2_PIX_FMT_SBGGR8\n");

  else if (pixelformat==V4L2_PIX_FMT_MJPEG)
    fprintf(stderr,"V4L2_PIX_FMT_MJPEG\n");
  else if (pixelformat==V4L2_PIX_FMT_JPEG)
    fprintf(stderr,"V4L2_PIX_FMT_JPEG\n");
  else if (pixelformat==V4L2_PIX_FMT_DV)
    fprintf(stderr,"V4L2_PIX_FMT_DV\n");
  else if (pixelformat==V4L2_PIX_FMT_MPEG)
    fprintf(stderr,"V4L2_PIX_FMT_MPEG\n");

  else if (pixelformat==V4L2_PIX_FMT_WNVA)
    fprintf(stderr,"V4L2_PIX_FMT_WNVA\n");
  else if (pixelformat==V4L2_PIX_FMT_SN9C10X)
    fprintf(stderr,"V4L2_PIX_FMT_SN9C10X\n");
  else fprintf(stderr,"UNKNOWN\n");
}
