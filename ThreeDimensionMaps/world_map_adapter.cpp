#include "world_map_adapter.h"

#include <stdlib.h>
#include <stdio.h>
int slices = 16;
int stacks = 16;
unsigned char video_color[640*480*3]={0};
unsigned char video_depth[640*480*3]={0};

GLfloat left_rotation[16]={0.0};
GLfloat right_rotation[16]={0.0};


GLfloat left_translation[16]={0.0};
GLfloat right_translation[16]={0.0};

double last_load;


void LoadDepth(int snap)
{
   char filename[60]={0};
   sprintf(filename,"memfs/DEPTH%u",snap);

   FILE * fp;
   fp = fopen(filename,"rb");
   if (fp == 0) return;

   unsigned int  ptrlim = 320*240*3;
   fread (video_depth , 1 , ptrlim , fp );
   fclose(fp);

   sprintf(filename,"memfs/COLOR%u",snap);
   fp = fopen(filename,"rb");
   if (fp == 0) return;

   fread (video_color , 1 , ptrlim , fp );
   fclose(fp);
}


int SaveTransformationMatrixToFile(char * filename,GLfloat * matrix,unsigned int cols,unsigned int rows)
{
    FILE *fd=0;
    fd = fopen(filename,"w");

    if (fd!=0)
	{
      unsigned int i=0;
       for ( i=0; i< cols * rows ; i ++ )
        {
          fprintf(fd,"%f\n",matrix[i]);
        }

	  fclose(fd);
	  return 1;
	}
  return 0;
}




void LoadMatrix4x4(char * filename,int snap,GLfloat * transformation)
{
   char filename_full[128]={0};
   sprintf(filename_full,"%s%u",filename,snap);

   FILE * fp;
   fp = fopen(filename_full,"r");
   if (fp == 0) return;

    fscanf(fp,"%f",&transformation[0]);
    fscanf(fp,"%f",&transformation[1]);
    fscanf(fp,"%f",&transformation[2]);
    fscanf(fp,"%f",&transformation[4]);

    fscanf(fp,"%f",&transformation[4]);
    fscanf(fp,"%f",&transformation[5]);
    fscanf(fp,"%f",&transformation[6]);
    fscanf(fp,"%f",&transformation[7]);

    fscanf(fp,"%f",&transformation[8]);
    fscanf(fp,"%f",&transformation[9]);
    fscanf(fp,"%f",&transformation[10]);
    fscanf(fp,"%f",&transformation[11]);

    fscanf(fp,"%f",&transformation[12]);
    fscanf(fp,"%f",&transformation[13]);
    fscanf(fp,"%f",&transformation[14]);
    fscanf(fp,"%f",&transformation[15]);
    fclose(fp);


}
