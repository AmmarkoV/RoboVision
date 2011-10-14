#ifndef ULTRASONIC_H_INCLUDED
#define ULTRASONIC_H_INCLUDED

#define numOfReadings 10

struct ultrasonic
{
  int echoPin;                                // SRF05 echo pin (digital 2)
  int initPin;    
  unsigned int proximity;
  int arrayIndex;                             // arrayIndex of the current item in the array
  int total;                                  // stores the cumlative total
  int averageDistance;                        // stores the average value
  int readings[numOfReadings];                    // stores the distance readings in an array
};

 int sampleUltrasonic(struct ultrasonic *ultr);
void setupUltrasonic(struct ultrasonic *ultr,int echopin,int initpin);
void transmitUltrasonicStatus(struct ultrasonic * sensor1,struct ultrasonic * sensor2);
void transmitDummyUltrasonicStatus();

#endif  
