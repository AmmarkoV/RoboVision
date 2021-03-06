#ifndef COMMAND_IMPLEMENTATION_H_INCLUDED
#define COMMAND_IMPLEMENTATION_H_INCLUDED

int IRCSay(char * what2say);
int Say(char * what2say);
int PlaySound(char * sndname);
int RecordSound(char * sndname, unsigned int delay);
int StopAllSounds();

void CalibratedView();
void FullDepthMap(unsigned char write_to_file);
void DrawMovement();
void DrawNewPalette(char R,char G,char B,char threshold);
void ConvolutionFilter(signed char * table,signed int divisor,unsigned int table_size);
void SobelNDerivative_in(int n);
int CheckAlarm(unsigned int flow1,unsigned int flow2);
void ClearFeatures();
void FindFeatures();
void GetFundamentalMatrix();
void DrawFeatures();
void Panoramic();
void SnapshotWithTimeStamp();
/*
int SaveRegisterToFile(char * filename , unsigned int reg_num);
void RecordWebCamSnap();
void PlaybackWebCamSnap();
void LiveWebCamSnap();*/


#endif // COMMAND_IMPLEMENTATION_H_INCLUDED
