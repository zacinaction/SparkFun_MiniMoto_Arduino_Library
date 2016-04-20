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


#ifndef SparkFunMiniMoto_h
#define SparkFunMiniMoto_h

#include <Arduino.h>

// I2C support constants
#define DRV8830_COMMUNICATION_TIMEOUT 15

// Fault constants
#define FAULT 0x01
#define ILIMIT 0x10
#define OTS 0x08
#define UVLO 0x04
#define OCP 0x02

class MiniMoto
{
  public:
    MiniMoto(byte addr);
    void init();
    void drive(int speed);
    void stop();
    void brake();
    uint8_t getFault();

  private:
    // I2CReadByte() -- Read a single byte from a register over I2C.
    // Input:
    //  - address = The 7-bit I2C address of the slave device.
    //  - subAddress = The register to be read from.
    // Output:
    //  - The byte read from the requested address.
    uint8_t I2CReadByte(uint8_t address, uint8_t subAddress);

    // I2CWriteByte() -- Write a byte out of I2C to a register in the device
    // Input:
    //  - address = The 7-bit I2C address of the slave device.
    //  - subAddress = The register to be written to.
    //  - data = Byte to be written to the register.
    void I2CWriteByte(uint8_t address, uint8_t subAddress, uint8_t data);
    
    uint8_t _addr;
};

#endif