/*Arduio example code for MD25 and LCD03 using the wire and software serial librarys*/

#include <SoftwareSerial.h>
#include <Wire.h>

#define md25Address 0x58                                    // Address of the MD25
#define softwareReg 0x0D                                    // Byte to read the software version
#define speed1 0x00                                         // Byte to send speed to first motor
#define speed2 0x01                                         // Byte to send speed to second motor
#define cmdByte 0x10                                        // Command byte
#define mvCsr 0x02                                          // Command for LCD03 to move cursor
#define hideCsr 0x04                                        // Byte to hide LCD03 cursor
#define clrScrn 0x0C                                        // Byte to clear LCD03 screen
#define encoderOne 0x02                                     // Byte to read motor encoder 1
#define encoderTwo 0x06                                     // Byte to read motor encoder 2
#define voltRead 0x0A                                       // Byte to read battery volts
#define rxPin 2                                             // software serial rx pin
#define txPin 3                                             // software serial tx pin

SoftwareSerial lcd_03 =  SoftwareSerial(rxPin, txPin);      // Defines software serial port for LCD03

int x=0;                                                    // int x stores a value to be used as speed data for the motors

void setup(){
  Wire.begin();
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  lcd_03.begin(9600);                                       // Begin serial for LCD03
  delay(100);                                               // Wait for everything to power up
  lcd_03.print(clrScrn, BYTE);          
  lcd_03.print(hideCsr, BYTE);
  lcd_03.print("MD25  Example   V:");
  int softVer = getSoft();                                  // Gets the software version of MD25
  lcd_03.print(softVer, DEC);                               // Print software version to the screen

  encodeReset();                                            // Cals a function that resets the encoder values to 0
}

void loop(){
 x = 255;                                                   // Put a value of 255 in x, this will dive motors forward at full speed
 
 do{                                                        // Start loop to drive motors forward
    Wire.beginTransmission(md25Address);                    // Drive motor 2 at speed value stored in x
    Wire.send(speed2);
    Wire.send(x);                                           
    Wire.endTransmission();
  
    Wire.beginTransmission(md25Address);                    // Drive motor 1 at speed value stored in x
    Wire.send(speed1);
    Wire.send(x);
    Wire.endTransmission();
 
    encoder1();                                             // Calls a function that reads and displays value of encoder 1 to LCD03
    encoder2();                                             // Calls a function that reads and displays value of encoder 2 to LCD03
    volts();                                                // Calls function that reads and displays battery volts
    if(encoder1() > 4500)                                   // When value of encoder 1 is greater than 4500 put 138 in x to slow motor down
      x = 138;
 }while(encoder1() < 5000);                                 // Loop untill encoder 1 is 5000 or more
 
 stopMotor();                                               // Calls function to stop motors
 delay(1000);
 
 x = 0;                                                     // Put a value of 0 in x, this will drive motors at full reverse speed
 
 do{                                                        // Start loop to drive motors reverse
    Wire.beginTransmission(md25Address);                    // Drive motor 2 at speed value stored in x
    Wire.send(speed2);
    Wire.send(x);
    Wire.endTransmission();
  
    Wire.beginTransmission(md25Address);                    // Drive motor 1 at speed value stored in x
    Wire.send(speed1);
    Wire.send(x);
    Wire.endTransmission();
  
    encoder1();                                             // Calls a function that reads and displays value of encoder 1 to LCD03
    encoder2();                                             // Calls a function that reads and displays value of encoder 2 to LCD03
    volts();                                                // Calls function that reads and displays battery volts
    if(encoder1() < 500)                                    // When value of encoder 1 is less than 500 put 118 in x to slow motor down
      x = 118;   
  }while(encoder1() > 0);                                   // Loop untill encoder 1 is 0 or less
  
  stopMotor();                                              // Calls function to stop motors
  delay(1000);
}

int getSoft(){                                              // Function that gets the software version
  Wire.beginTransmission(md25Address);                      // Send byte to read software version as a single byte
  Wire.send(softwareReg);
  Wire.endTransmission();
  
  Wire.requestFrom(md25Address, 1);                         // request 1 byte form MD25
  while(Wire.available() < 1);                              // Wait for it to arrive
  int software = Wire.receive();                            // Read it in
  
  return(software);
}

void encodeReset(){                                         // This function resets the encoder values to 0
  Wire.beginTransmission(md25Address);
  Wire.send(cmdByte);
  Wire.send(0x20);                                          // Putting the value 0x20 to reset encoders
  Wire.endTransmission(); 
}

long encoder1(){                                            // Function to read and display value of encoder 1 as a long
  Wire.beginTransmission(md25Address);                      // Send byte to get a reading from encoder 1
  Wire.send(encoderOne);
  Wire.endTransmission();
  
  Wire.requestFrom(md25Address, 4);                         // Request 4 bytes from MD25
  while(Wire.available() < 4);                              // Wait for 4 bytes to arrive
  long firstByte = Wire.receive();                          // Recieve 4 bytes highest byte first
  long secondByte = Wire.receive();
  long thirdByte = Wire.receive();
  long fourthByte = Wire.receive();
  
  long poss1 = (firstByte << 24) + (secondByte << 16) + (thirdByte << 8) + fourthByte;  // Put them together
  
  lcd_03.print(mvCsr, BYTE);  
  lcd_03.print(21, BYTE);                                   // Move cursor to space 21
  lcd_03.print("Encoder 1 ");
  lcd_03.print(poss1, DEC);                                 // Print encoder 1 value to LCD03
  lcd_03.print("   ");                                      // Print some spaces to clear space after encoder value
  delay(50);                                                // Wait for everything to make sure everything is sent
  
  return(poss1);                                            // returns value of encoder 1 position as a long
}

long encoder2(){                                            // Function to read and display velue of encoder 2 as a long
  Wire.beginTransmission(md25Address);                      // Send byte to get a reading from encoder 2                   
  Wire.send(encoderTwo);
  Wire.endTransmission();
  
  Wire.requestFrom(md25Address, 4);                         // Request 4 bytes from MD25
  while(Wire.available() < 4);                              // Wait for 4 bytes to become available
  long firstByte = Wire.receive();                          // Get four bytes highest byte first
  long secondByte = Wire.receive();
  long thirdByte = Wire.receive();
  long fourthByte = Wire.receive();
  
  long poss2 = (firstByte << 24) + (secondByte << 16) + (thirdByte << 8) + fourthByte;  // Put them together
  
  lcd_03.print(mvCsr, BYTE);
  lcd_03.print(41, BYTE);                                   // Move cursor to space 41
  lcd_03.print("Encoder 2 ");
  lcd_03.print(poss2, DEC);                                 // Print value of encoder 2 position
  lcd_03.print("   ");                                      // Print some spaces to clear space after encoder value
  delay(50);                                                // Wait for everything to make sure everything is sent
   
  return(poss2);                                            // Returns encoder 2 position as a long
}

void volts(){                                               // Function to read and display battery volts as a single byte
  Wire.beginTransmission(md25Address);                      // Send byte to read volts
  Wire.send(voltRead);
  Wire.endTransmission();
  
  Wire.requestFrom(md25Address, 1);                         // Request 1 byte form MD25
  while(Wire.available() < 1);                              // Wait for byte to arrive 
  int batteryVolts = Wire.receive();                        // Get byte
  
  lcd_03.print(mvCsr, BYTE);
  lcd_03.print(61, BYTE);                                   // Move cursor to space 61
  lcd_03.print("Battery v = ");
  lcd_03.print(batteryVolts/10, DEC);                       // Print batery volts / 10 to give you the number of whole volts
  lcd_03.print(".");                                        // As arduino has truncates floating point numbers we print a . here to act as a decimal point
  lcd_03.print(batteryVolts%10, DEC);                       // Prints Battery volts % 10 to give us the 
  
  delay(50);                                                // Wait to make sure everything is sent
}

void stopMotor(){                                           // Function to stop motors
  Wire.beginTransmission(md25Address);
  Wire.send(speed2);
  Wire.send(128);                                           // Sends a value of 128 to motor 2 this value stops the motor
  Wire.endTransmission();
  
  Wire.beginTransmission(md25Address);
  Wire.send(speed1);
  Wire.send(128);                                           // Sends a value of 128 to motor 1 this value stops the motor
  Wire.endTransmission();
}  
