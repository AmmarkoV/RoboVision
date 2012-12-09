#include <stdio.h>


int WIDTH = 640;
int HEIGHT = 480;
int THREADS_PER_BLOCK = 256;
#define REPEAT_TIMES 1

unsigned int inputByteSize;
unsigned int outputByteSize;

char * device_inputArray;
char * device_outputArray;

struct Image
{
  char * pixels;
  unsigned int size_x;
  unsigned int size_y;
  unsigned int depth;
  unsigned int image_size;
};

#define PPMREADBUFLEN 256

int ReadPPM(char * filename,struct Image * pic)
{
    FILE *pf=0;
    pf = fopen(filename,"rb");

    if (pf!=0 )
    {
        char buf[PPMREADBUFLEN], *t;
        unsigned int w=0, h=0, d=0;
        int r=0;

        t = fgets(buf, PPMREADBUFLEN, pf);
        if ( (t == 0) || ( strncmp(buf, "P6\n", 3) != 0 ) ) { fclose(pf); return 0; }
        do
        { /* Px formats can have # comments after first line */
           t = fgets(buf, PPMREADBUFLEN, pf);
           if ( t == 0 ) { fclose(pf); return 0; }
        } while ( strncmp(buf, "#", 1) == 0 );
        r = sscanf(buf, "%u %u", &w, &h);
        if ( r < 2 ) { fclose(pf); return 0; }
        // The program fails if the first byte of the image is equal to 32. because
        // the fscanf eats the space and the image is read with some bit less
        r = fscanf(pf, "%u\n", &d);
        if ( (r < 1) || ( d != 255 ) ) { fclose(pf); return 0; }

        if ( (w!=pic->size_x) || (h!=pic->size_y) )
           {
             fprintf(stderr,"Incorrect file size ( %s ) :P\n",filename);
             if ( w * h > pic->size_x * pic->size_y )
               {
                 fprintf(stderr,"File %s will lead to overflow stopping read..\n",filename);
                 fclose(pf);
                 return 0;
               }
           }

        if ( pic->pixels != 0 )
        {
            size_t rd = fread(pic->pixels,3, w*h, pf);
            fclose(pf);
            if ( rd < w*h )
            {
               return 0;
            }
            return 1;
        }
        fclose(pf);
    }
  return 0;
}

int WritePPM(char * filename,struct Image * pic)
{

    FILE *fd=0;
    fd = fopen(filename,"wb");

    if (fd!=0)
	{
     unsigned int n=0;

     fprintf(fd, "P6\n%d %d\n255\n", pic->size_x, pic->size_y);
     n = (unsigned int ) ( pic->size_x * pic->size_y ) ;

     fwrite(pic->pixels, 3, n, fd);

     fflush(fd);
     fclose(fd);

     return 1;
	}

  return 0;
}

int InitializeCUDAConvolution(int height, int width, int threads)
{
	WIDTH = width;
	HEIGHT = height;
	THREADS_PER_BLOCK = threads;

	unsigned int inputByteSize = WIDTH * HEIGHT * sizeof(char);
	cudaMalloc((void**) &device_inputArray, inputByteSize);

	unsigned int outputByteSize = WIDTH * HEIGHT * sizeof(char);
	cudaMalloc((void**) &device_outputArray, outputByteSize);

	return 1;
}

int closeCUDAConvolution()
{
	cudaFree(device_inputArray);
	cudaFree(device_outputArray);

	return 1;
}

int main()
{
 struct Image input={0};
 if (!ReadPPM("cudatest.ppm",&input)) { fprintf(stderr,"Could not read input image..\n"); return 1; }

 InitializeCUDAConvolution(input.size_x,input.size_y,64);

  //TODO :P

    closeCUDAConvolution();

 if (!WritePPM("cudaproc.ppm",&input)) { fprintf(stderr,"Could not write output image..\n"); return 1; }
 return 0;
}
