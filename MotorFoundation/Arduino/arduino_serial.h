#ifndef ARDUINO_SERIAL_H_INCLUDED
#define ARDUINO_SERIAL_H_INCLUDED


int InternalGetUltrasonicValue(int dev);
int InternalGetAccelerometerX(int dev);
int InternalGetAccelerometerY(int dev);

int arduino_ok();
int kickstart_arduino_thread(char * devname);
int kill_arduino_thread();

#endif // ARDUINO_SERIAL_H_INCLUDED
