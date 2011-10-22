#include "gsm_modem.h"
#include <stdio.h>
#include <stdlib.h>
#include "../InputParser/InputParser_C.h"
/*

IN ORDER FOR A SIMPLER APPROACH , AND NOT
REINVENTING THE WHEEL I USE gsm-utils , SO EVERYONE IS HAPPY xD

#define BAUDRATE B38400
int fd=0;
*/

#define MAX_LINE_SIZE 2048
#define MAX_SENDER_TELEPHONE 20
#define MAX_DATA_TELEPHONE 2048

int incoming_messages = 0;
char sender[MAX_SENDER_TELEPHONE]="+030GOD:P";
char data[MAX_DATA_TELEPHONE]={0};




int StartGSMModem(char * device,char * spool_dir)
{
  //THIS IS DONE BY THE RUNTIME THAT SPAWNS THE PROCESS
   return 0;
}

int StopGSMModem()
{
 //THIS IS DONE BY THE RUNTIME THAT SPAWNS THE PROCESS
   return 0;
}

int SendSMS(char * number,char * sms_message)
{
  FILE * pFile;
  pFile = fopen ("../RoboVisionRuntime/memfs/SMS_Send/message","w");
  if (pFile!=0)
  {
    fprintf (pFile,"%s\n",number);
    fprintf (pFile,"%s",sms_message);
    fclose (pFile);
    return 1;
  }
 return 0;
}



int ParseReceivedSMS(char * line)
{
   struct InputParserC * ipc=0;
   ipc = InputParser_Create(512,5);
   InputParser_SetDelimeter(ipc,0,'\'');
   char * pch;

   pch = strstr (line,"Originating address:");
   if ( pch != 0 )
    {
       //THIS LINE HAS THE ORIGNATING ADDRESS INFO!
       unsigned int words_count = InputParser_SeperateWords(ipc,line,0);
       if ( words_count > 0 )
        {
            if ( InputParser_GetWordLength(ipc,1)<MAX_SENDER_TELEPHONE ) InputParser_GetWord(ipc,1,sender,MAX_SENDER_TELEPHONE);
            fprintf(stderr,"Sender of message is %s\n",sender);
        }
      pch = 0;
    }
   pch = strstr (line,"User data: ");
   if ( pch != 0 )
    {
       //THIS LINE HAS THE DATA INFO!
       unsigned int words_count = InputParser_SeperateWords(ipc,line,0);
       if ( words_count > 0 )
        {
            if ( InputParser_GetWordLength(ipc,1)<MAX_DATA_TELEPHONE ) InputParser_GetWord(ipc,1,data,MAX_DATA_TELEPHONE);
            fprintf(stderr,"Data of message is %s\n",data);
        }
      pch = 0;
    }

 InputParser_Destroy(ipc);
 return 0;
}


int ReceiveSMS(char * number,unsigned int number_size,char * sms_message,int sms_message_size)
{
  FILE * pFile=0;
  pFile = fopen ("../RoboVisionRuntime/memfs/SMS_Receive/Received","r");
  if (pFile!=0)
   {
     int c=0;
     char line[MAX_LINE_SIZE]={0};
     int line_length=0;
     do
        {
          c = getc (pFile);

          if ( MAX_LINE_SIZE <= line_length )
             {
               line[MAX_LINE_SIZE-1]=0;
               ParseReceivedSMS(line);
               line_length=0;
             } else
          if (c == '\n')
            {
              line[line_length]=0; ++line_length;
              ParseReceivedSMS(line);
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
    return 1;
  }


  return 0;
}


