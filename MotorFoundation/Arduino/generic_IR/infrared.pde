/* Raw IR commander
This sketch/program uses the Arduno and a PNA4602 to
decode IR received. It then attempts to match it to a previously
recorded IR signal
Code is public domain, check out www.ladyada.net and adafruit.com
for more tutorials!
*/

// We need to use the 'raw' pin reading methods
// because timing is very important here and the digitalRead()
// procedure is slower!
//uint8_t IRpin = 2;
// Digital pin #2 is the same as Pin D2 see
// http://arduino.cc/en/Hacking/PinMapping168 for the 'raw' pin mapping
#define IRpin_PIN PIND
#define IRpin 2
int IRledPin=7;
int ledPin = 13; // LED PIN OUT  

// the maximum pulse we'll listen for - 65 milliseconds is a long time
#define MAXPULSE 65000

// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20

// What percent we will allow in variation to match the same code
#define FUZZINESS 20

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[100][2]; // pair is high and low pulse
uint8_t currentpulse = 0; // index for pulses we're storing

#include "ircodes.h"

int IRsignal[] = {

// ON, OFF (in 10's of microseconds)

	886, 438,

	58, 50,

	60, 50,

	60, 52,

	58, 50,

	60, 50,

	60, 52,

	58, 160,

	62, 50,

	58, 162,

	60, 160,

	60, 160,

	58, 52,

	60, 160,

	60, 160,

	60, 52,

	58, 160,

	60, 160,

	60, 162,

	58, 162,

	60, 50,

	58, 52,

	60, 50,

	60, 50,

	58, 52,

	60, 50,

	60, 50,

	58, 50,

	62, 160,

	60, 160,

	60, 160,

	60, 162,

	58, 162,

	60, 4020,

	884, 216,

	60, 2872,

	884, 216,

	60, 0};

void setup(void) 
{
  Serial.begin(38400);
 
  pinMode(ledPin, OUTPUT);
  pinMode(IRledPin, OUTPUT);
  Serial.println("Ready to decode IR!");
}

void loop(void) 
{
  int numberpulses;
  
  //numberpulses = listenForIR();
  
  if (Serial.available() > 0) 
       { 
	 byte incomingByte = Serial.read();
         
         if (incomingByte == 'r')
           {
             digitalWrite(ledPin, HIGH);    
             Serial.print("Waiting signal!\n"); 
             numberpulses = listenForIR();
               
             Serial.print("Heard ");
             Serial.print(numberpulses);
             Serial.println("-pulse long IR signal");
  
              printpulses();
  
             if (IRcompare(numberpulses, IRsignal)) { Serial.println("PLAY"); }  
           } else
         if (incomingByte == 'w')
           {
             digitalWrite(ledPin, HIGH);    
             Serial.print("Trying to retransmit signal!\n"); 
             SendLastCode(); 
	     Serial.print("Done!\n"); 
           } else 
          if (incomingByte == 'h') 
	   {
            digitalWrite(IRledPin, HIGH);
           } else 
          if (incomingByte == 'l') 
	   {
            digitalWrite(IRledPin, LOW);  
           }
           
           
          Serial.println("\nWaiting for serial command");
          digitalWrite(ledPin, LOW ); 
      }
  
     
  
  
  
}

boolean IRcompare(int numpulses, int Signal[]) {
  
  for (int i=0; i< numpulses-1; i++) {
    int oncode = pulses[i][1] * RESOLUTION / 10;
    int offcode = pulses[i+1][0] * RESOLUTION / 10;
    
    /*
Serial.print(oncode); // the ON signal we heard
Serial.print(" - ");
Serial.print(Signal[i*2 + 0]); // the ON signal we want
*/
    
    // check to make sure the error is less than FUZZINESS percent
    if ( abs(oncode - Signal[i*2 + 0]) <= (Signal[i*2 + 0] * FUZZINESS / 100)) {
      //Serial.print(" (ok)");
    } else {
      //Serial.print(" (x)");
      // we didn't match perfectly, return a false match
      return false;
    }
    
    /*
Serial.print(" \t"); // tab
Serial.print(offcode); // the OFF signal we heard
Serial.print(" - ");
Serial.print(Signal[i*2 + 1]); // the OFF signal we want
*/
    
    if ( abs(offcode - Signal[i*2 + 1]) <= (Signal[i*2 + 1] * FUZZINESS / 100)) {
      //Serial.print(" (ok)");
    } else {
      //Serial.print(" (x)");
      // we didn't match perfectly, return a false match
      return false;
    }
    
    //Serial.println();
  }
  // Everything matched!
  return true;
}

int listenForIR(void) {
  currentpulse = 0;
  
  while (1) 
  {
    uint16_t highpulse, lowpulse; // temporary storage timing
    highpulse = lowpulse = 0; // start out with no pulse length
  
// while (digitalRead(IRpin)) { // this is too slow!
    while (IRpin_PIN & (1 << IRpin)) {
       // pin is still HIGH

       // count off another few microseconds
       highpulse++;
       delayMicroseconds(RESOLUTION);

       // If the pulse is too long, we 'timed out' - either nothing
       // was received or the code is finished, so print what
       // we've grabbed so far, and then reset
       if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
         return currentpulse;
       }
    }
    // we didn't time out so lets stash the reading
    pulses[currentpulse][0] = highpulse;
  
    // same as above
    while (! (IRpin_PIN & _BV(IRpin))) {
       // pin is still LOW
       lowpulse++;
       delayMicroseconds(RESOLUTION);
       if ((lowpulse >= MAXPULSE) && (currentpulse != 0)) {
         return currentpulse;
       }
    }
    pulses[currentpulse][1] = lowpulse;

    // we read one high-low pulse successfully, continue!
    currentpulse++;
  }
}

void printpulses(void) {
  Serial.println("\n\r\n\rReceived: \n\rOFF \tON");
  for (uint8_t i = 0; i < currentpulse; i++) {
    Serial.print(pulses[i][0] * RESOLUTION, DEC);
    Serial.print(" usec, ");
    Serial.print(pulses[i][1] * RESOLUTION, DEC);
    Serial.println(" usec");
  }
  
  // print it in a 'array' format
  Serial.println("int IRsignal[] = {");
  Serial.println("// ON, OFF (in 10's of microseconds)");
  for (uint8_t i = 0; i < currentpulse-1; i++) {
    //Serial.print("\t"); // tab
    Serial.print(pulses[i][1] * RESOLUTION / 10, DEC);
    Serial.print(", ");
    Serial.print(pulses[i+1][0] * RESOLUTION / 10, DEC);
    Serial.println(",");
  }
  Serial.print("\t"); // tab
  Serial.print(pulses[currentpulse-1][1] * RESOLUTION / 10, DEC);
  Serial.print(", 0};");
}


void pulseIR(long microsecs) 
{
  // we'll count down from the number of microseconds we are told to wait
 
  cli();  // this turns off any background interrupts
 
  while (microsecs > 0) 
  {
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

 
void SendCode(int numpulses,int Signal[]) 
{
   for (int i=0; i< numpulses-1; i++) 
     {
        pulseIR(Signal[i*2+1]* RESOLUTION);  
        delayMicroseconds(Signal[i*2+0]* RESOLUTION);   
     }
}

void SendLastCode() 
{
   for (int i=0; i< currentpulse-1; i++) 
     { 
       pulseIR(pulses[i][0]* RESOLUTION);    
       delayMicroseconds(pulses[i][1]* RESOLUTION);   
     }
}

