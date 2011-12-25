#ifndef MD23_H
#define MD23_H
#include "usb_2_i2c.h"
#include <pthread.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif


struct motor
{
 float encoder;
 unsigned char power;
 unsigned char current;
 unsigned char finished;
 unsigned char nolimits;
};


struct md23_device
  {
    char name[32];
    struct i2c_device i2c_dev;

    struct motor past_motors[2];
    struct motor motors[2];
    struct motor future_motors[2];

    unsigned int md23_address;
    unsigned int md23_found;
    double voltage;
    unsigned char flush;

    pthread_t loop_thread;
    int md23_refresh_lag;

    int read_loop_active;
  };

char * MD23_GetVersion();

struct md23_device * MD23_Init(char * dev_name,unsigned int threading_on);
void MD23_Close(struct md23_device * dev);

int MD23_ok(struct md23_device * dev);

int MD23_MoveMotors(struct md23_device * dev,int motornum,unsigned char power,signed int direction);
int MD23_MoveMotorsDegrees(struct md23_device * dev,int motornum,unsigned char power,signed int degrees);
int MD23_MoveBothMotorsDifferentDegrees(struct md23_device * dev,unsigned char power1,signed int degrees1 , unsigned char power2,signed int degrees2);
int MD23_ZeroEncoders(struct md23_device * dev);

void MD23_Wait();
unsigned char MD23_MovementDone(struct md23_device * dev);
float MD23_GetEncoder(struct md23_device * dev,unsigned char param);

unsigned int MD23_IsClosed();

#ifdef __cplusplus
}
#endif

#endif // MD23_H
