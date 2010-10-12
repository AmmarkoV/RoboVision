const int numOfReadings = 10;                   // number of readings to take/ items in the array
// setup pins and variables for SRF05 sonar device
int sampleultrasonic=1;
int sampleaccelerometer=1;
unsigned int tickcount=0;
int headlights = 11;                 // LED connected to digital pin 13
 

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

struct memsic2125
{
  int xPin,yPin;
  int pulseX,pulseY;
  int accelerationX,accelerationY;
};
 
struct ultrasonic sensor1,sensor2;
struct memsic2125 accelerometer;

unsigned long pulseTime = 0;                    // stores the pulse in Micro Seconds
unsigned long distance = 0;                     // variable for storing the distance (cm)
unsigned int proximity=0;

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

//setup
void setup() 
{ 
  setupUltrasonic(&sensor1,2,3);
  setupUltrasonic(&sensor2,4,5);
  setupAccelerometer(&accelerometer,7,6);
  pinMode(headlights, OUTPUT);      // sets the digital pin as output

 // initialize the serial port, lets you view the
 // distances being pinged if connected to computer
     Serial.begin(38400);
 }
// execute


int sampleUltrasonic(struct ultrasonic *ultr)
{
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


void sampleAccelerometer(struct memsic2125 * accel)
{
  accel->pulseX = pulseIn(accel->xPin,HIGH);  
  accel->pulseY = pulseIn(accel->yPin,HIGH);
  accel->accelerationX = ((accel->pulseX / 10) - 500) * 8;
  accel->accelerationY = ((accel->pulseY / 10) - 500) * 8;
}
 


void loop() 
{
   digitalWrite(headlights, HIGH);   // sets the LED on

  ++tickcount;
  
  if ( sampleultrasonic )
  {
  //analogWrite(redLEDPin, redLEDValue);          // Write current value to LED pins
  //Serial.println(averageDistance, DEC);         // print out the average distance to the debugger
   int proximity1 = sampleUltrasonic(&sensor1);
   Serial.print("proximity(1,");
   Serial.print(proximity1);
   Serial.println(")");
   
   int proximity2 = sampleUltrasonic(&sensor2);
   Serial.print("proximity(2,");
   Serial.print(proximity2);
   Serial.println(")");
  }
  
   if ( sampleaccelerometer )
   {
    sampleAccelerometer(&accelerometer);
    Serial.print("accelerometers(");
    Serial.print(accelerometer.accelerationX);
    // print a tab character:
    Serial.print(",");
    Serial.print(accelerometer.accelerationY);
    Serial.println(")"); 
   }
   
   Serial.print("tickcount("); 
   Serial.print(tickcount); 
   Serial.println(")"); 
    
   Serial.println("#"); /*MARK THE END OF TRANSMISSION*/ 
   
   delay(100);                                   // wait 100 milli seconds before looping again
}


