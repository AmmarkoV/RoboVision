#include <stdio.h>
#include <stdlib.h>
#include "../VisualCortex.h"
#include <time.h>
#include <string.h>

#define PPMREADBUFLEN 256
 
unsigned int resolution_height , resolution_width ;

char out_filename[256]={0};

char filename0[256]={0};
unsigned char vid0[320*240*3]={0};

char filename1[256]={0};
unsigned char vid1[320*240*3]={0};

 
int LoadRegisterFromFile(char * filename,unsigned int size_x,unsigned int size_y,unsigned char * pixels)
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

        if ( (w!=size_x) || (h!=size_y) )
           {
             fprintf(stderr,"Incorrect file size ( %s ) :P\n",filename);
             if ( w * h > size_x * size_y )
               {
                 fprintf(stderr,"File %s will lead to overflow stopping read..\n",filename);
                 fclose(pf);
                 return 0;
               }
           }

        if ( pixels != 0 )
        {
            size_t rd = fread(pixels,3, w*h, pf);
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


int main(int argc, const char* argv[])
{
    printf("Visual Cortex %s !\n",VisCortx_Version());


   if ( argc > 5 )
     {
       resolution_width = atoi(argv[1]);
       resolution_height = atoi(argv[2]);
       strcpy(filename0,argv[3]);
       strcpy(filename1,argv[4]);
       strcpy(out_filename,argv[5]); 
     } else
     {
        fprintf(stderr,"Usage : stereo_disparity_my_tester RESOLUTIONX RESOLUTIONY left_image.ppm right_image.ppm output.ppm\n");
        fprintf(stderr,"ie : stereo_disparity_my_tester 320 240 image0.ppm image1.ppm depth.ppm\n");
        return 1;
     }
     
    if ( (resolution_width!=320) || (resolution_height!=240) ) { fprintf(stderr,"Visual Cortex only works for 320x240 images\n"); return 1; }
    
    VisCortx_Start(resolution_width,resolution_height); 
    
    //DISABLE STUFF THAT ARE NOT NEEDED FOR DISPARITY MAPPING..
    VisCortx_SetSetting(PASS_TO_FACE_DETECTOR,0);
    VisCortx_SetSetting(PASS_TO_FEATURE_DETECTOR,0);
    VisCortx_SetSetting(USE_OPENCV,0);
    VisCortx_SetSetting(CALCULATE_MOVEMENT_FLOW,0);  
    
    VisCortx_SetTime(10);
  
    if (!LoadRegisterFromFile(filename0,resolution_width,resolution_height,(unsigned char * ) &vid0)) { fprintf(stderr,"Could not load image %s ",filename0); }
    VisCortX_NewFrame(LEFT_EYE,resolution_width,resolution_height,3,(unsigned char * ) &vid0); 

    if (!LoadRegisterFromFile(filename1,resolution_width,resolution_height,(unsigned char * ) &vid1)) { fprintf(stderr,"Could not load image %s ",filename1); }
    VisCortX_NewFrame(RIGHT_EYE,resolution_width,resolution_height,3,(unsigned char * ) &vid1);
    
    VisCortx_FullDepthMap(0);
 
    ExecutePipeline();

    VisCortX_SaveVideoRegisterToFile(DEPTH_LEFT_VIDEO,out_filename);
    
    VisCortx_Stop();
    return 0;
}
