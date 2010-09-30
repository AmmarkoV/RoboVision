#ifndef ROBOVISIONSENSORLIB_H_INCLUDED
#define ROBOVISIONSENSORLIB_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif
int ConnectRoboVisionSensors(char * devname);
int DisconnectRoboVisionSensors();
int RoboVisionSensorsOK();
int GetUltrasonicValue(int dev);
int GetAccelerometerX(int dev);
int GetAccelerometerY(int dev);
int SendIRCode(char * ircodes,unsigned int numberofcodes,unsigned int microsecondpause);
#ifdef __cplusplus
}
#endif
#endif // ROBOVISIONSENSORLIB_H_INCLUDED
