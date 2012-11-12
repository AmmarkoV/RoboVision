/*
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 * ( code has been modified , original file found here http://zarb.org/~gc/html/libpng.html
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "image_storage.h"

#define PNG_DEBUG 3
#include <png.h>

void abort_(const char * s, ...)
{
        va_list args;
        va_start(args, s);
        vfprintf(stderr, s, args);
        fprintf(stderr, "\n");
        va_end(args);
       // abort();
}




int ReadPNG(char * filename,struct Image * pic)
{
  fprintf(stderr,"This needs work :P \n");
  return 0;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

unsigned int width, height;


   /* pic->pixels pic->size_x pic->size_y */
       char header[8];    // 8 is the maximum size that can be checked

        /* open file and test for it being a png */
        FILE *fp = fopen(filename, "rb");
        if (!fp)
                abort_("[read_png_file] File %s could not be opened for reading", filename);
        int i=fread(header, 1, 8, fp);
        if (i!=8) { fprintf(stderr,"error reading png header..\n"); }

        if (png_sig_cmp((png_byte*) header, 0, 8))
                abort_("[read_png_file] File %s is not recognized as a PNG file", filename);


        /* initialize stuff */
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr)
                abort_("[read_png_file] png_create_read_struct failed");

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
                abort_("[read_png_file] png_create_info_struct failed");

        if (setjmp(png_jmpbuf(png_ptr)))
                abort_("[read_png_file] Error during init_io");

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        width = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);

        number_of_passes = png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);


        /* read file */
        if (setjmp(png_jmpbuf(png_ptr)))
                abort_("[read_png_file] Error during read_image");

        row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
        unsigned int y;
        for (y=0; y<height; y++)
             {
                 row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
             }


        png_read_image(png_ptr, row_pointers);

        fclose(fp);
  return 0;
}

int WritePNG(char * filename,struct Image * pic)
{

// VARIABLES START ---------------
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

int width, height;
// -------------------------------


/* create file */
FILE *fp = fopen(filename, "wb");
if (!fp) { abort_("[write_png_file] File %s could not be opened for writing", filename); return 0; }


/* initialize stuff */
png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
if (!png_ptr) { abort_("[write_png_file] png_create_write_struct failed"); return 0; }

info_ptr = png_create_info_struct(png_ptr);
if (!info_ptr) { abort_("[write_png_file] png_create_info_struct failed"); return 0; }

if (setjmp(png_jmpbuf(png_ptr))) { abort_("[write_png_file] Error during init_io"); return 0; }

png_init_io(png_ptr, fp);


/* write header */
if (setjmp(png_jmpbuf(png_ptr))) { abort_("[write_png_file] Error during writing header"); return 0; }

png_set_IHDR(png_ptr, info_ptr, pic->size_x,pic->size_y,8,PNG_COLOR_TYPE_RGB , PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
png_write_info(png_ptr, info_ptr);


/* write bytes */
if (setjmp(png_jmpbuf(png_ptr))) { abort_("[write_png_file] Error during writing bytes"); return 0; }
row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * pic->size_y);
char * raw_pixels=pic->pixels;

unsigned int y;
for (y=0; y<pic->size_y; y++)
 {
   row_pointers[y] = (png_byte*) raw_pixels;
   raw_pixels+=3*pic->size_x;
 }

 png_write_image(png_ptr,row_pointers);


 /* end write */
 if (setjmp(png_jmpbuf(png_ptr))) { abort_("[write_png_file] Error during end of write"); return 0; }

 png_write_end(png_ptr, NULL);

 /* cleanup heap allocation */
 free(row_pointers);

 fclose(fp);
return 1;
}
