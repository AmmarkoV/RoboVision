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
unsigned char * vid0=0;

char filename1[256]={0};
unsigned char * vid1=0;


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


int DisparityMappingSettingsBenchmark()
{
    /*
    VisCortx_SetSetting(DEPTHMAP_COMPARISON_THRESHOLD,4000);
    VisCortx_SetSetting(DEPTHMAP_COMPARISON_THRESHOLD_LARGE_PATCH,4000);
    VisCortx_SetSetting(DEPTHMAP_COMPARISON_THRESHOLD_EXTRALARGE_PATCH,4000);


    VisCortx_SetSetting(PATCH_COMPARISON_SCORE_MIN,33000);
    VisCortx_SetSetting(PATCH_COMPARISON_EDGES_PERCENT_REQUIRED,3);
    VisCortx_SetSetting(PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_LARGE_PATCH,5);
    VisCortx_SetSetting(PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_EXTRALARGE_PATCH,8);
    VisCortx_SetSetting(PATCH_HIST_THRESHOLD_R,7);
    VisCortx_SetSetting(PATCH_HIST_THRESHOLD_G,7);
    VisCortx_SetSetting(PATCH_HIST_THRESHOLD_B,7);


    VisCortx_SetMetric(HORIZONTAL_BUFFER,9);
    VisCortx_SetMetric(VERTICAL_BUFFER,9);
    VisCortx_SetMetric(HORIZONTAL_BUFFER_LARGE,15);
    VisCortx_SetMetric(VERTICAL_BUFFER_LARGE,15);
    VisCortx_SetMetric(HORIZONTAL_BUFFER_EXTRALARGE,21);
    VisCortx_SetMetric(VERTICAL_BUFFER_EXTRALARGE,21);*/

    VisCortx_FullDepthMap(0);

    ExecutePipeline();

    VisCortX_SaveVideoRegisterToFile(DEPTH_LEFT_VIDEO,out_filename);

    unsigned int delay  = VisCortx_GetMetric(DEPTHMAP_DELAY_MICROSECONDS);
    unsigned int coverage  = VisCortx_GetMetric(LAST_DEPTH_MAP_COVERAGE);
    unsigned int too_close  = VisCortx_GetMetric(LAST_DEPTH_MAP_TOO_CLOSE_COVERAGE);

    unsigned int wndx=VisCortx_GetMetric(HORIZONTAL_BUFFER);
    unsigned int wndy=VisCortx_GetMetric(VERTICAL_BUFFER);
    unsigned int wndx_l=VisCortx_GetMetric(HORIZONTAL_BUFFER_LARGE);
    unsigned int wndy_l=VisCortx_GetMetric(VERTICAL_BUFFER_LARGE);
    unsigned int wndx_xl=VisCortx_GetMetric(HORIZONTAL_BUFFER_EXTRALARGE);
    unsigned int wndy_xl=VisCortx_GetMetric(VERTICAL_BUFFER_EXTRALARGE);



    unsigned int best_hist_thres=7; unsigned int hist_thres=7;

    unsigned int best_edges_percent=0; unsigned int edges_percent=0;

    unsigned int best_window_step=0; unsigned int window_step=0;

    unsigned int best_detail=1; unsigned int detail=1;

    unsigned int best_generalization=1; unsigned int generalization=1;



for (generalization=6; generalization<12; generalization++)
{
for (hist_thres=6; hist_thres<15; hist_thres++)
 {
for (edges_percent=1; edges_percent<10; edges_percent++)
{
 for (detail=1; detail<10; detail++)
   {
    for (window_step=1; window_step<10; window_step++)
     {
        VisCortx_SetMetric(HORIZONTAL_BUFFER,wndx+window_step);                 VisCortx_SetMetric(VERTICAL_BUFFER,wndy+window_step);
        VisCortx_SetMetric(HORIZONTAL_BUFFER_LARGE,wndx_l+2*window_step);       VisCortx_SetMetric(VERTICAL_BUFFER_LARGE,wndy_l+2*window_step);
        VisCortx_SetMetric(HORIZONTAL_BUFFER_EXTRALARGE,wndx_xl+3*window_step); VisCortx_SetMetric(VERTICAL_BUFFER_EXTRALARGE,wndy_xl+3*window_step);

        VisCortx_SetSetting(DEPTHMAP_DETAIL,detail);
        VisCortx_SetSetting(PATCH_COMPARISON_EDGES_PERCENT_REQUIRED,edges_percent);
        VisCortx_SetSetting(PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_LARGE_PATCH,edges_percent*2);
        VisCortx_SetSetting(PATCH_COMPARISON_EDGES_PERCENT_REQUIRED_EXTRALARGE_PATCH,edges_percent*3);


        VisCortx_SetSetting(DEPTHMAP_COMPARISON_DECIDES_FOR_MORE_PIXELS_RIGHT,generalization);
        VisCortx_SetSetting(DEPTHMAP_COMPARISON_DECIDES_FOR_MORE_PIXELS_DOWN,generalization*2);


        VisCortx_SetSetting(PATCH_HIST_THRESHOLD_R,hist_thres);     VisCortx_SetSetting(PATCH_HIST_THRESHOLD_R,hist_thres);     VisCortx_SetSetting(PATCH_HIST_THRESHOLD_R,hist_thres);

        VisCortx_FullDepthMap(0);

        ExecutePipeline();


        if (
                 ( delay <= VisCortx_GetMetric(DEPTHMAP_DELAY_MICROSECONDS) )
              && ( coverage <= VisCortx_GetMetric(LAST_DEPTH_MAP_COVERAGE) )
              //&& ( too_close <= VisCortx_GetMetric(LAST_DEPTH_MAP_TOO_CLOSE_COVERAGE) )
            )
            {
                VisCortX_SaveVideoRegisterToFile(DEPTH_LEFT_VIDEO,out_filename);

                delay  = VisCortx_GetMetric(DEPTHMAP_DELAY_MICROSECONDS);
                coverage  = VisCortx_GetMetric(LAST_DEPTH_MAP_COVERAGE);
                too_close  = VisCortx_GetMetric(LAST_DEPTH_MAP_TOO_CLOSE_COVERAGE);

                best_generalization = generalization;
                best_hist_thres = hist_thres;
                best_detail=detail;
                best_window_step=window_step;
            }
     }
   }
}
 }
}

  VisCortx_GetHyperVisorStatus(1,0,0);


  fprintf(stderr,"Best Generalization = %u \n",best_generalization);
  fprintf(stderr,"Best Histogram Threshold = %u \n",best_hist_thres);
  fprintf(stderr,"Best Edges Percent = %u \n",edges_percent);
  fprintf(stderr,"Best Detail = %u \n",best_detail);
  fprintf(stderr,"Best Window Step = %u \n",best_window_step);
  fprintf(stderr,"Best output delay %u ms , coverage %u %% , too close %u %%\n",delay,coverage,too_close);



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
    VisCortx_SetSetting(DEPTHMAP_USE_OPENCV,0);
    VisCortx_SetSetting(USE_OPENCV,0);
    VisCortx_SetSetting(DEPTHMAP_OPENCV_LIKE_OUTPUT,1);
    VisCortx_SetSetting(CALCULATE_MOVEMENT_FLOW,0);

    VisCortx_SetTime(10);

    VisCortX_NewFrame(LEFT_EYE,resolution_width,resolution_height,3,(unsigned char * ) vid0);

    VisCortX_NewFrame(RIGHT_EYE,resolution_width,resolution_height,3,(unsigned char * ) vid1);

    //Frames Are loaded and ready for our processing ..

    DisparityMappingSettingsBenchmark();

    //Our processing is done ....

    VisCortx_Stop();

    free(vid0);
    free(vid1);
    return 0;
}
