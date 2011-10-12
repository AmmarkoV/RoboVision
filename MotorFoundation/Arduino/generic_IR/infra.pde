 int sensorPin = 0; // ANALOG PIN 0
 int ledPin = 13; // LED PIN OUT  
 int IRPin = 5;   // IR PWM  PIN OUT
 
 
 byte ir_on[512]={0};  
 byte max_ir_code;
  
 // VARS USED IN LOOP
 int sensorValue = 0;
 long last_input_timer=10000 , timer = 0 ,just_in = 0; 

 
void setup() 
{
   pinMode(ledPin, OUTPUT);
   pinMode(IRPin, OUTPUT);
   Serial.begin(38400);
   Serial.println("start");
   
}
 
void mark_as_old_signal()
{
   timer=0;
   last_input_timer=10000;
}
 
void mark_as_new_signal()
{ 
} 
 
int this_is_a_new_signal()
{
   if ( last_input_timer > timer  ) { return 0; } else
   if ( timer > last_input_timer+300  ) { just_in = 1; max_ir_code=last_input_timer; return 0; }
   return 1;
}
 
void loop() 
{
  /* digitalWrite(ledPin, sensorValue); */  
    
   sensorValue = analogRead(sensorPin);
   
   if ( sensorValue < 1000 ) 
    { 
      last_input_timer=timer;                   
      
      if ( timer < 512 ) { //ir_on[timer]=1;
                           ir_on[timer]=(byte) ( 1024-sensorValue ) * (float) 0.2490  ; 
                         } 
    } else 
    {
      if ( timer < 512 ) { ir_on[timer]=0; }
    }
    
    
    
   
   delayMicroseconds(100); 
   
   if ( !this_is_a_new_signal() )  
      { 
       if ( just_in ) 
          { 
            digitalWrite(ledPin, HIGH );
            int i=0;
            
            Serial.print("IR Code Intercepted\n");
            for (i=0; i<max_ir_code; i++)
             { Serial.print(i);
               Serial.print(" - ");
               Serial.println((unsigned int ) ir_on[i]);
             }
            just_in = 0;
            Serial.print("Please wait\n");
            delay(3000);
            Serial.print("Hearing again\n");
            digitalWrite(ledPin, LOW );    
          }  
        mark_as_old_signal(); 
      }  else
      { 
        ++timer; 
      }   
      
 } 
