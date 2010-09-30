#include "arduino_serial.h"
#include "../../InputParser/InputParser_C.h"


#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/signal.h>
#include <sys/types.h>

#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */

int WORKS=0;
int FAILED=0;
volatile int STOP=0;
int wait_flag=1;  /* TRUE while no signal received */
char arduinodevice_name[100]={0};
int arduinothread_id=0;

unsigned int ultrasonic1=0,ultrasonic2=0,accelerometerX=0,accelerometerY=0,arduino_tickcount=0;

int InternalGetUltrasonicValue(int dev)
{
  if (dev==0 ) return ultrasonic1; else
  if (dev==1 ) return ultrasonic2;

  return 0;
}

int InternalGetAccelerometerX(int dev)
{
  return accelerometerX;
}


int InternalGetAccelerometerY(int dev)
{
  return accelerometerY;
}


int AnalyzeArduinoInput(struct InputParserC * ipc,char * arduinostring,unsigned int length)
{
 char line[256];
 unsigned int total_lines=0;
 unsigned int line_pointer=0;
 unsigned int arduino_string_pointer=0;

 while (arduino_string_pointer<length)
  {
       if (arduinostring[arduino_string_pointer]!='\n')
        {
           line[line_pointer++]=arduinostring[arduino_string_pointer];
        } else
        { /*Have a full line*/
           ++total_lines;
           line[line_pointer]=0;
           int res = InputParser_SeperateWords(ipc,line,0);
             if ( res > 0 )
              {
                 if (InputParser_WordCompare(ipc,0,"proximity",9) )
                  {
                      if ( InputParser_GetWordInt(ipc,1) == 1 )
                       {
                           ultrasonic1=InputParser_GetWordInt(ipc,2);
                           WORKS=1;
                       }

                      if ( InputParser_GetWordInt(ipc,1) == 2 )
                       {
                           ultrasonic2=InputParser_GetWordInt(ipc,2);
                           WORKS=1;
                       }
                  }

                 if (InputParser_WordCompare(ipc,0,"accelerometers",14) )
                  {
                     accelerometerX=InputParser_GetWordInt(ipc,1);
                     accelerometerY=InputParser_GetWordInt(ipc,2);
                     WORKS=1;
                  }

                 if (InputParser_WordCompare(ipc,0,"tickcount",9) )
                  {
                     arduino_tickcount=InputParser_GetWordInt(ipc,1);
                     WORKS=1;
                  }

              }
           //printf("Line %u : %s \n",total_lines,line);
           line_pointer=0;
        }
       ++arduino_string_pointer;
  }

 return 0;
}

void * Arduino_Thread(void * ptr)
{

struct InputParserC * ipc=0;
ipc = InputParser_Create(512,5);

int fd,res;
struct termios oldtio,newtio;

unsigned int overflow_buf_size=0;
char overflow_buf[255];

unsigned int buf_size=0;
char buf[255];

unsigned int clear_packet_size=0;
char clear_packet[512];

unsigned int terminal_symbol_position=0;

fd = open(arduinodevice_name, O_RDWR | O_NOCTTY );
if (fd <0) { perror(arduinodevice_name); FAILED=1; return(0); }

tcgetattr(fd,&oldtio); /* save current port settings */

bzero(&newtio, sizeof(newtio));
newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
newtio.c_iflag = IGNPAR;
newtio.c_oflag = 0;

/* set input mode (non-canonical, no echo,...) */
newtio.c_lflag = 0;

newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
newtio.c_cc[VMIN]     = 50;   /* blocking read until 5 chars received */

tcflush(fd, TCIFLUSH);
tcsetattr(fd,TCSANOW,&newtio);


while (STOP==0)     {
                       res = read(fd,buf,254);   /* returns after at least newtio.c_cc[VMIN] chars have been input */

                       if (res==0) { /*QUIET :P */  } else
                       if (res<0) { fprintf(stderr,"Error Reading Serial Stream \n"); WORKS=0;  } else
                       if (res>255) { fprintf(stderr,"Error , overflow Reading Serial Stream \n"); WORKS=0; } else
                       {
                          buf_size = res;
                          buf[res]=0;       /* so we can printf... */

                          /* START BUILDING THE ACTUAL FULL PACKET RECEIVED
                             IT IS TERMINATED WITH A # SO WE ARE TRYING TO MAKE A COMPLETE PACKET
                             FROM THE CHUNKS WE READ TIME AFTER TIME
                          */

                          clear_packet_size=0;


                          unsigned int i=0;
                          if (overflow_buf_size>0)
                          { /* WE HAVE AN OVERFLOWED MESSAGE TO ADD BEFORE THE CURRENT BUFFER TO THE PACKET*/
                            for (i=0; i<overflow_buf_size; i++) { clear_packet[i]=overflow_buf[i]; }
                            clear_packet_size=overflow_buf_size;
                          } /* CLEAR PACKET NOW CONTAINS ALL THE DATA THAT HAS BEEN OVERFLOWED FROM LAST TIME */


                          /*The character stream is not steady so we search for a # that marks the end of a "packet" */
                          terminal_symbol_position=0;
                          for (i=0; i<buf_size; i++)
                            {
                              if (buf[i]=='#') {
                                                 terminal_symbol_position=i;
                                                 break;
                                               }
                            }

                          if ( terminal_symbol_position != 0 )
                            { /* We have located a # , our packet is complete :D*/
                              for (i=0; i<terminal_symbol_position; i++) {  clear_packet[clear_packet_size++]=buf[i]; }
                              clear_packet[clear_packet_size]=0; /* for printf */
                              AnalyzeArduinoInput(ipc,clear_packet,clear_packet_size);
                              //printf("START===================\n");
                             // printf("%s\n",clear_packet);
                             // printf("END=====================\n");
                              overflow_buf_size=0;  // All older overflow has now been processed
                            } else
                            { /* We still havent found a # so all current input goes all into overflow_buf for the next loop*/
                              for (i=0; i<clear_packet_size; i++) {  overflow_buf[i]=clear_packet[i]; }
                              overflow_buf_size=clear_packet_size;
                            }

                           unsigned int starting_point_of_overflow=0;
                           if ( terminal_symbol_position != 0 ) { starting_point_of_overflow=terminal_symbol_position+1; }
                           for (i=starting_point_of_overflow; i<buf_size; i++) {  overflow_buf[overflow_buf_size++]=buf[i]; }

                          //printf("\n%d characters read\n", buf_size);


                          if (buf[0]=='z') STOP=1;
                       }
                    }

 tcsetattr(fd,TCSANOW,&oldtio);
 InputParser_Destroy(ipc);
 return 0;
}

int kickstart_arduino_thread(char * devname)
{
 ultrasonic1=0;
 ultrasonic2=0;
 accelerometerX=0;
 accelerometerY=0;
 arduino_tickcount=0;
 arduinothread_id=0;
 strcpy(arduinodevice_name,devname);
 pthread_create((pthread_t *) &arduinothread_id, NULL,Arduino_Thread,0);
 return 0;
}

int arduino_ok()
{
  if (FAILED) { return 0; }
  if (WORKS) { return 1; } else
             { return 0;}
  return 1;
}

int kill_arduino_thread()
{
  STOP=1;
  return 0;
}

