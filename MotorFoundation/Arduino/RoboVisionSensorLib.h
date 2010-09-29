#ifndef ROBOVISIONSENSORLIB_H_INCLUDED
#define ROBOVISIONSENSORLIB_H_INCLUDED

int ConnectRoboVisionDevice(char * devname);
int DisconnectRoboVisionDevice(char * devname);
int GetUltrasonicValue(int dev);
int GetAccelerometerX(int dev);
int GetAccelerometerY(int dev);
int SendIRCode(char * ircodes,unsigned int numberofcodes,unsigned int microsecondpause);

#endif // ROBOVISIONSENSORLIB_H_INCLUDED
