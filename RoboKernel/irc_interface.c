#include "irc_interface.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_t irc_loop_id=0;
void * IRCInterfaceLoop(void *ptr );

int IRC_INTERFACE_OK=0;
int STOP_IRC_INTERFACE=0;
unsigned int lines_parsed=0;
fpos_t pos;





int IRCSay(char * what2say)
{
 //IRC Messages need security checks but are disabled for debugging for now :P
 /*
 if (filename_stripper_found_attack(what2say))
   {
     //Weird Filename detected , aborting unsafe code
     fprintf(stderr,"INJECTION ATTEMPT AT IRCSAY FUNCTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
     return 666;
   }
   */
  if ( !IRC_INTERFACE_OK )
   {
       fprintf(stderr,"IRC Interface is dead \n");
       return 1;
   }


 char command_s[1024]={0};


 sprintf(command_s,"../IRCInterface/commands/say 127.0.0.1 6667 foss-aueb \"%s\" &",what2say); // Clear all running tts instances ( could also add a script that waits for them to end here )
// fprintf(stderr,"IRC COMMAND: %s " , command_s);
 fprintf(stderr,"Streaming to IRC\n");
 int i=system((const char * ) command_s);


 return i;
}



int EraseIRCInput()
{
  FILE * pFile=0;
  pFile = fopen ("../IRCInterface/log","w");
  if (pFile!=0 )
  {
    fclose(pFile);
    return 1;
  }
  return 0;
}



int IRCExtractLastInput()
{
 FILE * pFile=0;
  pFile = fopen ("../IRCInterface/log","r");

  struct InputParserC * ipc=0;
  ipc = InputParser_Create(512,5);
  char from_string[512]={0};
  unsigned int words_count=0;

  if (pFile!=0 )
    {

      /*
         READ COMMANDS HERE , USING AFSCRIPT!
      */


         if (lines_parsed>0) { fsetpos (pFile,&pos); }

         char line[512]={0};

         char * start_of_nickname=0;
         char * end_of_nickname=0;
         char * start_of_command=0;
         char * end_of_command=0;
         char * start_of_text=0;

         char * guarddog_mentioned=0;

         char nickname[512]={0};
         char ip[512]={0};
         char command[512]={0};
         char channel[512]={0};
         char text[512]={0};

         while( fgets(line,sizeof(line),pFile) )
         {
            if (strlen(line)<10)
            {

            }
            //An input line looks like this
            //<<< 1324146375601 :AmmarkoV!~AmmarkoV@ppp-94-68-242-133.home.otenet.gr PRIVMSG #foss-aueb :pro
            ++lines_parsed;


            start_of_nickname=strchr(line,':');
            if (start_of_nickname==0) { break; } else { start_of_nickname+=1; }

            end_of_nickname=strchr(start_of_nickname,'!');
            if (end_of_nickname==0) { break; } else { end_of_nickname-=1; }

            start_of_command=end_of_nickname;
            if (start_of_command==0) { break; } else { start_of_command+=2; }

            end_of_command=strchr(start_of_command,':');
            if (end_of_command==0) { break; } else { end_of_command-=1; }

            start_of_text=end_of_command+2;

            if ( (start_of_nickname!=0) && (end_of_nickname!=0) && (start_of_command!=0) && (end_of_command!=0) )
             {
               strncpy(nickname,start_of_nickname,end_of_nickname-start_of_nickname+1);
               strncpy(command,start_of_command,end_of_command-start_of_command+1);
               strncpy(text,start_of_text,512);

               fprintf(stderr,"Nickname : %s \n",nickname);
               fprintf(stderr,"Command : %s \n",command);
               fprintf(stderr,"Text : %s \n",text);

               guarddog_mentioned = strstr (text,"GuarddoG");
               if (guarddog_mentioned == 0 ) { guarddog_mentioned = strstr (text,"guarddog"); }
               if (guarddog_mentioned == 0 ) { guarddog_mentioned = strstr (text,"gddg"); }

               if ( guarddog_mentioned != 0 )
                 {
                   char output_string[512]={0};
                   IssueCommandInternal(text,nickname,output_string,512);
                   IRCSay(output_string);
                 }

             }




            fprintf(stderr,"Command %s has now returned from processing \n",line);
         }
      /*
         READ COMMANDS HERE!
      */

      fgetpos (pFile,&pos);


      fclose(pFile);
      return 1;
    }
 return 0;
}

void * IRCInterfaceLoop(void *ptr )
{
   EraseIRCInput();
   usleep(50000);

   IRC_INTERFACE_OK=1;
   IRCSay("IRC interface is now active\n");
   while ( !STOP_IRC_INTERFACE )
     {
        IRCExtractLastInput();

		usleep(50000);

     }
   IRCSay("IRC interface is now deactivated\n");
   IRC_INTERFACE_OK=0;
}



int StartIRCInterface()
{
    if (IRC_INTERFACE_OK==1)
      {
          fprintf(stderr,"IRC interface is already running fine :P \n ");
          return 0;
      }

    STOP_IRC_INTERFACE=0;
    IRC_INTERFACE_OK=0;
    if ( pthread_create( &irc_loop_id , NULL,  IRCInterfaceLoop ,(void*) 0) != 0 )
     {
         fprintf(stderr,"Error creating IRC loop \n");
         return 0;
     }

  return 1;
}



int StopIRCInterface()
{
     STOP_IRC_INTERFACE=1;
   return 1;
}
