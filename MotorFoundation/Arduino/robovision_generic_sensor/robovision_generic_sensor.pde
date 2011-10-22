#include <Servo.h>
#include "Ultrasonic.h" 
#include "Infrared.h" 
#include "ircodes.h" 
#include "MemsicAccelerometer.h"

#define TOTAL_LED_PINS 2
#define TOTAL_SERVOS 2 

unsigned int IRReceiver=2; 
unsigned int IRTransmitter=9; 

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
         
         switch (inB1)
          {
            case 'M' :    //MOVE SERVO
                           {int servo_num = inB2-'0'; 
                            if ( ( inB3>130 ) || ( inB3<40 ) ) {} else
                                                              { MoveServo(servo_num,inB3); 
                                                                return 1; } 
                           }                                     
            break;
            case 'S' :    //STOP SERVO
                                    if ( ( inB1 == 'S')&&( inB2 == 'S') ) 
                                                               { // Servo Stop 
                                                               }
                          break;
            case 'L' :    //LED CODE
                         if ( inB2 == 'A')  { // Light Activate 
                                              unsigned int light_number = inB3-'0';  
                                              return LightControl(light_number,1);
                                            } else
                         if ( inB2 == 'D' ) { // Light Deactivate
                                              unsigned int light_number = inB3-'0';  
                                              return LightControl(light_number,0);
                                            }  
         
            break;  
            case 'I' : if ( inB2 == 'R' )  //INFRARED CODE
                        {
                          if ( inB3 == 'R')  { // Receive
                                               Serial.println("Listen for Infrared\n");
                                               int numberpulses = listenForIR();
                                               Serial.print("Heard ");
                                               Serial.print(numberpulses);
                                               Serial.print(" pulses\n");
             
                                               if ( IRcompare(numberpulses,AirconditionON) )
                                                   { 
                                                      Serial.println("This is the aircondition ON signal!\n");
                                                   } else
                                               if ( IRcompare(numberpulses,AirConditionOFF) )
                                                   { 
                                                      Serial.println("This is the aircondition OFF signal!\n");
                                                   }   
             
                                               printpulses();
                                              return 1;
                                            } else
                          if ( inB3 == 'L' ) { // Last Infrared
                                                Serial.println("Sending Last Infrared\n"); 
                                                SendLastCode(); 
                                                Serial.println("done\n");
                                               return 1;
                                            } else
                          if ( inB3 == 'S' ) { // Spam Infrared
                                                Serial.println("Spamming Infrared\n");
                                                SpamInfrared(5000); // SPAM infrared for 5 seconds
                                                Serial.println("done\n");
                                               return 1;
                                            }  else
                          if ( inB3 == 'T' ) { // Transmit Infrared Preset
                                                byte memory_preset = Serial.read();
                                                Serial.print("Sending Infrared Preset ");
                                                Serial.println(memory_preset);
                                                SendInfraredPreset(memory_preset-'0');  
                                                Serial.println("done\n");
                                               return 1;
                                            }   
       
                    }         
             break; // END OF INFRARED CODE
             case 'U' :    //ULTRASONIC
                           if ( inB2 == 'G')    { // Servo Stop 
                                                } else
                           if ( inB2 == 'T')    { // Servo Stop 
                                                }
             break;           
             case 'A' :    //ACCELEROMETER
                           if ( inB2 == 'X')    { // Servo Stop 
                                                } else
                           if ( inB2 == 'Y')    { // Servo Stop 
                                                } else
                           if ( inB2 == 'Z')    { // Servo Stop 
                                                }
             break;              
             case 'P' :    //PROXIMITY
                           if ( inB2 == 'I')                     { // Servo Stop 
                                                                 } else
                           if (( inB2 == 'N')&&(inB3 == 'G'))    { // Servo Stop 
                                                                 }                      
                                                
             break;           
     
             case 'C' :    //CHECK STATUS
                           if (( inB2 == 'H')&&(inB3 == 'K'))    { 
                                                                   Serial.println("ARDUINO\n");
                                                                   SendEmptyBytes(42);
                                                                   return 1;
                                                                 }                         
             break;       
         
             case 'E' :    //End Autonomous State
                           if (( inB2 == 'A')&&(inB3 == 'S'))    { 
                                                                   autonomous_mode = 0;
                                                                   return 1;
                                                                 }                         
             break;
             case 'B' :    //Begin Autonomous State
                           if (( inB2 == 'A')&&(inB3 == 'S'))    { 
                                                                   autonomous_mode = 1;
                                                                   return 1;
                                                                 }                         
             break;       
             case 'F' :    //FLUSH 
                           if (( inB2 == 'L')&&(inB3 == 'U'))    { Serial.flush(); return 1; }                      
                                                
             break;               
             case 'X' :    //FLUSH
                           Serial.flush();  
                           return 1;                     
             break;
             case 'Z' :    //RESTART
                           Serial.println("RESET\n");
                           Serial.flush();  
                           resetFunc();                         
             break;                      
   
             default :
                          Serial.println("Uncatched input \n");
             break;
 }
           
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
    { 
        --time_to_power_down_servos; 
    }
   
   delay(100);  // wait 100 milli seconds before looping again
}
