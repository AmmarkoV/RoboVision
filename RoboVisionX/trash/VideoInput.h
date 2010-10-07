#ifndef VIDEOINPUT_H_INCLUDED
#define VIDEOINPUT_H_INCLUDED

int InitVideoInput();
int CloseVideoInput();
unsigned char * GetFrame(int webcam_id);

#endif // VIDEOINPUT_H_INCLUDED
