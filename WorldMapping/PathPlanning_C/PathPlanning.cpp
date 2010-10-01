#define _WIN32_WINNT 0x0400
#include "PathPlanning.h"
#include "PathPlan_core.h"
#include "PathPlan_printouts.h"
#include "PathPlan_objectpositions.h"
#include <stdio.h>

int path_start_x,path_start_y;
int path_end_x,path_end_y;


unsigned short  PathPlan_Version()
{         // . ( 0001 = 0.001)
	return 0002; }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// GET/SET POINTS!            START
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void PathPlan_SetStartPoint(unsigned short vpath_start_x,unsigned short vpath_start_y)
{
  path_start_x=vpath_start_x,path_start_y=vpath_start_y;
  printf("Set New StartPoint %d,%d\n",vpath_start_x,vpath_start_y);
}

void PathPlan_GetStartPoint(unsigned short &vpath_start_x,unsigned short &vpath_start_y)
{
  vpath_start_x=path_start_x,vpath_start_y=path_start_y;
}

void PathPlan_SetEndPoint(unsigned short vpath_end_x,unsigned short vpath_end_y)
{
  path_end_x=vpath_end_x,path_end_y=vpath_end_y;
  printf("Set New EndPoint %d,%d\n",vpath_end_x,vpath_end_y);
}

void PathPlan_GetEndPoint(unsigned short &vpath_end_x,unsigned short &vpath_end_y)
{
  vpath_end_x=path_end_x,vpath_end_y=path_end_y;
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// GET/SET POINTS!             END
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


int PathPlan_HTMLOutput(char * filename)
{
    return PrintoutHTML(filename);
}

unsigned short  PathPlan_Status(unsigned short type_of_data)
{
    unsigned short thedat=PathPlanningStatus(type_of_data);
	return thedat;
}

void  InitPathPlanning(unsigned short x_size,unsigned short y_size)
{
     CreatePathPlanningMatrix(x_size,y_size);
}

void  UnInitPathPlanning()
{
      DeletePathPlanningMatrix();
}


void  Set_Object_PathPlanning(unsigned short atx,unsigned short aty,unsigned short type,unsigned short passable)
{
    bool unpassable=false;
	if (passable==0) { unpassable=true; }
	if (passable==1) { passable=0; }
	SetNodeData(atx,aty,unpassable,passable);
}

unsigned short  Get_Object_PathPlanning(unsigned short atx,unsigned short aty,unsigned short type)
{
    return PathPlanning_Get_Object(atx,aty,type);
}

unsigned short  Get_Object_PathPlanningMem(unsigned short mem_place,unsigned short type)
{
    return PathPlanning_Get_ObjectMem(mem_place,type);
}


void  FindPath_PathPlanning(unsigned short x1,unsigned short y1,unsigned short start_direction,unsigned short x2,unsigned short y2)
{
    FindPath(x1,y1,start_direction,x2,y2);
    printf("Find Path called from  %d,%d to %d,%d\n",x1,y1,x2,y2);
}


