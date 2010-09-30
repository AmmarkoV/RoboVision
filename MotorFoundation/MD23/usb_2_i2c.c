#include "usb_2_i2c.h"

#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#define BAUDRATE B19200

int ALERTNESS = 20000;

int I2CLAG = 37000;

void i2c_delay()
{
 usleep(I2CLAG+ALERTNESS);
}

int i2c_transmit(struct i2c_device * dev,unsigned char * msg,int msglength)
{
  if ( dev->is_open==0 ) { return 0; }
  usleep(I2CLAG+ALERTNESS);
  int wr_num = write(dev->handle,msg,msglength);
  return wr_num;
}

int i2c_recieve(struct i2c_device * dev,unsigned char * msg,int msglength)
{
  if ( dev->is_open==0 ) { return 0; }
  usleep(I2CLAG+ALERTNESS);
  int re_num = read(dev->handle,msg,msglength);
  return re_num;
}

int i2c_open(struct i2c_device * dev)
{
  int fd = open(dev->name, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd ==-1)
    {
      return(0);
    }
  dev->handle=fd ;

  tcgetattr(dev->handle,& dev->oldtio); /* save current serial port settings */
  bzero(&dev->newtio, sizeof(dev->newtio)); /* clear struct for new port settings */

  cfsetospeed(&dev->newtio, BAUDRATE);
  dev->newtio.c_cflag = BAUDRATE  | CS8 | CLOCAL | CREAD | PARENB | CSTOPB | CSIZE;
  dev->newtio.c_iflag = IGNPAR;
  dev->newtio.c_oflag = 0;
  dev->newtio.c_lflag = 0;
  dev->newtio.c_cc[VMIN] = 0;      /* block untill n bytes are received */
  dev->newtio.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */

  fd = tcsetattr(dev->handle,TCSANOW,&dev->newtio);
  fcntl(dev->handle, F_SETFL, FNDELAY);
  if(fd==-1)
  {
    close(dev->handle);
    return(0);
  }

  dev->is_open=1;
  return 1;
}


int i2c_close(struct i2c_device * dev)
{
  close(dev->handle);
  tcsetattr(dev->handle,TCSANOW,&dev->oldtio);
  dev->is_open=0;
  return 1;
}


int i2c_Check_If_Availiable(struct i2c_device * dev)
{

  unsigned char mess[5]={0};

  mess[0] = 0x5A;    // USB_I2C Module Command
  mess[1] = 0x01;     // Get Revision
  mess[2] = 0;
  mess[3] = 0;

  i2c_transmit(dev,mess,4);


  mess[0] = 255;
  if ( i2c_recieve(dev,mess,1)>0 )
    {
      if (mess[0] != 255)  // if a software version response is returned
        {
          return 1;
        }
    }
  return 0;
}



