#ifndef ROBOVISIONSENSORLIB_H_INCLUDED
#define ROBOVISIONSENSORLIB_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif
int ConnectRoboVisionSensors(char * devname);
int DisconnectRoboVisionSensors();
int RoboVisionSensorsOK();
int SetLights(int light_num,int light_state);
int SetCameraPose(int heading,int pitch);
int GetCameraPose(int * heading,int * pitch);
int GetUltrasonicValue(int which_one);
int GetAccelerometerX();
int GetAccelerometerY();
int SendIRCode(char * ircodes,unsigned int numberofcodes,unsigned int microsecondpause);







#ifdef __cplusplus
}
#endif
#endif // ROBOVISIONSENSORLIB_H_INCLUDED
