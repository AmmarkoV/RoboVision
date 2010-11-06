#include "PixelFormats.h"



int VideoFormatNeedsDecoding(int videoformat,int bitdepth)
{
   switch (videoformat)
   {
      case V4L2_PIX_FMT_YUYV:
       return 1;
      break;
      case V4L2_PIX_FMT_RGB24:
       /* INPUT IS ALREADY RGB24 */
       return 0;
      break;
      default :
       printf("Video Format Needs Decoding but no handler implemnted, will propably output garbage on screen :S \n");
       return 0;
      break;
   };
  return 0;
}

int VideoFormatImplemented(int videoformat,int bitdepth)
{
   switch (videoformat)
   {
      case V4L2_PIX_FMT_YUYV:
       return 1;
      break;
      case V4L2_PIX_FMT_RGB24:
       /* INPUT IS ALREADY RGB24 */
       return 1;
      break;
      default :
       printf("Video Conversion Not Implemented, need to add handler for format , will propably output garbage on screen :S \n");
       return 0;
      break;
   };
  return 0;
}



void DebugSay(char * what)
{
 printf(" %s\n",what);
 return;
}

void PrintOutPixelFormat(int pix_format)
{
switch (pix_format)
     {
         case V4L2_PIX_FMT_YUYV :
          DebugSay((char *)"Setting pixel format to YUYV");
         break;
         case V4L2_PIX_FMT_VYUY :
          DebugSay((char *)"Setting pixel format to VYUY");
         break;
         case V4L2_PIX_FMT_YUV420 :
          DebugSay((char *)"Setting pixel format to YUV420");
         break;
         case V4L2_PIX_FMT_RGB24 :
          DebugSay((char *)"Setting pixel format to RGB24");
         break;
         case V4L2_PIX_FMT_BGR24 :
          DebugSay((char *)"Setting pixel format to BGR24");
         break;
         case V4L2_PIX_FMT_RGB32 :
          DebugSay((char *)"Setting pixel format to RGB32 ");
         break;
         case V4L2_PIX_FMT_YUV32 :
          DebugSay((char *)"Setting pixel format to YUV32 ");
         break;

         case V4L2_PIX_FMT_MJPEG :
          DebugSay((char *)"Setting pixel format to compressed MJPEG");
         break;
         case V4L2_PIX_FMT_JPEG :
          DebugSay((char *)"Setting pixel format to compressed JPEG");
         break;
         case V4L2_PIX_FMT_DV :
          DebugSay((char *)"Setting pixel format to compressed DV");
         break;
         case V4L2_PIX_FMT_MPEG :
          DebugSay((char *)"Setting pixel format to compressed MPEG ");
         break;

         default :
          DebugSay((char *)"Todo add pixel format to list :P");
         break;
     };
}

void PrintOutCaptureMode(int cap_mode)
{
switch (cap_mode)
     {
         case V4L2_BUF_TYPE_VIDEO_CAPTURE :
          DebugSay((char *)"Setting capture mode to Video Capture");
         break;
         case V4L2_BUF_TYPE_VIDEO_OUTPUT :
          DebugSay((char *)"Setting capture mode to Video Capture");
         break;
         case V4L2_BUF_TYPE_VIDEO_OVERLAY :
          DebugSay((char *)"Setting capture mode to Video Capture");
         break;
         case V4L2_BUF_TYPE_VBI_CAPTURE :
          DebugSay((char *)"Setting capture mode to Video Capture");
         break;
         case V4L2_BUF_TYPE_VBI_OUTPUT :
          DebugSay((char *)"Setting capture mode to Video Capture");
         break;
         case V4L2_BUF_TYPE_SLICED_VBI_CAPTURE :
          DebugSay((char *)"Setting capture mode to Video Capture");
         break;
         case V4L2_BUF_TYPE_SLICED_VBI_OUTPUT :
          DebugSay((char *)"Setting capture mode to Video Capture");
         break;
     };
}

void PrintOutFieldType(int field_type)
{
     switch (field_type)
     {
         case V4L2_FIELD_TOP :
          DebugSay((char *)"Setting pixel field to V4L2_FIELD_TOP");
         break;
         case V4L2_FIELD_INTERLACED :
          DebugSay((char *)"Setting pixel field to V4L2_FIELD_INTERLACED");
         break;
         case V4L2_FIELD_INTERLACED_TB :
          DebugSay((char *)"Setting pixel field to V4L2_FIELD_INTERLACED_TB");
         break;
         case V4L2_FIELD_INTERLACED_BT :
          DebugSay((char *)"Setting pixel field to V4L2_FIELD_INTERLACED_BT");
         break;
         case V4L2_FIELD_SEQ_TB :
          DebugSay((char *)"Setting pixel field to V4L2_FIELD_SEQ_TB");
         break;
         case V4L2_FIELD_SEQ_BT :
          DebugSay((char *)"Setting pixel field to V4L2_FIELD_SEQ_BT");
         break;

         default :
          DebugSay((char *)"Todo: Add video field type to list");
         break;
     };
}

