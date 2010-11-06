#ifndef PIXELFORMATS_H_INCLUDED
#define PIXELFORMATS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <linux/videodev2.h>


int VideoFormatNeedsDecoding(int videoformat,int bitdepth);
int VideoFormatImplemented(int videoformat,int bitdepth);

void PrintOutPixelFormat(int pix_format);
void PrintOutCaptureMode(int cap_mode);
void PrintOutFieldType(int field_type);


#endif
