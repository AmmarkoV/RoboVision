#include "command_hal.h"
#include "../../InputParser/InputParser_C.h"
#include "../MotorFoundation/MotorHAL.h"
#include "RoboVisionXMain.h"
#include "FeedScreenMemory.h"
#include "configuration.h"
#include "webinterface.h"

int Say(char * what2say)
{
 char command_s[1024]={0};
 sprintf(command_s,"echo \"%s\" | esddsp festival --tts",what2say);
 int i=system((const char * ) command_s);
 return i;
}


int PlaySound(char * sndname)
{
  char command_s[1024]={0};
  sprintf(command_s,"aplay Sounds/%s.wav",sndname);
  int i=system((const char * ) command_s);
  return i;
}

int IssueCommand(char * command)
{
  char outptstr[512]={0};
  char commandpayload_1[512]={0};
  char commandpayload_2[512]={0};
  struct InputParserC * ipc=0;
  ipc = InputParser_Create(512,5);


  unsigned int words_count = InputParser_SeperateWords(ipc,command,0);
    if ( words_count > 0 )
    {

      if (InputParser_WordCompareNoCase(ipc,0,(char*)"PANORAMIC",9)==1)
        {
           InputParser_GetWord(ipc,1,commandpayload_1,512);
           sprintf(outptstr,"From %s : Panoramic \n",commandpayload_1);
           WriteConsoleOutput(outptstr);
           /*COMMAND PAYLODE = NEXT FUNCTION*/
           Panoramic();
        }
          else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"KEEPCOLOR",9)==1)
        {
            //KEEPCOLOR(76,109,146,100)
           unsigned char R=InputParser_GetWordInt(ipc,1);
           unsigned char G=InputParser_GetWordInt(ipc,2);
           unsigned char B=InputParser_GetWordInt(ipc,3);
           unsigned char thresh=InputParser_GetWordInt(ipc,4);
           sprintf(outptstr,"Command Executing Color keeping ( %u , %u , %u ) with threshold %u \n",R,G,B,thresh);
           WriteConsoleOutput(outptstr);
           /*COMMAND PAYLODE = NEXT FUNCTION*/
           DrawNewPalette(R,G,B,thresh);
        }
          else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"MOTION ALARM",12)==1)
        {
           InputParser_GetWord(ipc,1,commandpayload_1,512);
           sprintf(outptstr,"From %s : Command Switching Motion Alarm \n",commandpayload_1);
           WriteConsoleOutput(outptstr);
           /*COMMAND PAYLODE = NEXT FUNCTION*/
           if (motion_lock_on==1) {motion_lock_on=0; } else
                                  {motion_lock_on=1; }
        }
          else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"SAVE REGISTER",13)==1)
        {
           InputParser_GetWord(ipc,1,commandpayload_1,512);
           int reg_num=InputParser_GetWordInt(ipc,2);
           InputParser_GetWord(ipc,3,commandpayload_2,512);
           sprintf(outptstr,"From %s : Command Parser Save Register %u to file : %s\n",commandpayload_2,reg_num,commandpayload_1);
           WriteConsoleOutput(outptstr);
           /*COMMAND PAYLODE = NEXT FUNCTION*/
           SaveRegisterToFile(commandpayload_1,reg_num);
        }
          else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"SWAP FEEDS",10)==1)
        {
           InputParser_GetWord(ipc,1,commandpayload_1,512);
           sprintf(outptstr,"From %s : Command Swapping Video Feeds \n",commandpayload_1);
           WriteConsoleOutput(outptstr);
           /*COMMAND PAYLODE = NEXT FUNCTION*/
           if (swap_inputs==1) {swap_inputs=0; } else
                               {swap_inputs=1; }
        }
          else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"WEB INTERFACE",13)==1)
        {
           InputParser_GetWord(ipc,1,commandpayload_1,512);
           sprintf(outptstr,"From %s : Command Switching WebInterface \n",commandpayload_1);
           WriteConsoleOutput(outptstr);
           /*COMMAND PAYLODE = NEXT FUNCTION*/
           if (web_interface==1) {web_interface=0; CloseWebInterface(); } else
                                 {web_interface=1; OpenWebInterface(); }
        }
          else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"DRAW MOVEMENT",13)==1)
        {
           InputParser_GetWord(ipc,1,commandpayload_1,512);
           sprintf(outptstr,"From %s : Command Drawing Movement\n",commandpayload_1);
           WriteConsoleOutput(outptstr);
           /*COMMAND PAYLODE = NEXT FUNCTION*/
           DrawMovement();
        }
          else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"FIND FEATURES",13)==1)
        {
           InputParser_GetWord(ipc,1,commandpayload_1,512);
           sprintf(outptstr,"From %s : Command Finding Features\n",commandpayload_1);
           WriteConsoleOutput(outptstr);
           /*COMMAND PAYLODE = NEXT FUNCTION*/
           FindFeatures();
        }
          else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"PLAYSOUND",9)==1)
        {
           InputParser_GetWord(ipc,1,commandpayload_1,512);
           InputParser_GetWord(ipc,2,commandpayload_2,512);
           sprintf(outptstr,"From %s : Command Parser Playing sound : %s\n",commandpayload_2,commandpayload_1);
           WriteConsoleOutput(outptstr);
           /*COMMAND PAYLODE = NEXT FUNCTION*/
           PlaySound(commandpayload_1);
        }
          else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"SAY",3)==1)
        {
           InputParser_GetWord(ipc,1,commandpayload_1,512);
           InputParser_GetWord(ipc,2,commandpayload_2,512);
           sprintf(outptstr,"From %s : Command Parser TTS Saying : %s\n",commandpayload_2,commandpayload_1);
           WriteConsoleOutput(outptstr);
           /*COMMAND PAYLODE = NEXT FUNCTION*/
           Say(commandpayload_1);
        }
          else
      /* NEED IMPROVEMENTS*/
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"DEPTHMAP",8)==1)
        {
           InputParser_GetWord(ipc,1,commandpayload_1,512);
           sprintf(outptstr,"From %s : Command Parser , Performing DepthMap \n",commandpayload_1);
           WriteConsoleOutput(outptstr);

           /*COMMAND PAYLODE = NEXT FUNCTION*/
           ActivateDepthMapButton();

        } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"FORWARD",7)==1)
        {
           InputParser_GetWord(ipc,1,commandpayload_1,512);
           sprintf(outptstr,"From %s : Command Parser , Going Up \n",commandpayload_1);
           WriteConsoleOutput(outptstr);

           /*COMMAND PAYLODE = NEXT FUNCTION*/
           RobotMove(20,5000);
        } else
       if (InputParser_WordCompareNoCase(ipc,0,(char*)"BACKWARD",8)==1)
        {
           InputParser_GetWord(ipc,1,commandpayload_1,512);
           sprintf(outptstr,"From %s : Command Parser , Going DOWN \n",commandpayload_1);
           WriteConsoleOutput(outptstr);

           /*COMMAND PAYLODE = NEXT FUNCTION*/
           RobotMove(20,-5000);
        } else
       if (InputParser_WordCompareNoCase(ipc,0,(char*)"LEFT",4)==1)
        {
            InputParser_GetWord(ipc,1,commandpayload_1,512);
            sprintf(outptstr,"From %s : Command Parser , Going LEFT \n",commandpayload_1);
            WriteConsoleOutput(outptstr);

            /*COMMAND PAYLODE = NEXT FUNCTION*/
            RobotRotate(20,10);
        } else
       if (InputParser_WordCompareNoCase(ipc,0,(char*)"RIGHT",5)==1)
        {
           InputParser_GetWord(ipc,1,commandpayload_1,512);
           sprintf(outptstr,"From %s : Command Parser , Going RIGHT \n",commandpayload_1);
           WriteConsoleOutput(outptstr);

           /*COMMAND PAYLODE = NEXT FUNCTION*/
            RobotRotate(20,(-1)*10);

        } else
        /*
          >>>>>>>>>>>>>>>>>>>>>>>>>>!!!WRONG COMMAND!!!<<<<<<<<<<<<<<<<<<<<<<<<
           */
        {
           sprintf(outptstr,"Wrong command from %s \n",commandpayload_1);
           WriteConsoleOutput(outptstr);
           return 0;
        }
    }
return 1;
}

int EraseConsoleOutput()
{
  FILE * pFile=0;
  pFile = fopen ("memfs/public_html/consoleout.dat","w");
  if (pFile!=NULL )
  {
    fclose(pFile);
    return 1;
  }
  return 0;
}

int WriteConsoleOutput(char * outstr)
{
  FILE * pFile=0;
  pFile = fopen ("memfs/public_html/consoleout.dat","a");
  if (pFile!=NULL )
  {
    printf("%s\n",outstr);
    fprintf(pFile,"%s\n",outstr);
    fclose(pFile);
    return 1;
  }
  return 0;
}
