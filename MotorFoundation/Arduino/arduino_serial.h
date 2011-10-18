#ifndef ARDUINO_SERIAL_H_INCLUDED
#define ARDUINO_SERIAL_H_INCLUDED

extern int fd;

int ArduinoInternalGetUltrasonicValue(int which_one);
int ArduinoInternalGetAccelerometerX();
int ArduinoInternalGetAccelerometerY();

int ArduinoInternalSetLights(int light_num,int light_state);
int ArduinoInternalSetCameraPose(int heading,int pitch);

int ArduinoOk();
int ArduinoThreadStart(char * devname);
int ArduinoStopThread();


#define TOTAL_ARDUINO_LIGHTS 2

struct arduino_connected_devices
{
   unsigned int ultrasonic1,ultrasonic2,accelerometerX,accelerometerY;
   unsigned int camera_pose_pitch;
   unsigned int camera_pose_heading;
   int lights[TOTAL_ARDUINO_LIGHTS];

};

extern struct arduino_connected_devices activated_state;
extern struct arduino_connected_devices future_state;

#endif // ARDUINO_SERIAL_H_INCLUDED
