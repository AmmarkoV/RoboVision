#ifndef ARDUINO_SERIAL_H_INCLUDED
#define ARDUINO_SERIAL_H_INCLUDED


int ArduinoInternalGetUltrasonicValue(int dev);
int ArduinoInternalGetAccelerometerX(int dev);
int ArduinoInternalGetAccelerometerY(int dev);

int ArduinoOk();
int ArduinoThreadStart(char * devname);
int ArduinoStopThread();

#endif // ARDUINO_SERIAL_H_INCLUDED
