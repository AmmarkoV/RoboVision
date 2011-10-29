#include "models.h"
#include "world_map_adapter.h"
#include "camera.h"
#include "tools.h"



#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


void DrawAxis()
{

    glBegin(GL_LINES);
      //glLineStipple(1, 0xF00F);
      glLineWidth(35.0);
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex3f(-1000,0,0);
      glVertex3f(1000,0,0);


      glColor3f(0.0f, 1.0f, 0.0f);
      glVertex3f(0,-1000,0);
      glVertex3f(0,1000,0);

      glColor3f(0.0f, 0.0f, 1.0f);
      glVertex3f(0,0,-1000);
      glVertex3f(0,0,1000);
    glEnd();

   glLineWidth(1.0);
}


void DrawFloor()
{
  glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   //glEnable(GL_NORMALIZE);
    glBegin(GL_QUADS);
      glLineWidth(15.0);
      glColorRGB(87,7,7);
      glVertex3f( 1000,-0.1,-1000);
      glVertex3f( 1000,-0.1,1000);
      glVertex3f(-1000,-0.1,1000);
      glVertex3f(-1000,-0.1,-1000);
    glEnd();
  glPopMatrix();

   glLineWidth(1.0);
}


void DrawAgentHead(unsigned int agent_num , float x , float y , float z , float heading , float pitch , float roll)
{
   glPushMatrix();
      glTranslated(x,y,z);
        glRotated(roll,0.0,0.0,1.0);
        glRotated(heading,0.0,1.0,0.0);
        glRotated(pitch,1.0,0.0,0.0);

      //22x6 ypsos 16
      glColorRGB(223,223,223);
      glBegin(GL_QUADS);
        //LEFT SIDE
        glVertex3f(-11,-8,3);
        glVertex3f(-11,8,3);
        glVertex3f(-11,8,-3);
        glVertex3f(-11,-8,-3);

        //RIGHT SIDE
        glVertex3f(11,-8,3);
        glVertex3f(11,8,3);
        glVertex3f(11,8,-3);
        glVertex3f(11,-8,-3);

        //FRONT SIDE
        glVertex3f(-11,-8,-3);
        glVertex3f(-11,8,-3);
        glVertex3f(11,8,-3);
        glVertex3f(11,-8,-3);


        //BACK SIDE
        glVertex3f(-11,-8,3);
        glVertex3f(-11,8,3);
        glVertex3f(11,8,3);
        glVertex3f(11,-8,3);


        //UP SIDE
        glVertex3f(-11,8,-3);
        glVertex3f(11,8,-3);
        glVertex3f(11,8,3);
        glVertex3f(-11,8,3);
      glEnd();

     glColorRGB(0,255,7);
     glutSolidCube(5);
     glTranslated(-x,-y,-z);
   glPopMatrix();
}

void DrawTire(float x,float y,float z, float heading , float pitch , float roll)
{
   glPushMatrix();
      glTranslated(x,y,z);
        glRotated(roll,0.0,0.0,1.0);
        glRotated(heading,0.0,1.0,0.0);
        glRotated(pitch,1.0,0.0,0.0);

float x2=0.0;
float y2=0.0;
float z2=0.0;

glBegin(GL_POLYGON);
glColor3f( 0.0, 0.0, 0.0 );
glVertex3f(x2,y2+0.5,z2+2.5);
glVertex3f(x2,y2+0.7,z2+2.45);
glVertex3f(x2,y2+0.85,z2+2.35);
glVertex3f(x2,y2+0.95,z2+2.2);
glVertex3f(x2,y2+1,z2+2);
glVertex3f(x2,y2+0.95,z2+1.8);
glVertex3f(x2,y2+0.85,z2+1.65);
glVertex3f(x2,y2+0.7,z2+1.55);
glVertex3f(x2,y2+0.5,z2+1.50);
glVertex3f(x2,y2+0.32,z2+1.55);
glVertex3f(x2,y2+0.15,z2+1.65);
glVertex3f(x2,y2+0.05,z2+1.8);
glVertex3f(x2,y2+0.04,z2+2);
glVertex3f(x2,y2+0.05,z2+2.2);
glVertex3f(x2,y2+0.15,z2+2.33);
glVertex3f(x2,y2+0.32,z2+2.45);
glVertex3f(x2,y2+0.5,z2+2.5);
glEnd();
     glTranslated(-x,-y,-z);
   glPopMatrix();
}

void DrawAgent(unsigned int agent_num , float x , float y , float z , float heading , float pitch , float roll)
{
   glPushMatrix();
      glTranslated(x,y,z);
        glRotated(roll,0.0,0.0,1.0);
        glRotated(heading,0.0,1.0,0.0);
        glRotated(pitch,1.0,0.0,0.0);

      //30x44 ypsos 38
      glColorRGB(0,255,7);
      glBegin(GL_QUADS);
        //LEFT SIDE
        glVertex3f(-15,0,44);
        glVertex3f(-15,38,44);
        glVertex3f(-15,38,0);
        glVertex3f(-15,0,0);

        //RIGHT SIDE
        glVertex3f(15,0,44);
        glVertex3f(15,38,44);
        glVertex3f(15,38,0);
        glVertex3f(15,0,0);

        //FRONT SIDE
        glVertex3f(-15,0,0);
        glVertex3f(-15,38,0);
        glVertex3f(15,38,0);
        glVertex3f(15,0,0);


        //BACK SIDE
        glVertex3f(-15,0,44);
        glVertex3f(-15,38,44);
        glVertex3f(15,38,44);
        glVertex3f(15,0,44);


        //UP SIDE
        glVertex3f(-15,38,0);
        glVertex3f(15,38,0);
        glVertex3f(15,38,44);
        glVertex3f(-15,38,44);
      glEnd();

      DrawTire(-16,5,0, 0 ,  0, 0 );
      DrawAgentHead(agent_num ,x,y+45,z,0.0,0.0,0.0);//heading,pitch,roll);

     glTranslated(-x,-y,-z);
   glPopMatrix();
}


void DrawEmptyDepthMap(int num,float transx,float transy,float transz,float heading , float pitch , float roll)
{
 glPushMatrix();
    glTranslated(transx,transy,transz);

    glRotated(roll,0.0,0.0,1.0);
    glRotated(heading,0.0,1.0,0.0);
    glRotated(pitch,1.0,0.0,0.0);

               glBegin(GL_QUADS);
                 glColor3f(1.0f, 0.0f, 0.0f);
                 glVertex3f(-160,-120,-85);
                 glVertex3f(160,-120,-85);
                 glVertex3f(160,120,-85);
                 glVertex3f(-160,120,-85);
               glEnd();



    glTranslated(-transx,-transy,-transz);
  glPopMatrix();
}


void DrawDepthMap(int num,float transx,float transy,float transz,float rotx,float roty, float rotz)
{
  int x,y,memplace=0;
  glPushMatrix();
  glLoadIdentity();

  glRotatef( roty, 0.0, 1.0, 0.0 );
    glTranslated(vx,vy,vz);

               glBegin(GL_QUADS);
                 glColor3f(1.0f, 0.0f, 0.0f);
                 glVertex3f(0,0,1);
                 glVertex3f(320,0,1);
                 glVertex3f(320,240,1);
                 glVertex3f(0,240,1);
               glEnd();

      glBegin(GL_QUADS);
      unsigned int scale_factor = 4;
       for (y=0; y<240; y++)
         { for (x=0; x<320; x++)
           {
             glColorRGB(video_color[memplace],video_color[memplace+1],video_color[memplace+2]);

            // ACTUAL VOXEL
            glVertex3f(x-1,-(y-1),scale_factor*video_depth[memplace]);
            glVertex3f(x+1,-(y-1),scale_factor*video_depth[memplace]);
            glVertex3f(x+1,-(y+1),scale_factor*video_depth[memplace]);
            glVertex3f(x-1,-(y+1),scale_factor*video_depth[memplace]);

            // ACTUAL TAIL
            glVertex3f(x-1,-(y-1),0);
            glVertex3f(x-1,-(y+1),0);
            glVertex3f(x+1,-(y-1),scale_factor*video_depth[memplace]);
            glVertex3f(x+1,-(y+1),scale_factor*video_depth[memplace]);

            glVertex3f(x-1,-(y-1),scale_factor*video_depth[memplace]);
            glVertex3f(x+1,-(y-1),scale_factor*video_depth[memplace]);
            glVertex3f(x+1,-(y+1),scale_factor*video_depth[memplace]);
            glVertex3f(x-1,-(y+1),scale_factor*video_depth[memplace]);


            memplace+=3;
           }
         }
      glEnd();
   //  glPopMatrix();


  glPopMatrix();

}

