#include "MasterWorld.h"

#define MAX_POINTS_ON_WORLD 4096
#define MAX_AGENTS_ON_WORLD 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Points voxels[MAX_POINTS_ON_WORLD]={0};
struct Agents agent[MAX_AGENTS_ON_WORLD]={0};
unsigned int total_points_number=0;


int Clear3DPointCloud()
{
   total_points_number=0;
   return 1;
}


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
  *x = agent[agent_num].x;
  *y = agent[agent_num].y;
  *z = agent[agent_num].z;

  *heading = agent[agent_num].heading;
  *pitch = agent[agent_num].pitch ;
  *yaw = agent[agent_num].yaw ;

  return 1;
}


int PrintFAgent(unsigned int agent_num)
{
  fprintf(stderr,"%0.2f , %0.2f , %0.2f\n",agent[agent_num].x,agent[agent_num].y,agent[agent_num].z);
  return 1;
}

int RotateAgent(unsigned int agent_num, float degrees)
{
   return 1;
}





int Save3DPointCloudToFile(char * filename)
{
  FILE *fd=0;
    fd = fopen(filename,"w");

    if (fd!=0)
	{

	  int i=0;
      while ( i<total_points_number )
      {
        fprintf(fd,"%f\n%f\n%f\n",voxels[i].x,voxels[i].y,voxels[i].z);
        fprintf(fd,"%u\n%u\n%u\n",voxels[i].r,voxels[i].g,voxels[i].b);
        ++i;
      }

     fclose(fd);
     return 1;
	}

   return 0;
}


int AdaptDepthMapTo2DMap(unsigned char * depthvalues,unsigned int depthvalues_length,float horizontal_fov,float * mapping_of_depth_values_to_cm)
{
    /*                 O
                /      O
             /         O
           /            O
   Viewer* ----------- O
          \         O
            \  O
               \



    */
  return 0;
}

