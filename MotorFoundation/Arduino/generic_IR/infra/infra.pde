 int sensorPin = 2; // DIGITAL PIN IN 2
 #define IRpin_PIN PIND
#define IRpin 2

 int ledPin = 13; // LED PIN OUT  
 int IRPin = 7;   // IR PWM  PIN OUT
 int microsecond_step_delay=100;
 
 #define IR_CODE_LIMIT 1024
 
 byte ir_on[IR_CODE_LIMIT]={0};  
 int max_ir_code;
  
 // VARS USED IN LOOP
 int sensorValue = 0;
 long last_input_timer=10000 , timer = 0 ,just_in = 0; 

 
void setup() 
{
   pinMode(sensorPin, INPUT);
   pinMode(ledPin, OUTPUT);
   pinMode(IRPin, OUTPUT);
   Serial.begin(38400);
   Serial.println("Waiting for Infrared");
   
}
 

void print_signal()
{
            digitalWrite(ledPin, HIGH );
            int i=0;
            
            Serial.print("IR Code Intercepted ( ");
            Serial.print( max_ir_code);
            Serial.print(" segments every ");
            Serial.print(microsecond_step_delay);
            Serial.print(" microseconds )\n");
            for (i=0; i<max_ir_code; i++)
             {  
               Serial.print((unsigned int ) ir_on[i]);
             }
            just_in = 0;
            Serial.print("\nPlease wait\n");
            delay(3000);
            Serial.print("Hearing again\n");
            digitalWrite(ledPin, LOW );    
}

void pulseIR(long microsecs) 
{
  // we'll count down from the number of microseconds we are told to wait
 
  cli();  // this turns off any background interrupts
 
  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   digitalWrite(IRledPin, HIGH);  // this takes about 3 microseconds to happen
   delayMicroseconds(10);         // hang out for 10 microseconds
   digitalWrite(IRledPin, LOW);   // this also takes about 3 microseconds
   delayMicroseconds(10);         // hang out for 10 microseconds
 
   // so 26 microseconds altogether
   microsecs -= 26;
  }
 
  sei();  // this turns them back on
}

int playback_signal()
{
  int i=0, led_on = 0 , transitions = 0;
  digitalWrite(IRPin, LOW );
  for (i=0; i<max_ir_code; i++)
   {  
    if ( ir_on[i] ) 
      { // WILL SET LED TO ON , IF NOT ALREADY ON 
        if (!led_on) { digitalWrite(IRPin, HIGH ); led_on=1; ++transitions; }                
      } else
      {
        if (led_on) { digitalWrite(IRPin, LOW ); led_on=0; ++transitions; }    
      }
    delayMicroseconds(microsecond_step_delay); 
   }
  digitalWrite(IRPin, LOW );  
  
  Serial.print(transitions);
  Serial.print(" transitions\n");
  return transitions;
}


void mark_as_old_signal()
{
   timer=0;
   last_input_timer=10000;
}
 
int this_is_a_new_signal()
{
   if ( last_input_timer > timer  ) { return 0; } else
   if ( timer > last_input_timer+(50000/microsecond_step_delay)  ) { just_in = 1; max_ir_code=last_input_timer; return 0; }
   return 1;
}
 
void loop() 
{
  /* digitalWrite(ledPin, sensorValue); */  
    
   sensorValue = digitalRead(sensorPin);
     
   
   if ( sensorValue < 1000 ) 
    { 
      last_input_timer=timer;                   
      if ( timer < IR_CODE_LIMIT ) {   ir_on[timer]=1;
                                     //ir_on[timer]=(byte) ( 1024-sensorValue ) * (float) 0.2490  ; 
                                   } else
                                   { Serial.print("\nOverflow!\n"); }
                                   
    } else 
    {
      if ( timer < IR_CODE_LIMIT ) { ir_on[timer]=0; }else
                                   { Serial.print("\nOverflow!\n"); }
    }
    
    
    if (Serial.available() > 0) 
       {
		// read the incoming byte:
		byte incomingByte = Serial.read();

		// say what you got:
		Serial.print("Trying to retransmit signal!\n"); 
                playback_signal();
                mark_as_old_signal(); 
		Serial.print("Done!\n"); 
	}
   
   delayMicroseconds(microsecond_step_delay); 
   
   if ( !this_is_a_new_signal() )  
      { 
       if ( just_in ) 
          { 
            print_signal();
          }  
        mark_as_old_signal(); 
      }  else
      { 
        ++timer; 
      }   
      
 } 
