#ifndef FRAMESOSD_H_INCLUDED
#define FRAMESOSD_H_INCLUDED


#include <wx/wx.h>
#include <wx/dc.h>
#include "../VisualCortex/VisualCortex.h"


int FeaturesAquired(unsigned int video_reg_num);
int DrawFeaturesAtFeed(wxPaintDC & dc , unsigned int x , unsigned int y , unsigned int video_reg_num , unsigned int point , unsigned int line);
int DrawFacesAtFeed(wxPaintDC & dc , unsigned int x , unsigned int y , unsigned int video_reg_num );



#endif // FRAMESOSD_H_INCLUDED
