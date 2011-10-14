#include "MemsicAccelerometer.h"

void sampleAccelerometer(struct memsic2125 * accel)
{
  accel->pulseX = pulseIn(accel->xPin,HIGH);  
  accel->pulseY = pulseIn(accel->yPin,HIGH);
  accel->accelerationX = ((accel->pulseX / 10) - 500) * 8;
  accel->accelerationY = ((accel->pulseY / 10) - 500) * 8;
}

void setupAccelerometer(struct memsic2125 * accel,int xpin,int ypin)
{
  pinMode(xpin, INPUT);
  pinMode(ypin, INPUT);
  accel->xPin=xpin;
  accel->yPin=ypin;
  accel->pulseX=0;
  accel->pulseY=0;
  accel->accelerationX=0;
  accel->accelerationY=0;
}

void transmitAccelerometerStatus(struct memsic2125 * accel)
{
   sampleAccelerometer(accel);
   Serial.print("accelerometers(");
   Serial.print(accel->accelerationX);
    // print a tab character:
   Serial.print(",");
   Serial.print(accel->accelerationY);
   Serial.println(")"); 
}


void transmitDummyAccelerometerStatus()
{ 
   Serial.print("accelerometers(");
   Serial.print(123);
    // print a tab character:
   Serial.print(",");
   Serial.print(123);
   Serial.println(")"); 
}
