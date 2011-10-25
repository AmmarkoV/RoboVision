#ifndef ARDUINO_SERIAL_H_INCLUDED
#define ARDUINO_SERIAL_H_INCLUDED

extern int fd;

int ArduinoInternalGetUltrasonicValue(int which_one);
int ArduinoInternalGetAccelerometerX();
int ArduinoInternalGetAccelerometerY();

int ArduinoInternalSetLights(int light_num,int light_state,int wait_for_it);
int ArduinoInternalSetNod(int motor,int pitch,int wait_for_it);
int ArduinoInternalSetCameraPose(int heading,int pitch,int wait_for_it);

int ArduinoOk();
int ArduinoThreadStart(char * devname);
int ArduinoStopThread();


#define TOTAL_ARDUINO_LIGHTS 2

struct arduino_connected_devices
{
   unsigned int ultrasonic1,ultrasonic2,accelerometerX,accelerometerY;
   unsigned int camera_pose_pitch;
   unsigned int camera_pose_heading;
   unsigned int camera_nod_pitch;
   unsigned int camera_nod_heading;

   int lights[TOTAL_ARDUINO_LIGHTS];

};

extern struct arduino_connected_devices activated_state;
extern struct arduino_connected_devices future_state;

#endif // ARDUINO_SERIAL_H_INCLUDED
