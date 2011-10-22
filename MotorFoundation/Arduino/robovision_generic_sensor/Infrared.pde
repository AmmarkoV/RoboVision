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
int IRledPin=9; 

// the maximum pulse we'll listen for - 65 milliseconds is a long time
#define MAXPULSE 65000
const uint8_t MAX_TOTAL_PULSES=130;

// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing default 20
#define RESOLUTION 10

// What percent we will allow in variation to match the same code
#define FUZZINESS 30

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses_high[MAX_TOTAL_PULSES]; //0
uint16_t pulses_low[MAX_TOTAL_PULSES];  //1

 
uint8_t currentpulse = 0; // index for pulses we're storing

#include "ircodes.h"
 
 

int IRcompare(int numpulses, int Signal[]) 
{
  
  for (int i=0; i< numpulses-1; i++) 
  {
    int oncode = pulses_low[i] * RESOLUTION / 10;
    int offcode = pulses_high[i+1] * RESOLUTION / 10;
    
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
      return 0;
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
      return 0;
    }
    
    //Serial.println();
  }
  // Everything matched!
  return 1;
}

int listenForIR(void) 
{
  currentpulse = 0;  
  while (1) 
  {
    uint16_t highpulse, lowpulse; // temporary storage timing
    highpulse = lowpulse = 0; // start out with no pulse length
  
    //while (digitalRead(IRpin))  // this is too slow!
    while (IRpin_PIN & (1 << IRpin)) 
    {
       // pin is still HIGH count off another few microseconds
       highpulse++;
       delayMicroseconds(RESOLUTION);

       // If the pulse is too long, we 'timed out' - either nothing
       // was received or the code is finished, so print what
       // we've grabbed so far, and then reset
       if ((highpulse >= MAXPULSE) && (currentpulse != 0)) 
        { 
          return currentpulse; 
        }
    }
    // we didn't time out so lets stash the reading
    pulses_high[currentpulse] = highpulse;
  
    // same as above
    while (! (IRpin_PIN & _BV(IRpin))) 
      {
        // pin is still LOW
        lowpulse++;
        delayMicroseconds(RESOLUTION);
        if ((lowpulse >= MAXPULSE) && (currentpulse != 0)) 
        { 
          return currentpulse; 
        }
      }
    pulses_low[currentpulse] = lowpulse;

    // we read one high-low pulse successfully, continue!
    currentpulse++;
    if ( currentpulse >= MAX_TOTAL_PULSES )
     {
       Serial.println("We have read too many pulses , they cannot fit in stack , failing..");
       return currentpulse;
     }
  }
  
 return currentpulse; 
}

void printpulses(void) 
{
  /*
  Serial.println("\n\r\n\rReceived: \n\rOFF \tON");
  for (uint8_t i = 0; i < currentpulse; i++) 
   {
     Serial.print(pulses_high[i] * RESOLUTION, DEC);
     Serial.print(" usec, ");
     Serial.print(pulses_low[i] * RESOLUTION, DEC);
     Serial.println(" usec");
   }*/
  
  // print it in a 'array' format
  
  Serial.print("int IRsignal_pulses = ");
  Serial.print((unsigned int) currentpulse);
  Serial.println(";");
  
  Serial.print("int IRsignal[] = {");
  Serial.println("// ON, OFF (in 10's of microseconds)");
  for (uint8_t i = 0; i < currentpulse-1; i++) 
   {
     Serial.print(pulses_low[i] * RESOLUTION / 10, DEC); Serial.print(",");
     Serial.print(pulses_high[i+1] * RESOLUTION / 10, DEC); Serial.println(",");
   }
  
  Serial.print(pulses_low[currentpulse-1] * RESOLUTION / 10, DEC);
  Serial.println(",0};");
}


void pulseON(long microsecs) 
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

void pulseON_2(int pulseTime) 
{
   unsigned long endPulse = micros() + pulseTime;        // create the microseconds to pulse for
    while( micros() < endPulse) 
    {
     digitalWrite(IRledPin, HIGH);                       // turn IR on
     delayMicroseconds(13);                              // half the clock cycle for 38Khz (26.32×10-6s) - e.g. the 'on' part of our wave
     digitalWrite(IRledPin, LOW);                        // turn IR off
     delayMicroseconds(13);                              // delay for the other half of the cycle to generate wave/ oscillation
   }
}

void pulseOFF(unsigned long startDelay) 
{
  unsigned long endDelay = micros() + startDelay;       // create the microseconds to delay for
  while(micros() < endDelay);
}

void takePicture() {
  for (int i=0; i < 2; i++) 
  {
    pulseON(2000);                                      // pulse for 2000 uS (Microseconds)
    pulseOFF(27850);                                    // turn pulse off for 27850 us
    pulseON(390);                                       // and so on
    pulseOFF(1580);
    pulseON(410);
    pulseOFF(3580);
    pulseON(400);
    pulseOFF(63200);
  }                                                     // loop the signal twice.
}


 
void SendCode(int numpulses,int Signal[]) 
{
   for (int i=0; i< numpulses-1; i++) 
     {
        pulseON(Signal[i*2+1]* RESOLUTION);  
        delayMicroseconds(Signal[i*2+0]* RESOLUTION);   
     }
}

void SendLastCode() 
{
   for (int i=0; i< currentpulse-1; i++) 
     { 
       pulseON(pulses_high[i]* RESOLUTION);    
       delayMicroseconds(pulses_low[i]* RESOLUTION);   
     }
}

void SpamInfrared(unsigned int how_much_time) 
{  
  digitalWrite(IRledPin, HIGH);       
  delay(1000);
  digitalWrite(IRledPin, LOW);       
  return;
  /*
  int i=how_much_time;
  while (  i > 1) 
    { 
      pulseON( how_much_time);    
      --i;
    }*/
}

void SendInfraredPreset(unsigned int preset) 
{
   switch (preset)
    {
      case 0 : SendCode(AirconditionOFF_pulses,AirConditionOFF); break;
      case 1 : SendCode(AirconditionON_pulses,AirconditionON);   break;
      case 2 : takePicture(); break;
      default :   Serial.println("Not a valid preset ( SendInfraredPreset ) ");
    }
}


