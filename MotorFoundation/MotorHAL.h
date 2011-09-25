#ifndef MOTORHAL_H_INCLUDED
#define MOTORHAL_H_INCLUDED


#include "../WorldMapping/MasterpathPlanning/MasterpathPlanning.h"

#ifdef __cplusplus
extern "C" {
#endif

unsigned int RobotInit(char * md23_device_id,char * arduino_device_id);
unsigned int RobotClose();
void RobotWait(unsigned int msecs);

unsigned int RobotBaseOk();

unsigned int RobotMoveJoystick(signed int joy_x,signed int joy_y);
unsigned int RobotRotate(unsigned char power,signed int degrees);
unsigned int RobotStartRotating(unsigned char power,signed int direction);
unsigned int RobotMove(unsigned char power,signed int distance);
unsigned int RobotStartMoving(unsigned char power,signed int direction);
unsigned int RobotManoeuvresPending();
void RobotStopMovement();

int RobotGetUltrasonic(unsigned int dev);
int RobotGetAccelerometerX(unsigned int dev);
int RobotGetAccelerometerY(unsigned int dev);
int RobotSetHeadlightsState(unsigned int scale_1_on,unsigned int scale_2_on,unsigned int scale_3_on);
int RobotIRTransmit(char * code,unsigned int code_size);


unsigned int RobotPrintPosition();

struct Map * RobotGetMapPointer();

#ifdef __cplusplus
}
#endif


#endif // MOTORHAL_H_INCLUDED
