#include <stdio.h>
#include <stdlib.h>
#include "../VisualCortex.h"
#include <time.h>
#include <string.h>

#define PPMREADBUFLEN 512
#define FORGIVEUNREADBYTES 2

unsigned int resolution_height=320 , resolution_width=240 ;

char out_filename[256]={0};

char filename0[256]={0};
unsigned char * vid0=0; //[320*240*3]={0};

char filename1[256]={0};
unsigned char * vid1=0; //[320*240*3]={0};


char * LoadRegisterFromFileInternal(char * filename,unsigned int * width,unsigned int * height)
{
    char * new_reg = 0;
    FILE *pf=0;
    pf = fopen(filename,"rb");

    if (pf!=0 )
    {
        char buf[PPMREADBUFLEN], *t;
        unsigned int w=0, h=0, d=0;
        int r=0;

        t = fgets(buf, PPMREADBUFLEN, pf);
        if ( (t == 0) || ( strncmp(buf, "P6\n", 3) != 0 ) ) { fclose(pf); fprintf(stderr,"Could not read first chars\n"); return 0; }
        do
        { /* Px formats can have # comments after first line */
           t = fgets(buf, PPMREADBUFLEN, pf);
           if ( t == 0 ) { fclose(pf); fprintf(stderr,"Could not read while discarding comments\n"); return 0; }
        } while ( strncmp(buf, "#", 1) == 0 );
        r = sscanf(buf, "%u %u", &w, &h);
        if ( r < 2 ) { fclose(pf); fprintf(stderr,"Could not read size dimensions\n"); return 0; }
        // The program fails if the first byte of the image is equal to 32. because
        // the fscanf eats the space and the image is read with some bit less
        r = fscanf(pf, "%u\n", &d);
        if ( (r < 1) || ( d != 255 ) ) { fclose(pf); fprintf(stderr,"the first byte of the image is equal to 32\n"); return 0; }


        *width=w;
        *height=h;
        new_reg  = (char *) malloc(w*h*3 * sizeof(char));

        if ( new_reg != 0 )
        {
            size_t rd = fread(new_reg,3, w*h, pf);
            fclose(pf);
            if ( rd < (w*h) - FORGIVEUNREADBYTES )
            {
               fprintf(stderr,"Read %u bytes instead of %u (that we expected)\n",rd,w*h);
               free(new_reg);
               return 0;
            }
            return new_reg;
        } else
        {
          fprintf(stderr,"Could not allocate enough memory for the image (%ux%u) \n",w,h);
        }
      fclose(pf);
    }
  return 0;
}


int main(int argc, const char* argv[])
{
    printf("Visual Cortex %s !\n",VisCortx_Version());


   if ( argc > 3 )
     {
       strcpy(filename0,argv[1]);
       strcpy(filename1,argv[2]);
       strcpy(out_filename,argv[3]);
     } else
     {
        fprintf(stderr,"Usage : VisCortx_Tester RESOLUTIONX RESOLUTIONY left_image.ppm right_image.ppm output.ppm\n");
        fprintf(stderr,"ie : VisCortx_Tester imageLEFT.ppm imageRIGHT.ppm depth.ppm\n");
        return 1;
     }


    vid0 = LoadRegisterFromFileInternal(filename0,&resolution_width,&resolution_height);
    if (vid0==0) { fprintf(stderr,"Could not load image %s ",filename0); return 1; }
    vid1 = LoadRegisterFromFileInternal(filename1,&resolution_width,&resolution_height);
    if (vid1==0) { fprintf(stderr,"Could not load image %s ",filename1); return 1; }

    VisCortx_Start(resolution_width,resolution_height,"./");

    //DISABLE STUFF THAT ARE NOT NEEDED FOR DISPARITY MAPPING..
    VisCortx_SetSetting(PASS_TO_FACE_DETECTOR,0);
    VisCortx_SetSetting(PASS_TO_FEATURE_DETECTOR,0);
    VisCortx_SetSetting(DEPTHMAP_USE_OPENCV,1);
    VisCortx_SetSetting(USE_OPENCV,1);
    VisCortx_SetSetting(DEPTHMAP_OPENCV_LIKE_OUTPUT,1);
    VisCortx_SetSetting(CALCULATE_MOVEMENT_FLOW,0);

    VisCortx_SetTime(10);

    VisCortX_NewFrame(LEFT_EYE,resolution_width,resolution_height,3,(unsigned char * ) vid0);

    VisCortX_NewFrame(RIGHT_EYE,resolution_width,resolution_height,3,(unsigned char * ) vid1);

    VisCortx_FullDepthMap(0);

    ExecutePipeline();

    VisCortX_SaveVideoRegisterToFile(DEPTH_LEFT_VIDEO,out_filename);

    VisCortx_Stop();

    free(vid0);
    free(vid1);
    return 0;
}

