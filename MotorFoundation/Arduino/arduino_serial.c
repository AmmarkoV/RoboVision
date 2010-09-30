#include "arduino_serial.h"

#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/signal.h>
#include <sys/types.h>

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

volatile int STOP=0;
int wait_flag=1;  /* TRUE while no signal received */

struct device_name { char devname[100]; };

int arduinothread_id=0;

void * Arduino_Thread(void * ptr);

void * Arduino_Thread(void * ptr)
{
int fd,res;
struct termios oldtio,newtio;

unsigned int overflow_buf_size=0;
char overflow_buf[255];

unsigned int buf_size=0;
char buf[255];

unsigned int clear_packet_size=0;
char clear_packet[512];

unsigned int terminal_symbol_position=0;

fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
if (fd <0) {perror(MODEMDEVICE); return(0); }

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
                       res = read(fd,buf,255);   /* returns after at least newtio.c_cc[VMIN] chars have been input */

                       if (res<0) { fprintf(stderr,"Error Reading Serial Stream \n"); } else
                       if (res>255) { fprintf(stderr,"Error , overflow Reading Serial Stream \n"); } else
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
                          for (i=0; i<res; i++)
                            {
                              if (buf[i]=='#') {
                                                 terminal_symbol_position=i;
                                                 break;
                                               }
                            }

                          if ( terminal_symbol_position != 0 )
                            { /* We have located a # , our packet is complete :D*/
                              for (i=0; i<terminal_symbol_position; i++) {  clear_packet[clear_packet_size++]=buf[i]; }
                              printf("START===================\n");
                              printf("%s\n",clear_packet);
                              printf("END=====================\n");
                              overflow_buf_size=0;  // All older overflow has now been processed
                            } else
                            { /* We still havent found a # so all current input goes all into overflow_buf for the next loop*/
                              for (i=0; i<clear_packet_size; i++) {  overflow_buf[i]=clear_packet[i]; }
                              overflow_buf_size=clear_packet_size;
                            }

                           unsigned int starting_point_of_overflow=0;
                           if ( terminal_symbol_position != 0 ) { starting_point_of_overflow=terminal_symbol_position+1; }
                           for (i=starting_point_of_overflow; i<buf_size; i++) {  overflow_buf[overflow_buf_size++]=buf[i]; }

                          printf("\n%d characters read\n", buf_size);


                          if (buf[0]=='z') STOP=1;
                       }
                    }

 tcsetattr(fd,TCSANOW,&oldtio);
 return 0;
}

int kickstart_arduino_thread()
{
 arduinothread_id=0;
 pthread_create((pthread_t *) &arduinothread_id, NULL,Arduino_Thread,0);
 return 0;
}

int kill_arduino_thread()
{
  STOP=1;
  return 0;
}

