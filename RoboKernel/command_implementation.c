#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_hal.h"
#include "configuration.h"
#include "command_implementation.h"
#include "visual_system.h"
#include <unistd.h>




int Say(char * what2say)
{
 char command_s[1024]={0};

 sprintf(command_s,"killall %s&",tts_command); // Clear all running tts instances ( could also add a script that waits for them to end here )
 int i=system((const char * ) command_s);
 fprintf(stderr,"%s " , command_s);


 sprintf(command_s,"echo \"%s\" | %s %s&",what2say,tts_command,tts_command_parameter);
 i=system((const char * ) command_s);
 return i;
}


int PlaySound(char * sndname)
{
  char command_s[1024]={0};
  sprintf(command_s,"%s ../DataSets/Sounds/%s.wav&",sound_play_command,sndname);
  fprintf(stderr," %s \n ",command_s);
  int i=system((const char * ) command_s);
  return i;
}

int RecordSound(char * sndname, unsigned int delay)
{
     char command_s[1024]={0};
    if ( delay != 0 )
     {
         sprintf(command_s,"%s -d %u %s memfs/public_html/%s.ogg&",sound_record_command,delay,sound_record_parameter,sndname);
     } else
     {
         sprintf(command_s,"%s %s memfs/public_html/%s.ogg&",sound_record_command,sound_record_parameter,sndname);
     }

    fprintf(stderr," %s \n ",command_s);
    int i=system((const char * ) command_s);
    return i;
}

int StopAllSounds()
{
  char command_s[1024]={0};
  sprintf(command_s,"killall %s",sound_play_command);
  int i=system((const char * ) command_s);

  sprintf(command_s,"killall %s",sound_record_command);
  i=system((const char * ) command_s);

  return i;
}


void CalibratedView()
{
  VisCortX_CopyVideoRegister(CALIBRATED_LEFT_EYE,LAST_LEFT_OPERATION,1,1);
  VisCortX_CopyVideoRegister(CALIBRATED_RIGHT_EYE,LAST_RIGHT_OPERATION,1,1);
}

void FullDepthMap(unsigned char write_to_file)
{

  VisCortx_FullDepthMap();
  VisCortX_CopyVideoRegister(DEPTH_LEFT_VIDEO,LAST_LEFT_OPERATION,1,1);
  VisCortX_ConvertVideoRegisterToColorDepth(LAST_LEFT_OPERATION,3);

  VisCortX_CopyVideoRegister(DEPTH_RIGHT_VIDEO,LAST_RIGHT_OPERATION,1,1);
  VisCortX_ConvertVideoRegisterToColorDepth(LAST_RIGHT_OPERATION,3);

//  if ( write_to_file == 1 ) DepthMapToFile(0);
}

void DrawMovement()
{
   VisCortX_CopyVideoRegister(MOVEMENT_LEFT,LAST_LEFT_OPERATION,1,1); /* CONVERTING 1 bit to 3bit :P*/
   VisCortX_ConvertVideoRegisterToColorDepth(LAST_LEFT_OPERATION,3);

   VisCortX_CopyVideoRegister(MOVEMENT_RIGHT,LAST_RIGHT_OPERATION,1,1);
   VisCortX_ConvertVideoRegisterToColorDepth(LAST_RIGHT_OPERATION,3); /* CONVERTING 1 bit to 3bit :P*/
}

void DrawNewPalette(char R,char G,char B,char threshold)
{
// KeepOnlyPixelsClosetoColor(R,G,B,threshold);
// VisCortX_CopyFromVideoToVideoRegister(GENERAL_1,LAST_LEFT_OPERATION);
// VisCortX_CopyFromVideoToVideoRegister(GENERAL_2,LAST_RIGHT_OPERATION);
}

void ConvolutionFilter(signed char * table,signed int divisor,unsigned int table_size)
{
  VisCortx_ConvolutionFilter(CALIBRATED_LEFT_EYE,LAST_LEFT_OPERATION,table,divisor,table_size); //CHECK
}

void SobelNDerivative_in(int n)
{
 SobelNDerivative(n);
}


void FindFeatures()
{
  //VisCortx_RemoveTimedoutTrackPoints(0,1);
  VisCortx_ClearTrackPoints(0);
  VisCortx_ClearTrackPoints(1);
  VisCortx_AutoAddTrackPoints(0);
  VisCortx_AutoAddTrackPoints(1);
  fprintf(stderr," FindFeatures() ok \n");
}


void ClearFeatures()
{
  VisCortx_ClearTrackPoints(0);
  VisCortx_ClearTrackPoints(1);
  fprintf(stderr," ClearFeatures() ok \n");
}

void GetFundamentalMatrix()
{
    float * table=0;
    table = (float *) malloc(9 * sizeof(float));
    if (table==0) { fprintf(stderr,"Error allocating memory for FundamentalMatrix structure"); return; }
    VisCortxGetFundamentalMatrix(table,9);
    free(table);
}


void DrawFeatures()
{
    VisCortx_DrawTrackPoints();
}


unsigned int Flow_Sufficient_For_Movement(unsigned int flow1,unsigned int flow2)
{
 if ( ( flow1 > 3000 ) || ( flow2 > 3000 ) )
     {
        return 1;
     }
  return 0;
}

void CheckAlarm(unsigned int flow1,unsigned int flow2)
{
    if ( Flow_Sufficient_For_Movement(flow1,flow2)==1 )
     {
        PlaySound((char *)"alarm.wav");
     }
}

void Panoramic()
{

    int max=8;
    int i=0;
    //for (i=0; i<max; i++)
     {
       IssueCommandInternal((char*)"LEFT",(char *)"GUI");
       usleep(50000);

       //VisCortx_FullDepthMap();
       IssueCommandInternal((char*)"DEPTHMAP",(char *)"GUI");

       IssueCommandInternal((char*)"DEPTHMAP TO FILE",(char *)"GUI");
//DepthMapToFile(i);
/*
       char filename[60]={0};
       sprintf(filename,"Stored/feed%u_0.jpeg",i);
       live_feeds[0].bmp->SaveFile(_U(filename),wxBITMAP_TYPE_JPEG);

       sprintf(filename,"Stored/feed%u_1.jpeg",i);
       live_feeds[1].bmp->SaveFile(_U(filename),wxBITMAP_TYPE_JPEG);

       sprintf(filename,"Stored/depth%u.ppm",i);
       SaveRegisterToFile(filename ,DEPTH_LEFT_VIDEO);*/

     }

     for (i=0; i<max; i++)
     {
       IssueCommandInternal((char*)"RIGHT",(char *)"GUI");
       usleep(50000);
     }
}


void DepthMapToFile(int snap)
{/*
   char filename[60]={0};
   FILE * fp;
   sprintf(filename,"Stored/DEPTH%u",snap);
   fp = fopen(filename,"wb");
   if (fp == 0) return;

   unsigned char * colour_array = VisCortx_ReadFromVideoRegister(LEFT_EYE,GetCortexMetric(RESOLUTION_X),GetCortexMetric(RESOLUTION_Y),3);
   unsigned char * depth_map_array = VisCortx_ReadFromVideoRegister(DEPTH_LEFT_VIDEO,GetCortexMetric(RESOLUTION_X),GetCortexMetric(RESOLUTION_Y),3);

   unsigned int ptr=0, ptrlim = GetCortexMetric(RESOLUTION_X)*GetCortexMetric(RESOLUTION_Y)*3;
   fwrite (depth_map_array , 1 , ptrlim , fp );

   fclose(fp);

   sprintf(filename,"Stored/COLOR%u",snap);
   fp = fopen(filename,"wb");
   if (fp == 0) return;

   ptr=0, ptrlim = 320*240*3;
   fwrite (colour_array, 1 , ptrlim , fp );
   fclose(fp);*/
}

void SnapshotWithTimeStamp()
{
               //  fprintf(stderr,"SnapshotWithTimeStamp()");
           /*      char outstr[256]={0};
                 strcpy(outstr,"memfs/snapshot");
                 time_t t;
                 struct tm *tmp;
                 t = time(NULL);
                 tmp = localtime(&t);
                 if (tmp == NULL)  { fprintf(stderr,"Local Time is null , cannot make a timestamp\n"); }
                 if (strftime(outstr, sizeof(outstr), "memfs/snapshot_%F_%T_" , tmp) == 0) { fprintf(stderr,"Time formatting failed\n"); }
                 //fprintf(stderr,"Timestamped filename is \"\n", outstr);
                 RecordOne((char *)outstr);*/
}
