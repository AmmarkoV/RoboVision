#ifndef FACEDETECTION_H_INCLUDED
#define FACEDETECTION_H_INCLUDED

void InitFaceRecognition(unsigned int x,unsigned int y);
void CloseFaceRecognition();

unsigned int RecognizeFaces(unsigned char * rgbdata);
void GetFaceNumber(char num,unsigned int *pos_x,unsigned int *pos_y,unsigned int *total_size);

#endif // FACEDETECTION_H_INCLUDED
