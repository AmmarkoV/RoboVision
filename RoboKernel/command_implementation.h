#ifndef COMMAND_IMPLEMENTATION_H_INCLUDED
#define COMMAND_IMPLEMENTATION_H_INCLUDED

int Say(char * what2say);
int PlaySound(char * sndname);

void FullDepthMap(unsigned char write_to_file);
void DrawMovement();
void DrawNewPalette(char R,char G,char B,char threshold);
void CheckAlarm(unsigned int flow1,unsigned int flow2);
void FindFeatures();
void DrawFeatures();
void Panoramic();
void SnapshotWithTimeStamp();
/*
int SaveRegisterToFile(char * filename , unsigned int reg_num);
void RecordWebCamSnap();
void PlaybackWebCamSnap();
void LiveWebCamSnap();*/


#endif // COMMAND_IMPLEMENTATION_H_INCLUDED
