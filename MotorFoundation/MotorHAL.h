#ifndef MOTORHAL_H_INCLUDED
#define MOTORHAL_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

unsigned int RobotInit(char * md23_device_id,char * arduino_device_id);
unsigned int RobotRotate(unsigned char power,signed int degrees);
unsigned int RobotStartRotating(unsigned char power,signed int direction);
unsigned int RobotMove(unsigned char power,signed int distance);
unsigned int RobotStartMoving(unsigned char power,signed int direction);
unsigned int RobotManoeuvresPending();
unsigned int RobotPrintPosition();

int RobotGetUltrasonic(unsigned int dev);
int RobotGetAccelerometerX(unsigned int dev);
int RobotGetAccelerometerY(unsigned int dev);

void RobotStopMovement();
void RobotWait(unsigned int msecs);
unsigned int RobotClose();

#ifdef __cplusplus
}
#endif


#endif // MOTORHAL_H_INCLUDED
