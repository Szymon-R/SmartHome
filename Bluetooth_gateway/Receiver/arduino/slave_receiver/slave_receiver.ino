// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <Wire.h>
#include "LED_driver.hpp"

union Buffer
{
  char array[4];
  float number;
};

int counter = 0;
Buffer b1;
bool data_valid = false;
LED display;
void setup() 
{
  Wire.begin(4);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  Serial.println("dupa");
  display.Init();
}



void loop() 
{
  float value = 0.01;
  display.SetValue(12.3f);
  delay(300);
    if (data_valid)
    {
      Serial.println(12.3);
      delay(300);
    }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) 
{
  //Serial.println("B");
  static bool counting = false;
  while (Wire.available())
  {
    unsigned char data = Wire.read();
    if (counting)
    {
      if (data == '$' && counter == 4 )
      {
        counting = false;
        counter = 0;
        data_valid = true;
        return;
      }
      else if (data == '$' && counter !=4)
      {
        counting = false;
        counter = 0;
        data_valid = false;
      }
      else
      {
        b1.array[counter] = data;
        ++counter;
        if (counter == 4)
        {
          //Serial.println(b1.number);
        }
      }
    }
    if (data == '#')
    {
      counting = true;
      counter = 0;
      data_valid = false;
    }
  }
}
/*
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) 
{
  //Serial.println("B");
  static bool counting = false;
  while (Wire.available())
  {
    unsigned char data = Wire.read();
    if (counting)
    {
      if (data == '$' && counter == 4 )
      {
        counting = false;
        counter = 0;
        data_valid = true;
        return;
      }
      else if (data == '$' && counter !=4)
      {
        counting = false;
        counter = 0;
        data_valid = false;
      }
      else
      {
        b1.array[counter] = data;
        ++counter;
        if (counter == 4)
        {
          //Serial.println(b1.number);
        }
      }
    }
    if (data == '#')
    {
      counting = true;
      counter = 0;
      data_valid = false;
    }
  }
}
*/
