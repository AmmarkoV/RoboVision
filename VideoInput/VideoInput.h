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

#ifndef VIDEOINPUT_H_INCLUDED
#define VIDEOINPUT_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


enum input_modes
{
   LIVE_ON=0,
   RECORDING_ON,
   RECORDING_ONE_ON,
   PLAYBACK_ON,
   PLAYBACK_ON_LOADED,
   WORKING,
   NO_VIDEO_AVAILIABLE
};



struct VideoFeedSettings
{
   unsigned int EncodingType;
   unsigned int PixelFormat; // YOU CAN CHOOSE ONE OF THE FOLLOWING
   unsigned int FieldType;
};

/*
       Pixel format         FOURCC                        depth  Description
  V4L2_PIX_FMT_RGB332  v4l2_fourcc('R','G','B','1')    8  RGB-3-3-2
  V4L2_PIX_FMT_RGB444  v4l2_fourcc('R','4','4','4')   16  xxxxrrrr ggggbbbb
  V4L2_PIX_FMT_RGB555  v4l2_fourcc('R','G','B','O')   16  RGB-5-5-5
  V4L2_PIX_FMT_RGB565  v4l2_fourcc('R','G','B','P')   16  RGB-5-6-5
  V4L2_PIX_FMT_RGB555X v4l2_fourcc('R','G','B','Q')   16  RGB-5-5-5 BE
  V4L2_PIX_FMT_RGB565X v4l2_fourcc('R','G','B','R')   16  RGB-5-6-5 BE
  V4L2_PIX_FMT_BGR24   v4l2_fourcc('B','G','R','3')   24  BGR-8-8-8
  V4L2_PIX_FMT_RGB24   v4l2_fourcc('R','G','B','3')   24  RGB-8-8-8
  V4L2_PIX_FMT_BGR32   v4l2_fourcc('B','G','R','4')   32  BGR-8-8-8-8
  V4L2_PIX_FMT_RGB32   v4l2_fourcc('R','G','B','4')   32  RGB-8-8-8-8
  V4L2_PIX_FMT_GREY    v4l2_fourcc('G','R','E','Y')    8  Greyscale
  V4L2_PIX_FMT_Y16     v4l2_fourcc('Y','1','6',' ')   16  Greyscale
  V4L2_PIX_FMT_PAL8    v4l2_fourcc('P','A','L','8')    8  8-bit palette
  V4L2_PIX_FMT_YVU410  v4l2_fourcc('Y','V','U','9')    9  YVU 4:1:0
  V4L2_PIX_FMT_YVU420  v4l2_fourcc('Y','V','1','2')   12  YVU 4:2:0
  V4L2_PIX_FMT_YUYV    v4l2_fourcc('Y','U','Y','V')   16  YUV 4:2:2
  V4L2_PIX_FMT_UYVY    v4l2_fourcc('U','Y','V','Y')   16  YUV 4:2:2
  V4L2_PIX_FMT_YUV422P v4l2_fourcc('4','2','2','P')   16  YVU422 planar
  V4L2_PIX_FMT_YUV411P v4l2_fourcc('4','1','1','P')   16  YVU411 planar
  V4L2_PIX_FMT_Y41P    v4l2_fourcc('Y','4','1','P')   12  YUV 4:1:1
  V4L2_PIX_FMT_YUV444  v4l2_fourcc('Y','4','4','4')   16  xxxxyyyy uuuuvvvv
  V4L2_PIX_FMT_YUV555  v4l2_fourcc('Y','U','V','O')   16  YUV-5-5-5
  V4L2_PIX_FMT_YUV565  v4l2_fourcc('Y','U','V','P')   16  YUV-5-6-5
  V4L2_PIX_FMT_YUV32   v4l2_fourcc('Y','U','V','4')   32  YUV-8-8-8-8

  two planes -- one Y, one Cr + Cb interleaved
  V4L2_PIX_FMT_NV12    v4l2_fourcc('N','V','1','2')   12  Y/CbCr 4:2:0
  V4L2_PIX_FMT_NV21    v4l2_fourcc('N','V','2','1')   12  Y/CrCb 4:2:0

   The following formats are not defined in the V4L2 specification
  V4L2_PIX_FMT_YUV410  v4l2_fourcc('Y','U','V','9')    9  YUV 4:1:0
  V4L2_PIX_FMT_YUV420  v4l2_fourcc('Y','U','1','2')   12  YUV 4:2:0
  V4L2_PIX_FMT_YYUV    v4l2_fourcc('Y','Y','U','V')   16  YUV 4:2:2
  V4L2_PIX_FMT_HI240   v4l2_fourcc('H','I','2','4')    8  8-bit color
  V4L2_PIX_FMT_HM12    v4l2_fourcc('H','M','1','2')    8  YUV 4:2:0 16x16 macroblocks

  see http://www.siliconimaging.com/RGB%20Bayer.htm
  V4L2_PIX_FMT_SBGGR8  v4l2_fourcc('B','A','8','1')    8  BGBG.. GRGR..
  V4L2_PIX_FMT_SBGGR16 v4l2_fourcc('B','Y','R','2')   16  BGBG.. GRGR..

  compressed formats
  V4L2_PIX_FMT_MJPEG    v4l2_fourcc('M','J','P','G')   Motion-JPEG
  V4L2_PIX_FMT_JPEG     v4l2_fourcc('J','P','E','G')   JFIF JPEG
  V4L2_PIX_FMT_DV       v4l2_fourcc('d','v','s','d')   1394
  V4L2_PIX_FMT_MPEG     v4l2_fourcc('M','P','E','G')   MPEG-1/2/4

   Vendor-specific formats
  V4L2_PIX_FMT_WNVA     v4l2_fourcc('W','N','V','A')   Winnov hw compress
  V4L2_PIX_FMT_SN9C10X  v4l2_fourcc('S','9','1','0')   SN9C10x compression
  V4L2_PIX_FMT_PWC1     v4l2_fourcc('P','W','C','1')   pwc older webcam
  V4L2_PIX_FMT_PWC2     v4l2_fourcc('P','W','C','2')   pwc newer webcam
  V4L2_PIX_FMT_ET61X251 v4l2_fourcc('E','6','2','5')   ET61X251 compression

*/


char * VideoInput_Version();


int VideoInput_InitializeLibrary(int number_of_inputs);
int VideoInput_DeinitializeLibrary();

int VideoInput_OpenFeed(int inpt,char * viddev,int width,int height,int bitdepth,int framespersecond,char snapshots_on,struct VideoFeedSettings videosettings);
int VideoInput_ReOpenFeed(int inpt);
int VideoInput_CloseFeed(int inpt);

int VideoInput_PauseFeed(int feednum);
int VideoInput_UnpauseFeed(int feednum);

void   VideoInput_SetCameraParameter(int webcam_id,int param_id,double * param);
double VideoInput_GetCameraParameter(int webcam_id,int param_id);


int VideoInput_FeedReceiveLoopAlive(int feed_num);

unsigned char * VideoInput_GetEmptyFrame();
unsigned char * VideoInput_GetFrame(int webcam_id);
unsigned int VideoInput_NewFrameAvailiable(int webcam_id);
void VideoInput_SignalFrameProcessed(int webcam_id);

void VideoInput_SaveFrameJPEGMemory(int webcam_id,char * mem,unsigned long * mem_size);

// Playback / Recording
void Play(char * filename);
void PlayOne(char * filename);
void Record(char * filename,int timestamp_filename,int compress);
void RecordOneInMem(char * filename,int timestamp_filename,int compress,char * mem,unsigned long * mem_size);
void RecordOne(char * filename,int timestamp_filename,int compress);
void Stop();
unsigned int VideoSimulationState();
#ifdef __cplusplus
}
#endif


#endif // VIDEOINPUT_H_INCLUDED
