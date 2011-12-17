/***************************************************************************
* Copyright (C) 2010 by Ammar Qammaz *
* ammarkov@gmail.com *
* *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or *
* (at your option) any later version. *
* *
* This program is distributed in the hope that it will be useful, *
* but WITHOUT ANY WARRANTY; without even the implied warranty of *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the *
* GNU General Public License for more details. *
* *
* You should have received a copy of the GNU General Public License *
* along with this program; if not, write to the *
* Free Software Foundation, Inc., *
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. *
***************************************************************************/

#include "image_storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define PPMREADBUFLEN 256

int compress_files=1;



int ConvertImageFormats(char * filenamein,char * filenameout)
{ //Needs imagemagick package :)
 char execstr[256]={0};
 sprintf(execstr,"convert %s %s",filenamein,filenameout);
 fprintf(stderr,"%s",execstr);

 int i = system(execstr);
 return i;
}

int ConvertSnapshotsToVideo(int framerate,int bitrate,char * filenameout)
{
 // ffmpeg -r 10 -b 1800 -i %03d.jpg test1800.mp4
 char execstr[256]={0};
 sprintf(execstr,"ffmpeg -r %u -b %uk -s 320x240 -i %%05d.jpg %s.mp4",framerate,bitrate,filenameout);
 int i = system(execstr);
 return i;
}

//ls -l | egrep -v '^d'




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


int ClearImage(struct Image * pic )
{
    return 0;
}






int PutPixel_inFrame(unsigned int x,unsigned int y,unsigned int R,unsigned int G,unsigned int B, unsigned char * frame,unsigned int depth, unsigned int size_x,unsigned int size_y)
{
    if ( ( x >= size_x ) || ( y >= size_y ) ) { return 0; }

    unsigned char * ptr;
    ptr = frame + ( y*depth*size_x ) + (x * depth);
    *ptr=R; ++ptr;
    *ptr=G; ++ptr;
    *ptr=B;

    return 1;
}

int DrawLine_inFrame( unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2 , unsigned int R,unsigned int G,unsigned int B , unsigned char * frame,unsigned int depth, unsigned int size_x,unsigned int size_y)
{
    unsigned int startx=x1 ,starty=y1 , endx = x2 , endy = y2 ;


    int t, distance;
    int xerr=0, yerr=0, delta_x, delta_y;
    int incx, incy;

    /* compute the distances in both directions */
    delta_x=endx-startx;
    delta_y=endy-starty;

    /* Compute the direction of the increment,
       an increment of 0 means either a horizontal or vertical
       line.
    */
    if(delta_x>0) incx=1;
    else if(delta_x==0) incx=0;
    else incx=-1;

    if(delta_y>0) incy=1;
    else if(delta_y==0) incy=0;
    else incy=-1;

    /* determine which distance is greater */
    delta_x=abs(delta_x);
    delta_y=abs(delta_y);
    if(delta_x>delta_y) distance=delta_x;
    else distance=delta_y;

    /* draw the line */
    for(t=0; t<=distance+1; t++)
    {
       PutPixel_inFrame(startx,starty,  R,G,B , frame,depth,size_x,size_y);

        xerr+=delta_x;
        yerr+=delta_y;
        if(xerr>distance)
        {
            xerr-=distance;
            startx+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            starty+=incy;
        }
    }


    /*
      unsigned int x,y,end,p;
      unsigned int dx = abs(x1 - x2);
      unsigned int dy = abs(y1 - y2);
      p = 2 * dy - dx;
      if(x1 > x2)
      { x = x2; y = y2; end = x1; }
      else
      { x = x1; y = y1; end = x2; }
      PutPixel_inFrame(x,y,  R,G,B , frame,depth,size_x,size_y);

      while(x < end)
      {
        x = x + 1;
        if(p < 0) { p = p + 2 * dy; }
                    else
                  { y = y + 1;
                    p = p + 2 * (dy - dx);
                  }
        PutPixel_inFrame(x,y,  R,G,B , frame,depth,size_x,size_y);
      }*/
    return 1;
}



