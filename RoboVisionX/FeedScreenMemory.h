#ifndef FEEDSCREENMEMORY_H_INCLUDED
#define FEEDSCREENMEMORY_H_INCLUDED
#include "../VisualCortex/VisualCortex.h"
 //#include "../VisionSubsystem/VisionSubsystem.h"
#include <wx/wx.h>

struct feed
{
   unsigned short x1,y1,x2,y2;
   unsigned char feed_type;
   //wxMemoryDC *memDC;
   void *frame;
   wxImage img;
   wxBitmap *bmp;
   bool bmp_allocated;
};

extern feed live_feeds[4];
extern wxBitmap *default_feed;
extern wxBitmap *default_patch;
extern int has_init;
extern int VideoFeedsNotAccessible;

void InitFeeds();
void CloseFeeds();

int SaveRegisterToFile(char * filename , unsigned int reg_num);

int SnapWebCams();
void GUI_FullDepthMap(unsigned char write_to_file);
unsigned int  DepthMap(int typeofdata,int px,int py);
void GUI_DrawOPFeeds();
void GUI_DrawRectFeed(unsigned int feed,unsigned int x1,unsigned int y1,unsigned int size_x,unsigned int size_y,unsigned int color);
void GUI_DrawNewPalette(char R,char G,char B,char threshold);
void GUI_DrawMovement();
void GUI_FindFeatures();
#endif // FEEDSCREENMEMORY_H_INCLUDED
