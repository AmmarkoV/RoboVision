#ifndef ROBOVISIONSENSORLIB_H_INCLUDED
#define ROBOVISIONSENSORLIB_H_INCLUDED

int ConnectRoboVisionSensors(char * devname);
int DisconnectRoboVisionSensors(char * devname);
int RoboVisionSensorsOK();
int GetUltrasonicValue(int dev);
int GetAccelerometerX(int dev);
int GetAccelerometerY(int dev);
int SendIRCode(char * ircodes,unsigned int numberofcodes,unsigned int microsecondpause);

#endif // ROBOVISIONSENSORLIB_H_INCLUDED
