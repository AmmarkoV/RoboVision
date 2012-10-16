#ifndef IMAGE_STORAGE_JPG_H_INCLUDED
#define IMAGE_STORAGE_JPG_H_INCLUDED


int ReadJPEG( char *filename,struct Image * pic);
int WriteJPEG( char *filename,struct Image * pic,char *mem,unsigned long * mem_size);

#endif // IMAGE_STORAGE_JPG_H_INCLUDED
