#include "arduino_serial.h"

#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/signal.h>
#include <sys/types.h>

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP=0;
int wait_flag=1;  /* TRUE while no signal received */

void signal_handler_IO (int status);   /* definition of signal handler */

struct device_name
{
   char devname[100];
};

int arduinothread_id=0;

void * Arduino_Thread(void * ptr);

void * Arduino_Thread(void * ptr)
{
int fd,res;
struct termios oldtio,newtio;
char buf[255];

fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
if (fd <0) {perror(MODEMDEVICE); exit(-1); }

tcgetattr(fd,&oldtio); /* save current port settings */

bzero(&newtio, sizeof(newtio));
newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
newtio.c_iflag = IGNPAR;
newtio.c_oflag = 0;

/* set input mode (non-canonical, no echo,...) */
newtio.c_lflag = 0;

newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
newtio.c_cc[VMIN]     = 45;   /* blocking read until 5 chars received */

tcflush(fd, TCIFLUSH);
tcsetattr(fd,TCSANOW,&newtio);


while (STOP==FALSE) {       /* loop for input */
  res = read(fd,buf,255);   /* returns after 5 chars have been input */
  buf[res]=0;       /* so we can printf... */
  printf(":%s:%d\n", buf, res);
  if (buf[0]=='z') STOP=TRUE;
}
tcsetattr(fd,TCSANOW,&oldtio);
}




/***************************************************************************
* signal handler. sets wait_flag to FALSE, to indicate above loop that     *
* characters have been received. *
***************************************************************************/

void signal_handler_IO (int status)
{
  printf("received SIGIO signal.\n");
  wait_flag = 0;
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

