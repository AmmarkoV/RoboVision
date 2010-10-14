/***************************************************************
 * Name:      WorldMappingMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2009-11-26
 * Copyright: Ammar Qammaz (http://62.103.22.50)
 * License:
 **************************************************************/

#include "WorldMappingMain.h"
#include <wx/msgdlg.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/colour.h>
#include <wx/utils.h>

#define OBSTACLE_UNCERTAINTY 1
#define OURROBOT 0

//(*InternalHeaders(WorldMappingFrame)
#include <wx/string.h>
#include <wx/intl.h>
//*)

//helper functions
enum wxbuildinfoformat
{
  short_f, long_f
};

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

//(*IdInit(WorldMappingFrame)
const long WorldMappingFrame::ID_STATICTEXT1 = wxNewId();
const long WorldMappingFrame::ID_TEXTCTRL1 = wxNewId();
const long WorldMappingFrame::ID_TEXTCTRL2 = wxNewId();
const long WorldMappingFrame::ID_STATICTEXT2 = wxNewId();
const long WorldMappingFrame::ID_TEXTCTRL3 = wxNewId();
const long WorldMappingFrame::ID_TEXTCTRL4 = wxNewId();
const long WorldMappingFrame::ID_BUTTON1 = wxNewId();
const long WorldMappingFrame::ID_STATICTEXT3 = wxNewId();
const long WorldMappingFrame::ID_TEXTCTRL5 = wxNewId();
const long WorldMappingFrame::ID_TEXTCTRL6 = wxNewId();
const long WorldMappingFrame::ID_STATICBOX1 = wxNewId();
const long WorldMappingFrame::ID_BUTTON4 = wxNewId();
const long WorldMappingFrame::ID_BUTTON5 = wxNewId();
const long WorldMappingFrame::ID_BUTTON6 = wxNewId();
const long WorldMappingFrame::ID_SPINCTRL1 = wxNewId();
const long WorldMappingFrame::ID_STATICTEXT4 = wxNewId();
const long WorldMappingFrame::ID_STATICTEXT5 = wxNewId();
const long WorldMappingFrame::ID_TEXTCTRL7 = wxNewId();
const long WorldMappingFrame::ID_STATICTEXT6 = wxNewId();
const long WorldMappingFrame::ID_BUTTON7 = wxNewId();
const long WorldMappingFrame::ID_BUTTON8 = wxNewId();
const long WorldMappingFrame::ID_BUTTON9 = wxNewId();
const long WorldMappingFrame::ID_SPINCTRL2 = wxNewId();
const long WorldMappingFrame::ID_BUTTON11 = wxNewId();
const long WorldMappingFrame::ID_BUTTON2 = wxNewId();
const long WorldMappingFrame::ID_BUTTON3 = wxNewId();
const long WorldMappingFrame::ID_STATICBOX2 = wxNewId();
const long WorldMappingFrame::ID_BUTTON10 = wxNewId();
const long WorldMappingFrame::ID_BUTTON12 = wxNewId();
const long WorldMappingFrame::ID_BUTTON13 = wxNewId();
const long WorldMappingFrame::ID_BUTTON14 = wxNewId();
const long WorldMappingFrame::ID_BUTTON15 = wxNewId();
const long WorldMappingFrame::idMenuQuit = wxNewId();
const long WorldMappingFrame::idMenuAbout = wxNewId();
const long WorldMappingFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(WorldMappingFrame,wxFrame)
  //(*EventTable(WorldMappingFrame)
  //*)
  EVT_PAINT(WorldMappingFrame::OnPaint)
  EVT_MOTION(WorldMappingFrame::OnMotion)
  EVT_JOYSTICK_EVENTS(WorldMappingFrame::OnJoystickEvent)

END_EVENT_TABLE()

inline wxString _U(const char String[] = "")
{
  return wxString(String, wxConvUTF8);
}

void TTS(char * what2say)
{
    wxString cmd; cmd.clear();
    cmd<<wxT("echo \"");
    cmd<<_U(what2say);
    cmd<<wxT("\"  | esddsp  festival --tts");

    wxShell(cmd);
}

WorldMappingFrame::WorldMappingFrame(wxWindow* parent,wxWindowID id)
{
  //(*Initialize(WorldMappingFrame)
  wxMenuItem* MenuItem2;
  wxMenuItem* MenuItem1;
  wxMenu* Menu1;
  wxMenuBar* MenuBar1;
  wxMenu* Menu2;

  Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
  SetClientSize(wxSize(1080,655));
  StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Start Point"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
  ptx1 = new wxTextCtrl(this, ID_TEXTCTRL1, _("0"), wxPoint(16,40), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
  pty1 = new wxTextCtrl(this, ID_TEXTCTRL2, _("0"), wxPoint(48,40), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
  StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("End Point"), wxPoint(16,72), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
  ptx2 = new wxTextCtrl(this, ID_TEXTCTRL3, _("0"), wxPoint(16,88), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
  pty2 = new wxTextCtrl(this, ID_TEXTCTRL4, _("0"), wxPoint(48,88), wxSize(32,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
  ButtonCalculate = new wxButton(this, ID_BUTTON1, _("Calculate"), wxPoint(16,208), wxSize(88,29), 0, wxDefaultValidator, _T("ID_BUTTON1"));
  StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Obstacle/Move"), wxPoint(16,304), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
  obsx = new wxTextCtrl(this, ID_TEXTCTRL5, _("0"), wxPoint(16,328), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
  obsy = new wxTextCtrl(this, ID_TEXTCTRL6, _("0"), wxPoint(64,328), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
  StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, _("World"), wxPoint(120,8), wxSize(800,536), 0, _T("ID_STATICBOX1"));
  SetEndPoint = new wxButton(this, ID_BUTTON4, _("S"), wxPoint(80,86), wxSize(24,29), 0, wxDefaultValidator, _T("ID_BUTTON4"));
  SetStartPoint = new wxButton(this, ID_BUTTON5, _("S"), wxPoint(80,38), wxSize(24,29), 0, wxDefaultValidator, _T("ID_BUTTON5"));
  ClearButton = new wxButton(this, ID_BUTTON6, _("Clear All"), wxPoint(16,504), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
  TimeMS = new wxSpinCtrl(this, ID_SPINCTRL1, _T("30"), wxPoint(16,176), wxSize(88,27), 0, 0, 200, 30, _T("ID_SPINCTRL1"));
  TimeMS->SetValue(_T("30"));
  StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Time Limit"), wxPoint(16,160), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
  StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("1 block equals"), wxPoint(8,544), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
  scale_cm = new wxTextCtrl(this, ID_TEXTCTRL7, _("15"), wxPoint(16,568), wxSize(40,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
  StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("cm"), wxPoint(64,576), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
  ButtonExecute = new wxButton(this, ID_BUTTON7, _("Execute"), wxPoint(16,240), wxSize(88,29), 0, wxDefaultValidator, _T("ID_BUTTON7"));
  PrintButton = new wxButton(this, ID_BUTTON8, _("Print"), wxPoint(16,272), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON8"));
  ButtonSimulateUltrasonic = new wxButton(this, ID_BUTTON9, _("Ultras. Sim"), wxPoint(16,360), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON9"));
  Rotation = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxPoint(16,122), wxSize(88,27), 0, 0, 360, 0, _T("ID_SPINCTRL2"));
  Rotation->SetValue(_T("0"));
  MoveButton = new wxButton(this, ID_BUTTON11, _("Move Sim"), wxPoint(16,424), wxSize(-1,-1), 0, wxDefaultValidator, _T("ID_BUTTON11"));
  RealMoveButton = new wxButton(this, ID_BUTTON2, _("Move"), wxPoint(16,456), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
  UltrasonicButton = new wxButton(this, ID_BUTTON3, _("Ultrasonic"), wxPoint(16,392), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
  StaticBox2 = new wxStaticBox(this, ID_STATICBOX2, _("Robot Control"), wxPoint(928,8), wxSize(144,232), 0, _T("ID_STATICBOX2"));
  RobotLeftButton = new wxButton(this, ID_BUTTON10, _("<"), wxPoint(944,64), wxSize(24,29), 0, wxDefaultValidator, _T("ID_BUTTON10"));
  RobotForwardButton = new wxButton(this, ID_BUTTON12, _("/\\"), wxPoint(968,32), wxSize(24,29), 0, wxDefaultValidator, _T("ID_BUTTON12"));
  RobotRightButton = new wxButton(this, ID_BUTTON13, _(">"), wxPoint(992,64), wxSize(24,29), 0, wxDefaultValidator, _T("ID_BUTTON13"));
  RobotBackButton = new wxButton(this, ID_BUTTON14, _("\\/"), wxPoint(968,96), wxSize(24,29), 0, wxDefaultValidator, _T("ID_BUTTON14"));
  RobotStopButton = new wxButton(this, ID_BUTTON15, _("o"), wxPoint(968,66), wxSize(24,24), 0, wxDefaultValidator, _T("ID_BUTTON15"));
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

  Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WorldMappingFrame::OnButtonCalculateClick);
  Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WorldMappingFrame::OnSetEndPointClick);
  Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WorldMappingFrame::OnSetStartPointClick);
  Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WorldMappingFrame::OnClearButtonClick);
  Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WorldMappingFrame::OnPrintButtonClick);
  Connect(ID_BUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WorldMappingFrame::OnButtonSimulateUltrasonicClick);
  Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&WorldMappingFrame::OnRotationChange);
  Connect(ID_BUTTON11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WorldMappingFrame::OnMoveButtonClick);
  Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WorldMappingFrame::OnRealMoveButtonClick);
  Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WorldMappingFrame::OnUltrasonicButtonClick);
  Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&WorldMappingFrame::OnQuit);
  Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&WorldMappingFrame::OnAbout);
  //*)
  draw_area=new wxBitmap(wxT("back.bmp"),wxBITMAP_TYPE_BMP);

  SetTitle(wxT("Pathfinding Simulations for GuarDDoG"));

  floor = CreateMap(101,101,10);
  SetMapUnit_In_cm(floor,15);
  SetAgentSize(floor,OURROBOT,1,1,37);
  SetAgentLocation(floor,OURROBOT,1,1) ;
  SetAgentHeading(floor,OURROBOT,1) ;
  //floor_plan = new Map(101,101);
  //floor_plan->SetActorPhysicalSize(4);
  //floor_plan->SetActorCurrentPosition(1,1,6);

  ptx1->SetValue(wxT("30")) , pty1->SetValue(wxT("30"));
  SetAgentLocation(floor,OURROBOT,30,30);

  ptx2->SetValue(wxT("69")) , pty2->SetValue(wxT("43"));
  SetAgentTargetLocation(floor,OURROBOT,69,43);

  TTS((char *)"World mapping started.");


  RobotInit((char*)"/dev/ttyUSB0",(char*)"/dev/ttyUSB1");
/*

  joy_stick = new wxJoystick(wxJOYSTICK1);
  if ( joy_stick != 0 )
  {
   if ( joy_stick->IsOk() )
   {
     TTS("Joystick is ok.");
     joy_stick->SetCapture(this,200);
   } else TTS("Could not detect a joystick.");
  }*/
}

WorldMappingFrame::~WorldMappingFrame()
{
  //(*Destroy(WorldMappingFrame)
  //*)
//  delete guarddog;
  delete draw_area;
  //delete floor_plan;
    RobotClose();
  DeleteMap(floor);

}

void WorldMappingFrame::OnQuit(wxCommandEvent& event)
{
  Close();
}

void WorldMappingFrame::OnAbout(wxCommandEvent& event)
{
  wxString msg = wxbuildinfo(long_f);
  wxMessageBox(msg, _("Welcome to..."));
}


void DrawSolvePath(wxMemoryDC &mem,struct Map *floorplancopy)
{
  unsigned int route_count=0;
  unsigned int x=0,y=0;

  wxBrush yellowback(wxColour(255,255,0),wxSOLID);
  wxPen yellow(wxColour(255,255,0),1,wxSOLID);

  while ( GetRouteWaypoint(floorplancopy,0,route_count,&x,&y) == 1 )
    {
      mem.SetPen(yellow);
      mem.SetBrush(yellowback);

      mem.DrawRectangle(x*10,y*10,10,10);
      ++route_count;
    }

  //printf("Drawing Level 1 Lines \n");
  wxPen redfat(wxColour(255,0,0),3,wxSOLID);
  unsigned int oldx=0,oldy=0;
  if ( GetStraightRouteWaypoint(floorplancopy,OURROBOT,0,&oldx,&oldy)==1 ) { mem.DrawCircle(oldx*10+5,oldy*10+5,3); }
  route_count=1;
  while ( GetStraightRouteWaypoint(floorplancopy,OURROBOT,route_count,&x,&y)==1 )
    {
      mem.SetPen(redfat);
      mem.DrawCircle(x*10+5,y*10+5,3);
      mem.DrawLine(oldx*10+5,oldy*10+5,x*10+5,y*10+5);
      ++route_count;
      oldx = x , oldy = y ;
    }
  return;
}

void DrawStartPoint(wxMemoryDC &mem,unsigned int startx,unsigned int starty)
{
  wxBrush greenback(wxColour(0,255,0),wxSOLID);
  wxPen green(wxColour(0,255,0),1,wxSOLID);


  if ( (startx!=0) || (starty!=0) )
    {

      mem.SetPen(green);
      mem.SetBrush(greenback);

      mem.DrawRectangle(startx*10,starty*10,10,10);
    }
}

void DrawEndPoint(wxMemoryDC &mem,unsigned int endx,unsigned int endy)
{
  wxBrush blueback(wxColour(0,0,255),wxSOLID);
  wxPen black(wxColour(0,0,0),1,wxSOLID);
  wxPen red(wxColour(255,0,0),1,wxSOLID);



  if ( (endx!=0) || (endy!=0) )
    {
      int parent=0;//Get_Object_PathPlanning(endx,endy,1);

      mem.SetBrush(blueback);
      if ( parent!=0 ) mem.SetPen(red);
      else
        mem.SetPen(black);

      mem.DrawRectangle(endx*10,endy*10,10,10);
    }
}




void DrawWorld(wxMemoryDC &mem,struct Map *floorplancopy,unsigned int startx,unsigned int starty,unsigned int endx,unsigned int endy)
{
  wxBrush blackback(wxColour(0,0,0),wxSOLID);
  wxBrush whiteback(wxColour(255,255,255),wxSOLID);
  wxBrush grayback(wxColour(123,123,123),wxSOLID);
  wxBrush redback(wxColour(255,0,0),wxSOLID);


  wxPen black(wxColour(0,0,0),1,wxSOLID);
  wxPen red(wxColour(255,0,0),1,wxSOLID);

  int obj=0;
  mem.SetPen(black);
  mem.SetBrush(whiteback);

  for (int y =0; y<100; y++ )
    {
      for (int x =0; x<100; x++ )
        {
          obj = ObstacleExists(floorplancopy,x,y);
          if ( obj!=0 )
            {
              mem.SetPen(red);
              mem.SetBrush(blackback);
            } else
            {
              obj = ObstacleRadiousExists(floorplancopy,x,y);
              if ( obj!=0 )
              {
                mem.SetPen(black);
                mem.SetBrush(grayback);
              }
            }

          mem.DrawRectangle(x*10,y*10,10,10);

          if ( obj!=0 )
            {
              mem.SetPen(black);
              mem.SetBrush(whiteback);
              obj = 0;
            }

        }
    }

  DrawSolvePath(mem,floorplancopy);

  DrawStartPoint(mem,startx,starty);
  DrawEndPoint(mem,endx,endy);

}

void CalibrateJoystickPos(wxJoystick * joy , wxPoint &pos,unsigned int MAX_POWER)
{
 if ( joy == 0 ) return;
 if ( ( joy->GetXMin()==0 ) | ( joy->GetXMax()==0 ) | ( joy->GetYMin()==0 ) | ( joy->GetYMax()==0 ) )  return;
 if  ( pos.x < 0 )
  {
     pos.x = -(signed int)MAX_POWER * pos.x / joy->GetXMin();
  } else
 if  ( pos.x > 0 )
  {
     pos.x = MAX_POWER * pos.x / joy->GetXMax();
  }

 if  ( pos.y < 0 )
  {
     pos.y = -(signed int)MAX_POWER * pos.y / joy->GetYMin();
  } else
 if  ( pos.y > 0 )
  {
     pos.y = MAX_POWER * pos.y / joy->GetYMax();
  }
  return;
}

void WorldMappingFrame::OnJoystickEvent(wxJoystickEvent& event)
{
   if ( joy_stick == 0 ) return;
   if ( joy_stick->IsOk() )
   {
     wxPoint pos=joy_stick->GetPosition();
     printf("Joystick Position is now %d,%d\n", pos.x ,  pos.y);
     if ( ( pos.x == 0 ) && ( pos.y == 0 ) )
      {
        RobotStopMovement();
      } else
      {
        CalibrateJoystickPos(joy_stick,pos,255);
        printf("GoJoystick %d,%d\n", pos.x ,  pos.y);
       // guarddog->GoJoystick(pos.x,pos.y);
      }
   }
}

void WorldMappingFrame::OnPaint(wxPaintEvent& event)
{


  wxPaintDC dc(this);
  wxMemoryDC mem;
  int x=130;
  int y=30;
  mem.SelectObject(*draw_area);

/*
  long startx,starty;
  ptx1->GetValue().ToLong(&startx);
  pty1->GetValue().ToLong(&starty);
  long endx,endy;
  ptx2->GetValue().ToLong(&endx);
  pty2->GetValue().ToLong(&endy);
*/
  unsigned int startx,starty;
  GetAgentLocation(floor,OURROBOT,&startx,&starty);

  unsigned int endx,endy;
  GetAgentTargetLocation(floor,OURROBOT,&endx,&endy);
  DrawWorld(mem,floor,startx,starty,endx,endy);

  dc.DrawBitmap(*draw_area,x,y,true);
}


int XYOverFeed(int &x , int &y )
{
  if ( (x>=130) && (x<=130+10*100) )
    {
      if ( (y>=30) && (y<=30+10*100) )
        {
          x =(int) ( x - 130 ) / 10;
          y =(int) ( y - 30 ) / 10;

          return 1;
        }
    }
  return 0;
}

void WorldMappingFrame::OnMotion(wxMouseEvent& event)
{
  wxSleep(0.01);
  if ( event.LeftIsDown()==1 )
    {
      int x=event.GetX();
      int y=event.GetY();

      if ( XYOverFeed(x,y)==1 )
        {
          // Meta ta X,Y tha periexoun nees plirofories
          if ( set_point_flag == 1 )
            {
              wxString tmp;
              tmp.clear() , tmp<<x , ptx1->SetValue(tmp);
              tmp.clear() , tmp<<y , pty1->SetValue(tmp);
              StatusBar1->SetStatusText(wxT("Set Start point"));

                 SetAgentLocation(floor,OURROBOT,x,y);

              wxCommandEvent nullevent;
              OnButtonCalculateClick(nullevent);
            }
          else
            if ( set_point_flag == 2 )
              {
                wxString tmp;
                tmp.clear() , tmp<<x , ptx2->SetValue(tmp);
                tmp.clear() , tmp<<y , pty2->SetValue(tmp);
                StatusBar1->SetStatusText(wxT("Set End point"));

                 SetAgentTargetLocation(floor,OURROBOT,x,y);

                wxCommandEvent nullevent;
                OnButtonCalculateClick(nullevent);
              }
            else
              {
                //floor_plan->SetObjectAt(x,y,BLOCKED);
                SetObstacle(floor,x,y,OBSTACLE_UNCERTAINTY) ;
                Refresh();
              }

          set_point_flag=0;
        }
    }
  else
    if ( event.RightIsDown()==1 )
      {
        int x=event.GetX();
        int y=event.GetY();
        if ( XYOverFeed(x,y)==1 )
          {
            //floor_plan->SetObjectAt(x,y,FREE);
            RemoveObstacle(floor,x,y,OBSTACLE_UNCERTAINTY);
            Refresh();
          }
      }
  return;
}


void WorldMappingFrame::OnSetEndPointClick(wxCommandEvent& event)
{
  set_point_flag=2;
  StatusBar1->SetStatusText(wxT("Waiting for End point click"));
}

void WorldMappingFrame::OnSetStartPointClick(wxCommandEvent& event)
{
  set_point_flag=1;
  StatusBar1->SetStatusText(wxT("Waiting for Start point click"));
}

void WorldMappingFrame::OnButtonCalculateClick(wxCommandEvent& event)
{
 // long startx,starty;
 // ptx1->GetValue().ToLong(&startx);
 // pty1->GetValue().ToLong(&starty);

  //floor_plan->SetActorCurrentPosition(startx,starty,6);
 // SetAgentLocation(floor,OURROBOT,startx,starty);

 // long endx,endy;
 // ptx2->GetValue().ToLong(&endx);
 // pty2->GetValue().ToLong(&endy);

 // SetAgentHeading(floor,OURROBOT,Rotation->GetValue()) ;

  //signed int res=floor_plan->FindPathTo(endx,endy,TimeMS->GetValue()*10);
 // signed int res=FindSponteneousPath(floor,OURROBOT,startx,starty,endx,endy,TimeMS->GetValue()*10) ;
   signed int res=FindPath(floor,OURROBOT,TimeMS->GetValue()*10);
  //floor_plan->FindPathTo(endx,endy,);
  if ( res <1 ) { TTS((char *)"Could not establish a route.");  } else
                { TTS((char *)"New route established."); }
  Refresh();
}

void WorldMappingFrame::OnClearButtonClick(wxCommandEvent& event)
{
  //floor_plan->Clear();
  ClearMap(floor);
  Refresh();
}

void WorldMappingFrame::OnPrintButtonClick(wxCommandEvent& event)
{
   //floor_plan->HTMLOutput("printout.html");
}

void WorldMappingFrame::OnButtonSimulateUltrasonicClick(wxCommandEvent& event)
{
  long ultrasonic_x,ultrasonic_y;
  obsx->GetValue().ToLong(&ultrasonic_x);
  obsy->GetValue().ToLong(&ultrasonic_y);
  AddObstacleSensedbyAgent(floor,OURROBOT,OBSTACLE_UNCERTAINTY,ultrasonic_x,ultrasonic_y) ;

    wxCommandEvent nullevent;
  OnButtonCalculateClick(nullevent);
}


void WorldMappingFrame::OnButtonSetRotationClick(wxCommandEvent& event)
{
  long rotation_value=Rotation->GetValue();
  SetAgentHeading(floor,OURROBOT,(float) rotation_value) ;
}


void WorldMappingFrame::OnRotationChange(wxSpinEvent& event)
{
  long rotation_value=Rotation->GetValue();
  SetAgentHeading(floor,OURROBOT,(float) rotation_value) ;
}

void WorldMappingFrame::OnButton2Click(wxCommandEvent& event)
{
}

void WorldMappingFrame::OnButton1Click(wxCommandEvent& event)
{
}

void WorldMappingFrame::OnMoveButtonClick(wxCommandEvent& event)
{
  long move_x,move_y;
  obsx->GetValue().ToLong(&move_x);
  obsy->GetValue().ToLong(&move_y);
  MoveAgentForward(floor,OURROBOT,move_x,move_y);
  wxCommandEvent nullevent;
  OnButtonCalculateClick(nullevent);

}


void WorldMappingFrame::OnRealMoveButtonClick(wxCommandEvent& event)
{
  long move_x,move_y;
  obsx->GetValue().ToLong(&move_x);
  obsy->GetValue().ToLong(&move_y);

  signed int move_command=0;
  if ( move_x < move_y ) { move_command=(signed int) move_x; } else
                         { move_command=(signed int) move_y; }

  RobotMove(20,(signed int) move_x*1000);
  MoveAgentForward(floor,OURROBOT,move_command,move_command);
    wxCommandEvent nullevent;
  OnButtonCalculateClick(nullevent);
}

void WorldMappingFrame::OnUltrasonicButtonClick(wxCommandEvent& event)
{
  long ultrasonic_x=RobotGetUltrasonic(0),ultrasonic_y=RobotGetUltrasonic(1);

  wxString int_to_str;
  int_to_str.Clear(); int_to_str<<ultrasonic_x;
  obsx->SetValue(int_to_str);

  int_to_str.Clear(); int_to_str<<ultrasonic_y;
  obsy->SetValue(int_to_str);

  AddObstacleSensedbyAgent(floor,OURROBOT,OBSTACLE_UNCERTAINTY,ultrasonic_x,ultrasonic_y) ;

    wxCommandEvent nullevent;
  OnButtonCalculateClick(nullevent);
}
