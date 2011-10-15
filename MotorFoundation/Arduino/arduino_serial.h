#ifndef ARDUINO_SERIAL_H_INCLUDED
#define ARDUINO_SERIAL_H_INCLUDED

extern int fd;

int ArduinoInternalGetUltrasonicValue(int which_one);
int ArduinoInternalGetAccelerometerX();
int ArduinoInternalGetAccelerometerY();

int ArduinoInternalSetCameraPose(int heading,int pitch);

int ArduinoOk();
int ArduinoThreadStart(char * devname);
int ArduinoStopThread();

#endif // ARDUINO_SERIAL_H_INCLUDED
