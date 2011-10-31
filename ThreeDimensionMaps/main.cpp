/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "models.h"
#include "world_map_adapter.h"
#include "camera.h"
#include "tools.h"

float robot_heading=0.0;

/* GLUT callback Handlers */
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 5800.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}









static void display(void)
{
  const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

    if ( (int) t % 3 == 0 ) {
                              if ( last_load != t )
                                {
                                  //robot_heading+=0.1;
                                  //LoadDepth(0);
                                  last_load = t;
                                }
                            }

  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(0,0,0);


    glRotatef(angle_x,-1.0,0,0); // Peristrofi gyrw apo ton x
    glRotatef(angle_y,0,-1.0,0); // Peristrofi gyrw apo ton y
    glRotatef(angle_z,0,0,-1.0);



   glTranslatef(-vx, -vy, -vz);

/*

    //rot+=0.1;


      int i=0,angle=0;
      //for (i=0; i<8; i++)
       {
         LoadDepth(i);
        LoadMatrix4x4((char*)"memfs/TOTAL_LEFT_ROTATION",0,left_rotation);
        LoadMatrix4x4((char*)"memfs/TOTAL_RIGHT_ROTATION",0,right_rotation);

       //  LoadMatrix4x4((char*)"memfs/LEFT_ROTATION",0,left_rotation);
       //  LoadMatrix4x4((char*)"memfs/RIGHT_ROTATION",0,right_rotation);

        // LoadMatrix4x4((char*)"memfs/LEFT_TRANSLATION",0,left_translation);
        // LoadMatrix4x4((char*)"memfs/RIGHT_TRANSLATION",0,right_translation);

         DrawDepthMap(0,0,0,0,0,angle,0);
         angle+=20;
       }
*/
    DrawAxis();
    DrawFloor();
    DrawAgent(0,  0.0 , 0.0  , 0.0  , robot_heading  , 0.0  , 0.0  );

    DrawDepthMap(0,  0.0 ,0.0 , 0.0 , robot_heading , 0.0 , 0.0 );
    //DrawDepthMapOLD(0,  0.0 ,0.0 , 0.0 , robot_heading , 0.0 , 0.0 );
    //DrawEmptyDepthMap(0, 0.0 , 0.0 , 0.0 , robot_heading , 0.0 , 0.0 );

 glPopMatrix();


 //   gluLookAt(vx,vy,vz,  vx,vy-20,vz,   0,1,0);


    glutSwapBuffers();
}


void PutPixel(unsigned int x, unsigned int y,unsigned int depth)
{
  unsigned int memplace = y*320 + x;
  video_depth[memplace]=depth;
 return;
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'f': vz+=10; break;
        case 'r': vz-=10; break;
        case 'd': vx+=10; break;
        case 'a': vx-=10; break;
        case 'w': vy+=10; break;
        case 's': vy-=10; break;
        case 'c': angle_y-=0.5; break;
        case 'z': angle_y+=0.5; break;

        case 'l': LoadDepth(0); break;
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
    }

    //fprintf(stderr,"X:%f Y:%f Z:%f \n",vx,vy,vz);

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main(int argc, char *argv[])
{

    fprintf(stderr,"Running from %s ",argv[0]);
    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GuarD DoG 3d View");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(0,0,0,1);
    //glEnable(GL_CULL_FACE);
   // glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


    LoadDepth(0);

    glutMainLoop();

    return EXIT_SUCCESS;
}
