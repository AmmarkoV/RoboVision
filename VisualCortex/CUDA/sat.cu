
//CUDA Kernel..
__global__  void addUpRow(char * pixel,unsigned int * SAT)
       {
          // threadIdx.x is a built-in variable  provided by CUDA at runtime
          int row = threadIdx.x;

          pixel += row * 320;

          int pixels_remaining = 319;
          unsigned int  * next_SAT = SAT + 1;
          while (pixels_remaining!=0)
           {
             *next_SAT = *SAT + *pixel;
             ++pixel;
             ++next_SAT;
             ++SAT;
             --pixels_remaining;
           }
       }

__global__  void addUpColumn(unsigned int * SAT)
       {
          // threadIdx.x is a built-in variable  provided by CUDA at runtime
          int column = threadIdx.x;

          SAT += column;

          int pixels_remaining = 239;
          unsigned int  * next_SAT = SAT + 320;
          while (pixels_remaining!=0)
           {
             *next_SAT += *SAT ;
             next_SAT+=320;
             SAT+=320;
             --pixels_remaining;
           }
       }

#include  <stdio.h>

#define PPMREADBUFLEN 256

struct Image
{
  char * pixels;
  unsigned int size_x;
  unsigned int size_y;
  unsigned int depth;
  unsigned int image_size;
};

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



int  main()
{
 fprintf(stderr,"CUDA Enabled Summed Area Table implementation..\n");
 struct Image input_img={0};
 input_img.size_x=320;
 input_img.size_y=240;
 ReadPPM("cudatest.ppm",&input_img);

 char * pixels;
 int pixels_size= input_img.size_x * input_img.size_y * 3 * sizeof(char);

 unsigned int * SAT;
 int SAT_size= input_img.size_x * input_img.size_y * 3 * sizeof(unsigned int);

 unsigned int SAT_Local[320*240];


 cudaMalloc((void**)&pixels, pixels_size);
 cudaMalloc((void**)& SAT, SAT_size);

 cudaMemcpy(pixels, input_img.pixels , pixels_size,  cudaMemcpyHostToDevice);
 //CUDA
 addUpRow<<<1,320>>>(pixels, SAT);
 addUpColumn<<<1,240>>>(SAT);

 cudaMemcpy(SAT_Local, &SAT, SAT_size,  cudaMemcpyDeviceToHost);

 cudaFree(pixels);
 cudaFree(SAT);
}
