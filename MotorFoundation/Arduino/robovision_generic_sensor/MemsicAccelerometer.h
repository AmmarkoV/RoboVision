#ifndef MEMSIC_ACCELEROMETER_H_INCLUDED
#define MEMSIC_ACCELEROMETER_H_INCLUDED




struct memsic2125
{
  int xPin,yPin;
  int pulseX,pulseY;
  int accelerationX,accelerationY;
};
 
void sampleAccelerometer(struct memsic2125 * accel);
void setupAccelerometer(struct memsic2125 * accel,int xpin,int ypin);
void transmitAccelerometerStatus(struct memsic2125 * accel);
void transmitDummyAccelerometerStatus();

#endif  
