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


struct PointData points[320*240]={0};
unsigned int current_points=0;

void LoadDepth(int snap)
{
   char filename[60]={0};
   sprintf(filename,"memfs/DEPTH%u",snap);

   FILE * fp;
   fp = fopen(filename,"rb");
   if (fp == 0) { fprintf(stderr,"Failed to load Depth data \n "); return; }

   unsigned int  ptrlim = 320*240*3;
   fread (video_depth , 1 , ptrlim , fp );
   fclose(fp);

   sprintf(filename,"memfs/COLOR%u",snap);
   fp = fopen(filename,"rb");
   if (fp == 0) { fprintf(stderr,"Failed to load Color data \n ");  return; }

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


void LoadDepthNew(int snap)
{
   FILE * fp;
   fp = fopen("memfs/DEPTH_MAP","r");
   if (fp == 0) { fprintf(stderr,"Failed to load Depth data \n "); return; }


   float smallestX=0.0,largestX=0.0,smallestY=0.0,largestY=0.0,smallestZ=0.0,largestZ=0.0;

   current_points=0;
   while (feof(fp)==0)
    {
      fscanf(fp,"%f",&points[current_points].X);
      fscanf(fp,"%f",&points[current_points].Y);
      fscanf(fp,"%f",&points[current_points].Z);
      fscanf(fp,"%u",&points[current_points].R);
      fscanf(fp,"%u",&points[current_points].G);
      fscanf(fp,"%u",&points[current_points].B);

      if (smallestX<points[current_points].X) { smallestX = points[current_points].X;}
      if (largestX >points[current_points].X) { largestX  = points[current_points].X;}
      if (smallestY<points[current_points].Y) { smallestY = points[current_points].Y;}
      if (largestY >points[current_points].Y) { largestY  = points[current_points].Y;}
      if (smallestZ<points[current_points].Z) { smallestZ = points[current_points].Z;}
      if (largestZ >points[current_points].Z) { largestZ  = points[current_points].Z;}


      if (points[current_points].X > 1000 )
       {
           fprintf(stderr,"%0.2f %0.2f %0.2f = RGB ( %u , %u , %u ) \n",points[current_points].X,points[current_points].Y,points[current_points].Z,points[current_points].R,points[current_points].G,points[current_points].B);
       }

     ++current_points;
    }

   fprintf(stderr,"Loaded %u points\n",current_points);
   fprintf(stderr,"Bounding box X ( %0.2f - %0.2f )   Y ( %0.2f - %0.2f )   Z ( %0.2f - %0.2f ) \n",smallestX,largestX,smallestY,largestY,smallestZ,largestZ);
   fclose(fp);
}



