#include "FeedScreenMemory.h"
#include "RoboVisionXMain.h"
#include <wx/wx.h>
#include <wx/dc.h>
#include <wx/utils.h>
#include "../MotorFoundation/MotorHAL.h"
#include "../RoboKernel/RoboKernel.h"

// TO SET VIDEO MODE V4L2_PIX_FMT_RGB24
#include <linux/videodev2.h>

wxMouseState mouse;
wxBitmap *default_feed;
wxBitmap *default_patch;


struct feed live_feeds[4]={{0},{0},{0},{0}};


unsigned int last_viscrtx_pass=0;
unsigned int resc_width=320,resc_height=240;
int VideoFeedsNotAccessible = 0;

unsigned int  DepthMap(int typeofdata,int px,int py)
{
   return VisCortx_Get_DepthMapData(typeofdata,px,py);
  // return VisSub_Get_DepthMapData(typeofdata,px,py);
}

void memcpy_1bit_2_3bit(unsigned char * dest,unsigned char * src,unsigned int length)
{
  register unsigned char  *px;
  register unsigned char  *r;
  register unsigned char  *g;
  register unsigned char  *b;
  unsigned char val;
  px = (unsigned char  *)  dest;

  for ( unsigned int ptr = 0; ptr < length; ptr++)
   {
       r = px++;
       g = px++;
       b = px++;

       val = ( unsigned char ) src[ptr];
       *r= val;
       *g= val;
       *b= val;
   }
}

void InitFeeds()
{
    default_feed=new wxBitmap(wxT("feed.bmp"),wxBITMAP_TYPE_BMP);
    default_patch=new wxBitmap(wxT("patch.bmp"),wxBITMAP_TYPE_BMP);
    live_feeds[0].bmp=default_feed;
    live_feeds[0].bmp_allocated = false;
    live_feeds[1].bmp=default_feed;
    live_feeds[1].bmp_allocated = false;
    live_feeds[2].bmp=default_feed;
    live_feeds[2].bmp_allocated = false;
    live_feeds[3].bmp=default_feed;
    live_feeds[3].bmp_allocated = false;


           live_feeds[0].frame = malloc(GetCortexMetric(RESOLUTION_X)*GetCortexMetric(RESOLUTION_Y)*3*sizeof (unsigned char) );
           live_feeds[1].frame = malloc(GetCortexMetric(RESOLUTION_X)*GetCortexMetric(RESOLUTION_Y)*3*sizeof (unsigned char) );
           live_feeds[2].frame = malloc(GetCortexMetric(RESOLUTION_X)*GetCortexMetric(RESOLUTION_Y)*3*sizeof (unsigned char) );
           live_feeds[3].frame = malloc(GetCortexMetric(RESOLUTION_X)*GetCortexMetric(RESOLUTION_Y)*3*sizeof (unsigned char) );


    StartRoboKernel();

       VideoFeedsNotAccessible=1;
       has_init=1;

}

void CloseFeeds()
{
    if ( has_init == 0 ) { return; }
    has_init=0;

    VideoFeedsNotAccessible=0;

    StopRoboKernel();

    delete default_feed;
    delete default_patch;
    if (live_feeds[0].bmp_allocated) { delete live_feeds[0].bmp; live_feeds[0].bmp_allocated = false; }
    if (live_feeds[1].bmp_allocated) { delete live_feeds[1].bmp; live_feeds[1].bmp_allocated = false; }
    if (live_feeds[2].bmp_allocated) { delete live_feeds[2].bmp; live_feeds[2].bmp_allocated = false; }
    if (live_feeds[3].bmp_allocated) { delete live_feeds[3].bmp; live_feeds[3].bmp_allocated = false; }

    if ( live_feeds[0].frame != 0 ) free( live_feeds[0].frame );
    if ( live_feeds[1].frame != 0 ) free( live_feeds[1].frame );
    if ( live_feeds[2].frame != 0 ) free( live_feeds[2].frame );
    if ( live_feeds[3].frame != 0 ) free( live_feeds[3].frame );


     fprintf(stderr,"Gracefull exit :) \n");
}


bool XYOverFeed(int x , int y , feed feedmem)
{
   if ( (x>=feedmem.x1) && (x<=feedmem.x2) )
     {
         if ( (y>=feedmem.y1) && (y<=feedmem.y2) )
         {
           return true;
         }
     }
   return false;
}

void CheckMousePosition()
{
    mouse = wxGetMouseState();
    int x=mouse.GetX();
    int y=mouse.GetY();

    if ( XYOverFeed(x,y,live_feeds[2]) ) {}  else
    if ( XYOverFeed(x,y,live_feeds[3]) ) {}
}


void PassVideoRegisterToFeed(unsigned int feednum,void * framedata,unsigned int bitsperpixel)
{
 void *frame=0;

 if ( live_feeds[feednum].bmp_allocated ) { delete live_feeds[feednum].bmp; live_feeds[feednum].bmp_allocated = false; }
 frame = framedata;

 if ( frame != 0)
 {
    live_feeds[feednum].img.SetData((unsigned char *)framedata,GetCortexMetric(RESOLUTION_X),GetCortexMetric(RESOLUTION_Y),true);
    if ( resc_width != GetCortexMetric(RESOLUTION_X) ) { live_feeds[feednum].img.Rescale(resc_width,resc_height); }
    live_feeds[feednum].bmp= new wxBitmap(live_feeds[feednum].img);
    live_feeds[feednum].bmp_allocated = true;
 }
}

int SaveRegisterToFile(char * filename , unsigned int reg_num)
{
   return VisCortX_SaveVideoRegisterToFile(reg_num,filename);
}

int SnapWebCams()
{

 if (has_init==0) { fprintf(stderr,"!"); VideoFeedsNotAccessible=1; return 0; }


 if (last_viscrtx_pass==VisCortx_GetTime()) { return 0; }
 last_viscrtx_pass=VisCortx_GetTime();

 VideoFeedsNotAccessible=0;

 if ( live_feeds[0].bmp_allocated ) { delete live_feeds[0].bmp; live_feeds[0].bmp_allocated = false; }
 if ( live_feeds[1].bmp_allocated ) { delete live_feeds[1].bmp; live_feeds[1].bmp_allocated = false; }

 void *frame=0 ,*frame2 = 0;

 // GET INPUT  ( SWAPED OR NOT! )
 frame=GetVideoRegister(LEFT_EYE); frame2=GetVideoRegister(RIGHT_EYE);
 // GET INPUT  ( SWAPED OR NOT! )


 if ( frame != 0)
 {
    live_feeds[0].img.SetData(GetVideoRegister(LEFT_EYE),GetCortexMetric(RESOLUTION_X),GetCortexMetric(RESOLUTION_Y),true);
    if ( resc_width != GetCortexMetric(RESOLUTION_X) ) { live_feeds[0].img.Rescale(resc_width,resc_height); }
    live_feeds[0].bmp= new wxBitmap(live_feeds[0].img);
    live_feeds[0].bmp_allocated = true;
 }

 if ( frame2 != 0)
 {
  live_feeds[1].img.SetData(GetVideoRegister(RIGHT_EYE),GetCortexMetric(RESOLUTION_X),GetCortexMetric(RESOLUTION_Y),true);
  if ( resc_width != GetCortexMetric(RESOLUTION_X) ) { live_feeds[1].img.Rescale(resc_width,resc_height); }
  live_feeds[1].bmp= new wxBitmap(live_feeds[1].img);
  live_feeds[1].bmp_allocated = true;
 }

  return 1 ;
}


inline wxString _U(const char String[] = "")
{
  return wxString(String, wxConvUTF8);
}

void GUI_DrawOPFeeds()
{
  PassVideoRegisterToFeed ( 2 , VisCortx_ReadFromVideoRegister(LAST_LEFT_OPERATION,GetCortexMetric(RESOLUTION_X),GetCortexMetric(RESOLUTION_Y),3),3 );
  PassVideoRegisterToFeed ( 3 , VisCortx_ReadFromVideoRegister(LAST_RIGHT_OPERATION,GetCortexMetric(RESOLUTION_X),GetCortexMetric(RESOLUTION_Y),3),3 );
}

void GUI_FullDepthMap(unsigned char write_to_file)
{
  IssueCommand((char *)"DEPTH MAP",0,0,(char *)"GUI");
  PassVideoRegisterToFeed ( 2 , VisCortx_ReadFromVideoRegister(LAST_LEFT_OPERATION,GetCortexMetric(RESOLUTION_X),GetCortexMetric(RESOLUTION_Y),3),3 );
  PassVideoRegisterToFeed ( 3 , VisCortx_ReadFromVideoRegister(LAST_RIGHT_OPERATION,GetCortexMetric(RESOLUTION_X),GetCortexMetric(RESOLUTION_Y),3),3 );
}


void GUI_DrawMovement()
{
 IssueCommand((char *)"DRAW MOVEMENT",0,0,(char *)"GUI");
 PassVideoRegisterToFeed ( 2 , VisCortx_ReadFromVideoRegister(LAST_LEFT_OPERATION,GetCortexMetric(RESOLUTION_X),GetCortexMetric(RESOLUTION_Y),3),3 );
 PassVideoRegisterToFeed ( 3 , VisCortx_ReadFromVideoRegister(LAST_RIGHT_OPERATION,GetCortexMetric(RESOLUTION_X),GetCortexMetric(RESOLUTION_Y),3),3 );
}

void GUI_DrawNewPalette(char R,char G,char B,char threshold)
{
 KeepOnlyPixelsClosetoColor(R,G,B,threshold);
 PassVideoRegisterToFeed ( 2 , VisCortx_ReadFromVideoRegister(LAST_LEFT_OPERATION,GetCortexMetric(RESOLUTION_X),GetCortexMetric(RESOLUTION_Y),3),3 );
 PassVideoRegisterToFeed ( 3 , VisCortx_ReadFromVideoRegister(LAST_RIGHT_OPERATION,GetCortexMetric(RESOLUTION_X),GetCortexMetric(RESOLUTION_Y),3),3 );
}

void GUI_FindFeatures()
{
  VisCortx_RemoveTimedoutTrackPoints(1);
  VisCortx_AutoAddTrackPoints(0);
  PassVideoRegisterToFeed ( 3 , VisCortx_ReadFromVideoRegister(LAST_LEFT_OPERATION,GetCortexMetric(RESOLUTION_X),GetCortexMetric(RESOLUTION_Y),3),3 );
  fprintf(stderr," FindFeatures() ok \n");
}
