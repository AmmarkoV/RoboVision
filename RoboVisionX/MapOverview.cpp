#include "MapOverview.h"
#include "RoboVisionXMain.h"
//(*InternalHeaders(MapOverview)
#include <wx/string.h>
#include <wx/intl.h>
//*)

#include <math.h>

#include "../RoboKernel/RoboKernel.h"
#include "../WorldMapping/MasterpathPlanning/MasterpathPlanning.h"
#include "../MotorFoundation/MotorHAL.h"


#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/colour.h>
#include <wx/utils.h>

int MapDrawX=30;
int MapDrawY=30;
unsigned int map_box_size=4;
unsigned int map_box_size_half=2;


#define PI 3.14159265

wxBitmap *draw_area=0;
unsigned int draw_area_width=866;
unsigned int draw_area_height=500;

unsigned int draw_area_actual_pointsX=200;
unsigned int draw_area_actual_pointsY=100;


unsigned int OURROBOT=0;
unsigned int OBSTACLE_UNCERTAINTY=1;


//(*IdInit(MapOverview)
const long MapOverview::ID_STATICBOX1 = wxNewId();
const long MapOverview::ID_STATICTEXT1 = wxNewId();
const long MapOverview::ID_TEXTCTRL1 = wxNewId();
const long MapOverview::ID_TEXTCTRL2 = wxNewId();
const long MapOverview::ID_STATICTEXT3 = wxNewId();
const long MapOverview::ID_TEXTCTRL3 = wxNewId();
const long MapOverview::ID_TEXTCTRL4 = wxNewId();
const long MapOverview::ID_CHECKBOX1 = wxNewId();
const long MapOverview::ID_BUTTON1 = wxNewId();
const long MapOverview::ID_BUTTON2 = wxNewId();
const long MapOverview::ID_BUTTON3 = wxNewId();
const long MapOverview::ID_STATICTEXT2 = wxNewId();
const long MapOverview::ID_SPINCTRL1 = wxNewId();
const long MapOverview::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(MapOverview,wxFrame)
	//(*EventTable(MapOverview)
	//*)
  EVT_PAINT(MapOverview::OnPaint)
  EVT_MOTION(MapOverview::OnMotion)
END_EVENT_TABLE()

MapOverview::MapOverview(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(MapOverview)
	Create(parent, id, _("Map Overview"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
	SetClientSize(wxSize(924,584));
	Move(wxDefaultPosition);
	StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, _("Map Overview"), wxPoint(16,8), wxSize(896,568), 0, _T("ID_STATICBOX1"));
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Current Position"), wxPoint(40,536), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCurPosX = new wxTextCtrl(this, ID_TEXTCTRL1, _("0"), wxPoint(152,532), wxSize(48,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	TextCurPosY = new wxTextCtrl(this, ID_TEXTCTRL2, _("0"), wxPoint(200,532), wxSize(48,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Target Position"), wxPoint(304,536), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	TextTargetPosX = new wxTextCtrl(this, ID_TEXTCTRL3, _("0"), wxPoint(400,532), wxSize(48,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	TextTargetPosY = new wxTextCtrl(this, ID_TEXTCTRL4, _("0"), wxPoint(448,532), wxSize(48,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	CheckBoxAutoUpdate = new wxCheckBox(this, ID_CHECKBOX1, _("Auto Update"), wxPoint(688,533), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBoxAutoUpdate->SetValue(false);
	ButtonExecute = new wxButton(this, ID_BUTTON1, _("Execute"), wxPoint(800,520), wxSize(104,48), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	ButtonSetCurPos = new wxButton(this, ID_BUTTON2, _("Set"), wxPoint(248,530), wxSize(32,29), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	ButtonSetTargetPos = new wxButton(this, ID_BUTTON3, _("Set"), wxPoint(496,530), wxSize(32,29), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Rotation"), wxPoint(544,536), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	SpinCtrlOrientation = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(608,532), wxSize(56,27), 0, -360, 360, 0, _T("ID_SPINCTRL1"));
	SpinCtrlOrientation->SetValue(_T("0"));
	Timer1.SetOwner(this, ID_TIMER1);
	Timer1.Start(100, false);

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MapOverview::OnButtonExecuteClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MapOverview::OnButtonSetCurPosClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MapOverview::OnButtonSetTargetPosClick);
	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&MapOverview::OnSpinCtrlOrientationChange);
	Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&MapOverview::OnTimer1Trigger);
	//*)

    draw_area = new wxBitmap(draw_area_width,draw_area_height,-1);
    set_point_flag=0;


}

MapOverview::~MapOverview()
{
    if (map_frame!=0) { map_frame=0; }


	//(*Destroy(MapOverview)
	//*)
}


void DrawSolvePath(wxMemoryDC &mem)
{
  unsigned int route_count=0;
  unsigned int x=0,y=0;

  wxBrush yellowback(wxColour(255,255,0),wxSOLID);
  wxPen yellow(wxColour(255,255,0),1,wxSOLID);


  while ( GetRouteWaypoint(GetWorldHandler(),0,route_count,&x,&y) == 1 )
    {
      mem.SetPen(yellow);
      mem.SetBrush(yellowback);

      mem.DrawRectangle(x*map_box_size,y*map_box_size,map_box_size,map_box_size);
      ++route_count;
    }

  //printf("Drawing Level 1 Lines \n");
  wxPen redfat(wxColour(255,0,0),3,wxSOLID);
  unsigned int oldx=0,oldy=0;
  if ( GetStraightRouteWaypoint(GetWorldHandler(),OURROBOT,0,&oldx,&oldy)==1 ) { mem.DrawCircle(oldx*10+5,oldy*10+5,3); }
  route_count=1;
  while ( GetStraightRouteWaypoint(GetWorldHandler(),OURROBOT,route_count,&x,&y)==1 )
    {
      mem.SetPen(redfat);
      mem.DrawCircle(x*map_box_size+map_box_size_half,y*map_box_size+map_box_size_half,3);
      mem.DrawLine(oldx*map_box_size+map_box_size_half,oldy*map_box_size+map_box_size_half,x*map_box_size+map_box_size_half,y*map_box_size+map_box_size_half);
      ++route_count;
      oldx = x , oldy = y ;
    }
  return;
}

void DrawStartPoint(wxMemoryDC &mem,unsigned int startx,unsigned int starty,float heading)
{
  wxBrush greenback(wxColour(0,255,0),wxSOLID);
  wxPen green(wxColour(0,255,0),1,wxSOLID);

  fprintf(stderr,"Drawing Start point @ %u,%u heading %0.2f\n",startx,starty,heading);

  // We have a box sized 6x12
  // and want to rotate it accordign to heading
  /*

   |
   |       * X',Y'
   |      -         * X,Y
   |  r  -       -
   |    -     -
   |   -   -  r
   |  - -
   |_-_________________

  (old coordinates are (x, y) and the new coordinates are (x', y'))

   q = initial angle, f = angle of rotation.

   x = r cos q
   y = r sin q

   x' = r cos ( q + f ) = r cos q cos f - r sin q sin f
   y' = r sin ( q + w ) = r sin q cos f + r cos q sin f

   hence:
   x' = x cos f - y sin f
   y' = y cos f + x sin f


   We also asume

-,-        0,+        + . -

          XL,YL
X4,Y4________________  X1,Y1
   |                |
XB |        *       |   XF
 YB|                |      YF
   |________________|
X3,Y3                  X2,Y2
-.+          0,-          + , +
           XR,YR
  */

  float box_width  = robot_length/1000;
  float box_height = robot_width/1000;


  float XA1 = 0.0+box_width/2 ;
  float YA1 = 0.0-box_height/2 ;
  float XA2 = 0.0+box_width/2 ;
  float YA2 = 0.0+box_height/2 ;
  float XA3 = 0.0-box_width/2 ;
  float YA3 = 0.0+box_height/2 ;
  float XA4 = 0.0-box_width/2 ;
  float YA4 = 0.0-box_height/2 ;

  float XAF = 0.0+box_width/2 ;
  float YAF = 0.0;
  float XAB=  0.0-box_width/2 ;
  float YAB = 0.0;
  float XAR = 0.0;
  float YAR = 0.0+box_height/2 ;
  float XAL = 0.0 ;
  float YAL = 0.0-box_height/2 ;

  float XB1 , YB1, XB2, YB2, XB3 , YB3, XB4, YB4 , XBF, YBF , XBB, YBB  , XBR, YBR  , XBL, YBL ;

  XB1 =  startx + (XA1 * cos(heading*PI/180)) - (YA1 * sin(heading*PI/180));
  YB1 =  starty + (YA1 * cos(heading*PI/180)) + (XA1 * sin(heading*PI/180));


  XB2 =  startx + XA2 * cos(heading*PI/180) - YA2 * sin(heading*PI/180);
  YB2 =  starty + YA2 * cos(heading*PI/180) + XA2 * sin(heading*PI/180);


  XB3 =  startx + XA3 * cos(heading*PI/180) - YA3 * sin(heading*PI/180);
  YB3 =  starty + YA3 * cos(heading*PI/180) + XA3 * sin(heading*PI/180);


  XB4 =  startx + XA4 * cos(heading*PI/180) - YA4 * sin(heading*PI/180);
  YB4 =  starty + YA4 * cos(heading*PI/180) + XA4 * sin(heading*PI/180);

  XBF =  startx + XAF * cos(heading*PI/180) - YAF * sin(heading*PI/180);
  YBF =  starty + YAF * cos(heading*PI/180) + XAF * sin(heading*PI/180);

  XBB =  startx + XAB * cos(heading*PI/180) - YAB * sin(heading*PI/180);
  YBB =  starty + YAB * cos(heading*PI/180) + XAB * sin(heading*PI/180);

  XBR =  startx + XAR * cos(heading*PI/180) - YAR * sin(heading*PI/180);
  YBR =  starty + YAR * cos(heading*PI/180) + XAR * sin(heading*PI/180);

  XBL =  startx + XAL * cos(heading*PI/180) - YAL * sin(heading*PI/180);
  YBL =  starty + YAL * cos(heading*PI/180) + XAL * sin(heading*PI/180);


  fprintf(stderr,"We have points %0.2f,%0.2f  %0.2f,%0.2f  %0.2f,%0.2f %0.2f,%0.2f  \n",XB1,YB1,XB2,YB2,XB3,YB3,XB4,YB4);

  if ( (startx!=0) || (starty!=0) )
    {
     mem.SetPen(green);
     mem.SetBrush(greenback);

     mem.DrawLine((signed int)XB1,(signed int)YB1,(signed int)XB2,(signed int)YB2);
     mem.DrawLine((signed int)XB2,(signed int)YB2,(signed int)XB3,(signed int)YB3);
     mem.DrawLine((signed int)XB3,(signed int)YB3,(signed int)XB4,(signed int)YB4);
     mem.DrawLine((signed int)XB4,(signed int)YB4,(signed int)XB1,(signed int)YB1);

     //ARROW
     mem.DrawLine((signed int)XBB,(signed int)YBB,(signed int)XBF,(signed int)YBF);
     mem.DrawLine((signed int)XBF,(signed int)YBF,(signed int)XBL,(signed int)YBL);
     mem.DrawLine((signed int)XBF,(signed int)YBF,(signed int)XBR,(signed int)YBR);


     mem.DrawRectangle(startx*map_box_size-3,starty*map_box_size-3,map_box_size+6,map_box_size+6);
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

      mem.DrawRectangle(endx*map_box_size-3,endy*map_box_size-3,map_box_size+6,map_box_size+6);
    }
}




void DrawWorld(wxMemoryDC &mem,struct Map *floorplancopy)
{
  wxBrush blackback(wxColour(0,0,0),wxSOLID);
  wxPen white(wxColour(255,255,255),1,wxSOLID);
  wxBrush whiteback(wxColour(255,255,255),wxSOLID);
  wxBrush grayback(wxColour(123,123,123),wxSOLID);
  wxBrush redback(wxColour(255,0,0),wxSOLID);

  mem.SetPen(white);
  mem.SetBrush(whiteback);
  mem.DrawRectangle(0,0,draw_area_width,draw_area_height);

  wxPen black(wxColour(0,0,0),1,wxSOLID);
  wxPen red(wxColour(255,0,0),1,wxSOLID);

  int obj=0;
  mem.SetPen(black);
  mem.SetBrush(whiteback);

  for (unsigned int y =0; y<draw_area_actual_pointsY; y++ )
    {
      for (unsigned int x =0; x<draw_area_actual_pointsX; x++ )
        {

          obj = ObstacleExists(GetWorldHandler(),x,y);
          if ( obj!=0 )
            {
              mem.SetPen(red);
              mem.SetBrush(blackback);
            } else
            {
              obj = ObstacleRadiousExists(GetWorldHandler(),x,y);
              if ( obj!=0 )
              {
                mem.SetPen(black);
                mem.SetBrush(grayback);
              }
            }

          mem.DrawRectangle(x*map_box_size,y*map_box_size,map_box_size,map_box_size);
           obj=1;
          if ( obj!=0 )
            {
              mem.SetPen(white); //black for boxes everywhere!
              mem.SetBrush(whiteback);
              obj = 0;
            }

        }
    }

  DrawSolvePath(mem);



  unsigned int endx=0,endy=0;
  GetAgentTargetLocation(GetWorldHandler(),OURROBOT,&endx,&endy);

  DrawEndPoint(mem,endx,endy);

  // Draw Start point drawn second in case both points are the same ( we are on target :P )

  unsigned int startx=0,starty=0;
  GetAgentLocation(GetWorldHandler(),OURROBOT,&startx,&starty);

  DrawStartPoint(mem,startx,starty,GetAgentHeading(GetWorldHandler(),OURROBOT));


}


void MapOverview::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  wxMemoryDC mem;
  mem.SelectObject(*draw_area);
  mem.Clear();

  DrawWorld(mem,GetWorldHandler());

  dc.DrawBitmap(*draw_area,MapDrawX,MapDrawY,1);
}


int XYOverFeed(int &x , int &y )
{
  if ( (x>=MapDrawX) && (x<=MapDrawX+map_box_size*draw_area_actual_pointsX) )
    {
      if ( (y>=MapDrawY) && (y<=MapDrawY+map_box_size*draw_area_actual_pointsY) )
        {
          x =(int) ( x - MapDrawX ) / map_box_size;
          y =(int) ( y - MapDrawY ) / map_box_size;

          return 1;
        }
    }
  return 0;
}

void MapOverview::OnMotion(wxMouseEvent& event)
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
              tmp.clear() , tmp<<x , TextCurPosX->SetValue(tmp);
              tmp.clear() , tmp<<y , TextCurPosY->SetValue(tmp);
              //StatusBar1->SetStatusText(wxT("Set Start point"));

                 SetAgentLocation(GetWorldHandler(),OURROBOT,x,y);

              wxCommandEvent nullevent;
              OnButtonExecuteClick(nullevent);
            }
          else
            if ( set_point_flag == 2 )
              {
                wxString tmp;
                tmp.clear() , tmp<<x , TextTargetPosX->SetValue(tmp);
                tmp.clear() , tmp<<y , TextTargetPosY->SetValue(tmp);
                //StatusBar1->SetStatusText(wxT("Set End point"));

                 SetAgentTargetLocation(GetWorldHandler(),OURROBOT,x,y);

                wxCommandEvent nullevent;
                OnButtonExecuteClick(nullevent);
              }
            else
              {
                SetObstacle(GetWorldHandler(),x,y,OBSTACLE_UNCERTAINTY) ;
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
            RemoveObstacle(GetWorldHandler(),x,y,OBSTACLE_UNCERTAINTY);
            Refresh();
          }
      }
  return;
}



void MapOverview::OnButtonExecuteClick(wxCommandEvent& event)
{
   signed int res=FindPath(GetWorldHandler(),OURROBOT,100); //TimeMS->GetValue()*10
  //floor_plan->FindPathTo(endx,endy,);
//  if ( res <1 ) { TTS((char *)"Could not establish a route.");  } else
//                { TTS((char *)"New route established."); }
  Refresh();
}

void MapOverview::OnButtonSetCurPosClick(wxCommandEvent& event)
{
  set_point_flag=1;
}

void MapOverview::OnButtonSetTargetPosClick(wxCommandEvent& event)
{
  set_point_flag=2;
}

void MapOverview::OnTimer1Trigger(wxTimerEvent& event)
{
}

void MapOverview::OnSpinCtrlOrientationChange(wxSpinEvent& event)
{
    SetAgentHeading(GetWorldHandler(),OURROBOT,(float) SpinCtrlOrientation->GetValue());
    Refresh();
}
