/*
    This file is a part of the wiringBone library
    Copyright (C) 2015 Abhraneel Bera

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA
*/

#ifndef TWI_H
#define TWI_H

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include "i2c-dev.h"

#define TWI_BUFFER_LENGTH 32

static void (*twi_onSlaveTransmit)(void);
static void (*twi_onSlaveReceive)(uint8_t*, int);

int fd_i2c, regAddress;

void twi_init()
{
  if((fd_i2c = open("/dev/i2c-1", O_RDWR)) < 0)
  perror("i2c open failed");
}

int twi_setAddress(int deviceAddress)
{
  if((ioctl(fd_i2c, I2C_SLAVE, deviceAddress)) < 0)
  return -1;

  return fd_i2c;
}

uint8_t twi_writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop)
{
  int fd = twi_setAddress(address);
  if(TWI_BUFFER_LENGTH < length)
  return 1;
  //if(sendStop == 1)  // To be implemented
  //{
    if(length == 0)
    {
      regAddress = 0;
      return 0;
    }
    else if(length == 1)
    {
      regAddress = data[0];
      if(i2c_smbus_write_byte(fd, data[0]) < 0)
      {
        perror("Failed to write byte");
        return 1;
      }
    }
    else
    {
      regAddress = 0;
      if(i2c_smbus_write_i2c_block_data(fd, data[0], length-1, data+1) < 0)
      {
        perror("Failed to write bytes");
        return 1;
      }
    }
  //}
  //else
  //{ // To be implemented 
  //}
  return 0;
}

uint8_t twi_readFrom(uint8_t address, uint8_t* data, uint8_t length, uint8_t sendStop)
{
  int fd = twi_setAddress(address);
  if(TWI_BUFFER_LENGTH < length)
  return 0;
  //if(sendStop == 1)  // To be implemented
  //{
      if(i2c_smbus_read_i2c_block_data(fd, regAddress, length, data) < 0)
      {
        perror("Failed to read bytes");
        return 0;
      }
  //}
  //else
  //{ // To be implemented 
  //}
  return length;
}

uint8_t twi_transmit(const uint8_t* data, uint8_t length)
{
  // Function for slave mode. Not required.
  // Dummy
  return 0;
}

void twi_attachSlaveRxEvent( void (*function)(uint8_t*, int) )
{
  twi_onSlaveReceive = function;
}

void twi_attachSlaveTxEvent( void (*function)(void) )
{
  twi_onSlaveTransmit = function;
}

void setWriteError()
{
  // Dummy
}
#endif