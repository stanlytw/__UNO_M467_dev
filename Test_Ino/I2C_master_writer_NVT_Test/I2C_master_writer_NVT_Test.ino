// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup()
{
  
  Serial1.begin(115200);
  Serial1.print("I2C Test");
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop()
{
  //Serial.print("**I am alive2:");
  Wire.beginTransmission(0x68); // transmit to device #4
  Wire.write(0xCC); // transmit to device #4
  Wire.write(0x33); // transmit to device #4
  Wire.endTransmission();    // stop transmitting

  x++;
  delay(1000);
  Serial1.print("I2C Looping:  ");
  Serial1.print(x, DEC);
  Serial1.println(" times");
}
