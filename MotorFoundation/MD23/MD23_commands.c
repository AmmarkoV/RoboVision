#include "usb_2_i2c.h"
#include "MD23_commands.h"
#include <stdio.h>
#include <stdlib.h>

int MD23_ok(struct md23_device * dev)
{
  if ( dev == 0 ) return 0;
  if ( (dev->i2c_dev.is_open==1) && ( dev->md23_found == 1 ) )
    {
      return 1;
    }
  printf("MD23_ok returning false.. Not ok!\n");
  return 0;
}


void MD23_MarkMotorShut(struct md23_device * dev,unsigned char motor_id)
{
  dev->past_motors[motor_id].finished=1;
  dev->motors[motor_id].finished=1;
  dev->future_motors[motor_id].finished=1;

  dev->past_motors[motor_id].power=128;
  dev->motors[motor_id].power=128;
  dev->future_motors[motor_id].power=128;
}


int MD23_Zero_encoders(struct md23_device * dev,char nocheck)
{
  //first I will just perform a check to see if the usb-i2c is still available
  printf("MD23_Zero_encoders\n");
  if ( (nocheck==1)||  (MD23_ok(dev) == 1)  )
    {
      unsigned char mess[9]={0};
      mess[0] = 0x55;                                        // USB_I2C tx/rx to md23
      mess[1] = (unsigned char)(dev->md23_address & 0xfe);    // md23 address and write bit (read bit cleared)
      mess[2] = 16;                                          // register 16 (command reg)
      mess[3] = 1;                                           // 1 data byte
      mess[4] = 0x20;                                        // zero encoders command
      dev->motors[0].encoder=0.0;
      dev->motors[1].encoder=0.0;
      i2c_transmit(&dev->i2c_dev,mess,5);
      i2c_recieve(&dev->i2c_dev,mess,1);                       // grab the response from usb-i2c
      if (mess[0] != 255)
        {
          return 1;  // if we connect, set indicator
        }
    }
  return 0;
}

int MD23_Stop_motors(struct md23_device * dev,char nocheck)
{
  //first I will just perform a check to see if the usb-i2c is still available
  printf("MD23_Stop_motors\n");
  if ( (nocheck==1)||  ( MD23_ok(dev) == 1)   )
    {
      unsigned char mess[9]={0};
      mess[0] = 0x55;                                        // USB_I2C tx/rx to md23
      mess[1] = (unsigned char)(dev->md23_address & 0xfe);    // md23 address and write bit (read bit cleared)
      mess[2] = 0;                                           // register 0
      mess[3] = 2;                                           // 2 data bytes
      mess[4] = 128;                                         // stop motor at 128
      mess[5] = 128;                                         // stop motor at 128
      dev->motors[0].power=128;
      dev->motors[1].power=128;
      i2c_transmit(&dev->i2c_dev,mess,6);
      i2c_recieve(&dev->i2c_dev,mess,1);                       // grab the response from usb-i2c
      if (mess[0] != 255)
        {
          return 1;  // if we connect, set indicator
        }
    }
  return 0;
}



int MD23_search_on_I2C(struct md23_device * dev)
{
  //first I will just perform a check to see if the usb-i2c is still available
  dev->md23_found=0;
  dev->md23_address = 0xb0 ; // Start address!
  dev->md23_address -= 2;

  if (i2c_Check_If_Availiable(&dev->i2c_dev)==1)
    {
      unsigned char mess[5]={0};
      printf("Scanning for MD23 @ ");
      while ( dev->md23_found == 0 )
        {
          dev->md23_address += 2;                                  // scans addresses B0,B2,B4,B6,B8,BA,BC and BE
          if (dev->md23_address > 0xbe)
            {
              dev->md23_address = 0xb0;
              dev->md23_found = 0;
              printf("\n");
              return dev->md23_found;
            }
          printf(" %X ",dev->md23_address);
          mess[0] = 0x55;                                                   // USB_I2C tx/rx to md23
          mess[1] = (unsigned char) (dev->md23_address | 0x01);                            // md23 address and read bit set
          mess[2] = 13;                                                     // start at version register
          mess[3] = 1;                                                      // and read 1 registers
          i2c_transmit(&dev->i2c_dev,mess,4);

          mess[0]=255;
          i2c_recieve(&dev->i2c_dev,mess,1);
          if (mess[0] != 255)
            {
              printf("OK!");
              dev->md23_found = 1;
            }                           // if we connect, set indicator
        }
    }
  else
    {
      printf("Cannot scan for MD23 without a working I2C device\n");
    }

  printf("\n");
  return ( dev->md23_found );
}



int MD23_movemotor(struct md23_device * dev,unsigned char motor_id,unsigned char power1,unsigned char power2)
{

  if (MD23_ok(dev) == 1)
    {
      unsigned char mess[7]={0};
      int bits_2_transmit=5; // 5 Bits if only one motor selected ( 0,1 = motor_id )
      mess[0] = 0x55;                           // USB_I2C tx/rx to md23
      mess[1] = (dev->md23_address & 0xfe);    // md23 address and write bit (read bit cleared)
      mess[2] = 0; // <- Default
      mess[3] = 1;  // 1 data byte
      mess[4] = power1;   //sends position of slider bar, data is a byte
      // TA PARAPANW EINAI RYTHMISEIS GIA SINGLE MOTOR  ( 0 i 1 , an exoume valei motor_id=2 tote ginontai ligo overrided )

      if (motor_id==0)
        {
          mess[2] = 0;
          dev->motors[0].power=power1;
          if ( power1 == 128 )
            {
              MD23_MarkMotorShut(dev,0);
            }
        }
      else  // register 0
        if (motor_id==1)
          {
            mess[2] = 1;
            dev->motors[1].power=power1;
            if ( power1 == 128 )
              {
                MD23_MarkMotorShut(dev,1);
              }
          }
        else  // register 1
          if (motor_id==2)
            {
              mess[2] = 0;
              mess[3] = 2;
              dev->motors[0].power=power1;
              dev->motors[1].power=power2;
              mess[4] = power1;
              mess[5] = power2;
              bits_2_transmit=6; // 6 Bits if both motors selected ( 2 = motor_id )
              if ( power1 == 128 )
                {
                  MD23_MarkMotorShut(dev,0);
                }
              if ( power2 == 128 )
                {
                  MD23_MarkMotorShut(dev,1);
                }
            } // register both



      //printf("Run MoveMotor , id =  %d  - power1 =  %d  , power2 =   %d  \n",motor_id,dev->motors[0].power,dev->motors[1].power);
      if ( i2c_transmit(&dev->i2c_dev,mess,bits_2_transmit) <bits_2_transmit ) printf("Errot sending \n");

      mess[0] = 255;
      if ( i2c_recieve(&dev->i2c_dev,mess,1)<1 ) printf("Errot receiving \n");
      if (mess[0] != 255)
        {
          return 1;  // if we connect, set indicator
        }
      return 0;
    }
  return 0;
}



int MD23_ReadStatus(struct md23_device * dev,char nocheck)
{
  if  ((nocheck==1)||(MD23_ok(dev) == 1) )
    {
      unsigned char mess[37]={0};
      mess[0] = 0x55;                               // USB_I2C tx/rx to md23
      mess[1] = (dev->md23_address | 0x01);          // md23 address and read bit set
      mess[2] = 0;                                  // start at register 0
      mess[3] = 14;                                 // and read 14 registers
      i2c_transmit(&dev->i2c_dev,mess,4);
      mess[13]=255;
      unsigned int ret=i2c_recieve(&dev->i2c_dev,mess,14);


      if (ret==14)
        {
          dev->voltage = mess[10];           // move the battery voltage into a double, for decimal divide
          dev->voltage /= 10;

          dev->motors[0].encoder = (mess[2] << 24) + (mess[3] << 16) + (mess[4] << 8) + mess[5];
          dev->motors[1].encoder = (mess[6] << 24) + (mess[7] << 16) + (mess[8] << 8) + mess[9];

          dev->motors[0].power=mess[0];
          dev->motors[1].power=mess[1];
          dev->motors[0].current=mess[11];
          dev->motors[1].current=mess[12];
          return 1;
        }
      else
        {
          dev->md23_found = 0;
          return 0;
        }
    }
  return 0;
}

