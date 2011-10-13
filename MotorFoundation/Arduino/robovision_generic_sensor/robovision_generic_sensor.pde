#define TOTAL_LED_PINS 2
int ledPins[TOTAL_LED_PINS];

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup(void) 
{
  Serial.begin(38400);
 
  ledPins[0]=13; // THERE ARE 2 LED PINS ON GUARDDOG
  ledPins[1]=13; // THERE ARE 2 LED PINS ON GUARDDOG
  
  pinMode(ledPins[0], OUTPUT);
  Serial.println("Waiting for Input!");
}


int LightControl(unsigned int light_number,int state)
{
  if (light_number>=TOTAL_LED_PINS) { Serial.print(light_number); return 0; }
  if (state) { digitalWrite(ledPins[light_number], HIGH ); return 1; } else
             { digitalWrite(ledPins[light_number], LOW );  return 1; }
}




int SerialInputReceiver()
{
         byte inB1 = Serial.read();
         byte inB2 = Serial.read();
         
         //This will become a switch statement :P
         // SERVO CONTROLS ----------------------
         if ( ( inB1 == 'S')&&( inB2 == 'M') ) 
           { // Servo Move
           } else
         if ( ( inB1 == 'S')&&( inB2 == 'S') ) 
           { // Servo Stop
           } else
         
         
         // LIGHT CONTROLS -----------------------
         if ( ( inB1 == 'L')&&( inB2 == 'A') ) 
           { // Light Activate 
             unsigned int light_number = Serial.read()-'0';  
             return LightControl(light_number,1);
           } else
         if ( ( inB1 == 'L')&&( inB2 == 'D') ) 
           { // Light Deactivate
             unsigned int light_number = Serial.read()-'0';  
             return LightControl(light_number,0);
           } else
         
         
         // INFRARED TRANSMIT -----------------------    
         if ( ( inB1 == 'I')&&( inB2 == 'R') ) 
           { // Servo Move
           } else
           
           
         // ULTRASONIC CONTROL -----------------------    
         if ( ( inB1 == 'U')&&( inB2 == 'G') ) 
           { // Servo Move
           } else
         if ( ( inB1 == 'U')&&( inB2 == 'T') ) 
           { // Servo Move
           } else
           
                     
         // ACCELEROMETER CONTROL -----------------------    
         if ( ( inB1 == 'A')&&( inB2 == 'X') ) 
           { // Servo Move
           } else
         if ( ( inB1 == 'A')&&( inB2 == 'Y') ) 
           { // Servo Move
           } else
         if ( ( inB1 == 'A')&&( inB2 == 'Z') ) 
           { // Servo Move
           }  
           
         // PROXIMITY IR CONTROL -----------------------    
         if ( ( inB1 == 'P')&&( inB2 == 'I') ) 
           { // Servo Move
           } else
           
           
           
         // CONTROLLER STATE  -----------------------    
         if ( ( inB1 == 'C')&&( inB2 == 'H') ) 
           { // Servo Move
           } else
         if ( ( inB1 == 'Z')&&( inB2 == 'Z') ) 
           { // Reset Arduino
             resetFunc();  //call reset 
           } else
         if ( ( inB1 == 'E')&&( inB2 == 'A') ) 
           { // Servo Move
           }  else
         if ( ( inB1 == 'S')&&( inB2 == 'A') ) 
           { // Servo Move
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
  
     
  
  
  
}
