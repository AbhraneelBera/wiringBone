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

// Standard header files
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "CommonDefines.h"
#include "SPI.h"
#include "OVERLAY.h"

uint8_t bitsPerWord = 8;
uint8_t delayUsecs = 0;

SPIClass::SPIClass(int deviceID)
{
  switch(deviceID)
  {
    case 0: sprintf(this -> device , "/dev/spidev1.0"); break;
    case 1: sprintf(this -> device , "/dev/spidev2.0"); break;
    default: perror("SPI initialization error");
  }
}

void SPIClass::begin()
{
  SPISettings defaultSetting;

  if((fd = open(this -> device, O_RDWR)) < 0)
  {
    perror("SPI device open failed");
    exit(-1);
  }

  if(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bitsPerWord) < 0)
  {
    perror("SPI device bits per word set failed");
  }
  this -> beginTransaction(defaultSetting);
}

uint8_t SPIClass::transfer(uint8_t data)
{
  this -> spiTransfer(&data, 1);
  return data;
}

uint16_t SPIClass::transfer16(uint16_t data)
{
  union
  {
    uint16_t val;
    struct
    {
      uint8_t lsb;
      uint8_t msb;
    };
  } value;
  value.val = data;
  if(this -> bit_order == MSBFIRST)
  {
   this -> spiTransfer(&(value.msb), 1);
   this -> spiTransfer(&(value.lsb), 1);
  }
  else
  {
   this -> spiTransfer(&(value.lsb), 1);
   this -> spiTransfer(&(value.msb), 1);
  }
  data = value.val;
  return data;
}

void SPIClass::transfer(void *buf, size_t count)
{
  this -> spiTransfer((unsigned char*)buf, count);
}

void SPIClass::end()
{
  close(fd);
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
  if(ioctl(this -> fd, SPI_IOC_WR_LSB_FIRST, &bitOrder) < 0)
  perror("SPI set bit order failed");
  this -> bit_order = bitOrder;
}

void SPIClass::setDataMode(uint8_t dataMode)
{
  switch(dataMode)
  {
    case SPI_MODE0 : if(ioctl(this -> fd, SPI_IOC_WR_MODE, SPI_MODE_0) < 0)
                     perror("SPI set data mode failed");
                     break;
    case SPI_MODE1 : if(ioctl(this -> fd, SPI_IOC_WR_MODE, SPI_MODE_1) < 0)
                     perror("SPI set data mode failed");
                     break;
    case SPI_MODE2 : if(ioctl(this -> fd, SPI_IOC_WR_MODE, SPI_MODE_2) < 0)
                     perror("SPI set data mode failed");
                     break;
    case SPI_MODE3 : if(ioctl(this -> fd, SPI_IOC_WR_MODE, SPI_MODE_3) < 0)
                     perror("SPI set data mode failed");
                     break;
    default : perror("Invalid data mode");
  }
}

void SPIClass::setClockDivider(uint8_t clockDiv)
{
  uint32_t clock = 16000000 / clockDiv;
  if(ioctl(this -> fd, SPI_IOC_WR_MAX_SPEED_HZ, &clock) < 0)
  perror("SPI set clock failed");
  this -> speed = clock;
}

void SPIClass::setClock(uint32_t clock_hz)
{
  if(ioctl(this -> fd, SPI_IOC_WR_MAX_SPEED_HZ, &clock_hz) < 0)
  perror("SPI set clock failed");
  this -> speed = clock_hz;
}

int SPIClass::spiTransfer(unsigned char *data, int length)
{
  struct spi_ioc_transfer spiDevice;
  spiDevice.tx_buf = (unsigned long) &data;
  spiDevice.rx_buf = (unsigned long) &data;
  spiDevice.len = length;
  spiDevice.delay_usecs = delayUsecs;
  spiDevice.bits_per_word = bitsPerWord;
  spiDevice.speed_hz = speed;

  if(ioctl(this -> fd, SPI_IOC_MESSAGE(1), &spiDevice) < 0)
  {
    perror("SPI transfer failed");
    return -1;
  }
  return 0;
}

#if (P9_17_MODE == spi) || (P9_18_MODE == spi) || (P9_21_MODE == spi) || (P9_22_MODE == spi)
SPIClass SPI(0);
#endif
#if (P9_28_MODE == spi) || (P9_29_MODE == spi) || (P9_30_MODE == spi) || (P9_31_MODE == spi) || (P9_42_MODE == spi)
SPIClass SPI1(1);
#endif