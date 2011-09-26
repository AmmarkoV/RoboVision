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

GLfloat left_transformation[16]={0.0};
GLfloat right_transformation[16]={0.0};



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

   unsigned int  ptrlim = 320*240*3;
   fread (video_depth , 1 , ptrlim , fp );
   fclose(fp);

   sprintf(filename,"COLOR%u",snap);
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


void LoadTransformation(int snap)
{
   char filename[60]={0};
   sprintf(filename,"RIGHT_TRANSFORMATION%u",snap);

   FILE * fp;
   fp = fopen(filename,"r");
   if (fp == 0) return;

    fscanf(fp,"%f",&right_transformation[0]);
    fscanf(fp,"%f",&right_transformation[1]);
    fscanf(fp,"%f",&right_transformation[2]);

    right_transformation[3]=0.0;

    fscanf(fp,"%f",&right_transformation[4]);
    fscanf(fp,"%f",&right_transformation[5]);
    fscanf(fp,"%f",&right_transformation[6]);

    right_transformation[7]=0.0;

    fscanf(fp,"%f",&right_transformation[8]);
    fscanf(fp,"%f",&right_transformation[9]);
    fscanf(fp,"%f",&right_transformation[10]);

    right_transformation[11]=0.0;

    right_transformation[12]=0.0;
    right_transformation[13]=0.0;
    right_transformation[14]=0.0;
    right_transformation[15]=1.0;

    fclose(fp);
    //SaveTransformationMatrixToFile((char* )"RIGHT_TRANSFORMATION_OGL0",right_transformation,4,4); This for debugging

   sprintf(filename,"LEFT_TRANSFORMATION%u",snap);
   fp = fopen(filename,"r");
   if (fp == 0) return;

    fscanf(fp,"%f",&left_transformation[0]);
    fscanf(fp,"%f",&left_transformation[1]);
    fscanf(fp,"%f",&left_transformation[2]);

    left_transformation[3]=0.0;

    fscanf(fp,"%f",&left_transformation[4]);
    fscanf(fp,"%f",&left_transformation[5]);
    fscanf(fp,"%f",&left_transformation[6]);

    left_transformation[7]=0.0;

    fscanf(fp,"%f",&left_transformation[8]);
    fscanf(fp,"%f",&left_transformation[9]);
    fscanf(fp,"%f",&left_transformation[10]);

    left_transformation[11]=0.0;

    left_transformation[12]=0.0;
    left_transformation[13]=0.0;
    left_transformation[14]=0.0;
    left_transformation[15]=1.0;

   fclose(fp);
    //SaveTransformationMatrixToFile((char *)"LEFT_TRANSFORMATION_OGL0",left_transformation,4,4); This for debugging

}

void glColorRGB(unsigned char R,unsigned char G,unsigned char B)
{
  glColor3f((float) R/255,(float) G/255,(float) B/255);
}



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

void DrawDepthMap(int num,float transx,float transy,float transz,float rotx,float roty, float rotz)
{
  int x,y,memplace=0;
  glPushMatrix();
  glLoadIdentity();


    glRotatef( roty, 0.0, 1.0, 0.0 );
    glRotatef( rot, 0.0, 1.0, 0.0 );
    glTranslated(vx,vy,vz);



    DrawAxis();

               glBegin(GL_QUADS);
                 glColor3f(1.0f, 0.0f, 0.0f);
                 glVertex3f(0,0,1);
                 glVertex3f(320,0,1);
                 glVertex3f(320,240,1);
                 glVertex3f(0,240,1);
               glEnd();


    glPushMatrix();
     glLoadIdentity();
     glMultMatrixf(left_transformation);
      glBegin(GL_QUADS);
       for (y=0; y<240; y++)
         { for (x=0; x<320; x++)
           {
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
         LoadTransformation(i);
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
