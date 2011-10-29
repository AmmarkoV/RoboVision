#ifndef WORLD_MAP_ADAPTER_H_INCLUDED
#define WORLD_MAP_ADAPTER_H_INCLUDED


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

extern int slices;
extern int stacks;
extern unsigned char video_color[640*480*3];
extern unsigned char video_depth[640*480*3];

extern GLfloat left_rotation[16];
extern GLfloat right_rotation[16];


extern GLfloat left_translation[16];
extern GLfloat right_translation[16];

extern double last_load;

void LoadDepth(int snap);
int SaveTransformationMatrixToFile(char * filename,GLfloat * matrix,unsigned int cols,unsigned int rows);
void LoadMatrix4x4(char * filename,int snap,GLfloat * transformation);


#endif // WORLD_MAP_ADAPTER_H_INCLUDED
