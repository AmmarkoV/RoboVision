#include "configuration.h"
#include <stdio.h>
#include <stdlib.h>

#include "../InputParser/InputParser_C.h"
#include "../VisualCortex/VisualCortex.h"
#include "../VideoInput/VideoInput.h"

char video_device_1[MAX_STR]="/dev/video0";
char video_device_2[MAX_STR]="/dev/video1";
char rd01_device[MAX_STR]="/dev/ttyUSB0";
char arduino_device[MAX_STR]="/dev/ttyUSB1";
char user[MAX_STR]="guarddog";
char group[MAX_STR]="guarddog";
char parentdir[MAX_STR]="/home/guarddog/RoboVisionRuntime/";

double camera_params_1[12]={0};
double camera_params_2[12]={0};

unsigned int fps=8;
unsigned int draw_on=1;
unsigned int web_interface=1;
unsigned int web_interface_snaptime=3000;
unsigned int motion_lock_on=0;
unsigned int swap_inputs=0;


char * Int2Str(signed int val)
{
  // ver 1.0
  if (val==0) return (char*) "0";


  char negative=0;
  if (val<0)
    {
      negative=1;
      val=val*(-1);
    }

  int base=10;
  static char buf[32] = {0};
  int i = 30;
  for (; val && i ; --i, val /= base) buf[i] = "0123456789abcdef"[val % base];

  if ( negative==1)
    {
      buf[i]='-';
      --i;
    }
  return &buf[i+1];
}

int Str2Int(char * inpt)
{
  // ver 1.0
  if (inpt==0) { return 0; }

  int intresult=0,multiplier=1,curnum=0;
  int startdigit=0;
  if (inpt[0]=='-')
    {
      startdigit=1;
    }

  int error_flag=0;
  int i;
  for (i=strlen(inpt)-1; i>=startdigit; i--)
    {
      curnum=inpt[i]-'0';
      if ((curnum>=0)&(curnum<=9))
        {
          intresult=intresult+(multiplier*curnum);
          multiplier=multiplier*10;
        }
      else
        {
          error_flag=1;
        }
    }

  if (inpt[0]=='-')
    {
      intresult = intresult * (-1);
    }
  return intresult;
}





void ParseConfigString(char * inpt)
{
  struct InputParserC * ipc=0;
  ipc = InputParser_Create(512,5);


  unsigned int words_count = InputParser_SeperateWords(ipc,inpt,0);
  if ( words_count > 0 )
    {

      if (InputParser_WordCompareNoCase(ipc,0,(char*)"CAMERA_OFFSETS",14)==1)
        {
           VisCortx_SetSetting(DEPTHMAP_VERT_OFFSET_UP,InputParser_GetWordInt(ipc,1));
           VisCortx_SetSetting(DEPTHMAP_VERT_OFFSET_DOWN,InputParser_GetWordInt(ipc,2));
        }
      else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"WEBINTERFACE_DELAY",14)==1)
        {
           web_interface_snaptime=InputParser_GetWordInt(ipc,1);
        }
      else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"DRAWFPS",7)==1)
        {
           fps=InputParser_GetWordInt(ipc,1);
           printf("Draw FPS = %u \n",fps);
        }
      else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"DRAWON",6)==1)
        {
           draw_on=InputParser_GetWordInt(ipc,1);
           printf("Draw On = %u \n",draw_on);
        }
      else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"ALARM",5)==1)
        {
           if ( InputParser_GetWordInt(ipc,1)==1 )  { fprintf(stderr,"Activating Alarm from config \n"); }
        }
      else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"RD01_CONTROLLER",15)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,rd01_device,MAX_STR);
        }
      else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"ARDUINO_CONTROLLER",18)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,arduino_device,MAX_STR);
        }
      else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"VIDEO1",6)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,video_device_1,MAX_STR);
        }
      else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"VIDEO2",6)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,video_device_2,MAX_STR);
        }
      else
      if ( (InputParser_WordCompareNoCase(ipc,0,(char*)"VIDEO1_CAMERA_PARAMETERS",24)==1) || (InputParser_WordCompareNoCase(ipc,0,(char*)"VIDEO2_CAMERA_PARAMETERS",24)==1) )
        {
           fprintf(stderr,"CAMERA_PARAMETERS(");
           char str[MAX_STR]={0};
           InputParser_GetWord(ipc,1,str,MAX_STR);
           double fx = atof ( str );

           InputParser_GetWord(ipc,2,str,MAX_STR);
           double fy = atof ( str );

           InputParser_GetWord(ipc,3,str,MAX_STR);
           double cx = atof ( str );

           InputParser_GetWord(ipc,4,str,MAX_STR);
           double cy = atof ( str );

           unsigned int VReg=0;
           if (InputParser_WordCompareNoCase(ipc,0,(char*)"VIDEO2_CAMERA_PARAMETERS",24)==1) { VReg=1; }

           if (VReg==0)
            {
                camera_params_1[2]=fx;
                camera_params_1[3]=fy;
                camera_params_1[4]=cx;
                camera_params_1[5]=cy;
            } else
           if (VReg==1)
            {
                camera_params_2[2]=fx;
                camera_params_2[3]=fy;
                camera_params_2[4]=cx;
                camera_params_2[5]=cy;
            }
        }
      else
      if ( (InputParser_WordCompareNoCase(ipc,0,(char*)"VIDEO1_DISTORTION_PARAMETERS",28)==1) || (InputParser_WordCompareNoCase(ipc,0,(char*)"VIDEO2_DISTORTION_PARAMETERS",28)==1) )
        {
           char str[MAX_STR]={0};
           InputParser_GetWord(ipc,1,str,MAX_STR);
           double k1 = atof ( str );

           InputParser_GetWord(ipc,2,str,MAX_STR);
           double k2 = atof ( str );

           InputParser_GetWord(ipc,3,str,MAX_STR);
           double p1 = atof ( str );

           InputParser_GetWord(ipc,4,str,MAX_STR);
           double p2 = atof ( str );

           InputParser_GetWord(ipc,5,str,MAX_STR);
           double k3 = atof ( str );

           unsigned int VReg=0;
           if (InputParser_WordCompareNoCase(ipc,0,(char*)"VIDEO2_DISTORTION_PARAMETERS",28)==1) { VReg=1; }

           if (VReg==0)
            {
                camera_params_1[6]=k1;
                camera_params_1[7]=k2;
                camera_params_1[8]=k3;
                camera_params_1[9]=p1;
                camera_params_1[10]=p2;
            } else
           if (VReg==1)
            {
                camera_params_2[6]=k1;
                camera_params_2[7]=k2;
                camera_params_2[8]=k3;
                camera_params_2[9]=p1;
                camera_params_2[10]=p2;
            }
        }


void VisCortx_SetDistortionParemeters(unsigned int reg_cam,double k1,double k2,double p1,double p2,double k3);

    }

    InputParser_Destroy(ipc);
}



void LoadConfiguration()
{
  char line[2048];
  int line_length=0;
  FILE * pFile;
  pFile = fopen ("guard.ini","r");
  if (pFile!=NULL )
    {
      int c=0;
      do
        {
          c = getc (pFile);
          if (c == '\n')
            {
              line[line_length]=0;
              ++line_length;
              ParseConfigString(line);
              line_length=0;
            }
          else
            {
              line[line_length]=c;
              ++line_length;
            }
        }
      while (c != EOF);
      fclose (pFile);
    }
  else
    {
//      wxMessageBox(wxT("Δεν ήταν δυνατή η ανάκτηση των ρυθμίσεων του προγράμματος"),wxT("ClinicDB"));
    }


}








