/***************************************************************
 * Name:      RoboVisionXMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2009-11-01
 * Copyright: Ammar Qammaz (http://ammar.gr)
 * License:
 **************************************************************/
#include "RoboVisionXMain.h"
#include "FeedScreenMemory.h"
#include "CortexSettings.h"
#include "RememberImage.h"

#include "FramesOSD.h"
#include "version.h"


#include "../MotorFoundation/MotorHAL.h"
#include "../RoboKernel/RoboKernel.h"
#include "../WorldMapping/MasterWorld/MasterWorld.h"


#include <wx/msgdlg.h>
#include <wx/wx.h>
//(*InternalHeaders(RoboVisionXFrame)
#include <wx/string.h>
#include <wx/intl.h>
#include <wx/font.h>
//*)

#include <wx/stopwatch.h>

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

//(*IdInit(RoboVisionXFrame)
const long RoboVisionXFrame::ID_STATICBOX1 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT1 = wxNewId();
const long RoboVisionXFrame::ID_BUTTON1 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT2 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT3 = wxNewId();
const long RoboVisionXFrame::ID_BUTTON2 = wxNewId();
const long RoboVisionXFrame::ID_STATICBOX2 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT4 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT5 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT6 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT7 = wxNewId();
const long RoboVisionXFrame::ID_TEXTCTRL1 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT8 = wxNewId();
const long RoboVisionXFrame::ID_GAUGE1 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT9 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT10 = wxNewId();
const long RoboVisionXFrame::ID_TEXTCTRL2 = wxNewId();
const long RoboVisionXFrame::ID_BUTTON3 = wxNewId();
const long RoboVisionXFrame::ID_BUTTON4 = wxNewId();
const long RoboVisionXFrame::ID_BUTTON5 = wxNewId();
const long RoboVisionXFrame::ID_BUTTON6 = wxNewId();
const long RoboVisionXFrame::ID_BUTTON7 = wxNewId();
const long RoboVisionXFrame::ID_BUTTON8 = wxNewId();
const long RoboVisionXFrame::ID_CHECKBOX1 = wxNewId();
const long RoboVisionXFrame::ID_CHOICE1 = wxNewId();
const long RoboVisionXFrame::ID_CHOICE2 = wxNewId();
const long RoboVisionXFrame::ID_BUTTON9 = wxNewId();
const long RoboVisionXFrame::ID_BUTTON10 = wxNewId();
const long RoboVisionXFrame::ID_BUTTON11 = wxNewId();
const long RoboVisionXFrame::ID_BUTTON12 = wxNewId();
const long RoboVisionXFrame::ID_BUTTON13 = wxNewId();
const long RoboVisionXFrame::ID_BUTTON14 = wxNewId();
const long RoboVisionXFrame::ID_CHECKBOX2 = wxNewId();
const long RoboVisionXFrame::ID_SLIDER1 = wxNewId();
const long RoboVisionXFrame::ID_SLIDER2 = wxNewId();
const long RoboVisionXFrame::ID_CHECKBOX3 = wxNewId();
const long RoboVisionXFrame::ID_GAUGE2 = wxNewId();
const long RoboVisionXFrame::ID_GAUGE3 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT11 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT12 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT13 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT14 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT15 = wxNewId();
const long RoboVisionXFrame::ID_CHECKBOX4 = wxNewId();
const long RoboVisionXFrame::ID_STATICTEXT16 = wxNewId();
const long RoboVisionXFrame::idMenuOpenSnapshots = wxNewId();
const long RoboVisionXFrame::idMenuQuit = wxNewId();
const long RoboVisionXFrame::idMenuAbout = wxNewId();
const long RoboVisionXFrame::ID_STATUSBAR1 = wxNewId();
const long RoboVisionXFrame::ID_TIMER1 = wxNewId();
//*)


BEGIN_EVENT_TABLE(RoboVisionXFrame,wxFrame)
    //(*EventTable(RoboVisionXFrame)
    //*)
    EVT_TIMER(-1,RoboVisionXFrame::OnTimerEvent)
    EVT_PAINT(RoboVisionXFrame::OnPaint)
    EVT_MOTION(RoboVisionXFrame::OnMotion)

END_EVENT_TABLE()

unsigned int WAIT_TIME_REDRAW = 250; // WAIT TIME PER FRAME
unsigned int WAIT_TIME_STARTUP = 5000;
int position = 0;
char * version_code_name = (char *) " -  Luben ";

inline wxString _U(const char String[] = "")
{
  return wxString(String, wxConvUTF8);
}


RoboVisionXFrame::RoboVisionXFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(RoboVisionXFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuItem* MenuItem3;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, _("RoboVision X"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(924,584));
    StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, _("Position Status"), wxPoint(680,32), wxSize(232,168), 0, _T("ID_STATICBOX1"));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Dist. Traveled :"), wxPoint(688,54), wxSize(128,17), 0, _T("ID_STATICTEXT1"));
    ButtonDepthMap = new wxButton(this, ID_BUTTON1, _("Depth Map"), wxPoint(680,392), wxSize(120,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Left Cam"), wxPoint(15,0), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Right Cam"), wxPoint(340,0), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    MotionAlarmButton = new wxButton(this, ID_BUTTON2, _("Motion Alarm"), wxPoint(808,392), wxSize(104,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    StaticBox2 = new wxStaticBox(this, ID_STATICBOX2, _("General"), wxPoint(680,200), wxSize(232,72), 0, _T("ID_STATICBOX2"));
    UptimeLabel = new wxStaticText(this, ID_STATICTEXT4, _("Uptime :"), wxPoint(688,220), wxSize(72,17), 0, _T("ID_STATICTEXT4"));
    Uptime = new wxStaticText(this, ID_STATICTEXT5, _("0"), wxPoint(760,220), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    StaticText4 = new wxStaticText(this, ID_STATICTEXT6, _("Flow : "), wxPoint(688,235), wxSize(48,17), 0, _T("ID_STATICTEXT6"));
    Flow = new wxStaticText(this, ID_STATICTEXT7, _("0"), wxPoint(736,235), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    Status = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxPoint(680,280), wxSize(232,56), wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    StaticText5 = new wxStaticText(this, ID_STATICTEXT8, _("Battery"), wxPoint(680,8), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    Battery = new wxGauge(this, ID_GAUGE1, 100, wxPoint(736,8), wxSize(176,16), 0, wxDefaultValidator, _T("ID_GAUGE1"));
    StaticText6 = new wxStaticText(this, ID_STATICTEXT9, _("FrameRate :"), wxPoint(688,248), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    FrameRate = new wxStaticText(this, ID_STATICTEXT10, _("0"), wxPoint(776,248), wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    CommandBox = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxPoint(680,336), wxSize(200,27), wxTE_PROCESS_ENTER, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    Ok = new wxButton(this, ID_BUTTON3, _("OK"), wxPoint(880,336), wxSize(32,24), 0, wxDefaultValidator, _T("ID_BUTTON3"));
    SwapFeeds = new wxButton(this, ID_BUTTON4, _("Swap Feeds"), wxPoint(744,368), wxSize(64,24), 0, wxDefaultValidator, _T("ID_BUTTON4"));
    wxFont SwapFeedsFont(6,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    SwapFeeds->SetFont(SwapFeedsFont);
    RecordButton = new wxButton(this, ID_BUTTON5, _("Rec"), wxPoint(680,416), wxSize(40,24), 0, wxDefaultValidator, _T("ID_BUTTON5"));
    PlayButton = new wxButton(this, ID_BUTTON6, _("Play"), wxPoint(720,416), wxSize(48,24), 0, wxDefaultValidator, _T("ID_BUTTON6"));
    LiveButton = new wxButton(this, ID_BUTTON7, _("Live"), wxPoint(768,416), wxSize(144,24), 0, wxDefaultValidator, _T("ID_BUTTON7"));
    AddTrackPoint = new wxButton(this, ID_BUTTON8, _("+ Track"), wxPoint(680,368), wxSize(64,24), 0, wxDefaultValidator, _T("ID_BUTTON8"));
    Autonomous = new wxCheckBox(this, ID_CHECKBOX1, _("Auto"), wxPoint(680,440), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    Autonomous->SetValue(false);
    Choice1 = new wxChoice(this, ID_CHOICE1, wxPoint(8,504), wxSize(320,30), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    Choice1->Append(_("Automatic Feed"));
    Choice1->Append(_("Left Eye"));
    Choice1->Append(_("Right Eye"));
    Choice1->Append(_("Last Left Eye"));
    Choice1->Append(_("Last Right Eye"));
    Choice1->Append(_("Background Left"));
    Choice1->Append(_("Background Right"));
    Choice1->Append(_("Movement Left"));
    Choice1->Append(_("Movemenet Right"));
    Choice1->Append(_("Group Movement Left"));
    Choice1->Append(_("Group Movement Right"));
    Choice1->Append(_("Difference Left"));
    Choice1->Append(_("Difference Right"));
    Choice1->Append(_("Depth Left "));
    Choice1->Append(_("Depth Right"));
    Choice1->Append(_("Edges Left"));
    Choice1->Append(_("Edges Right"));
    Choice1->Append(_("General 1"));
    Choice1->Append(_("General 2 "));
    Choice1->Append(_("General 3"));
    Choice1->Append(_("General 4"));
    Choice2 = new wxChoice(this, ID_CHOICE2, wxPoint(340,504), wxSize(320,30), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
    Choice2->Append(_("Automatic Feed"));
    Choice2->Append(_("Left Eye"));
    Choice2->Append(_("Right Eye"));
    Choice2->Append(_("Last Left Eye"));
    Choice2->Append(_("Last Right Eye"));
    Choice2->Append(_("Background Left"));
    Choice2->Append(_("Background Right"));
    Choice2->Append(_("Movement Left"));
    Choice2->Append(_("Movemenet Right"));
    Choice2->Append(_("Group Movement Left"));
    Choice2->Append(_("Group Movement Right"));
    Choice2->Append(_("Difference Left"));
    Choice2->Append(_("Difference Right"));
    Choice2->Append(_("Depth Left "));
    Choice2->Append(_("Depth Right"));
    Choice2->Append(_("Edges Left"));
    Choice2->Append(_("Edges Right"));
    Choice2->Append(_("General 1"));
    Choice2->Append(_("General 2 "));
    Choice2->Append(_("General 3"));
    Choice2->Append(_("General 4"));
    ConfigurationButton = new wxButton(this, ID_BUTTON9, _("Configuration"), wxPoint(808,368), wxSize(104,24), 0, wxDefaultValidator, _T("ID_BUTTON9"));
    LeftButton = new wxButton(this, ID_BUTTON10, _("<"), wxPoint(840,84), wxSize(24,29), 0, wxDefaultValidator, _T("ID_BUTTON10"));
    RightButton = new wxButton(this, ID_BUTTON11, _(">"), wxPoint(888,84), wxSize(24,29), 0, wxDefaultValidator, _T("ID_BUTTON11"));
    UpButton = new wxButton(this, ID_BUTTON12, _("/\\"), wxPoint(864,56), wxSize(24,29), 0, wxDefaultValidator, _T("ID_BUTTON12"));
    DownButton = new wxButton(this, ID_BUTTON13, _("\\/"), wxPoint(864,112), wxSize(24,29), 0, wxDefaultValidator, _T("ID_BUTTON13"));
    StopButton = new wxButton(this, ID_BUTTON14, _("O"), wxPoint(864,84), wxSize(23,28), 0, wxDefaultValidator, _T("ID_BUTTON14"));
    DrawFeeds = new wxCheckBox(this, ID_CHECKBOX2, _("Draw Feeds"), wxPoint(736,440), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    DrawFeeds->SetValue(true);
    MovementHorizontal = new wxSlider(this, ID_SLIDER1, 10, 0, 20, wxPoint(840,136), wxSize(72,25), 0, wxDefaultValidator, _T("ID_SLIDER1"));
    MovementVertical = new wxSlider(this, ID_SLIDER2, 10, 0, 20, wxPoint(816,56), wxSize(24,80), wxSL_VERTICAL, wxDefaultValidator, _T("ID_SLIDER2"));
    lowcpuusage = new wxCheckBox(this, ID_CHECKBOX3, _("Low CPU"), wxPoint(840,440), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
    lowcpuusage->SetValue(false);
    Ultrasonic1 = new wxGauge(this, ID_GAUGE2, 255, wxPoint(696,104), wxSize(100,16), 0, wxDefaultValidator, _T("ID_GAUGE2"));
    Ultrasonic2 = new wxGauge(this, ID_GAUGE3, 255, wxPoint(696,120), wxSize(100,16), 0, wxDefaultValidator, _T("ID_GAUGE3"));
    StaticText7 = new wxStaticText(this, ID_STATICTEXT11, _("Ultrasonic"), wxPoint(688,88), wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    DistanceTraveled = new wxStaticText(this, ID_STATICTEXT12, _("0"), wxPoint(696,70), wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    wxFont DistanceTraveledFont(13,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    DistanceTraveled->SetFont(DistanceTraveledFont);
    MeterComment = new wxStaticText(this, ID_STATICTEXT13, _("Meters"), wxPoint(720,72), wxDefaultSize, 0, _T("ID_STATICTEXT13"));
    LeftUltrasonicLabel = new wxStaticText(this, ID_STATICTEXT14, _("L"), wxPoint(688,104), wxDefaultSize, 0, _T("ID_STATICTEXT14"));
    RightUltrasonicLabel = new wxStaticText(this, ID_STATICTEXT15, _("R"), wxPoint(688,120), wxDefaultSize, 0, _T("ID_STATICTEXT15"));
    SaveLoadStreamOfSnapshots = new wxCheckBox(this, ID_CHECKBOX4, _("Rec/Play Stream of Snapshots"), wxPoint(680,464), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
    SaveLoadStreamOfSnapshots->SetValue(false);
    PoseString = new wxStaticText(this, ID_STATICTEXT16, _("X:0.0   Y:0.0  Z:0.0  heading: 0.0 "), wxPoint(680,176), wxSize(232,17), 0, _T("ID_STATICTEXT16"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu1, idMenuOpenSnapshots, _("Open Snapshot\tF2"), _("Opens a recorded snapshot"), wxITEM_NORMAL);
    Menu1->Append(MenuItem3);
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
    Timer1.Start(70, false);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnButtonDepthMapClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnMotionAlarmButtonClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnOkClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnSwapFeedsClick);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnRecordButtonClick);
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnPlayButtonClick);
    Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnLiveButtonClick);
    Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnAddTrackPointClick);
    Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnAutonomousClick1);
    Connect(ID_BUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnConfigurationButtonClick);
    Connect(ID_BUTTON10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnLeftButtonClick);
    Connect(ID_BUTTON11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnRightButtonClick);
    Connect(ID_BUTTON12,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnUpButtonClick);
    Connect(ID_BUTTON13,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnDownButtonClick);
    Connect(ID_BUTTON14,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnStopButtonClick);
    Connect(ID_SLIDER1,wxEVT_SCROLL_TOP|wxEVT_SCROLL_BOTTOM|wxEVT_SCROLL_LINEUP|wxEVT_SCROLL_LINEDOWN|wxEVT_SCROLL_PAGEUP|wxEVT_SCROLL_PAGEDOWN|wxEVT_SCROLL_THUMBTRACK|wxEVT_SCROLL_THUMBRELEASE|wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&RoboVisionXFrame::OnMovementHorizontalCmdScroll);
    Connect(ID_SLIDER1,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&RoboVisionXFrame::OnMovementHorizontalCmdScroll);
    Connect(ID_SLIDER1,wxEVT_SCROLL_THUMBRELEASE,(wxObjectEventFunction)&RoboVisionXFrame::OnMovementHorizontalCmdScroll);
    Connect(ID_SLIDER2,wxEVT_SCROLL_TOP|wxEVT_SCROLL_BOTTOM|wxEVT_SCROLL_LINEUP|wxEVT_SCROLL_LINEDOWN|wxEVT_SCROLL_PAGEUP|wxEVT_SCROLL_PAGEDOWN|wxEVT_SCROLL_THUMBTRACK|wxEVT_SCROLL_THUMBRELEASE|wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&RoboVisionXFrame::OnMovementVerticalCmdScroll);
    Connect(ID_SLIDER2,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&RoboVisionXFrame::OnMovementVerticalCmdScroll);
    Connect(ID_SLIDER2,wxEVT_SCROLL_THUMBRELEASE,(wxObjectEventFunction)&RoboVisionXFrame::OnMovementVerticalCmdScroll);
    Connect(ID_CHECKBOX4,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&RoboVisionXFrame::OnSaveSnapshotsClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&RoboVisionXFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&RoboVisionXFrame::OnAbout);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&RoboVisionXFrame::OnTimer1Trigger);
    //*)


   Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&RoboVisionXFrame::OnOkClick);

    Timer1.Stop();

    // fps has the value of the Frames Drawn in the screen per second
    // obviously for remote desktop and on the guarddog limited CPU we may want it to be very low
    // 1 frame per second means 1000

    wxString WindowTitle; WindowTitle.Clear();
    WindowTitle << wxT("RoboVision X ");
    WindowTitle << _U(FULLVERSION_STRING);
    WindowTitle << wxT(" / VisualCortex ");
    WindowTitle << _U(VisCortx_Version());
    WindowTitle << _U(version_code_name);
    SetTitle(WindowTitle);

    uptimer = new wxStopWatch();

    InitFeeds();
    Status->AppendText(wxT("RoboVision GUI ok..\n"));
    Battery->SetValue(100);
    RedrawWindow();
    GUI_Shutdown=0;
    add_new_track_point=0;
    tick_count=0; // INTERNAL TIMER
    last_draw=0;
    mouse_x=0,mouse_y=0;
    dpth_x=0,dpth_y=0;
    dpth_on=0;

    DrawFeeds->SetValue(false); fprintf(stderr,"Switching off Video Drawing\n");
    Timer1.Start(WAIT_TIME_REDRAW, false);  //No fps limit


    feed_0_x=10;
    feed_0_y=15;

    feed_1_x=feed_0_x+default_feed->GetWidth()+10;
    feed_1_y=feed_0_y;

    feed_2_x=feed_0_x;
    feed_2_y=feed_0_y+default_feed->GetHeight()+10;

    feed_3_x=feed_1_x;
    feed_3_y=feed_2_y;

  }



void RoboVisionXFrame::RedrawWindow()
{
    Refresh();
}


RoboVisionXFrame::~RoboVisionXFrame()
{
    //(*Destroy(RoboVisionXFrame)
    //*)
    fprintf(stderr,"Closing all subsystems\n");
    CloseFeeds();
    fprintf(stderr,"Waiting for things to end..\n");


    unsigned int wait_time = 0; // ms
    unsigned int time_increment = 50; // ms
    unsigned int MAX_WAIT_TIME_FOR_CLOSE = 5000; // ms
    while ( ( !EverythingClosed() )&& (wait_time<MAX_WAIT_TIME_FOR_CLOSE) )
      {
          wxMilliSleep(time_increment);
          wait_time += time_increment;
      }
    if (wait_time<MAX_WAIT_TIME_FOR_CLOSE)  { fprintf(stderr,"RoboKernel Reported that everything exited ok\n"); } else
                                            { fprintf(stderr,"Timed out while waiting for Robokernel to close everything down, exiting..\n"); }
    fprintf(stderr,"Program is halting now...\n");
    wxMilliSleep(1000);

}

void RoboVisionXFrame::OnQuit(wxCommandEvent& event)
{
    GUI_Shutdown=1;
    WAIT_TIME_STARTUP = uptimer->Time() + 10000;
    DrawFeeds->SetValue(false);
    fprintf(stderr,"Closing window , on quit called \n");
    wxMilliSleep(500);
    Close();
}

void RoboVisionXFrame::OnTimerEvent(wxTimerEvent &event)
{

}



void RoboVisionXFrame::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this); // OnPaint events should always create a wxPaintDC

    if ( !RoboKernelAlive() ) { fprintf(stderr,"Robo Kernel died , window paint message dismissed\n"); return; }
    //if ( VideoFeedsNotAccessible== 1 ) { return; }

     if ( uptimer->Time() < WAIT_TIME_STARTUP ) { /*Wait for warmup :P */ return ; }
     if ( !DrawFeeds->IsChecked() ) { return ; }
     if (GUI_Shutdown) { return ; }

     dc.DrawBitmap(*live_feeds[0].bmp,feed_0_x,feed_0_y,0); //FEED 1
     dc.DrawBitmap(*live_feeds[1].bmp,feed_1_x,feed_1_y,0); //FEED 2
     dc.DrawBitmap(*live_feeds[2].bmp,feed_2_x,feed_2_y,0); //FEED 3
     dc.DrawBitmap(*live_feeds[3].bmp,feed_3_x,feed_3_y,0); //FEED 4


     if ( dpth_on == 1 )
     {
       unsigned int x1,y1,sizex,sizey;
       x1 = DepthMap ( 4 , dpth_x , dpth_y );    y1 = DepthMap ( 5 , dpth_x , dpth_y );
       sizex = DepthMap ( 8 , dpth_x , dpth_y ); sizey = DepthMap ( 9 , dpth_x , dpth_y );

       wxPen red(wxColour(255,0,0),1,wxSOLID);
       dc.SetPen(red);
       dc.SetBrush(*wxTRANSPARENT_BRUSH);
       dc.DrawRectangle(feed_0_x+x1,feed_0_y+y1,sizex,sizey);
       dc.DrawRectangle(mouse_x,mouse_y-250,3,3);
       dc.DrawRectangle(mouse_x,mouse_y,3,3);


       x1 = DepthMap ( 6 , dpth_x , dpth_y ); y1 = DepthMap ( 7 , dpth_x , dpth_y );
       dc.DrawRectangle(feed_1_x+x1,feed_1_y+y1,sizex,sizey);

       wxString msg; msg.clear();
       msg<<wxT("Left-Right-Mouse X/Y : ");

       msg<<DepthMap ( 4 , dpth_x , dpth_y ); msg<<wxT("/");
       msg<<DepthMap ( 5 , dpth_x , dpth_y ); msg<<wxT(" - ");

       msg<<DepthMap ( 6 , dpth_x , dpth_y ); msg<<wxT("/");
       msg<<DepthMap ( 7 , dpth_x , dpth_y ); msg<<wxT(" - ");

       msg<<dpth_x; msg<<wxT("/"); msg<<dpth_y;

       msg<<wxT("\nDepth : ");
       msg<<DepthMap ( 1 , dpth_x , dpth_y );
       msg<<wxT(" ( ");
       msg<<DepthMap ( 18 , dpth_x , dpth_y );
       msg<<wxT(" ) ");
       msg<<wxT("\nScore : ");
       msg<<DepthMap ( 2 , dpth_x , dpth_y );
       msg<<wxT(" Choices Rejected : ");
       msg<<DepthMap ( 15 , dpth_x , dpth_y );
       msg<<wxT("\nMovement Count : ");
       msg<<DepthMap ( 13 , dpth_x , dpth_y );
       msg<<wxT("( diff = ");
       msg<<DepthMap ( 14 , dpth_x , dpth_y );
       msg<<wxT(")");
       msg<<wxT("\nEdge Count : ");
       msg<<DepthMap ( 3 , dpth_x , dpth_y );
       msg<<wxT("/");
       msg<<sizex*sizey;
       msg<<wxT("\nSignature : ");
       struct PatchSignature sig;
       VisCortx_GetPatchDescriptor(LEFT_EYE,x1,y1,sizex,sizey,&sig);
       msg<<sig.segment[0]; msg<<wxT(" , ");
       msg<<sig.segment[1]; msg<<wxT(" , ");
       msg<<sig.segment[2]; msg<<wxT(" , ");
       msg<<sig.segment[3]; msg<<wxT("\n");
       msg<<sig.segment[4]; msg<<wxT(" , ");
       msg<<sig.segment[5]; msg<<wxT(" ");


       dc.SetTextForeground(wxColour(255,0,0));
       dc.DrawText(msg,12,17);

     }





        wxPen pink_marker(wxColour(255,20,147),1,wxSOLID);
        wxPen marker(wxColour(255,255,0),1,wxSOLID);
        wxPen red_marker(wxColour(255,0,0),1,wxSOLID);
        wxPen green_marker(wxColour(255,0,0),1,wxSOLID);
        dc.SetPen(marker);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);


          // THE CODE IS SEGMENTED IN A WAY TO MINIMIE DC.SETPEN CALLS , THEY WINDOW SYSTEM SEEMS TO HANG
          // FOR MORE THAN 100 FEATURES * 4 * 2 :P

       if ( FeaturesAquired(CALIBRATED_LEFT_EYE) )
       {
         dc.SetPen(marker);
        DrawFeaturesAtFeed( dc , feed_0_x , feed_0_y , CALIBRATED_LEFT_EYE , 1 ,0 ); // DRAW TRACK POINTS ON LEFT EYE

         dc.SetPen(red_marker);
        DrawFeaturesAtFeed( dc , feed_0_x , feed_0_y , CALIBRATED_LEFT_EYE , 0 ,1 ); // DRAW TRACK LINES ON LEFT EYE
       }

       if ( FeaturesAquired(CALIBRATED_RIGHT_EYE) )
       {
         dc.SetPen(marker);
        DrawFeaturesAtFeed( dc , feed_1_x , feed_1_y , CALIBRATED_RIGHT_EYE , 1 ,0 ); // DRAW TRACK POINTS ON RIGHT EYE

         dc.SetPen(red_marker);
        DrawFeaturesAtFeed( dc , feed_1_x , feed_1_y , CALIBRATED_RIGHT_EYE , 0 ,1 ); // DRAW TRACK LINES ON RIGHT EYE
       }


       //------------------------------------ FACES ---------------------------------------------*/
        /*We want to draw faces on left eye ---------------------------------------------------------------------------------*/

       dc.SetPen(pink_marker);
        DrawFacesAtFeed( dc , feed_0_x , feed_0_y , CALIBRATED_LEFT_EYE ); // DRAW FACES ON LEFT EYE
        DrawFacesAtFeed( dc , feed_1_x , feed_1_y , CALIBRATED_RIGHT_EYE ); // DRAW FACES ON RIGHT EYE


/*
       if ( Autonomous->IsChecked() )
          {
             if ( (VisCortx_GetFaces(CALIBRATED_LEFT_EYE,0,TOTAL_POINTS)>0) || (VisCortx_GetFaces(CALIBRATED_RIGHT_EYE,0,TOTAL_POINTS)>0) )
             {
               IssueCommand((char *) "PLAYSOUND(alarm)",0,0,(char *)"GUI");
             }
          }
*/
      dc.SetPen(marker);




   if ( uptimer->Time() - last_draw < 4500 ) { return; }

    unsigned int ultrasonic_value;
    ultrasonic_value = RobotGetUltrasonic(1);
    if ( ultrasonic_value <255 ) { Ultrasonic1->SetValue(ultrasonic_value); }
    ultrasonic_value = RobotGetUltrasonic(0);
    if ( ultrasonic_value <255 ) { Ultrasonic2->SetValue(ultrasonic_value); }

     wxString msg;
     msg.Printf( wxT("%u | %u ") , VisCortx_GetMetric(CHANGES_LEFT) , VisCortx_GetMetric(CHANGES_RIGHT) );
     Flow->SetLabel(msg);


     unsigned int video_frame_rate = GetCortexMetric(VIDEOINPUT_PROCESSING_DELAY_MICROSECONDS) / 1000 ;
     if ( video_frame_rate != 0 ) { video_frame_rate = 1000 / video_frame_rate; }

     //TODO REMOVE frame_rate  , it does not reflect real performance , it is replaced with a new metric coming through the VisualCortex

     msg.Clear() , msg.Printf( wxT("%u fps - %u ms ") , video_frame_rate , (unsigned int) (GetCortexMetric(VIDEOINPUT_PROCESSING_DELAY_MICROSECONDS)/1000) );
     FrameRate->SetLabel(msg);

     msg.Clear() , msg.Printf( wxT("%u ms") , uptimer->Time() );
     Uptime->SetLabel(msg);

     float x=0.0,y=0.0,z=0.0,heading=0.0,pitch=0.0,yaw=0.0;
     GetAgent(0,&x,&y,&z,&heading,&pitch,&yaw);
     msg.Clear()  , msg.Printf( wxT("X:%0.2f Y:%0.2f Z:%0.2f heading:%0.2f") , x,y,z,heading );

     PoseString->SetLabel(msg);

     last_draw =  uptimer->Time();
}

void RoboVisionXFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to Robovision X , written by Ammar Qammaz"));
}

void RoboVisionXFrame::OnTimer1Trigger(wxTimerEvent& event)
{
   if ( GUI_Shutdown == 1 ) { return ; }
   if ( !RoboKernelAlive() ) { fprintf(stderr,"Robo Kernel died\n"); return; }
   // DEN XREIAZETAI GIATI XEIRIZETAI TO SNAPWEBCAMS TO EVENT! -> TO AFINW SAN REMINDR OTI DEN XREIAZETAI if ( VideoFeedsNotAccessible== 1 ) { return; }
   if ( uptimer->Time() < WAIT_TIME_STARTUP ) { /*Wait for warmup :P */ return ; }


   if ( SnapWebCams() == 1 ) {/* Tracking is now handled by the VisCortexInternally */ } else
                             { return; }



    ++tick_count;

  if ( uptimer->Time() < WAIT_TIME_STARTUP ) { return ; }

  if ( !DrawFeeds->IsChecked() ) { return ; }

  Refresh(); // <- This draws the window!
 // RedrawWindow();
}


void RoboVisionXFrame::OnButtonDepthMapClick(wxCommandEvent& event)
{
    if ( GUI_Shutdown == 1 ) { return ; }
    wxStopWatch sw;
    unsigned char write_to_file=1;
    if (lowcpuusage->IsChecked()) { write_to_file = 0; }
    GUI_FullDepthMap(write_to_file);
    sw.Pause();


    unsigned int dpthmap_ms = VisCortx_GetMetric(DEPTHMAP_DELAY_MICROSECONDS)/1000 ;
    unsigned int dpthmap_total = VisCortx_GetMetric(TOTAL_DEPTHMAPS_PERFORMED) ;
    unsigned int avg_ms = VisCortx_GetMetric(TOTAL_DEPTHMAP_DELAY_MICROSECONDS)/1000 ;
    if (dpthmap_total != 0 ) { avg_ms = avg_ms / dpthmap_total; }

    wxString msg;
    msg.Printf( wxT("DepthMap(%u ms,avrg. %u ms,gui %u ms)\n") ,   dpthmap_ms , avg_ms, sw.Time() );
    Status->AppendText( msg );
    Refresh();

    dpth_on=1;
}

inline int XYOverRect(int x , int y , int rectx1,int recty1,int rectx2,int recty2)
{
  if ( (x>=rectx1) && (x<=rectx2) )
    {
      if ( (y>=recty1) && (y<=recty2) )
        {
          return 1;
        }
    }
  return 0;
}


void RoboVisionXFrame::DrawPatchComp(unsigned int basedon,unsigned int x,unsigned int y)
{
    dpth_x = x;
    dpth_y = y;
    dpth_on = 1;
}




void RoboVisionXFrame::OnMotion(wxMouseEvent& event)
{
  if ( GUI_Shutdown == 1 ) { return ; }
  if ( !RoboKernelAlive() ) { fprintf(stderr,"Robo Kernel died\n"); return; }
  if ( uptimer->Time() < WAIT_TIME_STARTUP ) { /*Wait for warmup :P */ return ; }

  wxSleep(0.01);
  int x=event.GetX();
  int y=event.GetY();

  dpth_on = 0;

      int fd_rx1,fd_rx2,fd_ry1,fd_ry2;
      fd_rx1=10;
      fd_ry1=15+default_feed->GetHeight()+10;
      fd_rx2=fd_rx1 + default_feed->GetWidth();
      fd_ry2=fd_ry1 + default_feed->GetHeight();

      if ( XYOverRect(x,y,fd_rx1,fd_ry1,fd_rx2,fd_ry2)==1 )
       {
         DrawPatchComp(0,x-fd_rx1,y-fd_ry1);
         mouse_x=x;
         mouse_y=y;
       }




   if ( event.LeftIsDown()==1 )
   {
      if ( XYOverRect(x,y,fd_rx1,fd_ry1,fd_rx2,fd_ry2)==1 )
       {
           fprintf(stderr,"Opening Remember Image segment dialog\n");
             RememberImage* RIFrame = new RememberImage(0);
             RIFrame->camera = LEFT_EYE;
             RIFrame->patch_x =  DepthMap ( 4 , dpth_x , dpth_y );
             RIFrame->patch_y =  DepthMap ( 5 , dpth_x , dpth_y );
             RIFrame->patch_width = DepthMap ( 8 , dpth_x , dpth_y );
             RIFrame->patch_height = DepthMap ( 9 , dpth_x , dpth_y );
             VisCortx_GetPatchDescriptor(RIFrame->camera,RIFrame->patch_x,RIFrame->patch_y,RIFrame->patch_width,RIFrame->patch_height,&RIFrame->sig);
             RIFrame->PrepareMenu();
             RIFrame->ready = 1;
    	     RIFrame->ShowModal();
    	    delete RIFrame;
       }



     int fd_rx1,fd_rx2,fd_ry1,fd_ry2;
     fd_rx1=10 , fd_rx2=fd_rx1 + default_feed->GetWidth();
     fd_ry1=15 , fd_ry2=fd_ry1 + default_feed->GetHeight();
    if (add_new_track_point==1)
    {
     if ( XYOverRect(x,y,fd_rx1,fd_ry1,fd_rx2,fd_ry2)==1 )
       {

            wxString msg;
            msg.Printf( wxT("Adding Track Point ( %u , %u )\n") ,x-fd_rx1,y-fd_ry1 );
            Status->AppendText( msg );
            Refresh();
            VisCortx_AddTrackPoint(0,x-fd_rx1,y-fd_ry1,1);
            add_new_track_point=0;
      }
    }


    }
}

void RoboVisionXFrame::OnMotionAlarmButtonClick(wxCommandEvent& event)
{
    IssueCommand((char *) "MOTION ALARM",0,0,(char *)"GUI");
}

void RoboVisionXFrame::OnSwapFeedsClick(wxCommandEvent& event)
{

    IssueCommand((char *) "SWAP FEEDS",0,0,(char *)"GUI");
}

void RoboVisionXFrame::OnRecordButtonClick(wxCommandEvent& event)
{
    if (SaveLoadStreamOfSnapshots->IsChecked())
    {
      IssueCommand((char *) "TOGGLE AUTO RECORD SNAPSHOTS(500)",0,0,(char *)"GUI"); fprintf(stderr,"Toggle switch assumes 500ms stream record\n");
      IssueCommand((char *) "RECORD SNAPSHOT(1)",0,0,(char *)"GUI"); fprintf(stderr,"Recording filenames intended with timestap\n");
    } else
    if (!SaveLoadStreamOfSnapshots->IsChecked())
    {
      IssueCommand((char *) "TOGGLE AUTO RECORD SNAPSHOTS(0)",0,0,(char *)"GUI");
      IssueCommand((char *) "RECORD SNAPSHOT",0,0,(char *)"GUI");
    }
}

void RoboVisionXFrame::OnPlayButtonClick(wxCommandEvent& event)
{
    if (SaveLoadStreamOfSnapshots->IsChecked())
    {
      IssueCommand((char *) "TOGGLE AUTO RECORD SNAPSHOTS(0)",0,0,(char *)"GUI");
      IssueCommand((char *) "PLAYBACK OF SNAPSHOT STREAM NOT IMPLEMENTED!",0,0,(char *)"GUI");
    } else
    if (!SaveLoadStreamOfSnapshots->IsChecked())
    {
      IssueCommand((char *) "PLAYBACK SNAPSHOT",0,0,(char *)"GUI");
    }
}

void RoboVisionXFrame::OnLiveButtonClick(wxCommandEvent& event)
{
    IssueCommand((char *) "PLAYBACK LIVE",0,0,(char *)"GUI");
}

void RoboVisionXFrame::OnAddTrackPointClick(wxCommandEvent& event)
{
    add_new_track_point=1;
}

void RoboVisionXFrame::OnConfigurationButtonClick(wxCommandEvent& event)
{
  CortexSettings *cortset=new CortexSettings(this,0,wxDefaultPosition,wxSize(500,300));
  cortset->ShowModal();
  delete cortset;

}

void RoboVisionXFrame::PrintStats()
{
  wxString msg;
  msg.Clear();
  msg<<wxT("Disparity Map :\n");
  msg<<wxT("Patch Denies Histogram/PatchCompare ");
  msg<<VisCortx_GetMetric(HISTOGRAM_DENIES);
  msg<<wxT("/");
  msg<<VisCortx_GetMetric(COMPAREPATCH_ALGORITHM_DENIES);
  msg<<wxT("\n");
  Status->AppendText( msg );
  Refresh();
}

void RoboVisionXFrame::OnOkClick(wxCommandEvent& event)
{
    //CommandBox
    wxString wxscommand=CommandBox->GetValue();
    char cstring[1024]={0};
    strncpy(cstring, (const char*)wxscommand.mb_str(wxConvUTF8), 1023);

    if ( IssueCommand(cstring,0,0,(char *)"GUI")==0 )
    {
        wxString msg;
        msg<<wxT("Unknown Command\n");
        IssueCommand((char *)"Say(Command not recognized.)",0,0,(char *)"GUI");
        Status->AppendText( msg );
        Refresh();
    } else
    {
      /* WE HOPE(!) that the new command has Video output on the bottom 2 feeds */
      GUI_DrawOPFeeds();
    }
}



void RoboVisionXFrame::OnSaveSnapshotsClick(wxCommandEvent& event)
{
    /*
   if (SaveLoadStreamOfSnapshots->IsChecked())
    {
       IssueCommand((char *) "TOGGLE AUTO RECORD SNAPSHOTS(500)",0,0,(char *)"GUI");
    } else
   if (!SaveLoadStreamOfSnapshots->IsChecked())
    {
       IssueCommand((char *) "TOGGLE AUTO RECORD SNAPSHOTS(0)",0,0,(char *)"GUI");
    }
*/
}

void RoboVisionXFrame::OnAutonomousClick(wxCommandEvent& event)
{
//    IssueCommand((char *) "AUTONOMOUS MODE",0,0,(char *)"GUI");
   if ( Autonomous->IsChecked() ) { IssueCommand((char *) "AUTONOMOUS MODE(1)",0,0,(char *)"GUI");  } else
                                  { IssueCommand((char *) "AUTONOMOUS MODE(0)",0,0,(char *)"GUI");  }
}




void RoboVisionXFrame::OnUpButtonClick(wxCommandEvent& event)
{
  RobotMove(20,5000);
}

void RoboVisionXFrame::OnDownButtonClick(wxCommandEvent& event)
{
  RobotMove(20,-5000);
}

void RoboVisionXFrame::OnLeftButtonClick(wxCommandEvent& event)
{
  RobotRotate(20,10);
}

void RoboVisionXFrame::OnRightButtonClick(wxCommandEvent& event)
{
 RobotRotate(20,(-1)*10);
}

void RoboVisionXFrame::OnStopButtonClick(wxCommandEvent& event)
{
  RobotStopMovement();
  MovementVertical->SetValue(10);
  MovementHorizontal->SetValue(10);
}

void RoboVisionXFrame::OnMovementVerticalCmdScroll(wxScrollEvent& event)
{
    signed int startpos=MovementVertical->GetValue();
    signed int pos;
    if ( startpos <8 )
      {
         pos=8-startpos;
         fprintf(stderr,"Vertical UP %d ",pos);
         RobotStartMoving(10*pos,(signed int) 1);
      } else
    if ( startpos >12 )
      {
         pos=startpos-12;
         fprintf(stderr,"Vertical DOWN %d ",pos);
         RobotStartMoving(10*pos,(signed int) -1);
      } else
      {
         RobotStopMovement();
         MovementVertical->SetValue(10);
      }
}

void RoboVisionXFrame::OnMovementHorizontalCmdScroll(wxScrollEvent& event)
{
    signed int startpos=MovementHorizontal->GetValue();
    signed int pos=0;
    if ( startpos <8 )
      {
         pos=8-startpos;
         fprintf(stderr,"Horizontal LEFT %d ",pos);
         RobotStartRotating(10*pos,1);
      } else
    if ( startpos >12 )
      {
         pos=startpos-12;
         fprintf(stderr,"Horizontal RIGHT %d ",pos);
         RobotStartRotating(10*pos,-1);
      } else
      {
         RobotStopMovement();
         MovementHorizontal->SetValue(10);
      }
}

void RoboVisionXFrame::OnAutonomousClick1(wxCommandEvent& event)
{
   if ( Autonomous->IsChecked() ) { IssueCommand((char *) "AUTONOMOUS MODE(1)",0,0,(char *)"GUI");  } else
                                  { IssueCommand((char *) "AUTONOMOUS MODE(0)",0,0,(char *)"GUI");  }
}
