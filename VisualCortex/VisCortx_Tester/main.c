#include <stdio.h>
#include <stdlib.h>
#include "../VisualCortex.h"
#include <time.h>
unsigned char vid[320*240*3]={0};

void FillPATCHColor(unsigned int px, unsigned int py , unsigned int patch_x,unsigned int patch_y,unsigned char color)
{
        int x=px,y=py,i=0;
        for (y=py; y<py+patch_y; y++)
        {
         i = y * 320 * 3 + ( px*3 );
         for (x=px; x<px+patch_x; x++)
         {
           vid[i++]=color;
           vid[i++]=color;
           vid[i++]=color;
         }
        }
}

void FillRandom()
{
    srand ( time(NULL) );


        int i=0;
        for (i=0; i<320*240*3; i++)
        {
         vid[i]=rand() % 256;
        }
}

int main()
{
    printf("Visual Cortex %s !\n",VisCortx_Version());

    VisCortx_Start(320,240);

         FillRandom();
        //FillPATCHColor(0,0,30,30,255);
        //FillPATCHColor(0,30,30,30,212);
        //FillPATCHColor(30,0,30,30,123);

        VisCortx_WriteToVideoRegister(LEFT_EYE,320,240,3,(unsigned char * ) &vid);
        VisCortx_SelfCheck();

    VisCortx_Stop();
    return 0;
}
