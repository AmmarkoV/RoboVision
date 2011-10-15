#include "arduino_serial.h"
#include "../../InputParser/InputParser_C.h"
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */

int WORKS=0;
int FAILED=0;
volatile int STOP=0;
int wait_flag=1;  /* TRUE while no signal received */
char arduinodevice_name[100]={0};
int arduinothread_id=0;
int fd=0;

unsigned int ultrasonic1=0,ultrasonic2=0,accelerometerX=0,accelerometerY=0,arduino_tickcount=0;
unsigned int last_camera_pose_pitch=0,last_camera_pose_heading=0;
unsigned int camera_pose_pitch=0,camera_pose_heading=0;

int ArduinoInternalGetUltrasonicValue(int which_one)
{
  if (which_one==0 ) return ultrasonic1; else
  if (which_one==1 ) return ultrasonic2;

  return 0;
}

int ArduinoInternalGetAccelerometerX()
{
  return accelerometerX;
}


int ArduinoInternalSetCameraPose(int heading,int pitch)
{
  camera_pose_pitch=pitch;
  camera_pose_heading=heading;
  return 1;
}


int ArduinoInternalGetAccelerometerY()
{
  return accelerometerY;
}

int ArduinoMoveServo(int dev, int servo_num , int degrees)
{
  // THIS MUST TRANSMIT
  // M(servo_num)(degrees) TODO
  char command[10]={"M00\0"};

  command[1]=servo_num+'0';
  command[2]=(char) degrees;

  int res = write(fd,command,3);   /* Flash LED */
  if (res<3) { return 0; }
  return 1;
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

int ArduinoCodeStartup(int fd)
{
    fprintf(stderr,"Arduino Code startup running \n");

    int res = write(fd,(unsigned char *) "EAS",3);   /* End autonomous state */



    res = write(fd,(unsigned char *) "CHK",3);   /* returns after at least newtio.c_cc[VMIN] chars have been input */
    if (res<0) { fprintf(stderr,"Cannot write to arduino\n"); return 0; }
    if (res!=3) {  fprintf(stderr,"Could not send all 3 bytes to arduino\n"); return 0;  }

    usleep(100000);

    char buf[256]={0};
    fprintf(stderr,"Receiving..\n");
    int chk_acknowledged = 0;
    while ( !chk_acknowledged  )
     {
       res = write(fd,(unsigned char *) "CHK",3);
       res = read(fd,buf,8);   /* returns after at least newtio.c_cc[VMIN] chars have been input */
       //if (res == 0 )                     { fprintf(stderr," "); }
       if ( strncmp(buf,"ARDUINO",7)==0 ) { chk_acknowledged=1; } else
                                          {
                                            fprintf(stderr,"%s",buf);
                                          }
     }

    if ( strncmp(buf,"ARDUINO",7)==0 )
      {
      fprintf(stderr,"Got back answer \n"); WORKS=1;
      }
    res = write(fd,"LA0",3);   /* Flash LED */
    usleep(1000);
    res = write(fd,"LD0",3);   /* Flash LED */
    res = write(fd,"BAS",3);   /* Startup arduino automatic sending*/

    fprintf(stderr,"Arduino Code success\n");
    return 1;
}


void * Arduino_Thread(void * ptr)
{

int res;
struct termios oldtio,newtio;


unsigned int clear_packet_size=0;
char clear_packet[512];

unsigned int terminal_symbol_position=0;

fprintf(stderr,"Trying to open arduino @ %s \n",arduinodevice_name);
fd = open(arduinodevice_name, O_RDWR | O_NOCTTY  ); // | O_NDELAY
if (fd <0) { perror(arduinodevice_name); FAILED=1; return(0); }

tcgetattr(fd,&oldtio); /* save current port settings */

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


tcflush(fd, TCIFLUSH);
tcsetattr(fd,TCSANOW,&newtio);

usleep (10000);

if (!ArduinoCodeStartup(fd))
 {
     STOP=1;  FAILED=1;
 } else
 {
     WORKS=1;
 }



struct InputParserC * ipc=0;
ipc = InputParser_Create(512,5);

unsigned int overflow_buf_size=0;
char overflow_buf[255];

unsigned int buf_size=0;
char buf[255];

while (STOP==0)     {

                       if ( last_camera_pose_pitch != camera_pose_pitch)
                         {
                               ArduinoMoveServo(fd,0,camera_pose_pitch);
                               last_camera_pose_pitch=camera_pose_pitch;
                         }


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

 res=write(fd,"EAS",3);
 close(fd);

 tcsetattr(fd,TCSANOW,&oldtio);
 InputParser_Destroy(ipc);
 return 0;
}

int ArduinoThreadStart(char * devname)
{
 ultrasonic1=0;
 ultrasonic2=0;
 accelerometerX=0;
 accelerometerY=0;
 arduino_tickcount=0;
 arduinothread_id=0;
 strcpy(arduinodevice_name,devname);

 struct stat st;
 if(stat(arduinodevice_name,&st) == 0) { printf(" Arduino device is present\n");  } else
                                       { printf(" Arduino device is not present\n"); FAILED=1;  return 0; }

 int res=pthread_create((pthread_t *) &arduinothread_id, NULL,Arduino_Thread,0);
 if ( res != 0 ) { printf(" Arduino thread did not start\n"); FAILED=1;  }
 usleep(100000);

 return (res==0);
}


int ArduinoOk()
{
  if (FAILED) { return 0; }
  if (WORKS) { return 1; } else
             { return 0;}
  return 1;
}

int ArduinoStopThread()
{
  STOP=1;
  return 1;
}

