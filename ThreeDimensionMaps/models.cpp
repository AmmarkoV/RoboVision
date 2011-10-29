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
      glLineWidth(15.0);
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


void DrawAgent(unsigned int agent_num , float x , float y , float z , float heading , float pitch , float roll)
{
   glPushMatrix();
      glTranslated(x,y,z);
        glRotated(roll,0.0,0.0,1.0);
        glRotated(heading,0.0,1.0,0.0);
        glRotated(pitch,1.0,0.0,0.0);



     glColorRGB(255,7,7);
     glutSolidCube(15);
     glTranslated(-x,-y,-z);
   glPopMatrix();
}





void DrawDepthMap(int num,float transx,float transy,float transz,float rotx,float roty, float rotz)
{
  int x,y,memplace=0;
  glPushMatrix();
  glLoadIdentity();


  glRotatef( roty, 0.0, 1.0, 0.0 );
    glTranslated(vx,vy,vz);



    DrawAxis();

               glBegin(GL_QUADS);
                 glColor3f(1.0f, 0.0f, 0.0f);
                 glVertex3f(0,0,1);
                 glVertex3f(320,0,1);
                 glVertex3f(320,240,1);
                 glVertex3f(0,240,1);
               glEnd();


   // glPushMatrix();
    // glLoadIdentity();
      //glRotatef( roty, 0.0, 1.0, 0.0 );
     // glRotatef( rot, 0.0, 1.0, 0.0 );
     // glTranslated(vx,vy,vz);

    // glMultMatrixf(left_translation);

     // glMultMatrixf(left_rotation);

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

