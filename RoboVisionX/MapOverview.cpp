#include "MapOverview.h"
#include "RoboVisionXMain.h"
//(*InternalHeaders(MapOverview)
#include <wx/string.h>
#include <wx/intl.h>
//*)


#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/colour.h>
#include <wx/utils.h>

wxBitmap *draw_area=0;
unsigned int draw_area_width=866;
unsigned int draw_area_height=500;

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
	CheckBoxAutoUpdate = new wxCheckBox(this, ID_CHECKBOX1, _("Auto Update"), wxPoint(536,536), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBoxAutoUpdate->SetValue(false);
	ButtonExecute = new wxButton(this, ID_BUTTON1, _("Execute"), wxPoint(664,532), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MapOverview::OnButtonExecuteClick);
	//*)

    draw_area = new wxBitmap(draw_area_width,draw_area_height,-1);

}

MapOverview::~MapOverview()
{
    if (map_frame!=0) { map_frame=0; }


	//(*Destroy(MapOverview)
	//*)
}


void DrawSolvePath(wxMemoryDC &mem,struct Map *floorplancopy)
{/*
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
    }*/
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

  for (int y =0; y<100; y++ )
    {
      for (int x =0; x<100; x++ )
        {

         // obj = ObstacleExists(floorplancopy,x,y);
          if ( obj!=0 )
            {
              mem.SetPen(red);
              mem.SetBrush(blackback);
            } else
            {
           //   obj = ObstacleRadiousExists(floorplancopy,x,y);
              if ( obj!=0 )
              {
                mem.SetPen(black);
                mem.SetBrush(grayback);
              }
            }

          mem.DrawRectangle(x*10,y*10,10,10);
           obj=1;
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


void MapOverview::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  wxMemoryDC mem;
  int x=30;
  int y=30;
  mem.SelectObject(*draw_area);
  mem.Clear();

  unsigned int startx=10,starty=10;
  //GetAgentLocation(floor,OURROBOT,&startx,&starty);

  unsigned int endx=11,endy=11;
  //GetAgentTargetLocation(floor,OURROBOT,&endx,&endy);
//  DrawWorld(mem,floor,startx,starty,endx,endy);


  DrawStartPoint(mem,startx,starty);
  DrawEndPoint(mem,endx,endy);


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

void MapOverview::OnMotion(wxMouseEvent& event)
{
  wxSleep(0.01);
  /*
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
      }*/
  return;
}



void MapOverview::OnButtonExecuteClick(wxCommandEvent& event)
{
}
