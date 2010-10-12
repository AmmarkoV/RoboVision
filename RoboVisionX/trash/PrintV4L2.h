#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <linux/videodev.h>
#include <linux/videodev2.h>

void print(struct v4l2_capability cap);
void print(struct v4l2_format fmt);

void print(struct v4l2_pix_format pix);
void print(struct v4l2_window window);
void print(struct v4l2_vbi_format vbi);
void print(struct v4l2_queryctrl ctrl);
void print(struct v4l2_control  control);
//void print(struct v4l2_sliced_vbi_format sliced);

//void print(struct v4l2_cropcap cropcap);

void print(struct v4l2_clip *clip);
void print(struct v4l2_rect rect);
void print(v4l2_buf_type type);
void print(v4l2_field field);
void print(v4l2_colorspace colorspace);

void printPixelFormat(__u32 pixelformat);
