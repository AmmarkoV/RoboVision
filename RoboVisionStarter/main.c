#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE_SIZE 2048
#define MAX_STR 512

char ENVDIR[MAX_STR]="../robot/";
char INITIALIZATION_CONFIGURATION_PATH[MAX_STR]="../robot/guard.ini";
char V4L2_CONVERT_SO[MAX_LINE_SIZE]="/usr/lib32/libv4l/v4l2convert.so";
char V4L1_CONVERT_SO[MAX_LINE_SIZE]="/usr/lib32/libv4l/v4l2convert.so";


int v4l2_compat = 0;
int v4l1_compat = 0;


char FileExists(char * filename)
{
 FILE *fp = fopen(filename,"r");
 if( fp ) { /* exists */ fclose(fp); return 1; } // else { /* doesnt exist */ }
 return 0;
}



int find_environment_robot_directory()
{
   if (ENVDIR==0) { fprintf(stderr,"find_environment_robot_directory called with empty string.. \n"); return 0; }
   if (FileExists("/robot/guard.ini"))   { strcpy(ENVDIR,"/robot/"); }   else
   if (FileExists("../robot/guard.ini")) { strcpy(ENVDIR,"../robot/"); } else
                                         { ENVDIR[0]=0; ENVDIR[1]=0; return 0; }


   //Trying to find v4l2convert.so location
   if (FileExists("/usr/lib/libv4l/v4l2convert.so"))                    { strcpy(V4L2_CONVERT_SO,"/usr/lib/libv4l/v4l2convert.so"); }   else
   if (FileExists("/usr/lib/x86_64-linux-gnu/libv4l/v4l2convert.so"))   { strcpy(V4L2_CONVERT_SO,"/usr/lib/x86_64-linux-gnu/libv4l/v4l2convert.so"); }   else
   if (FileExists("/usr/lib32/libv4l/v4l2convert.so"))                  { strcpy(V4L2_CONVERT_SO,"/usr/lib32/libv4l/v4l2convert.so"); }   else
   if (FileExists("/usr/lib/i386-linux-gnu/libv4l/v4l2convert.so"))     { strcpy(V4L2_CONVERT_SO,"/usr/lib/i386-linux-gnu/libv4l/v4l2convert.so"); }


   return (ENVDIR[0]!=0);
}

void ParseConfigString(char * inpt)
{
   if (strstr(inpt,"USE_V4L2_COMPATIBILITY")!=0) { fprintf(stderr,"Will use v4l2 compatibility mode..!\n"); v4l2_compat = 1; } else
   if (strstr(inpt,"USE_V4L1_COMPATIBILITY")!=0) { fprintf(stderr,"Will use v4l1 compatibility mode..!\n"); v4l1_compat = 1; }

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
    }
  else
    {
      fprintf(stderr,"Cannot open guard.ini \n");
    }


}


int guarddog_running()
{
    int guarddog_is_running=0;

    char output[1024]={0};
    char command[1024]={0};
    strcpy(command,"ps -A | grep robovision");

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
    while (fgets(output, 1024 , fp) != 0)
    {
        ++i;
        if (strstr("robovision",output)!=0) { guarddog_is_running=1; break; }
        fprintf(stderr,"\n\nline %u = %s \n",i,output);
    }
    /* close */
    pclose(fp);

    return guarddog_is_running;
}


int start_guarddog(int noinput,int window)
{
  char command[1024]={0};
  strcpy(command,"sudo sh -c '");

  if ( (v4l2_compat)|| (v4l1_compat) )
  {
    strcat(command,"LD_PRELOAD=");
    if (v4l2_compat) { strcat(command,V4L2_CONVERT_SO); } else
    if (v4l1_compat) { strcat(command,V4L1_CONVERT_SO); }
    strcat(command," ");
  }


  if (window) { strcat(command,"robovisionx"); } else
              { strcat(command,"robovision");
                if (noinput) { strcat(command," noinput");  }
              }

  strcat(command,"'");

  fprintf(stderr,"Will run : %s \n",command);
  int i = system (command);
  return 0;
}

int stop_guarddog()
{
  int i = system ("sudo killall robovision");
      i = system ("sudo killall robovisionx");
      i = system ("sudo killall gsmsmsd");
  return 0;
}











int main(int argc, char *argv[])
{
    printf("RoboVisionStarter kickstarted..!\n");


    if ( argc <1 )   { fprintf(stderr,"Something weird is happening , argument zero should be executable path :S \n"); return 1; } else
    if ( argc == 1 ) { fprintf(stderr,"Usage robovisionstarter start|stop \n"); } else
                     {

                       LoadConfiguration();
                       int start=0;
                       int stop=0;
                       int no_cli_input=1;
                       int window=0;


                       int i=0;
                       for (i=0; i<argc; i++)
                       {
                          if (strcmp(argv[i],"start")==0) { start=1; } else
                          if (strcmp(argv[i],"stop")==0)  { stop=1;  } else
                          if (strcmp(argv[i],"cliinput")==0) { no_cli_input=0;  } else
                          if (strcmp(argv[i],"window")==0)  { window=1;  }
                       }


                       if (start) { start_guarddog(no_cli_input,window); } else
                       if (stop)  { stop_guarddog(); }

                     }
    return 0;
}
