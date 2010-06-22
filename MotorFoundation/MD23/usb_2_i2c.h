#ifndef _USB_2_I2C_H_
#define _USB_2_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <termios.h>

struct i2c_device
  {
    char name[32];
    int handle;
    int is_open;
    struct termios oldtio,newtio;
  };

extern int ALERTNESS;


void i2c_delay();
int i2c_transmit(struct i2c_device * dev,unsigned char * msg,int msglength);
int i2c_recieve(struct i2c_device * dev,unsigned char * msg,int msglength);
int i2c_open(struct i2c_device * dev);
int i2c_close(struct i2c_device * dev);

int i2c_Check_If_Availiable(struct i2c_device * dev);


#ifdef __cplusplus
}
#endif


#endif

