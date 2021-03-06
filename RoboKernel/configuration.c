#include "configuration.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../InputParser/InputParser_C.h"
#include "../VisualCortex/VisualCortex.h"
#include "../VideoInput/VideoInput.h"

#define MAX_LINE_SIZE 2048


int system_autonomous=0;

char video_device_1[MAX_STR]="/dev/video0";
char video_device_2[MAX_STR]="/dev/video1";

char arduino_device[MAX_STR]="";
char arduino_device_name[MAX_STR]="FTDI FT232R USB UART A9007Wk3";

char joystick_device[MAX_STR]="/dev/input/js0";

char rd01_device[MAX_STR]="";
char rd01_device_name[MAX_STR]="FTDI FT232R USB UART A2001mqz";

char gsm_modem[MAX_STR]="";
char gsm_modem_name[MAX_STR]="ZTE,Incorporated ZTE WCDMA Technologies MSM P673M2COSD010000";

char user[MAX_STR]="guarddog";
char group[MAX_STR]="guarddog";
//char parentdir[MAX_STR]="/home/guarddog/RoboVisionRuntime/";

char sound_play_command[MAX_STR]="paplay";

char sound_record_command[MAX_STR]="arecord";
char sound_record_parameter[MAX_STR]="-f cd -t raw | oggenc - -r -o";

char tts_app[MAX_STR]="festival";
char tts_pre_command[MAX_STR]="echo \"";
char tts_command[MAX_STR]="\" | festival";
char tts_command_parameter[MAX_STR]="--tts";

unsigned int IRC_Interface_Enabled=0;
unsigned int Web_Interface_Enabled=0;




char ENVDIR[MAX_STR]="../robot/";
char INITIALIZATION_CONFIGURATION_PATH[MAX_STR]="/robot/guard.ini";
char SNAPSHOT_PATH[MAX_STR]="/robot/permfs/Snapshots/snapshot";
char WEB_SERVER_ROOT[MAX_STR]="/robot/memfs/www/";
char CONSOLE_OUT_PATH[MAX_STR]="/robot/memfs/www/consoleout.dat";
char COMMANDS_PATH[MAX_STR]="/robot/memfs/www/commands.dat";
char VIEWERS_PATH[MAX_STR]="/robot/memfs/www/viewers.dat";
char SENSORS_PATH[MAX_STR]="/robot/memfs/www/sensors.dat";

char SMS_SEND_PATH[MAX_STR]="/robot/permfs/SMSSend/message";
char SMS_RECEIVE_PATH[MAX_STR]="/robot/permfs/SMSReceive/Received";


char CLIPART_PATH[MAX_STR]="/robot/permfs/Clipart/";





double camera_params_1[12]={0};
double camera_params_2[12]={0};

unsigned int fps=8;
unsigned int draw_on=1;
unsigned int web_interface=1;
unsigned int web_interface_snaptime=3000;
unsigned int motion_lock_on=0;
unsigned int swap_inputs=0;



char FileExistsConf(char * filename)
{
 FILE *fp = fopen(filename,"r");
 if( fp ) { /* exists */
            fclose(fp);
            return 1;
          }
          else
          { /* doesnt exist */ }
 return 0;
}

int quickcat(char * outfilename,char *infilename1,char * infilename2)
{
  strcpy(outfilename,infilename1);
  strcat(outfilename,infilename2);
  return 1;
}


char * get_environment_robot_directory()
{
  return ENVDIR;
}

int find_environment_robot_directory()
{
  if (ENVDIR==0) { fprintf(stderr,"find_environment_robot_directory called with empty string.. \n"); return 0; }

  //This means an installation on the actual guarddog Robot ( /robot/ )
  if (FileExistsConf("/robot/guard.ini"))   { strcpy(ENVDIR,"/robot/"); }   else
  {
   int found_dir_using_getcwd=0;
   char cwd_guard_file[1024]={0};
   char cwd[1024]={0};
   if (getcwd(cwd, sizeof(cwd)) != 0 )
     {
       fprintf(stdout, "Current working dir: %s\n", cwd);
       int where_to_check_for_slash=strlen(cwd);
       if (where_to_check_for_slash>0) { --where_to_check_for_slash;}
       if (cwd[where_to_check_for_slash]!='/')
         {
           ++where_to_check_for_slash;
           cwd[where_to_check_for_slash]='/';
           cwd[where_to_check_for_slash+1]=0;
           strcpy(cwd_guard_file,cwd);
           strcat(cwd_guard_file,"robot/guard.ini");

           fprintf(stdout, "Testing for guard.ini @ %s , dir is %s \n",cwd_guard_file,cwd);
           //Ok what this code segment does is check for /home/ammar/Documents/Programming/RoboVision/robot/guard.ini
           //if we cwd is /home/ammar/Documents/Programming/RoboVision/ we will be ok , but what if we are on /home/ammar/Documents/Programming/RoboVision/RoboVisionX ?
           //The next step resolves this case
           if (FileExistsConf(cwd_guard_file))   { strcpy(ENVDIR,cwd); strcat(ENVDIR,"robot/"); found_dir_using_getcwd=1; }

          if (!found_dir_using_getcwd)
          {
              //We didnt find the guard.ini so checking one more time on the parent directory , if we cd'd to RoboVisionX or RoboVisionCLI dirs this will be the correct dir..!
              --where_to_check_for_slash;
              while ( (where_to_check_for_slash>0)&&(cwd[where_to_check_for_slash]!='/') ) {  --where_to_check_for_slash; }
              if (where_to_check_for_slash>0)
                 {
                    //the slash of the parent dir is in  where_to_check_for_slash so appending null termination..
                    cwd[where_to_check_for_slash+1]=0;
                    strcpy(cwd_guard_file,cwd);
                    strcat(cwd_guard_file,"robot/guard.ini");
                    fprintf(stdout, "Testing for guard.ini @ %s , dir is %s \n",cwd_guard_file,cwd);
                    if (FileExistsConf(cwd_guard_file))   { strcpy(ENVDIR,cwd); strcat(ENVDIR,"robot/"); found_dir_using_getcwd=1; }
                 }
          }
         }
      }

    if (found_dir_using_getcwd)               { /* We found ENVDIR after a successfull getcwd call */ } else
    if (FileExistsConf("../robot/guard.ini")) { strcpy(ENVDIR,"../robot/"); } else
                                              { ENVDIR[0]=0; ENVDIR[1]=0; }
   }


   fprintf(stderr,"Setting Environment directory to : %s \n",ENVDIR);

   quickcat(INITIALIZATION_CONFIGURATION_PATH,ENVDIR,"guard.ini");
   quickcat(SNAPSHOT_PATH,ENVDIR,"permfs/Snapshots/snapshot");
   quickcat(WEB_SERVER_ROOT,ENVDIR,"memfs/www");
   quickcat(CONSOLE_OUT_PATH,ENVDIR,"memfs/www/consoleout.dat");
   quickcat(COMMANDS_PATH,ENVDIR,"memfs/www/commands.dat");
   quickcat(VIEWERS_PATH,ENVDIR,"memfs/www/viewers.dat");
   quickcat(SENSORS_PATH,ENVDIR,"memfs/www/sensors.dat");
   quickcat(SMS_SEND_PATH,ENVDIR,"permfs/SMSSend/message");
   quickcat(SMS_RECEIVE_PATH,ENVDIR,"permfs/SMSReceive/Received");
   quickcat(CLIPART_PATH,ENVDIR,"permfs/Clipart/");


   return (ENVDIR[0]!=0);
}


int filename_stripper_found_attack(char * filename)
{
   unsigned int i=0;
   unsigned int length=strlen(filename);

   while (i<length)
     {
        if ( (filename[i]=='_') )  { /* UNDERSCORES ARE ACCEPTABLE , OK */} else
        if ( (filename[i]==' ') )  { /* Empty Spaces are acceptable , OK */} else
        if ( (filename[i]=='.') )  { /* Dots are acceptable , OK */} else
        if ( (filename[i]=='!') )  { /* Exclamation marks are acceptable , OK */} else
        if ( (filename[i]>='0') && (filename[i]<='9') )  { /* CHARACTER IS A NUMBER , OK */} else
        if ( (filename[i]>='A') && (filename[i]<='Z') )  { /* CHARACTER IS A CAPITAL LETTER , OK */} else
        if ( (filename[i]>='a') && (filename[i]<='z') )  { /* CHARACTER IS A LOWERCASE LETTER , OK */} else
           {
               return 1;
           }

        ++i;
     }


   return 0;
}

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

  //int error_flag=0;
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
          //error_flag=1;
        }
    }

  if (inpt[0]=='-')
    {
      intresult = intresult * (-1);
    }

  //if (error_flag) { return -1; }
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
           VisCortx_SetSetting(DEPTHMAP_VERT_SHIFT_UP,InputParser_GetWordInt(ipc,1));
           VisCortx_SetSetting(DEPTHMAP_VERT_SHIFT_DOWN,InputParser_GetWordInt(ipc,2));
           VisCortx_SetSetting(DEPTHMAP_VERT_OFFSET_UP,InputParser_GetWordInt(ipc,3));
           VisCortx_SetSetting(DEPTHMAP_VERT_OFFSET_DOWN,InputParser_GetWordInt(ipc,4));
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
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"RD01_CONTROLLER_NAME",20)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,rd01_device_name,MAX_STR);
        }
      else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"ARDUINO_CONTROLLER",18)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,arduino_device,MAX_STR);
        }
      else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"ARDUINO_CONTROLLER_NAME",23)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,arduino_device_name,MAX_STR);
        }
      else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"GSM_MODEM",9)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,gsm_modem,MAX_STR);
        }
      else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"GSM_MODEM_NAME",14)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,gsm_modem_name,MAX_STR);
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
        }else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"SOUND_PLAY_COMMAND",18)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,sound_play_command,MAX_STR);
        }else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"TTS_APP",7)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,tts_app,MAX_STR);
        }else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"TTS_PRE_COMMAND",15)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,tts_pre_command,MAX_STR);
        }else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"TTS_COMMAND",11)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,tts_command,MAX_STR);
        }else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"TTS_COMMAND_PARAMETER",21)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,tts_command_parameter,MAX_STR);
        }else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"SOUND_RECORD_COMMAND",20)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,sound_record_command,MAX_STR);
        }else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"SOUND_RECORD_COMMAND_PARAMETER",30)==1)
        {
           if ( InputParser_GetWordLength(ipc,1)<MAX_STR ) InputParser_GetWord(ipc,1,sound_record_parameter,MAX_STR);
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
                //VisCortx_SetDistortionParemeters(LEFT_EYE,k1,k2,p1,p2,k3);
            } else
           if (VReg==1)
            {
                camera_params_2[6]=k1;
                camera_params_2[7]=k2;
                camera_params_2[8]=k3;
                camera_params_2[9]=p1;
                camera_params_2[10]=p2;
                //VisCortx_SetDistortionParemeters(RIGHT_EYE,k1,k2,p1,p2,k3);
            }

        }

    }

    InputParser_Destroy(ipc);
}


int GetDeviceFilename(char * device_filename,char * output,unsigned int size_of_output)
{


    strcpy(output,"");
    char command[1024]={0};
    strcpy(command,"../Scripts/Tools/GetDeviceFilename.sh \"");
    strcat(command,device_filename);
    strcat(command,"\"");

    FILE *fp;

    /* Open the command for reading. */
     fp = popen(command, "r");
     if (fp == 0 )
       {
         fprintf(stderr,"Failed to run command\n");
         return 0;
       }

 /* Read the output a line at a time - output it. */
  unsigned int i=0;
  while (fgets(output, size_of_output , fp) != 0)
    {
        ++i;
        //fprintf(stderr,"\n\nline %u = %s \n",i,output);
        break;
    }


  /* close */
  pclose(fp);

  if( !strstr(output,"/dev/") )
    { //If thre is no /dev/ put it in !
      char fullpath[1024]={0};
      strcpy(fullpath,"/dev/");
      strcat(fullpath,output);
      strcpy(output,fullpath);
    }

  InputParser_ClearNonCharacters(output,strlen(output));
  InputParser_TrimCharacters(output,strlen(output),' ');

  if (i==1) { return 1; }
  return 0;
}



int RefreshDeviceNumbering()
{
    /*
       NOTICE THAT !IF! A STATIC PATH IS GIVEN FOR ARDUINO DEVICE ( i.e. /dev/ttyUSB0 )
       AND THE MD23 DEVICE IS SCANNED ON THE USB AT /dev/ttyUSB0 , BOTH WILL BE CONSIDERED
       VALID , SO IT IS BEST TO USE AUTOMATIC DETECTION , AND RESORT TO STATIC CONFIGURATIONS
       ONLY FOR DEBUGGING REASONS.,
    */
    char path[1024]={0};
    if ( GetDeviceFilename(arduino_device_name,path,1024) )
         {
           fprintf(stderr,"SWITCHING TO REAL ARDUINO THAT IS %s\n",path);
           strcpy(arduino_device,path);
         }

    if ( GetDeviceFilename(rd01_device_name,path,1024) )
         {
           fprintf(stderr,"SWITCHING TO REAL RD01 THAT IS %s\n",path);
           strcpy(rd01_device,path);
         }

    if ( GetDeviceFilename(gsm_modem_name,path,1024) )
         {
           fprintf(stderr,"SWITCHING TO REAL GSM DEVICE IS %s\n",path);
           strcpy(gsm_modem,path);
         }


    return 1;
}

void LoadConfiguration()
{
  char line[MAX_LINE_SIZE];

  if ( !find_environment_robot_directory() )
     {
        fprintf(stderr,"ERROR : Could not find a robot/ directory so stopping load configuration , many things will fail..\n");
        return;
     }


  FILE * pFile;
  pFile = fopen (INITIALIZATION_CONFIGURATION_PATH ,"r");
  if (pFile!=0 )
    {
      int line_length=0;
      int c=0;
      do
        {
          c = getc (pFile);

          if ( MAX_LINE_SIZE-1 <= line_length )
             {
               fprintf(stderr,"Oveflow while loading configuration file \n");
               line[MAX_LINE_SIZE-1]=0;
               ParseConfigString(line);
               line_length=0;
             } else
          if (c == '\n')
            {
              ParseConfigString(line);
              line_length=0;
            }
          else
            {
              line[line_length]=c;
              ++line_length;
              line[line_length]=0; // always append null termination ;P
            }
        }
      while (c != EOF);
      fclose (pFile);


      RefreshDeviceNumbering();
    }
  else
    {
      fprintf(stderr,"Cannot open guard.ini \n");
    }


}








