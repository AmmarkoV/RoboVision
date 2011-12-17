/***************************************************************
 * Name:      VideoInputGUITesterMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2011-04-28
 * Copyright: Ammar Qammaz (http://62.103.22.50)
 * License:
 **************************************************************/

#include "VideoInputGUITesterMain.h"
#include <wx/msgdlg.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/wx.h>
#include <wx/utils.h>
#include "../VideoInput.h"
#include <linux/videodev2.h>

//(*InternalHeaders(VideoInputGUITesterDialog)
#include <wx/string.h>
#include <wx/intl.h>
//*)

struct VideoFeedSettings videosettings;
wxBitmap *default_feed=0;
wxBitmap *camera_feed=0;
wxImage  img;

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(VideoInputGUITesterDialog)
const long VideoInputGUITesterDialog::ID_BUTTON1 = wxNewId();
const long VideoInputGUITesterDialog::ID_STATICBOX1 = wxNewId();
const long VideoInputGUITesterDialog::ID_BUTTON2 = wxNewId();
const long VideoInputGUITesterDialog::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(VideoInputGUITesterDialog,wxDialog)
    //(*EventTable(VideoInputGUITesterDialog)
    //*)
        EVT_PAINT(VideoInputGUITesterDialog::OnPaint)
END_EVENT_TABLE()

VideoInputGUITesterDialog::VideoInputGUITesterDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(VideoInputGUITesterDialog)
    Create(parent, id, _("Video Input GUI Tester"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    SetClientSize(wxSize(397,355));
    ButtonQuit = new wxButton(this, ID_BUTTON1, _("Quit"), wxPoint(288,296), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, _("Video Input"), wxPoint(32,24), wxSize(340,260), 0, _T("ID_STATICBOX1"));
    ButtonSaveSnapshot = new wxButton(this, ID_BUTTON2, _("Save Snapshot"), wxPoint(40,296), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    DrawTimer.SetOwner(this, ID_TIMER1);
    DrawTimer.Start(100, false);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VideoInputGUITesterDialog::OnButtonQuitClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VideoInputGUITesterDialog::OnButtonSaveSnapshotClick);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&VideoInputGUITesterDialog::OnDrawTimerTrigger);
    //*)

    default_feed=new wxBitmap(wxT("empty.bmp"),wxBITMAP_TYPE_BMP);
    InitVideoInputs(1);
    
    unsigned int BITRATE = 0;
    //videosettings.PixelFormat=V4L2_PIX_FMT_YUYV; BITRATE=16; // <- Common compressed setting for UVC webcams
    videosettings.PixelFormat=V4L2_PIX_FMT_RGB24; BITRATE=24;   //   <- Common raw setting for UVC webcams ( Run Compat )

    InitVideoFeed(0,(char *) "/dev/video0",320,240,BITRATE,1,videosettings);
}

VideoInputGUITesterDialog::~VideoInputGUITesterDialog()
{
    //(*Destroy(VideoInputGUITesterDialog)
    //*)


}

void VideoInputGUITesterDialog::OnQuit(wxCommandEvent& event)
{
    Close();
    CloseVideoInputs();
}

unsigned char * GetFrame(int webcam_id);

void VideoInputGUITesterDialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to Video Input tester gui..."));
}

void VideoInputGUITesterDialog::OnDrawTimerTrigger(wxTimerEvent& event)
{
    Refresh();
}


void UpdateImageOfCamera()
{
  if (NewFrameAvailiable(0))
   {
     if ( camera_feed != 0 ) { delete camera_feed; camera_feed=0; }

     void *frame=GetFrame(0);
       if ( frame != 0)
        {
            img.SetData((unsigned char *)frame,320,240,true);
            camera_feed = new wxBitmap(img);
        }
   }
  }


void VideoInputGUITesterDialog::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  UpdateImageOfCamera();
  if ( camera_feed != 0 )
       {
         dc.DrawBitmap(*camera_feed,43,44,true);
       }
         else
       {
        dc.DrawBitmap(*default_feed,43,44,true);
       }


}


void VideoInputGUITesterDialog::OnButtonQuitClick(wxCommandEvent& event)
{
    Close();
}

void VideoInputGUITesterDialog::OnButtonSaveSnapshotClick(wxCommandEvent& event)
{
   RecordOne((char *) "snapshot",1,1);
}
