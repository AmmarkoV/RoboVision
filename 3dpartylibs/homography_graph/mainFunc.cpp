#include <stdlib.h>

#include <GL/glut.h>
#include "homography_pose.h"

const float BOX_SIZE = 7.0f; //The length of each side of the cube
homography_pose *homoPose;
void initRendering() {
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_NORMALIZE);
   glEnable(GL_COLOR_MATERIAL);

}
void drawScene() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glTranslatef(0.0f, 0.0f, -100.0f);

   GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

   GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
   GLfloat lightPos[] = {-2 * BOX_SIZE, BOX_SIZE, 4 * BOX_SIZE, 1.0f};
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

   glMultMatrixf(homoPose->tranMat);
   glutSolidTeapot(BOX_SIZE);
   glutPostRedisplay();
   glutSwapBuffers();
}
void handleResize(int w, int h) {
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}
void keyFunc(unsigned char key, int x, int y)
{
}
void idleFunc() {

   homoPose->cvForLoop();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(400, 400);

   glutCreateWindow("cube");
   initRendering();
   homoPose=new homography_pose();
   homoPose->init();
   glutDisplayFunc(drawScene);
   glutIdleFunc(idleFunc);
   glutReshapeFunc(handleResize);
   glutKeyboardFunc(keyFunc);
   glutMainLoop();

   return 0;
}
