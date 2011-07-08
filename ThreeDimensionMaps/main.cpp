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

#include <stdlib.h>

static int slices = 16;
static int stacks = 16;
unsigned char video_color[640*480*3]={0};
unsigned char video_depth[640*480*3]={0};
float vx=-160.0,vy=120.0,vz=-270.0,rot=0.0;
double last_load;
/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 800.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}


void LoadDepth(int snap)
{
   char filename[60]={0};
   sprintf(filename,"DEPTH%u",snap);

   FILE * fp;
   fp = fopen(filename,"rb");
   if (fp == 0) return;

   unsigned int ptr=0, ptrlim = 320*240*3;
   fread (video_depth , 1 , ptrlim , fp );
   fclose(fp);

   sprintf(filename,"COLOR%u",snap);
   fp = fopen(filename,"rb");
   if (fp == 0) return;

   //ptr=0, ptrlim = 320*240*3;
   fread (video_color , 1 , ptrlim , fp );
   fclose(fp);
}

void glColorRGB(unsigned char R,unsigned char G,unsigned char B)
{
  glColor3f((float) R/255,(float) G/255,(float) B/255);
}



void DrawDepthMap(int num,float transx,float transy,float transz,float rotx,float roty, float rotz)
{
  int x,y,memplace=0;
  glPushMatrix();
  glLoadIdentity();


           glRotatef( roty, 0.0, 1.0, 0.0 );
           glRotatef( rot, 0.0, 1.0, 0.0 );
           glTranslated(vx,vy,vz);
           glBegin(GL_QUADS);
            for (y=0; y<240; y++)
         { for (x=0; x<320; x++)
           {
            //if ( video_depth[memplace]==0 ) glColor3f(0.0f, 0.0f, 0.0f); else
                                              glColorRGB(video_color[memplace],video_color[memplace+1],video_color[memplace+2]);

            // ACTUAL VOXEL
            glVertex3f(x-1,-(y-1),2*video_depth[memplace]);
            glVertex3f(x+1,-(y-1),2*video_depth[memplace]);
            glVertex3f(x+1,-(y+1),2*video_depth[memplace]);
            glVertex3f(x-1,-(y+1),2*video_depth[memplace]);

            // ACTUAL TAIL
            glVertex3f(x-1,-(y-1),0);
            glVertex3f(x-1,-(y+1),0);
            glVertex3f(x+1,-(y-1),2*video_depth[memplace]);
            glVertex3f(x+1,-(y+1),2*video_depth[memplace]);

            glVertex3f(x-1,-(y-1),2*video_depth[memplace]);
            glVertex3f(x+1,-(y-1),2*video_depth[memplace]);
            glVertex3f(x+1,-(y+1),2*video_depth[memplace]);
            glVertex3f(x-1,-(y+1),2*video_depth[memplace]);


            memplace+=3;
           }
         }
           glEnd();
          glPopMatrix();

}






static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;


    if ( (int) t % 3 == 0 ) { if ( last_load != t )
                                {
                                  LoadDepth(0);
                                  last_load = t;
                                }
                            }
    //rot+=0.1;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);

      int i=0,angle=0;
      //for (i=0; i<8; i++)
       {
         LoadDepth(i);
         DrawDepthMap(0,0,0,0,0,angle,0);
         angle+=20;
       }


    gluLookAt(vx,vy,vz,  vx,vy-20,vz,   0,1,0);
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
        case 'r': vz+=10; break;
        case 'f': vz-=10; break;
        case 'a': vx+=10; break;
        case 'd': vx-=10; break;
        case 's': vy+=10; break;
        case 'w': vy-=10; break;
        case 'z': rot-=0.5; break;
        case 'c': rot+=0.5; break;

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
    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GuarD DoG 3d View");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
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

    glutMainLoop();

    return EXIT_SUCCESS;
}
