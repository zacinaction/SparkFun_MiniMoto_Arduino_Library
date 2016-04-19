/****************************************************************
Example code demonstrating the use of the Arduino Library for
the SparkFun MiniMoto board, which uses the TI DRV8830 IC for I2C
low-voltage DC motor control.

This code is beerware; if you use it, please buy me (or any other
SparkFun employee) a cold beverage next time you run into one of
us at the local.

17 Sep 2013- Mike Hord, SparkFun Electronics

Code developed in Arduino 1.0.5, on a Fio classic board.

**Updated for Arduino 1.6.4 5/2015**
****************************************************************/


#include <Arduino.h>
#include "SparkFunMiniMoto.h"

// The address of the part is set by a jumper on the board. 
//  See datasheet or schematic for details; default is 0xD0.
MiniMoto::MiniMoto(byte addr)
{
  _addr = addr;
}

// Return the fault status of the DRV8830 chip. Also clears any existing faults.
byte MiniMoto::getFault()
{
  byte buffer = 0;
  byte clearFault = 0x80;
  buffer = I2CreadByte(_addr, 0x01);
  I2CWriteByte(_addr, 0x01, clearFault);
  return buffer;
}

// Send the drive command over I2C to the DRV8830 chip. Bits 7:2 are the speed
//  setting; range is 0-63. Bits 1:0 are the mode setting:
//  - 00 = HI-Z
//  - 01 = Reverse
//  - 10 = Forward
//  - 11 = H-H (brake)
void MiniMoto::drive(int speed)
{
  byte regValue = 0x80;             // Before we do anything, we'll want to
                                    //  clear the fault status. To do that
                                    //  write 0x80 to register 0x01 on the
                                    //  DRV8830.
  I2CWriteByte(_addr, 0x01, regValue); // Clear the fault status.

  regValue = (byte)abs(speed);      // Find the byte-ish abs value of the input
  if (regValue > 63) regValue = 63; // Cap the value at 63.
  regValue = regValue<<2;           // Left shift to make room for bits 1:0
  if (speed < 0) regValue |= 0x01;  // Set bits 1:0 based on sign of input.
  else           regValue |= 0x02;
  
  I2CWriteByte(_addr, 0x00, regValue);
}

// Coast to a stop by hi-z'ing the drivers.
void MiniMoto::stop()
{
  byte regValue = 0;                // See above for bit 1:0 explanation.
  
  I2CWriteByte(_addr, 0x00, regValue); 
}

// Stop the motor by providing a heavy load on it.
void MiniMoto::brake()
{
  byte regValue = 0x03;                // See above for bit 1:0 explanation.
  
  I2CWriteByte(_addr, 0x00, regValue); 
}

void MiniMoto::I2CreadByte(uint8_t address, uint8_t subAddress)
{
  int timeout = DRV8830_COMMUNICATION_TIMEOUT;
  uint8_t data; // `data` will store the register data  
  
  Wire.beginTransmission(address);         // Initialize the Tx buffer
  Wire.write(subAddress);                  // Put slave register address in Tx buffer
  Wire.endTransmission(true);             // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom(address, (uint8_t) 1);  // Read one byte from slave register address 
  while ((Wire.available() < 1) && (timeout-- > 0))
    delay(1);
  
  if (timeout <= 0)
    return 255; //! Bad! 255 will be misinterpreted as a good value.
  
  data = Wire.read();                      // Fill Rx buffer with result
  return data;                             // Return data read from slave register
}

void MiniMoto::::I2CwriteByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
  Wire.beginTransmission(address);  // Initialize the Tx buffer
  Wire.write(subAddress);           // Put slave register address in Tx buffer
  Wire.write(data);                 // Put data in Tx buffer
  Wire.endTransmission();           // Send the Tx buffer
}
