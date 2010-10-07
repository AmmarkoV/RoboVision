#include <stdlib.h>
#include <stdio.h>

unsigned char *yuv420p_to_rgb24(int width, int height,
				unsigned char *pIn0, unsigned char *pOut0=NULL);
unsigned char *yuv420_to_rgb24(int width, int height,
			       unsigned char *pIn0, unsigned char *pOut0=NULL);
unsigned char *yuv411p_to_rgb24(int width, int height,
				unsigned char *pIn0, unsigned char *pOut0=NULL);

//unsigned char *YUV420_to_RGB24(unsigned char *b1,int width,int height,unsigned char *b2=NULL);
