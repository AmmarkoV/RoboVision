#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../RoboVisionSensorLib.h"
#include <unistd.h>


#define BAUDRATE B38400

int TestHere()
{

int fd,res;
struct termios oldtio,newtio;


fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY  | O_NDELAY  ); //
if (fd <0) { perror("/dev/ttyUSB0");  return(0); }

if( tcgetattr(fd,&oldtio) < 0 )  /* save current port settings */
  { fprintf(stderr,"Could not get termios attribute \n"); }

bzero(&newtio, sizeof(newtio));

    // 8N1
    newtio.c_cflag &= ~PARENB;
    newtio.c_cflag &= ~CSTOPB;
    newtio.c_cflag &= ~CSIZE;
    newtio.c_cflag |= CS8;
    newtio.c_cflag |= BAUDRATE;

    // no flow control
    newtio.c_cflag &= ~CRTSCTS;

    newtio.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    newtio.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    newtio.c_oflag &= ~OPOST; // make raw
    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    newtio.c_cc[VMIN]  = 0;
    newtio.c_cc[VTIME] = 20;
/*
newtio.c_cflag = ~(PARENB | CSTOPB | CSIZE | CRTSCTS) | CS8 | CREAD | CLOCAL;
newtio.c_iflag = ~(IXON | IXOFF | IXANY);
newtio.c_lflag = ~(ICANON | ECHO | ECHOE | ISIG);
newtio.c_oflag = ~OPOST;
newtio.c_cc[VTIME] = 0;
newtio.c_cc[VMIN]  = 0;
*/

/*
newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
newtio.c_iflag = IGNPAR;
newtio.c_oflag = 0;
newtio.c_lflag = 0; // set input mode (non-canonical, no echo,...)
newtio.c_cc[VTIME]    = 0;   // inter-character timer unused
newtio.c_cc[VMIN]     = 50;   // blocking read until 5 chars received
*/

tcflush(fd, TCIFLUSH);
if ( tcsetattr(fd,TCSANOW,&newtio) < 0 )
  { fprintf(stderr,"Could not set termios attribute \n"); }


char buf[256]={0};
char CHK[6]={"CHK\0"};


 while (1)
       {
           usleep(1000*1000);
           res = write(fd,CHK,3);   /* returns after at least newtio.c_cc[VMIN] chars have been input */
           usleep(1000*1000);
           res = read(fd,buf,254);   /* returns after at least newtio.c_cc[VMIN] chars have been input */
           fprintf(stderr,"Sent : %s Received : %s \n",CHK,buf);
       }




 close(fd);

 tcsetattr(fd,TCSANOW,&oldtio);

}







int main()
{

//TestHere();
//return 0;

    printf("Hello world , I will now start sampling the sensors!!\n");
    if (! ConnectRoboVisionSensors("/dev/ttyUSB0") )
      {
          fprintf(stderr,"exiting\n");
          return 1;
      }




      while (1)
       {
        printf("Ultrasonic 1 : %u \n ", GetUltrasonicValue(0) );
        printf("Ultrasonic 2 : %u \n ", GetUltrasonicValue(1) );
        printf("Accelerometers ( X : %u , Y : %u ) \n ", GetAccelerometerX(0), GetAccelerometerY(0) );
        usleep(1000*1000);
        SetLights(0,1);
        SetLights(1,1);
        SetLights(2,1);
         usleep(1000*1000);
        SetLights(0,0);
        SetLights(1,0);
        SetLights(2,0);

       }
    DisconnectRoboVisionSensors();

    return 0;
}
