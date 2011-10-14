/***************************************************************
 * Name:      InternetVideoTransmissionMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2011-10-14
 * Copyright: Ammar Qammaz (http://ammar.gr)
 * License:
 **************************************************************/

#include "InternetVideoTransmissionMain.h"
#include "network_framework.h"
#include <wx/msgdlg.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/wx.h>
#include <wx/utils.h>

//(*InternalHeaders(InternetVideoTransmissionFrame)
#include <wx/string.h>
#include <wx/intl.h>
//*)

#include "../VideoInput.h"
#include <linux/videodev2.h>

struct VideoFeedSettings videosettings;
wxBitmap *default_feed=0;
wxBitmap *camera_feed=0,*camera_feed2=0;
wxImage  img,img2;


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

//(*IdInit(InternetVideoTransmissionFrame)
const long InternetVideoTransmissionFrame::ID_TEXTCTRL1 = wxNewId();
const long InternetVideoTransmissionFrame::ID_STATICTEXT1 = wxNewId();
const long InternetVideoTransmissionFrame::ID_BUTTON1 = wxNewId();
const long InternetVideoTransmissionFrame::idMenuQuit = wxNewId();
const long InternetVideoTransmissionFrame::idMenuAbout = wxNewId();
const long InternetVideoTransmissionFrame::ID_STATUSBAR1 = wxNewId();
const long InternetVideoTransmissionFrame::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(InternetVideoTransmissionFrame,wxFrame)
    //(*EventTable(InternetVideoTransmissionFrame)
    //*)
        EVT_PAINT(InternetVideoTransmissionFrame::OnPaint)
END_EVENT_TABLE()

InternetVideoTransmissionFrame::InternetVideoTransmissionFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(InternetVideoTransmissionFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(706,401));
    PeerIP = new wxTextCtrl(this, ID_TEXTCTRL1, _("127.0.0.1"), wxPoint(104,312), wxSize(200,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Peer IP :"), wxPoint(40,320), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    ButtonConnect = new wxButton(this, ID_BUTTON1, _("Connect"), wxPoint(312,312), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    Timer1.SetOwner(this, ID_TIMER1);
    Timer1.Start(25, false);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&InternetVideoTransmissionFrame::OnButtonConnectClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&InternetVideoTransmissionFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&InternetVideoTransmissionFrame::OnAbout);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&InternetVideoTransmissionFrame::OnTimer1Trigger);
    //*)

    default_feed=new wxBitmap(wxT("empty.bmp"),wxBITMAP_TYPE_BMP);
    InitVideoInputs(1);

    unsigned int BITRATE = 0;
    //videosettings.PixelFormat=V4L2_PIX_FMT_YUYV; BITRATE=16; // <- Common compressed setting for UVC webcams
    videosettings.PixelFormat=V4L2_PIX_FMT_RGB24; BITRATE=24;   //   <- Common raw setting for UVC webcams ( Run Compat )

    InitVideoFeed(0,(char *) "/dev/video0",320,240,BITRATE,1,videosettings);
    peer_feed = ( char * ) malloc(320*240*3*sizeof(char));


    StartupNetworkServer();
}

InternetVideoTransmissionFrame::~InternetVideoTransmissionFrame()
{
    //(*Destroy(InternetVideoTransmissionFrame)
    //*)
}

void InternetVideoTransmissionFrame::OnQuit(wxCommandEvent& event)
{
    network_receive_stop=1;
    network_transmit_stop=1;
    CloseVideoInputs();
    wxSleep(1);
    Close();

}



unsigned char * GetFrame(int webcam_id);

void InternetVideoTransmissionFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to Video Input tester gui..."));
}


void UpdateImageOfCamera()
{
  if (NewFrameAvailiable(0))
   {
     if ( camera_feed != 0 )  { delete camera_feed; camera_feed=0; }
     if ( camera_feed2 != 0 ) { delete camera_feed2; camera_feed2=0; }

     void *frame=GetFrame(0);
       if ( frame != 0)
        {
            img.SetData((unsigned char *)frame,320,240,true);
            camera_feed = new wxBitmap(img);
        }

     void *frame2=peer_feed;
       if ( frame != 0)
        {
            img2.SetData((unsigned char *)frame2,320,240,true);
            camera_feed2 = new wxBitmap(img2);
        }

   }
  }


void InternetVideoTransmissionFrame::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  UpdateImageOfCamera();
  if ( camera_feed != 0 )
       {
         dc.DrawBitmap(*camera_feed,30,30,true);
       }
         else
       {
        dc.DrawBitmap(*default_feed,30,30,true);
       }

  if ( camera_feed2 != 0 )
       {
         dc.DrawBitmap(*camera_feed2,360,30,true);
       }
         else
       {
        dc.DrawBitmap(*default_feed,360,30,true);
       }
}



void InternetVideoTransmissionFrame::OnTimer1Trigger(wxTimerEvent& event)
{
    Refresh();
}

void InternetVideoTransmissionFrame::OnButtonConnectClick(wxCommandEvent& event)
{
 char ip_cstr[200]={0};
 strcpy( ip_cstr, (const char*)PeerIP->GetValue().mb_str(wxConvUTF8) );
 StartupNetworkClient(ip_cstr,1234);
 ButtonConnect->Disable();
}
