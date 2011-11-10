#include "models.h"
#include "world_map_adapter.h"
#include "camera.h"
#include "tools.h"

#include <stdlib.h>
#include <stdio.h>

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
      float dimension = 10000;
      glVertex3f( dimension ,-0.01,-dimension );
      glVertex3f( dimension ,-0.01, dimension );
      glVertex3f(-dimension ,-0.01, dimension );
      glVertex3f(-dimension ,-0.01,-dimension );
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





void DrawDepthMapClassic(int num,float transx,float transy,float transz,float heading , float pitch , float roll)
{
  unsigned int x,y,memplace=0;
  float rect_left_x = 0.0;
  float rect_right_x = 0.0;
  float rect_top_y = 0.0;
  float rect_bottom_y = 0.0;
  float depth = 0.0;


  glPushMatrix();

    glTranslated(transx,transy,transz);

    glRotated(roll,0.0,0.0,1.0);
    glRotated(heading,0.0,1.0,0.0);
    glRotated(pitch,1.0,0.0,0.0);


/*
               glBegin(GL_QUADS);
                 glColor3f(1.0f, 0.0f, 0.0f);
                 glVertex3f(-160,-45,-85);
                 glVertex3f(160,-45,-85);
                 glVertex3f(160,120,-85);
                 glVertex3f(-160,120,-85);
               glEnd();

               glBegin(GL_QUADS);
                 glColor3f(1.0f, 0.0f, 0.0f);
                 glVertex3f(-160,-45,-250);
                 glVertex3f(160,-45,-250);
                 glVertex3f(160,120,-250);
                 glVertex3f(-160,120,-250);
               glEnd();*/

      glBegin(GL_QUADS);
      float scale_start = -255;
      float scale_factor = 3; //-1;



       memplace=0;
       for (y=0; y<240; y++)
         { for (x=0; x<320; x++)
           {
             glColorRGB(video_color[memplace],video_color[memplace+1],video_color[memplace+2]);


             rect_left_x = x;
             rect_left_x -= 161;

             rect_right_x = rect_left_x + 1;

             rect_top_y = y;
             rect_top_y = (-1) * rect_top_y;
             rect_top_y += 45+120+28;

             rect_bottom_y = rect_top_y+1;


             depth  = (float) video_depth[memplace];
             depth  *= (float) scale_factor;
             depth  += (float ) scale_start;

             //fprintf(stderr," Point %fx%f -> %fx%f = %f \n",rect_left_x,rect_top_y,rect_right_x,rect_bottom_y    ,depth);

            // ACTUAL VOXEL
             glVertex3f(rect_left_x  ,  rect_top_y  , depth);
             glVertex3f(rect_right_x ,  rect_top_y  , depth);
             glVertex3f(rect_right_x ,rect_bottom_y , depth);
             glVertex3f(rect_left_x  ,rect_bottom_y , depth);

            // ACTUAL TAIL

             glVertex3f(rect_left_x  ,  rect_top_y  , depth-30);
             glVertex3f(rect_left_x  ,rect_bottom_y , depth-30);
             glVertex3f(rect_right_x ,  rect_top_y  , depth);
             glVertex3f(rect_right_x ,rect_bottom_y , depth);

            glVertex3f(rect_left_x  ,  rect_top_y  ,  depth);
            glVertex3f(rect_right_x ,  rect_top_y  ,  depth);
            glVertex3f(rect_right_x ,rect_bottom_y ,  depth);
            glVertex3f(rect_left_x  ,rect_bottom_y ,  depth);


            memplace+=3;
           }
         }
      glEnd();

    glTranslated(-transx,-transy,-transz);

  glPopMatrix();

}






void DrawDepthMap(int num,float transx,float transy,float transz,float heading , float pitch , float roll)
{
  return DrawDepthMapClassic(num,transx,transy,transz,heading,pitch,roll) ;
  //DEBUG SWITCH

  unsigned int x,y,memplace=0;
  float rect_left_x = 0.0;
  float rect_right_x = 0.0;
  float rect_top_y = 0.0;
  float rect_bottom_y = 0.0;
  float depth = 0.0;


  glPushMatrix();

    glTranslated(transx,transy,transz);

    glRotated(roll,0.0,0.0,1.0);
    glRotated(heading,0.0,1.0,0.0);
    glRotated(pitch,1.0,0.0,0.0);


      glBegin(GL_QUADS);
      float scale_start = -255;
      float scale_factor = 3; //-1;



       memplace=0;
       unsigned int point_iterator = 0;
       while (point_iterator<current_points)
       {

             x=points[current_points].X;
             y=points[current_points].Y;
             depth=points[current_points].Z;
             glColorRGB(points[current_points].R,points[current_points].G,points[current_points].B);


             rect_left_x = x;
             rect_left_x -= 161;

             rect_right_x = rect_left_x + 1;

             rect_top_y = y;
             rect_top_y = (-1) * rect_top_y;
             rect_top_y += 45+120+28;

             rect_bottom_y = rect_top_y+1;


             depth  = (float) video_depth[memplace];
             depth  *= (float) scale_factor;
             depth  += (float ) scale_start;

             //fprintf(stderr," Point %fx%f -> %fx%f = %f \n",rect_left_x,rect_top_y,rect_right_x,rect_bottom_y    ,depth);

            // ACTUAL VOXEL
             glVertex3f(rect_left_x  ,  rect_top_y  , depth);
             glVertex3f(rect_right_x ,  rect_top_y  , depth);
             glVertex3f(rect_right_x ,rect_bottom_y , depth);
             glVertex3f(rect_left_x  ,rect_bottom_y , depth);

            // ACTUAL TAIL

             glVertex3f(rect_left_x  ,  rect_top_y  , depth-30);
             glVertex3f(rect_left_x  ,rect_bottom_y , depth-30);
             glVertex3f(rect_right_x ,  rect_top_y  , depth);
             glVertex3f(rect_right_x ,rect_bottom_y , depth);

            glVertex3f(rect_left_x  ,  rect_top_y  ,  depth);
            glVertex3f(rect_right_x ,  rect_top_y  ,  depth);
            glVertex3f(rect_right_x ,rect_bottom_y ,  depth);
            glVertex3f(rect_left_x  ,rect_bottom_y ,  depth);


           ++point_iterator;
       }

       for (y=0; y<240; y++)
         { for (x=0; x<320; x++)
           {


            memplace+=3;
           }
         }
      glEnd();

    glTranslated(-transx,-transy,-transz);

  glPopMatrix();

}

























