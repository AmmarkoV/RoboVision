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

#ifndef IMAGE_STORAGE_H_INCLUDED
#define IMAGE_STORAGE_H_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

struct Image
{
  char * pixels;
  unsigned int size_x;
  unsigned int size_y;
  unsigned int depth;
  unsigned int image_size;
};

extern int compress_files;

int ReadPPM(char * filename,struct Image * pic);
int WritePPM(char * filename,struct Image * pic);

int ClearImage(struct Image * pic );

int ConvertImageFormats(char * filenamein,char * filenameout);
int ConvertSnapshotsToVideo(int framerate,int bitrate,char * filenameout);

int DrawLine_inFrame( unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2 , unsigned int R,unsigned int G,unsigned int B , unsigned char * frame,unsigned int depth, unsigned int size_x,unsigned int size_y);

int ReallocEmptyFrame(unsigned int new_size_x,unsigned int new_size_y);

#ifdef __cplusplus
}
#endif


#endif // IMAGE_STORAGE_H_INCLUDED
