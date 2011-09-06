/***************************************************************
 * Name:      RoboVisionXMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2009-11-01
 * Copyright: Ammar Qammaz (http://62.103.22.50)
 * License:
 **************************************************************/

char * ROBOGUI_VERSION=(char *) "0.39";

int position = 0;

#include "RoboVisionXMain.h"
#include "FeedScreenMemory.h"
#include "CortexSettings.h"
#include "RememberImage.h"

#include "../MotorFoundation/MotorHAL.h"
#include "../RoboKernel/RoboKernel.h"

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
    StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, _("Motor Status"), wxPoint(680,40), wxSize(232,192), 0, _T("ID_STATICBOX1"));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Dist. Traveled :"), wxPoint(688,56), wxSize(128,17), 0, _T("ID_STATICTEXT1"));
    ButtonDepthMap = new wxButton(this, ID_BUTTON1, _("Depth Map"), wxPoint(680,392), wxSize(120,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Left Cam"), wxPoint(15,0), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Right Cam"), wxPoint(340,0), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    MotionAlarmButton = new wxButton(this, ID_BUTTON2, _("Motion Alarm"), wxPoint(808,392), wxSize(104,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    StaticBox2 = new wxStaticBox(this, ID_STATICBOX2, _("General"), wxPoint(680,160), wxSize(232,152), 0, _T("ID_STATICBOX2"));
    UptimeLabel = new wxStaticText(this, ID_STATICTEXT4, _("Uptime :"), wxPoint(688,184), wxSize(72,17), 0, _T("ID_STATICTEXT4"));
    Uptime = new wxStaticText(this, ID_STATICTEXT5, _("0"), wxPoint(760,184), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    StaticText4 = new wxStaticText(this, ID_STATICTEXT6, _("Flow : "), wxPoint(688,200), wxSize(48,17), 0, _T("ID_STATICTEXT6"));
    Flow = new wxStaticText(this, ID_STATICTEXT7, _("0"), wxPoint(736,200), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    Status = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxPoint(680,248), wxSize(232,88), wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    StaticText5 = new wxStaticText(this, ID_STATICTEXT8, _("Battery"), wxPoint(680,8), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    Battery = new wxGauge(this, ID_GAUGE1, 100, wxPoint(736,8), wxSize(176,16), 0, wxDefaultValidator, _T("ID_GAUGE1"));
    StaticText6 = new wxStaticText(this, ID_STATICTEXT9, _("FrameRate :"), wxPoint(688,216), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    FrameRate = new wxStaticText(this, ID_STATICTEXT10, _("0"), wxPoint(776,216), wxDefaultSize, 0, _T("ID_STATICTEXT10"));
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
    Ultrasonic1 = new wxGauge(this, ID_GAUGE2, 255, wxPoint(696,120), wxSize(100,16), 0, wxDefaultValidator, _T("ID_GAUGE2"));
    Ultrasonic2 = new wxGauge(this, ID_GAUGE3, 255, wxPoint(696,136), wxSize(100,16), 0, wxDefaultValidator, _T("ID_GAUGE3"));
    StaticText7 = new wxStaticText(this, ID_STATICTEXT11, _("Ultrasonic"), wxPoint(688,104), wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    DistanceTraveled = new wxStaticText(this, ID_STATICTEXT12, _("0"), wxPoint(704,78), wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    wxFont DistanceTraveledFont(13,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    DistanceTraveled->SetFont(DistanceTraveledFont);
    MeterComment = new wxStaticText(this, ID_STATICTEXT13, _("Meters"), wxPoint(760,80), wxDefaultSize, 0, _T("ID_STATICTEXT13"));
    LeftUltrasonicLabel = new wxStaticText(this, ID_STATICTEXT14, _("L"), wxPoint(684,120), wxDefaultSize, 0, _T("ID_STATICTEXT14"));
    RightUltrasonicLabel = new wxStaticText(this, ID_STATICTEXT15, _("R"), wxPoint(684,136), wxDefaultSize, 0, _T("ID_STATICTEXT15"));
    SaveLoadStreamOfSnapshots = new wxCheckBox(this, ID_CHECKBOX4, _("Rec/Play Stream of Snapshots"), wxPoint(680,464), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
    SaveLoadStreamOfSnapshots->SetValue(false);
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
    WindowTitle << _U(ROBOGUI_VERSION);
    WindowTitle << wxT(" / VideoInput ");
    WindowTitle << _U(VideoInput_Version());
    WindowTitle << wxT(" / VisualCortex ");
    WindowTitle << _U(VisCortx_Version());
    SetTitle(WindowTitle);

    siren = new wxSound(wxT("Sounds/alarm.wav"),false);

    uptimer = new wxStopWatch();

    InitFeeds();
    Status->AppendText(wxT("RoboVision GUI ok..\n"));
    Battery->SetValue(100);
    RedrawWindow();
    shutdown=0;
    add_new_track_point=0;
    tick_count=0; // INTERNAL TIMER

    DrawFeeds->SetValue(false); fprintf(stderr,"Switching off Video Drawing\n");
    Timer1.Start(120, false);  //No fps limit


    // CONFIGURATION HAS BEEN READ SO , APPLY IT!
/*    if ( fps == 0 ) Timer1.Start(120, false); else //No fps limit
    if ( fps == 1 ) Timer1.Start(3000, false); else // 1 = Very low fps 0.3 fps
                    Timer1.Start(1000/fps, false);

    if ( draw_on == 0 ) { DrawFeeds->SetValue(false); fprintf(stderr,"Switching off Video Drawing\n"); } else
                        { DrawFeeds->SetValue(true);  fprintf(stderr,"Switching on Video Drawing\n"); }
*/
    feed_0_x=10;
    feed_0_y=15;

    feed_1_x=feed_0_x+default_feed->GetWidth()+10;
    feed_1_y=15;

    feed_2_x=feed_0_x;
    feed_2_y=feed_0_y+default_feed->GetHeight()+10;

    feed_3_x=feed_1_x;
    feed_3_y=feed_2_y;

    IssueCommand((char *)"Say(Robo Vision X started!)",0,0,(char *)"GUI");
}



void RoboVisionXFrame::RedrawWindow()
{
    Refresh();
}


RoboVisionXFrame::~RoboVisionXFrame()
{
    //(*Destroy(RoboVisionXFrame)
    //*)
    CloseFeeds();
}

void RoboVisionXFrame::OnQuit(wxCommandEvent& event)
{
    shutdown=1;
    Close();
}

void RoboVisionXFrame::OnTimerEvent(wxTimerEvent &event)
{

}



void RoboVisionXFrame::OnPaint(wxPaintEvent& event)
{
 //   if ( has_init == 0 ) { return; }
 //   if ( shutdown == 1 ) { return ; }
    if ( !RoboKernelAlive() ) { fprintf(stderr,"Robo Kernel died\n"); return; }
 //   if ( VideoFeedsNotAccessible== 1 ) { return; }

    if ( uptimer->Time() < 3000 ) { return ; }


     if ( !DrawFeeds->IsChecked() ) { return ; }
    wxPaintDC dc(this);

    // int x=10;
     //int y=15;
     //FEED 1
     dc.DrawBitmap(*live_feeds[0].bmp,feed_0_x,feed_0_y,true);


     //x+=default_feed->GetWidth()+10;
     //FEED 2
     dc.DrawBitmap(*live_feeds[1].bmp,feed_1_x,feed_1_y,true);


     //x=10;
     //y+=default_feed->GetHeight()+10;
     //FEED 3
     dc.DrawBitmap(*live_feeds[2].bmp,feed_2_x,feed_2_y,true);


     //x+=default_feed->GetWidth()+10;
     //FEED 4
     dc.DrawBitmap(*live_feeds[3].bmp,feed_3_x,feed_3_y,true);

     if ( dpth_on == 1 )
     {
       unsigned int x1,y1,sizex,sizey;
       x1 = DepthMap ( 4 , dpth_x , dpth_y );
       y1 = DepthMap ( 5 , dpth_x , dpth_y );
       sizex = DepthMap ( 8 , dpth_x , dpth_y );
       sizey = DepthMap ( 9 , dpth_x , dpth_y );


       wxPen red(wxColour(255,0,0),1,wxSOLID);
       dc.SetPen(red);
       dc.SetBrush(*wxTRANSPARENT_BRUSH);
       dc.DrawRectangle(feed_0_x+x1,feed_0_y+y1,sizex,sizey);
       dc.DrawRectangle(mouse_x,mouse_y-250,3,3);
       dc.DrawRectangle(mouse_x,mouse_y,3,3);


       x1 = DepthMap ( 6 , dpth_x , dpth_y );
       y1 = DepthMap ( 7 , dpth_x , dpth_y );
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
       msg<<wxT("\nScore : ");
       msg<<DepthMap ( 2 , dpth_x , dpth_y );
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


     if (  (VisCortx_GetFeature(CALIBRATED_LEFT_EYE,0,TOTAL_POINTS)>0) || (VisCortx_GetFeature(CALIBRATED_RIGHT_EYE,0,TOTAL_POINTS)>0) )
     {
        unsigned int i=0;
        wxPen pink_marker(wxColour(255,20,147),1,wxSOLID);
        wxPen marker(wxColour(255,255,0),1,wxSOLID);
        wxPen red_marker(wxColour(255,0,0),1,wxSOLID);
        wxPen green_marker(wxColour(255,0,0),1,wxSOLID);
        dc.SetPen(marker);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);

        //------------------------------------ FEATURES ---------------------------------------------*/
        /*We want to draw features on left eye ---------------------------------------------------------------------------------*/
       if (VisCortx_GetFeature(CALIBRATED_LEFT_EYE,0,TOTAL_POINTS)>0  )
       {
        for ( i=0; i<VisCortx_GetFeature(CALIBRATED_LEFT_EYE,0,TOTAL_POINTS); i++ )
         {
            if ( VisCortx_GetFeature(CALIBRATED_LEFT_EYE,i,FEATURE_IS_LOST) == 0 )
            {
               dc.SetPen(marker);
             dc.DrawRectangle(feed_0_x+VisCortx_GetFeature(CALIBRATED_LEFT_EYE,i,FEATURE_X),feed_0_y+VisCortx_GetFeature(CALIBRATED_LEFT_EYE,i,FEATURE_Y),3,3);
              dc.SetPen(red_marker);
             dc.DrawLine(feed_0_x+VisCortx_GetFeature(CALIBRATED_LEFT_EYE,i,FEATURE_X),feed_0_y+VisCortx_GetFeature(CALIBRATED_LEFT_EYE,i,FEATURE_Y),
                         feed_0_x+VisCortx_GetFeature(CALIBRATED_LEFT_EYE,i,FEATURE_LAST_X),feed_0_y+VisCortx_GetFeature(CALIBRATED_LEFT_EYE,i,FEATURE_LAST_Y));
            }
         }
       }

       /*We want to draw features on right eye ---------------------------------------------------------------------------------*/
       if (VisCortx_GetFeature(CALIBRATED_RIGHT_EYE,0,TOTAL_POINTS)>0  )
       {
        for ( i=0; i<VisCortx_GetFeature(CALIBRATED_RIGHT_EYE,0,TOTAL_POINTS); i++ )
         {
           if ( VisCortx_GetFeature(CALIBRATED_RIGHT_EYE,i,FEATURE_IS_LOST) == 0 )
            {
              dc.SetPen(marker);
             dc.DrawRectangle(feed_1_x+VisCortx_GetFeature(CALIBRATED_RIGHT_EYE,i,FEATURE_X),feed_1_y+VisCortx_GetFeature(CALIBRATED_RIGHT_EYE,i,FEATURE_Y),3,3);
              dc.SetPen(red_marker);
             dc.DrawLine(feed_1_x+VisCortx_GetFeature(CALIBRATED_RIGHT_EYE,i,FEATURE_X),feed_1_y+VisCortx_GetFeature(CALIBRATED_RIGHT_EYE,i,FEATURE_Y),
                         feed_1_x+VisCortx_GetFeature(CALIBRATED_RIGHT_EYE,i,FEATURE_LAST_X),feed_1_y+VisCortx_GetFeature(CALIBRATED_RIGHT_EYE,i,FEATURE_LAST_Y));
            }
         }
       }




       //------------------------------------ FACES ---------------------------------------------*/
        /*We want to draw faces on left eye ---------------------------------------------------------------------------------*/
       if (VisCortx_GetFaces(CALIBRATED_LEFT_EYE,0,TOTAL_POINTS)>0  )
       {
         for ( i=0; i<VisCortx_GetFaces(CALIBRATED_LEFT_EYE,0,TOTAL_POINTS); i++ )
         {
            if ( VisCortx_GetFaces(CALIBRATED_LEFT_EYE,i,FEATURE_IS_LOST) == 0 )
            {
               dc.SetPen(pink_marker);
               dc.DrawCircle(feed_0_x+VisCortx_GetFaces(CALIBRATED_LEFT_EYE,i,FEATURE_X),feed_0_y+VisCortx_GetFaces(CALIBRATED_LEFT_EYE,i,FEATURE_Y),VisCortx_GetFaces(CALIBRATED_LEFT_EYE,i,FEATURE_Z));
            }
         }
       }

        /*We want to draw faces on left eye ---------------------------------------------------------------------------------*/
       if (VisCortx_GetFaces(CALIBRATED_RIGHT_EYE,0,TOTAL_POINTS)>0  )
       {
         for ( i=0; i<VisCortx_GetFaces(CALIBRATED_RIGHT_EYE,0,TOTAL_POINTS); i++ )
         {
            if ( VisCortx_GetFaces(CALIBRATED_RIGHT_EYE,i,FEATURE_IS_LOST) == 0 )
            {
               dc.SetPen(pink_marker);
               dc.DrawCircle(feed_1_x+VisCortx_GetFaces(CALIBRATED_RIGHT_EYE,i,FEATURE_X),feed_1_y+VisCortx_GetFaces(CALIBRATED_RIGHT_EYE,i,FEATURE_Y),VisCortx_GetFaces(CALIBRATED_RIGHT_EYE,i,FEATURE_Z));
            }
         }
       }


      dc.SetPen(marker);
     }


     if ( uptimer->Time() - last_draw < 4500 ) { return; }

    unsigned int ultrasonic_value;
    ultrasonic_value = RobotGetUltrasonic(1);
    if ( ultrasonic_value <255 ) { Ultrasonic1->SetValue(ultrasonic_value); }
    ultrasonic_value = RobotGetUltrasonic(0);
    if ( ultrasonic_value <255 ) { Ultrasonic2->SetValue(ultrasonic_value); }

     wxString msg;
     msg.Printf( wxT("%u | %u ") , VisCortx_GetMetric(CHANGES_LEFT) , VisCortx_GetMetric(CHANGES_RIGHT) );
     Flow->SetLabel(msg);

     msg.Clear() , msg.Printf( wxT("%u fps - %u ms ") , frame_rate , (unsigned int) (GetCortexMetric(VIDEOINPUT_PROCESSING_DELAY_MICROSECONDS)/1000) );
     FrameRate->SetLabel(msg);

     msg.Clear() , msg.Printf( wxT("%u ms") , uptimer->Time() );
     Uptime->SetLabel(msg);



     last_draw =  uptimer->Time();
}

void RoboVisionXFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to Robovision X , written by Ammar Qammaz"));
}

void RoboVisionXFrame::OnTimer1Trigger(wxTimerEvent& event)
{

   if ( shutdown == 1 ) { return ; }
   if ( !RoboKernelAlive() ) { fprintf(stderr,"Robo Kernel died\n"); return; }
   // DEN XREIAZETAI GIATI XEIRIZETAI TO SNAPWEBCAMS TO EVENT! -> TO AFINW SAN REMINDR OTI DEN XREIAZETAI if ( VideoFeedsNotAccessible== 1 ) { return; }

   wxStopWatch sw1;
   if ( SnapWebCams() == 1 )
    {/* Tracking is now handled by the VisCortexInternally */ }
   sw1.Pause();

   if ( sw1.Time() == 0 ) { frame_rate = 1000; } else
                         { frame_rate = 1000 / sw1.Time(); }


    ++tick_count;
    if ( tick_count%2 == 0 )
    {
      if ( Autonomous->IsChecked() )
       {
        if ( ( VisCortx_GetMetric(CHANGES_LEFT) >2000 ) || ( VisCortx_GetMetric(CHANGES_RIGHT)>2000 ) )
        {

         wxStopWatch sw2;
          GUI_FullDepthMap(0);
         sw2.Pause();

         wxString msg;
         msg.Printf( wxT("Full DepthMap ( %u ms )\n") , sw2.Time() );
         Status->AppendText( msg );

         IssueCommand((char *) "DEPTH MAP",0,0,(char *)"GUI");

/*
         if (!lowcpuusage->IsChecked())
         {
          IssueCommand((char *) "PLAYSOUND(alarm)",0,0,(char *)"GUI");
          // DEMO MOVEMENT
          ++position;
          if ( position <= 2 ) { IssueCommand((char *) "LEFT",0,0,(char *)"GUI"); } else
          if ( position <= 4 ) { IssueCommand((char *) "RIGHT",0,0,(char *)"GUI"); } else
                               {position=0;}

         }
*/

        Refresh();
        }
       }
    }

  if ( uptimer->Time() < 5000 ) { return ; }

  if ( !DrawFeeds->IsChecked() ) { return ; }

  RedrawWindow();
}


void RoboVisionXFrame::OnButtonDepthMapClick(wxCommandEvent& event)
{
    wxStopWatch sw;
    unsigned char write_to_file=1;
    if (lowcpuusage->IsChecked()) { write_to_file = 0; }
    GUI_FullDepthMap(write_to_file);
    sw.Pause();

    wxString msg;
    msg.Printf( wxT("Full DepthMap ( %u ms )\n") , sw.Time() );
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
  //if ( shutdown == 1 ) { return ; }
  if ( !RoboKernelAlive() ) { fprintf(stderr,"Robo Kernel died\n"); return; }

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
    char cstring[1024];
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

/*
 *
 *      ROBOT MOTOR CONTROL PANEL
 *
 */


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
    signed int pos;
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
