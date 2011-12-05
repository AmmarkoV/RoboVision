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


char arduinodevice_name[100]={0};

struct termios oldtio,newtio;
int fd=0;

int WORKS=0;
int FAILED=0;
volatile int STOP=0;
int wait_flag=1;  /* TRUE while no signal received */
int arduinothread_id=0;

int arduino_tickcount=0;

struct arduino_connected_devices activated_state={0};
struct arduino_connected_devices future_state={0};




/*
   MEMO

  int tcflush( fd, queue_selector );

  The queue_selector flags are:

  o  TCIFLUSH  0  flush any data not yet read from the input buffer

  o  TCOFLUSH  1  flush any data written to the output buffer but not
     yet transmitted

  o  TCIOFLUSH 2  flush both buffers
*/


int ArduinoProtocolSend(char * command , unsigned int length)
{
  if ( (length!=3) || ( strlen(command)!=3 ) )
    {
        fprintf(stderr,"Incorrect Payload for ArduinoProtocolSend , not sending it\n");
        return 0;
    }

  int res = write(fd,command,3);
  tcflush(fd, TCOFLUSH);
  if (res<3) { fprintf(stderr,"Command Failed to be send , this may mean arduino device is dead ( code %i ) \n",res); return 0; }

  return 1;
}





int ArduinoProtocolReceive(char * command , unsigned int length)
{
  return 0;
}


/*
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                           ACTUAL COMMANDS START
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
*/
int ArduinoNodServo(int dev, int servo_num , int degrees)
{
  // THIS MUST TRANSMIT
  // M(servo_num)(degrees)
  char command[10]={"N00\0"};

  command[1]=servo_num+'0';
  command[2]=(char) degrees;

  return ArduinoProtocolSend(command,3);
}

int ArduinoMoveServo(int dev, int servo_num , int degrees)
{
  // THIS MUST TRANSMIT
  // M(servo_num)(degrees)
  char command[10]={"M00\0"};

  command[1]=servo_num+'0';
  command[2]=(char) degrees;

  return ArduinoProtocolSend(command,3);
}

int ArduinoControlLights(int dev,int light_number,int light_state)
{
  // THIS MUST TRANSMIT
  // L(A or D)(1/0)
  char command[10]={"LA0\0"};

 if ( light_state ) { command[1]='A'; } else
                    { command[1]='D'; }
  command[2]=light_number+'0';

  return ArduinoProtocolSend(command,3);
}

int ArduinoAutoSampleMode(int dev,int true_for_on)
{
  // THIS MUST TRANSMIT
  // BAS / EAS (1/0)
  char command[10]={"BAS\0"};
  if ( !true_for_on ) { command[0]='E'; }
  return ArduinoProtocolSend(command,3);
}

int ArduinoFlushInput(int dev)
{
  // THIS MUST TRANSMIT FLU
  char command[10]={"FLU\0"};
  return ArduinoProtocolSend(command,3);
}


int ArduinoRestart(int dev)
{
  // THIS MUST TRANSMIT ZZZ
  char command[10]={"ZZZ\0"};
  return ArduinoProtocolSend(command,3);
}

int ArduinoCheck(int dev)
{
  // THIS MUST TRANSMIT CHK
  char command[10]={"CHK\0"};
  return ArduinoProtocolSend(command,3);
}
/*
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                           ACTUAL COMMANDS END
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
*/




/*
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                           HIGH LEVEL COMMANDS FOR RELAY START
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
*/
int ArduinoInternalGetUltrasonicValue(int which_one)
{
  if (which_one==0 ) return activated_state.ultrasonic1; else
  if (which_one==1 ) return activated_state.ultrasonic2;

  return 0;
}

int ArduinoInternalGetAccelerometerX()
{
  return activated_state.accelerometerX;
}


int ArduinoInternalGetAccelerometerY()
{
  return activated_state.accelerometerY;
}


int ArduinoInternalSetNod(int motor,int pitch,int wait_for_it)
{
  future_state.camera_nod_pitch=pitch;
  //future_state.camera_nod_heading=heading; HEADING SERVO NOT WORKING :P

  if ( wait_for_it )
    {
      int max_time = 1000;
      while ( ( max_time > 0 ) &&
              ( (future_state.camera_nod_pitch != activated_state.camera_nod_pitch)
                //|| (future_state.camera_nod_heading != activated_state.camera_nod_heading)
              )
            )
               {
                  --max_time;
                  usleep(1000);
               }
    }

  return 1;
}


int ArduinoInternalSetCameraPose(int heading,int pitch,int wait_for_it)
{
  future_state.camera_pose_pitch=pitch;
  //future_state.camera_pose_heading=heading; HEADING SERVO NOT WORKING :P

  if ( wait_for_it )
    {
      int max_time = 1000;
      while ( ( max_time > 0 ) &&
              ( (future_state.camera_pose_pitch != activated_state.camera_pose_pitch)
                //|| (future_state.camera_pose_heading != activated_state.camera_pose_heading)
              )
            )
               {
                  --max_time;
                  usleep(1000);
               }
    }


  return 1;
}


int ArduinoInternalSetLights(int light_num,int light_state,int wait_for_it)
{
  //fprintf(stderr,"ArduinoInternalSetLights(%u,%u) \n",light_num,light_state);
  if( light_num>=TOTAL_ARDUINO_LIGHTS)  { return 0; }
  //fprintf(stderr,"SET IS DONE ArduinoInternalSetLights(%u,%u) \n",light_num,light_state);
  future_state.lights[light_num]=light_state;

    if ( wait_for_it )
    {
      int max_time = 1000;
      while ( ( max_time > 0 ) &&
              ( future_state.lights[light_num]!= activated_state.lights[light_num] ) )
               {
                  --max_time;
                  usleep(1000);
               }
    }

  return 1;
}
/*
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                           HIGH LEVEL COMMANDS FOR RELAY END
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
*/


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
                           activated_state.ultrasonic1=InputParser_GetWordInt(ipc,2);
                           WORKS=1;
                       }

                      if ( InputParser_GetWordInt(ipc,1) == 2 )
                       {
                           activated_state.ultrasonic2=InputParser_GetWordInt(ipc,2);
                           WORKS=1;
                       }
                  }

                 if (InputParser_WordCompare(ipc,0,"accelerometers",14) )
                  {
                     activated_state.accelerometerX=InputParser_GetWordInt(ipc,1);
                     activated_state.accelerometerY=InputParser_GetWordInt(ipc,2);
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
    ArduinoAutoSampleMode(fd,0);


    fprintf(stderr,"Trying to receive a check response..\n");
    unsigned int chk_acknowledged = 0 , time_waited = 0 ;
    while ( !chk_acknowledged  )
     {

       if ( ArduinoCheck(fd) ) { chk_acknowledged=1; }
       ++time_waited;
       usleep(1000);

       if ( time_waited > 1000 ) { fprintf(stderr,"Arduino Failed to respond \n"); return 0;}

     }

    // IF We reach here it means things work!
    WORKS = 1;


    if (!ArduinoControlLights(fd,0,1)) { fprintf(stderr,"Arduino does not respond to lights command\n"); return 0; }
    usleep(10*1000);
    if (!ArduinoControlLights(fd,0,0)) { fprintf(stderr,"Arduino does not respond to lights command\n"); return 0; }

    //if (!ArduinoAutoSampleMode(fd,1)) { fprintf(stderr,"Arduino does not respond to AutoSample command\n"); return 0; }

    fprintf(stderr,"Arduino Code success\n");
    return 1;
}


unsigned int ConnectToArduino()
{
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

usleep (5000*1000);
fprintf(stderr,"Lets See if things will work\n");

if (!ArduinoCodeStartup(fd))
 {
     STOP=1;  FAILED=1;
     fprintf(stderr,"Giving up with Arduino\n");

     close(fd);
     tcsetattr(fd,TCSANOW,&oldtio);

     return 0;
 } else
 {
     WORKS=1;
 }

 return 1;
}


void * Arduino_Thread(void * ptr)
{

struct InputParserC * ipc=0;
ipc = InputParser_Create(512,5);

fprintf(stderr,"Arduino receive/send thread is now starting \n");
int arduino_loop_tick_count = 0;
while (STOP==0)     {

                       if ( activated_state.camera_pose_pitch != future_state.camera_pose_pitch)
                         {
                               ArduinoMoveServo(fd,0,future_state.camera_pose_pitch);
                               activated_state.camera_pose_pitch=future_state.camera_pose_pitch;
                         }

                       if ( activated_state.camera_nod_pitch != future_state.camera_nod_pitch)
                         {
                               ArduinoNodServo(fd,0,future_state.camera_nod_pitch);
                               activated_state.camera_nod_pitch=future_state.camera_nod_pitch;
                         }


                       if ( activated_state.lights[0] != future_state.lights[0])
                         {
                               ArduinoControlLights(fd,0,future_state.lights[0]);
                               activated_state.lights[0]=future_state.lights[0];
                         }

                    /*
                    if ( arduino_loop_tick_count%100 == 0 )
                       {
                         int i=write(fd,"XXX",3); //TRY TO FLUSH EVERY 1 SEC
                         if (i!=3) { fprintf(stderr,"Error flushing arduino\n"); }
                       }
                    */


                    usleep (50*1000);
                    ++arduino_loop_tick_count;
                    }

 ArduinoAutoSampleMode(fd,0);
 fprintf(stderr,"Closing arduino \n");
 close(fd);
 tcsetattr(fd,TCSANOW,&oldtio);

 usleep(10000);

 InputParser_Destroy(ipc);
 return 0;
}

int ArduinoThreadStart(char * devname)
{
 struct arduino_connected_devices empty_state={0};
 activated_state=empty_state;
 future_state=empty_state;

 arduino_tickcount=0;
 arduinothread_id=0;
 strcpy(arduinodevice_name,devname);

 struct stat st;
 if(stat(arduinodevice_name,&st) == 0) { printf(" Arduino device is present\n");  } else
                                       { printf(" Arduino device is not present\n"); FAILED=1;  return 0; }


 if ( !ConnectToArduino() )
   {
       fprintf(stderr,"Arduino device seems to be present but i could not connect to it!\n");
       FAILED=1;
       return 0;
   }

 int res=pthread_create((pthread_t *) &arduinothread_id, NULL,Arduino_Thread,0);
 if ( res != 0 ) { printf(" Arduino thread did not start\n"); FAILED=1;  }

 return (res==0);
}


int ArduinoOk()
{
  if (FAILED) { return 0; }

  if (WORKS)  { return 1; } else
              { return 0;}
  return 1;
}

int ArduinoStopThread()
{
  STOP=1;
  return 1;
}

