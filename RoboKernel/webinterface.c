#include "RoboKernel.h"
#include <pthread.h>
#include "webinterface.h"
#include "command_hal.h"
#include "visual_system.h"
#include "gsm_modem.h"
#include "configuration.h"
#include "../InputParser/InputParser_C.h"
#include "../MotorFoundation/MotorHAL.h"
#include <unistd.h>

int web_interface_thread_stop = 0;
int last_snapshot=0;
int count_snapshot=0;



pthread_t webinterface_loop_id=0;
void * WebInterfaceLoop(void *ptr );

int OpenWebInterface()
{
   EraseConsoleOutput();
   last_snapshot=0;
   count_snapshot=0;

   FILE * pFile=0;
   pFile = fopen ("memfs/public_html/commands.dat","w");
   if (pFile!=0 )
      {
        fclose(pFile);

            if ( pthread_create( &webinterface_loop_id , NULL,  WebInterfaceLoop ,0) != 0 )
                {
                      fprintf(stderr,"Error creating web interface loop \n");
                      return 0;
                }


        return 1;
       }
   return 0;
}


int CloseWebInterface()
{
 web_interface_thread_stop = 1;

 int i=0;
 i=system((const char *)"cp empty.jpeg memfs/public_html/feed0.jpeg");
 if ( i != 0 ) fprintf(stderr,"Error cleaning web interface :P \n");
 i=system((const char *)"cp empty.jpeg memfs/public_html/feed1.jpeg");
 if ( i != 0 ) fprintf(stderr,"Error cleaning web interface :P \n");
 i=system((const char *)"cp empty.jpeg memfs/public_html/feed2.jpeg");
 if ( i != 0 ) fprintf(stderr,"Error cleaning web interface :P \n");
 i=system((const char *)"cp empty.jpeg memfs/public_html/feed3.jpeg");
 if ( i != 0 ) fprintf(stderr,"Error cleaning web interface :P \n");
 EraseConsoleOutput();
 return 1;
}


int WebIntNeedsNewSnapshot()
{

  FILE * pFile=0;
  pFile = fopen ("memfs/public_html/viewers.dat","r");
  if (pFile!=0 )
    {
     fclose(pFile);
      if( remove( "memfs/public_html/viewers.dat" ) != 0 )
       {
        fprintf(stderr,"Could not clear viewers data\n");
        return 0;
       }
       fprintf(stderr,"Stream to viewers\n");

       return 1;
    }
 return 0;
}

int WebIntHasNewCommand()
{

  FILE * pFile=0;
  pFile = fopen ("memfs/public_html/commands.dat","r");

  struct InputParserC * ipc=0;
  ipc = InputParser_Create(512,5);
  char from_string[512]={0};
  unsigned int words_count=0;

  if (pFile!=0 )
    {

      /*
         READ COMMANDS HERE , USING AFSCRIPT!
      */
         char line[256]={0};
         while( fgets(line,sizeof(line),pFile) )
         {
            words_count = InputParser_SeperateWords(ipc,line,0);
            if ( words_count>0 ) { InputParser_GetWord(ipc,words_count-1,from_string,256); } else
                                 { strcpy(from_string,"unknown"); }
            fprintf(stderr,"Command Received : %s\n",line);
            char output_string[512]={0};
            IssueCommandInternal(line,from_string,output_string,512);
            fprintf(stderr,"Command %s has now returned from processing \n",line);
         }
      /*
         READ COMMANDS HERE!
      */

      fclose(pFile);

      if( remove( "memfs/public_html/commands.dat" ) != 0 )
       {
        fprintf(stderr,"Could not clear viewers data\n");
        return 0;
       }
       fprintf(stderr,"Stream to command\n");

       return 1;
    }
 return 0;
}

int UpdateSensorsOnNetworkInterface()
{
  FILE * pFile=0;
  pFile = fopen ("memfs/public_html/sensors.dat","w");
  if (pFile!=0 )
  {
    fprintf(pFile,"%u\n",RobotGetUltrasonic(0));
    fprintf(pFile,"%u\n",RobotGetUltrasonic(1));
    fprintf(pFile,"%d\n",RobotGetAccelerometerX(0));
    fprintf(pFile,"%d\n",RobotGetAccelerometerY(0));
    fprintf(pFile,"#\n"); /*Signal the end of the transmission*/
    fclose(pFile);
    return 1;
  }
  return 0;
}


int UpdateNetworkInterface(char * pica,char * picb,char * picc,char * picd,unsigned int xsize,unsigned int ysize)
{
 if (WebIntNeedsNewSnapshot()==1)
  {
    ++count_snapshot;
    fprintf(stderr,"Snapshot #%u taken \n",count_snapshot);

    signed int i=0;
    if ( VisCortX_SaveVideoRegisterToFile(LEFT_EYE,(char *) "memfs/public_html/feed0.ppm") == 0 ) fprintf(stderr,"Could not save feed 0 to .ppm");
    if ( VisCortX_SaveVideoRegisterToFile(RIGHT_EYE,(char *) "memfs/public_html/feed1.ppm")== 0 ) fprintf(stderr,"Could not save feed 1 to .ppm");
    if ( VisCortX_SaveVideoRegisterToFile(LAST_LEFT_OPERATION,(char *) "memfs/public_html/feed2.ppm")== 0 ) fprintf(stderr,"Could not save feed 2 to .ppm");
    if ( VisCortX_SaveVideoRegisterToFile(LAST_RIGHT_OPERATION,(char *) "memfs/public_html/feed3.ppm")== 0 ) fprintf(stderr,"Could not save feed 3 to .ppm");

    UpdateSensorsOnNetworkInterface();

    /* system */
    i=system((const char *)"scripts/webinterface_convert.sh");
    if ( i != 0 ) fprintf(stderr,"Error (%d) converting image of feeds\n",i);
  }

  WebIntHasNewCommand();

  return 1;
}



int TakeCareOfNetworkInterface(unsigned int clock_time)
{
  /*fprintf(stderr,"WebInterface calling initializations at %u last snapshot %u\n",clock_time,last_snapshot);*/
  unsigned int time_elapsed=clock_time-last_snapshot;
  if ( clock_time < last_snapshot ) { /* Clock wrapped around This will fire at once !*/ } else
  if ( time_elapsed < web_interface_snaptime ) { return 0; }
  /*fprintf(stderr,"WebInterface passed to initializations elapsed %u ms\n",time_elapsed);*/
  last_snapshot=clock_time;
  return UpdateNetworkInterface(0,0,0,0,0,0);
}


int EraseConsoleOutput()
{
  FILE * pFile=0;
  pFile = fopen ("memfs/public_html/consoleout.dat","w");
  if (pFile!=0 )
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
  if (pFile!=0 )
  {
    printf("%s\n",outstr);
    fprintf(pFile,"%s\n",outstr);
    fclose(pFile);
    return 1;
  }
  return 0;
}


void * WebInterfaceLoop(void *ptr )
{
  /*SMS STUFF START*/
    char number[20]={0};
    char data[2000]={0};
  /*SMS STUFF END*/


    while ( web_interface_thread_stop == 0)
     {
        usleep(100 * 1000); // 100 ms
        TakeCareOfNetworkInterface(clock_count);
        ReceiveSMS(number,20,data,2000);
     }

  web_interface_thread_stop=2;
  return 0;
}
