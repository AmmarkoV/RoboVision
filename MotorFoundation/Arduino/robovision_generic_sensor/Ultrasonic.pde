#include "Ultrasonic.h"

int sampleUltrasonic(struct ultrasonic *ultr)
{
  unsigned long pulseTime = 0;                    // stores the pulse in Micro Seconds
  unsigned long distance = 0;                     // variable for storing the distance (cm) 
  
  digitalWrite(ultr->initPin, HIGH);                    // send 10 microsecond pulse
  delayMicroseconds(10);                  // wait 10 microseconds before turning off
  digitalWrite(ultr->initPin, LOW);                     // stop sending the pulse
  pulseTime = pulseIn(ultr->echoPin, HIGH);             // Look for a return pulse, it should be high as the pulse goes low-high-low
  distance = pulseTime/58;                        // Distance = pulse time / 58 to convert to cm.
  ultr->total= ultr->total - ultr->readings[ultr->arrayIndex];           // subtract the last distance
  ultr->readings[ultr->arrayIndex] = distance;                // add distance reading to array
  ultr->total= ultr->total + ultr->readings[ultr->arrayIndex];            // add the reading to the total
 
  ultr->arrayIndex = ultr->arrayIndex + 1;                    // go to the next item in the array

  int proximity=0;

  if (ultr->arrayIndex >= numOfReadings)  { ultr->arrayIndex = 0; }
  ultr->averageDistance = ultr->total / numOfReadings;      // calculate the average distance
  // if the distance is less than 255cm then change the brightness of the LED
  //if (ultr->averageDistance < 255) { proximity = 255 - ultr->averageDistance;   }     // this means the smaller the distance the brighterthe LED.
  proximity=ultr->averageDistance;
  
  return proximity;
}

void setupUltrasonic(struct ultrasonic *ultr,int echopin,int initpin)
{
  pinMode(initpin, OUTPUT);                     // set init pin 3 as output
  pinMode(echopin, INPUT);                      // set echo pin 2 as input
  ultr->echoPin=echopin;
  ultr->initPin=initpin;
  for (int thisReading = 0; thisReading < numOfReadings; thisReading++)  { ultr->readings[thisReading] = 0; }
  ultr->proximity=0;
  ultr->arrayIndex=0;                             // arrayIndex of the current item in the array
  ultr->total=0;                                  // stores the cumlative total
  ultr->averageDistance=0;  
}



void transmitUltrasonicStatus(struct ultrasonic * sensor1,struct ultrasonic * sensor2)
{
   int proximity1 = sampleUltrasonic(sensor1);
   Serial.print("proximity(1,");
   Serial.print(proximity1);
   Serial.println(")");
   
   int proximity2 = sampleUltrasonic(sensor2);
   Serial.print("proximity(2,");
   Serial.print(proximity2);
   Serial.println(")");
}


void transmitDummyUltrasonicStatus()
{ 
   int proximity1 = 123;
   Serial.print("proximity(1,");
   Serial.print(proximity1);
   Serial.println(")");
   
   int proximity2 = 123;
   Serial.print("proximity(2,");
   Serial.print(proximity2);
   Serial.println(")");
}
