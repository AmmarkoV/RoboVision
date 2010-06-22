#ifndef PATHPLANNING_H_INCLUDED
#define PATHPLANNING_H_INCLUDED

unsigned short  PathPlan_Version();

void PathPlan_SetStartPoint(unsigned short vpath_start_x,unsigned short vpath_start_y);
void PathPlan_GetStartPoint(unsigned short &vpath_start_x,unsigned short &vpath_start_y);

void PathPlan_SetEndPoint(unsigned short vpath_end_x,unsigned short vpath_end_y);
void PathPlan_GetEndPoint(unsigned short &vpath_end_x,unsigned short &vpath_end_y);

void  InitPathPlanning(unsigned short x_size,unsigned short y_size);
void  UnInitPathPlanning();

void  Set_Object_PathPlanning(unsigned short atx,unsigned short aty,unsigned short type,unsigned short passable);
unsigned short  Get_Object_PathPlanning(unsigned short atx,unsigned short aty,unsigned short type);
unsigned short  Get_Object_PathPlanningMem(unsigned short mem_place,unsigned short type);
void  FindPath_PathPlanning(unsigned short x1,unsigned short y1,unsigned short start_direction,unsigned short x2,unsigned short y2);
#endif // PATHPLANNING_H_INCLUDED
