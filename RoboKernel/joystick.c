#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "RoboKernel.h"
#include "configuration.h"
#include "joystick.h"


pthread_t joystick_polling_loop_id=0;
void * JoystickPollingLoop(void *ptr );

int JOYSTICK_OK=0;
int STOP_JOYSTICK_THREAD=0;

static int joystick_fd = -1;

signed int Minimum_Axis_X=-32767;
signed int Maximum_Axis_X=32767;
signed int Minimum_Axis_Y=-32767;
signed int Maximum_Axis_Y=32767;




signed int RAW_Stick1_AxisX=0;
signed int RAW_Stick2_AxisX=0;
signed int RAW_Stick3_AxisX=0;
signed int RAW_Stick1_AxisY=0;
signed int RAW_Stick2_AxisY=0;
signed int RAW_Stick3_AxisY=0;

signed int Calibrated_Stick1_AxisX=0;
signed int Calibrated_Stick2_AxisX=0;
signed int Calibrated_Stick3_AxisX=0;
signed int Calibrated_Stick1_AxisY=0;
signed int Calibrated_Stick2_AxisY=0;
signed int Calibrated_Stick3_AxisY=0;








void CalibrateJoystickPos(signed int * axis_x,signed int * axis_y,unsigned int MAX_POWER)
{

 if ( !JOYSTICK_OK ) { return; }

 if ( ( Minimum_Axis_X==0 ) | ( Maximum_Axis_X==0 ) | ( Minimum_Axis_Y==0 ) | ( Maximum_Axis_Y==0 ) ) { return; }

 if  ( *axis_x < 0 ) { *axis_x = -(signed int)MAX_POWER * *axis_x / Minimum_Axis_X; } else
 if  ( *axis_x > 0 ) { *axis_x = MAX_POWER * *axis_x / Maximum_Axis_X; }

 if  ( *axis_y < 0 ) { *axis_y = -(signed int)MAX_POWER * *axis_y / Minimum_Axis_Y; } else
 if  ( *axis_y > 0 ) { *axis_y = MAX_POWER * *axis_y / Maximum_Axis_Y; }

 return;
}


int ExecuteButtonCommand(int button_num)
{
    fprintf(stderr,"Execute button %u \n",button_num);
  switch (button_num)
  {
      case 1: break;
      case 2: break;
      case 3: break;
/*L1*/case 4: IssueCommand((char *) "PLAYSOUND(whistle_at_girl)",0,0,(char *)"JOYSTICK"); break;
/*R1*/case 5: IssueCommand((char *) "PLAYSOUND(alarm)",0,0,(char *)"JOYSTICK");  break;
/*L2*/case 6: IssueCommand((char *) "STOP SOUNDS",0,0,(char *)"JOYSTICK");  break;
/*R2*/case 7: IssueCommand((char *) "PLAYSOUND(kleftis_itan_o_pateras_sou)",0,0,(char *)"JOYSTICK"); break;
      case 8: break;
      case 9: break;
      case 10: break;
      case 11: break;
      case 12: break;
      case 13: break;
      case 14: break;
      case 15: break;
      case 16: break;
      default : break;
  };
  return 1;
}




int HandleJoystickEvent(struct js_event *jse)
{
        if ( jse->type == JS_EVENT_INIT) { return 0; }

		if (jse->type == JS_EVENT_AXIS)
		{

			switch (jse->number)
			{
			case 0: RAW_Stick1_AxisX = jse->value;
				break;
			case 1: RAW_Stick1_AxisY = jse->value;
				break;
			case 2: RAW_Stick2_AxisX = jse->value;
				break;
			case 3: RAW_Stick2_AxisY = jse->value;
				break;
			case 4: RAW_Stick3_AxisX = jse->value;
				break;
			case 5: RAW_Stick3_AxisY = jse->value;
				break;
			default:
				break;
			}

           Calibrated_Stick1_AxisX=RAW_Stick1_AxisX;
		   Calibrated_Stick1_AxisY=RAW_Stick1_AxisY;
		   CalibrateJoystickPos(&Calibrated_Stick1_AxisX,&Calibrated_Stick1_AxisY,255);
           printf("Joystick Stick 1 %d,%d\n",Calibrated_Stick1_AxisX,Calibrated_Stick1_AxisY);


           Calibrated_Stick2_AxisX=RAW_Stick2_AxisX;
		   Calibrated_Stick2_AxisY=RAW_Stick2_AxisY;
		   CalibrateJoystickPos(&Calibrated_Stick2_AxisX,&Calibrated_Stick2_AxisY,255);
           printf("Joystick Stick 2 %d,%d\n",Calibrated_Stick2_AxisX,Calibrated_Stick2_AxisY);

           Calibrated_Stick3_AxisX=RAW_Stick3_AxisX;
		   Calibrated_Stick3_AxisY=RAW_Stick3_AxisY;
		   CalibrateJoystickPos(&Calibrated_Stick3_AxisX,&Calibrated_Stick3_AxisY,255);
           printf("Joystick Stick 3 %d,%d\n",Calibrated_Stick3_AxisX,Calibrated_Stick3_AxisY);


          char inptstr[512]={0};
          if ( (Calibrated_Stick1_AxisX!=0 ) || ( Calibrated_Stick1_AxisY!=0 ) )
           {
            sprintf(inptstr , (char *) "JOYSTICK INPUT(%u,%u)",Calibrated_Stick1_AxisX,Calibrated_Stick1_AxisY);
            IssueCommand(inptstr,0,0,(char *)"GUI");
           }
          if ( (Calibrated_Stick2_AxisX!=0 ) || ( Calibrated_Stick2_AxisY!=0 ) )
           {
            //Stick 2 not used yet :)
            //sprintf(inptstr , (char *) "JOYSTICK INPUT(%u,%u)",Calibrated_Stick1_AxisX,Calibrated_Stick1_AxisY);
            //IssueCommand(inptstr,0,0,(char *)"GUI");
           }
		}

		 else

		if (jse->type == JS_EVENT_BUTTON)
		{
			if (jse->number < 10)
			{
				switch (jse->value)
				{
				  case 0:
				 // case 1: wjse->button[jse.number] = jse.value; break;
				  default: break;
				}
			}

		    if(jse->value) { ExecuteButtonCommand(jse->number); }


        }
 return 1;
}








int open_joystick(char *joystick_device)
{
	joystick_fd = open(joystick_device, O_RDONLY | O_NONBLOCK); /* read write for force feedback? */
	if (joystick_fd < 0) { return joystick_fd; }

	/* maybe ioctls to interrogate features here? */
	return joystick_fd;
}

int read_joystick_event(struct js_event *jse)
{
	int bytes;

	bytes = read(joystick_fd, jse, sizeof(*jse));

	if (bytes == -1) { return 0; }

	if (bytes == sizeof(*jse)) { return 1; }

	printf("Unexpected bytes from joystick:%d\n", bytes);
	return -1;
}

void close_joystick()
{
	close(joystick_fd);
}


void * JoystickPollingLoop(void *ptr )
{
    int fd, rc;

	struct js_event jse;

	fd = open_joystick(joystick_device);
	if (fd < 0)
	{
		printf("joystick open failed.\n");
		STOP_JOYSTICK_THREAD=1;
		return 0;
	}

   JOYSTICK_OK=1;
   while ( !STOP_JOYSTICK_THREAD )
     {

		rc = read_joystick_event(&jse);
		usleep(1000);
		if (rc == 1)
		 {
			//printf("Event: time %8u, value %8hd, type: %3u, axis/button: %u\n",jse.time, jse.value, jse.type, jse.number);
            HandleJoystickEvent(&jse);

		 }

     }
  JOYSTICK_OK=0;


  return 0;
}



int StartJoystickControl()
{
    STOP_JOYSTICK_THREAD=0;
    JOYSTICK_OK=0;
    if ( pthread_create( &joystick_polling_loop_id , NULL,  JoystickPollingLoop ,(void*) 0) != 0 )
     {
         fprintf(stderr,"Error creating Joystick loop \n");
         return 0;
     }
    return 1;
}

int StopJoystickControl()
{
    STOP_JOYSTICK_THREAD=1;
    JOYSTICK_OK=0;
    return 0;
}
