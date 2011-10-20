#include <Servo.h>
#include "Ultrasonic.h" 
#include "Infrared.h" 
#include "ircodes.h" 
#include "MemsicAccelerometer.h"

#define TOTAL_LED_PINS 2
#define TOTAL_SERVOS 2 

unsigned int IRReceiver=2; 
unsigned int IRTransmitter=7; 

struct ultrasonic ultrasonicsensor1,ultrasonicsensor2;
struct memsic2125 accelerometer;
int ledPins[TOTAL_LED_PINS];
Servo servo1,servo2,servo3;

int autonomous_mode = 0;
int time_to_power_down_servos = 0;


void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup(void) 
{
  Serial.begin(38400);
 
  ledPins[0]=13; // THERE ARE 2 LED PINS ON GUARDDOG
  ledPins[1]=13; // THERE ARE 2 LED PINS ON GUARDDOG
  
  servo1.attach(14); //analog pin 0
  delay(100);
  servo1.write(70);
  delay(200); 
  servo1.write(90);
  delay(200);
  servo1.detach();
  
  pinMode(ledPins[0], OUTPUT);
  pinMode(IRTransmitter, OUTPUT);
  //pinMode(IRReceiver, INPUT);
  //Serial.println("Waiting for Input!");
  
  
  setupAccelerometer(&accelerometer,6,5);
  
  Serial.flush(); 
}


int LightControl(unsigned int light_number,int state)
{
  if (light_number>=TOTAL_LED_PINS) { Serial.print(light_number); return 0; }
  if (state) { digitalWrite(ledPins[light_number], HIGH ); return 1; } else
             { digitalWrite(ledPins[light_number], LOW );  return 1; }
}


int MoveServo(unsigned int servo_number,int rot_degrees)
{
  if (servo_number>=TOTAL_SERVOS) { return 0; }
  switch (servo_number)
  {
   case 0 : 
     
     if (!servo1.attached() )
              { servo1.attach(14); delay(45); }
     
     servo1.write(rot_degrees);
     time_to_power_down_servos=10;
     break;
     
   default :
   break;
  }
  return 0;  
}

void SendEmptyBytes(int number)
{
  for (int i=0; i<number-3; i++) { Serial.print(" "); }
  Serial.print("\n#\n");             
}

int SerialInputReceiver()
{
         byte inB1 = Serial.read();
         byte inB2 = Serial.read();
         byte inB3 = Serial.read();
         
         //This will become a switch statement :P
         // SERVO CONTROLS ----------------------
         if ( inB1 == 'M' ) 
           { // Servo Move
              int servo_num = inB2-'0'; 
              if ( ( inB3>130 ) || ( inB3<40 ) ) {} else
                                                 { MoveServo(servo_num,inB3); 
                                                   return 1;
                                                 } 
           } else
         if ( ( inB1 == 'S')&&( inB2 == 'S') ) 
           { // Servo Stop 
           } else
         
         
         // LIGHT CONTROLS -----------------------
         if ( ( inB1 == 'L')&&( inB2 == 'A') ) 
           { // Light Activate 
             unsigned int light_number = inB3-'0';  
             return LightControl(light_number,1);
           } else
         if ( ( inB1 == 'L')&&( inB2 == 'D') ) 
           { // Light Deactivate
             unsigned int light_number = inB3-'0';  
             return LightControl(light_number,0);
           } else
         
         
         // INFRARED Receive -----------------------    
         if ( ( inB1 == 'I')&&( inB2 == 'R')&&( inB3 == 'R') ) 
           { // IR receiver 
             Serial.println("Listen for Infrared\n");
             int numberpulses = listenForIR();
             
             if ( IRcompare(numberpulses,AmmarAirCondition) )
               { 
                 Serial.println("This is the aircondition ON Off signal!\n");
               } else
             if ( IRcompare(numberpulses,Aircondition2) )
               { 
                 Serial.println("This is the 2 aircondition ON Off signal!\n");
               }   
             
              printpulses();
              return 1;
           } else
         // INFRARED TRANSMIT -----------------------    
         if ( ( inB1 == 'I')&&( inB2 == 'R')&&( inB3 == 'T') ) 
           { // IR tranmission 
             Serial.println("Sending Last Infrared\n");
             /*digitalWrite(IRTransmitter, HIGH );
             delay(1000);
             digitalWrite(IRTransmitter, LOW );*/
             SendLastCode(); 
             Serial.println("done\n");
             return 1;
           } else
           
           
         // ULTRASONIC CONTROL -----------------------    
         if ( ( inB1 == 'U')&&( inB2 == 'G') ) 
           { // Ultrasonic last sample get
           } else
         if ( ( inB1 == 'U')&&( inB2 == 'T') ) 
           { // Ultrasonic grab a new sample and return it
           } else
           
                     
         // ACCELEROMETER CONTROL -----------------------    
         if ( ( inB1 == 'A')&&( inB2 == 'X') ) 
           { // Get accelerometer X axis
           } else
         if ( ( inB1 == 'A')&&( inB2 == 'Y') ) 
           { // Get accelerometer Y axis
           } else
         if ( ( inB1 == 'A')&&( inB2 == 'Z') ) 
           { // Get accelerometer Z axis
           }  
            else
 
         // PROXIMITY IR CONTROL -----------------------    
         if ( ( inB1 == 'P')&&( inB2 == 'I') ) 
           { // Proximity Control
           } else
           
         // PROXIMITY IR CONTROL -----------------------    
         if ( ( inB1 == 'P')&&( inB2 == 'N')&&( inB3 == 'G')  ) 
           { // Proximity Control
           } else  
           
         // CONTROLLER STATE  -----------------------    
         if ( ( inB1 == 'C')&&( inB2 == 'H') &&( inB3 == 'K') ) 
           { // Check Arduino State
             Serial.println("ARDUINO\n");
             SendEmptyBytes(42);
             return 1;
           } else
         if ( ( inB1 == 'Z')&&( inB2 == 'Z') &&( inB3 == 'Z')) 
           { // Reset Arduino
             Serial.println("RESET\n");
             resetFunc();  //call reset 
           } else
         if ( ( inB1 == 'E')&&( inB2 == 'A') &&( inB3 == 'S') ) 
           { // End Autonomous reporting mode 
              autonomous_mode = 0;
              return 1;
           }  else
         if ( ( inB1 == 'B')&&( inB2 == 'A')&&( inB3 == 'S')  ) 
           { // Begin Autonomous reporting mode
             autonomous_mode = 1;
             return 1;
           }  else
           
         // FLUSH INPUT WHEN RECEIVING FLU OR any X ( X XX XXX etc )---------------------    
         if ( ( inB1 == 'F')&&( inB2 == 'L')&&( inB3 == 'U') ) 
           { // Flush
             Serial.flush();  
             return 1;
           } else
         if ( ( inB1 == 'X')||( inB2 == 'X')||( inB3 == 'X') ) 
           { // Flush
             Serial.flush();  
             return 1;
           } else

 
           
 return 0;
}
void loop(void) 
{ 
  
  if (Serial.available() >= 3) 
       { 
           
	  if ( SerialInputReceiver() ) { Serial.println("OK\n"); } else
                                       { Serial.println("FAIL\n"); }   
           
         
      }
   
   if ( autonomous_mode )
   {   
     transmitDummyAccelerometerStatus();
     //sampleAccelerometer(&accelerometer);
     transmitAccelerometerStatus(&accelerometer);
     
     transmitDummyUltrasonicStatus();
     Serial.println("#"); /*MARK THE END OF TRANSMISSION , AND THAT ARDUINO IS ALIVE */
   }
   
   
   
   if ( (servo1.attached()) && ( time_to_power_down_servos == 1) )
    {
        servo1.detach();
    }
   if ( time_to_power_down_servos > 0 )
     { --time_to_power_down_servos; }
   
   delay(100);                                   // wait 100 milli seconds before looping again
}
