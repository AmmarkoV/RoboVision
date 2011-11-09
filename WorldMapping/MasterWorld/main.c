#include "MasterWorld.h"

#define MAX_POINTS_ON_WORLD 4096
#define MAX_AGENTS_ON_WORLD 3

struct Points voxels[MAX_POINTS_ON_WORLD]={0};
struct Agents agent[MAX_AGENTS_ON_WORLD]={0};
unsigned int total_points_number=0;




int Add3DPoint(float x,float y,float z , unsigned char r ,unsigned char g ,unsigned char b)
{
   if ( total_points_number >= MAX_POINTS_ON_WORLD)  { return 0; }

   voxels[total_points_number].x=x;
   voxels[total_points_number].y=y;
   voxels[total_points_number].z=z;

   voxels[total_points_number].r=r;
   voxels[total_points_number].g=g;
   voxels[total_points_number].b=b;



   return 1;
}



int SetAgent(unsigned int agent_num,float x,float y,float z , float heading , float pitch , float yaw)
{
  agent[agent_num].x = x;
  agent[agent_num].y = y;
  agent[agent_num].z = z;

  agent[agent_num].heading = heading;
  agent[agent_num].pitch = pitch;
  agent[agent_num].yaw = yaw;

  return 1;
}



int GetAgent(unsigned int agent_num,float * x,float * y,float * z , float * heading , float * pitch , float * yaw)
{
  agent[agent_num].x = *x;
  agent[agent_num].y = *y;
  agent[agent_num].z = *z;

  agent[agent_num].heading = *heading;
  agent[agent_num].pitch = *pitch;
  agent[agent_num].yaw = *yaw;

  return 1;
}

int RotateAgent(unsigned int agent_num, float degrees)
{
   return 1;
}

