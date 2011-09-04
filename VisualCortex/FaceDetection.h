#ifndef FACEDETECTION_H_INCLUDED
#define FACEDETECTION_H_INCLUDED

void InitFaceRecognition(unsigned int x,unsigned int y);
void CloseFaceRecognition();

unsigned int RecognizeFaces(unsigned int vid_reg);

#endif // FACEDETECTION_H_INCLUDED
