#include "MD23.h"
#include "MD23_commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void * MD23_loop(void * ptr);
char * MD23_Lib_Version = "0.04";
// ===============================================
// ======================================
// LOW LEVEL MD23 COMMANDS
// ======================================
// ===============================================
char * MD23_GetVersion()
{
  return MD23_Lib_Version;
}

void MotorNowIsPast(struct  md23_device * dev,unsigned int motor_num)
{
    dev->past_motors[motor_num]=dev->motors[motor_num];
}

void MotorNowIsFinished(struct  md23_device * dev,unsigned int motor_num)
{
    dev->motors[motor_num].finished=1;
    dev->motors[motor_num].nolimits=1;
    // Motor has no future :P

    dev->future_motors[motor_num].power=128;
    dev->future_motors[motor_num].encoder=dev->motors[motor_num].encoder;
}



void Thread_ReachTarget(struct  md23_device * dev)
{
   if ( MD23_ok(dev) == 0 ) return;

     signed int change_power[2];
     change_power[0]=-1;
     change_power[1]=-1;

  if (dev->flush==1)
    {
      dev->flush=0;
      MD23_Zero_encoders(dev,0);
    }

  if ( ( dev->motors[0].finished==1 )&&( dev->motors[1].finished==1 ) ) { return; }

  int motor_id=0;

  for ( motor_id=0; motor_id<2; motor_id++)
  {
    if ( dev->motors[motor_id].finished==1 ) {} else
    if (  dev->motors[motor_id].nolimits==0 )
    {
       if  ( dev->past_motors[motor_id].encoder <  dev->future_motors[motor_id].encoder  )
       {
         // KSEKINISAME ME THETIKI FORA
         if  ( dev->motors[motor_id].encoder >=  dev->future_motors[motor_id].encoder  )
         {  // TO STATE TWN MOTORS KSEPERASE TO ORIO!
            fprintf(stderr,"PANW / TO STATE TWN MOTORS KSEPERASE TO ORIO!\n");
            MotorNowIsFinished(dev,motor_id);
            change_power[motor_id]=128; // KILL MOTOR
         }
       } else
       if  ( dev->past_motors[motor_id].encoder >  dev->future_motors[motor_id].encoder  )
       {
         // KSEKINISAME ME ARNITIKI FORA
         if  ( dev->motors[motor_id].encoder <=  dev->future_motors[motor_id].encoder  )
         {  // TO STATE TWN MOTORS KSEPERASE TO ORIO!
            fprintf(stderr,"KATW / TO STATE TWN MOTORS KSEPERASE TO ORIO!\n ");
            MotorNowIsFinished(dev,motor_id);
            change_power[motor_id]=128; // KILL MOTOR
         }
       }

       if ( (dev->past_motors[motor_id].power==dev->motors[motor_id].power) && (dev->future_motors[motor_id].power!=dev->motors[motor_id].power) )
        { //WE NEED TO GO TO A NEW POWER STATE!
          change_power[motor_id]=dev->future_motors[motor_id].power;
        }
    } else
    { // MOTOR HAS NO LIMITS FREE SPIN !
      if ( dev->motors[motor_id].power !=  dev->future_motors[motor_id].power )
         {
          change_power[motor_id]=dev->future_motors[motor_id].power;
         }
    }
  }


  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // AN EXOUME TIN IDIA ENTOLI ME TIN KATASTASI POU EIMASTE DEN XREIAZETAI NA TIN KSANASTEILOUME
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  if ( change_power[0] == dev->motors[0].power ) change_power[0] = -1;
  if ( change_power[1] == dev->motors[1].power ) change_power[1] = -1;
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // APPLY CHANGES!
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

 if ( ( change_power[0]==-1 ) && ( change_power[1]==-1 ) ) { } else
                                                            printf ("CHANGE TO MOTORS WILL BE  %d , %d \n",change_power[0],change_power[1]);
 if ( ( change_power[0]!=-1 ) && ( change_power[1]!=-1 ) )
 {
  if (MD23_movemotor(dev,2,change_power[0],change_power[1]) == 0) printf("Failed to set both motors!\n");
 }
  else
  if ( change_power[0]!=-1  )
 {
  if (MD23_movemotor(dev,0,change_power[0],change_power[0]) == 0) printf("Failed to set motor #0!\n");
 }
  else
  if ( change_power[1]!=-1  )
 {
  if (MD23_movemotor(dev,1,change_power[1],change_power[1]) == 0) printf("Failed to set motor #1!\n");
 }

 return;
}

/*


*/



struct md23_device * MD23_Init(char * dev_name,unsigned int threading_on)
{
  struct md23_device * dev=0;
  fprintf(stderr,"Care malloc \n");
  dev = ( struct md23_device * ) malloc( sizeof(struct md23_device) );
  strcpy(dev->i2c_dev.name,dev_name);
  if ( i2c_open(&dev->i2c_dev)  == 0 )
    {
      printf("Could not connect to Device %s \n",dev->i2c_dev.name);
      return 0;
    }

  if ( i2c_Check_If_Availiable(&dev->i2c_dev)==1 )
    {

      printf("I2C Availiable!\n");
      if ( MD23_search_on_I2C(dev) == 1 )
        {
          printf("Got Md23 address at %d \n",dev->md23_address);
          MD23_ReadStatus(dev,0);

          // AT START WE NEED TO SET OUR MOTORS AS INACTIVE!
          dev->past_motors[0].power=128;
          dev->past_motors[1].power=128;
          dev->motors[0].power=128;
          dev->motors[1].power=128;
          dev->future_motors[0].power=128;
          dev->future_motors[1].power=128;

          printf("Read Initial Status of MD23 \n");
          if ( threading_on == 1 )
            {
              printf("Initializing receive thread for MD23\n");
              pthread_create( &dev->loop_thread, NULL,  MD23_loop ,(void*) dev);
            }
        }
          else
        {
          printf("Could not find MD23 Device on I2C %s \n",dev->i2c_dev.name);
          MD23_Close(dev);
          return 0;
        }

    }
  else
    {
      printf("Could not find I2C Device on %s \n",dev->i2c_dev.name);
      MD23_Close(dev);
      return 0;
    }

 return dev;
}


void MD23_Close(struct md23_device * dev)
{
    if ( MD23_ok(dev) == 0 ) return;

    i2c_close(&dev->i2c_dev);
    sleep(1);
    free(dev);
}


int ZeroEncoders(struct md23_device * dev)
{
  if ( MD23_ok(dev) == 0 ) return 0;
  dev->flush=1;
  return 1;
}


inline void flip_power_direction(unsigned char *ret_power,signed char *ret_direction)
{
// Kanonika sto power 000 - 128 Simainei kinisi pros ta pisw
// Kanonika sto power 128 - 255 Simainei kinisi pros ta empros
// Tha prepei loipon na metatrepsoume tin kinisi pros ta empros se 129 - 255 kai tin kinisi pros ta pisw se 0 - 127
  fprintf(stderr,"flip_power_direction %u %d -> ",*ret_power,*ret_direction);
  unsigned char power=*ret_power;
  signed char direction=*ret_direction;

  if ( power == 0 )
    {
      power=128;  // An dn exoume valei power tote den tha metakinithoume
    } else
  if ( direction == 0 )
    {
      power=128;  // An dn exoume valei direction tote den tha metakinithoume
    } else

  {

  if ( direction > 0 )
    {
      unsigned char init_power=power;
      power=127+(power/2);
      // if ( power == 127 ) { power = 128; }  Power == 0 will be returned so it cannot happen!
      if ( (power==128) && (init_power>0) )
        {
          power++;
        }
    }
  else
    if ( direction < 0 )
      {
        unsigned char init_power=power;
        power=128-(power/2);
        // if ( power == 127 ) { power = 128; }  Power == 0 will be returned so it cannot happen!
        if ( (power==128) && (init_power>0) )
          {
            power--;
          }
      }

  }

  *ret_power=power;
  *ret_direction=direction;
  fprintf(stderr," %u %d \n ",*ret_power,*ret_direction);
  return;
}


int MD23_MoveMotors(struct md23_device * dev,int motornum,unsigned char power,signed int direction)
{
  if ( MD23_ok(dev) == 0 ) return 0;

  signed char power_direction=0;
  if ( direction == 0 ) power_direction=0; else
    if ( direction > 0 )  power_direction=1; else
      if ( direction < 0 )  power_direction=-1;

  flip_power_direction(&power,&power_direction);

  if ( (motornum == 0) || (motornum==2) )
    {
      MotorNowIsPast(dev,0);
      fprintf(stderr,"Set future motor 0 to %u \n",dev->future_motors[0].power);
      dev->future_motors[0].power=power;
      dev->motors[0].nolimits=1;
      dev->motors[0].finished=0;
    }
  if ( (motornum == 1) || (motornum==2) )
    {
      MotorNowIsPast(dev,1);
      fprintf(stderr,"Set future motor 1 to %u \n",dev->future_motors[1].power);
      dev->future_motors[1].power=power;
      dev->motors[1].nolimits=1;
      dev->motors[1].finished=0;
    }
  return 1;
}

int MD23_MoveMotorsDegrees(struct md23_device * dev,int motornum,unsigned char power,signed int degrees)
{
  if ( MD23_ok(dev) == 0 ) return 0;

    fprintf(stderr,"MD23_MoveMotorsDegrees motor : %u power : %u  direction : %d \n ",motornum,power,degrees);

  signed char power_direction=0;
  if ( degrees == 0 ) power_direction=0; else
    if ( degrees > 0 )  power_direction=1; else
      if ( degrees < 0 )  power_direction=-1;

  flip_power_direction(&power,&power_direction);
  if ( (motornum == 0) || (motornum==2) )
    {
      MotorNowIsPast(dev,0);
      dev->future_motors[0].power=power;
      dev->future_motors[0].encoder=(float) (dev->motors[0].encoder+degrees);
      fprintf(stderr,"Set future motor 0 to %u %f \n",dev->future_motors[0].power,dev->future_motors[0].encoder);
      dev->motors[0].nolimits=0;
      dev->motors[0].finished=0;
    }

  if ( (motornum == 1) || (motornum==2) )
    {
      MotorNowIsPast(dev,1);
      dev->future_motors[1].power=power;
      dev->future_motors[1].encoder=(float) (dev->motors[1].encoder+degrees);
      fprintf(stderr,"Set future motor 1 to %u %f \n",dev->future_motors[1].power,dev->future_motors[1].encoder);
      dev->motors[1].nolimits=0;
      dev->motors[1].finished=0;
    }


  return 1;
}


int MD23_MoveBothMotorsDifferentDegrees(struct md23_device * dev,unsigned char power1,signed int degrees1 , unsigned char power2,signed int degrees2)
{
  fprintf(stderr,"MD23_MoveBothMotorsDifferentDegrees( %u %d , %u %d )\n",power1,degrees1,power2,degrees2);
  if ( MD23_ok(dev) == 0 ) return 0;

  MotorNowIsPast(dev,0);
  MotorNowIsPast(dev,1);

  // MOTOR 1
  signed char power_direction=0;
  if ( degrees1 == 0 ) power_direction=0; else
    if ( degrees1 > 0 )  power_direction=1; else
      if ( degrees1 < 0 )  power_direction=-1;

  flip_power_direction(&power1,&power_direction);

      dev->future_motors[0].power=power1;
      dev->future_motors[0].encoder=dev->motors[0].encoder+(float) degrees1;
      fprintf(stderr,"Set future motor 0 from %u %f to %u %f \n",dev->motors[0].power,dev->motors[0].encoder,dev->future_motors[0].power,dev->future_motors[0].encoder);
      dev->motors[0].nolimits=0;
      dev->motors[0].finished=0;


  // MOTOR 2
  power_direction=0;
  if ( degrees2 == 0 ) power_direction=0; else
    if ( degrees2 > 0 )  power_direction=1; else
      if ( degrees2 < 0 )  power_direction=-1;

  flip_power_direction(&power2,&power_direction);

      dev->future_motors[1].power=power2;
      dev->future_motors[1].encoder=dev->motors[1].encoder+(float) degrees2;
      fprintf(stderr,"Set future motor 1 from %u %f to %u %f \n",dev->motors[1].power,dev->motors[1].encoder,dev->future_motors[1].power,dev->future_motors[1].encoder);
      dev->motors[1].nolimits=0;
      dev->motors[1].finished=0;



  return 1;
}

int MD23_RemoveMotorsLimits(struct md23_device * dev,int motornum)
{
  if ( MD23_ok(dev) == 0 ) return 0;

  if ( (motornum == 0) || (motornum==2) )  dev->motors[0].nolimits=1;
  if ( (motornum == 1) || (motornum==2) )  dev->motors[1].nolimits=1;
  return 1;
}

int MD23_ZeroEncoders(struct md23_device * dev)
{
  if ( MD23_ok(dev) == 0 ) return 0;
  dev->flush=1;

  fprintf(stderr,"Waiting for main thread to clear encoders ");
  int max_time=20,wait_time=0;
  while ( (dev->motors[0].encoder!=0.0) && (dev->motors[1].encoder!=0.0) )
   {
     fprintf(stderr,".");
     ++wait_time;
     usleep(5000);
     if ( wait_time > max_time )  break;
   }

  if ( wait_time >= max_time ) { fprintf(stderr," timed out.. \n"); return 0; } else
                                 fprintf(stderr," cleared \n");
  return 1;
}


unsigned char MD23_MovementDone(struct md23_device * dev)
{
  if ( MD23_ok(dev) == 0 ) return 0;

    if ( ( dev->motors[0].power==128 ) && ( dev->motors[1].power==128 ) && ( dev->future_motors[0].power==128 ) && ( dev->future_motors[1].power==128 ) )
    {
      // THE MOTORS ARE DISABLED , AND A NEW MOVEMENT ORDER IS NOT PENDING ?
      return 1;
    }
  return 0;
}


float MD23_GetEncoder(struct md23_device * dev,unsigned char param)
{
  if ( MD23_ok(dev) == 0 ) return 0;

  // 0 = Count #0 , 1 = Count #1
  if ( param < 2 )
    {
      return dev->motors[param].encoder;
    }

  return 0;
}

void MD23_Wait()
{
    usleep(6000);
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// MAIN MOTOR HYPERVISOR LOOP
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void * MD23_loop(void * ptr)
{

  struct  md23_device *loopdev = (struct md23_device *) ptr;
  if ( loopdev == 0 ) { fprintf(stderr,"MD23Loop Called with Invalid Pointer\n"); }
  unsigned int lag = 40000;

  loopdev->read_loop_active=1;
  printf("Started MD23 Loop\n");

  while ( MD23_ok(loopdev) == 1 )
    {
      usleep(lag);

      MD23_ReadStatus(loopdev,0);
      Thread_ReachTarget(loopdev);
    }

  printf("Exiting MD23 Read Loop for device %s \n",loopdev->i2c_dev.name);
  loopdev->read_loop_active=0;

  return (void *) 0;
}


// ===============================================
// ======================================
// LOW LEVEL MD23 COMMANDS
// ======================================
// ===============================================
