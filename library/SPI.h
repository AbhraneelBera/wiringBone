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

#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "PINS.h"

#define MSBFIRST 0
#define LSBFIRST 1

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

#define SPI_CLOCK_DIV2   2
#define SPI_CLOCK_DIV4   4
#define SPI_CLOCK_DIV8   8
#define SPI_CLOCK_DIV16  16
#define SPI_CLOCK_DIV32  32
#define SPI_CLOCK_DIV64  64
#define SPI_CLOCK_DIV128 128

class SPISettings
{
  public:
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
    {
      this -> clock = clock;
      this -> bitOrder = bitOrder;
      this -> dataMode = dataMode;
    }
    SPISettings()
    {
      SPISettings(4000000, MSBFIRST, SPI_MODE0);
    }
  uint32_t clock;
  uint8_t bitOrder;
  uint8_t dataMode;
  friend class SPI;
};

class SPIClass
{
  public:
    SPIClass(int deviceID);

    void begin();
    inline void beginTransaction(SPISettings settings)
    {
      this -> setClock(settings.clock);
      this -> setBitOrder(settings.bitOrder);
      this -> setDataMode(settings.dataMode);
    }
    uint8_t transfer(uint8_t data);
    uint16_t transfer16(uint16_t data);
    void transfer(void *buf, size_t count);
    inline void endTransaction(void)
    {
    }
    void end();
    void setBitOrder(uint8_t bitOrder);
    void setDataMode(uint8_t dataMode);
    void setClockDivider(uint8_t clockDiv);

    void setClock(uint32_t clock_hz);

  private:
    int fd;
    char device[16];
    uint32_t speed;
    uint8_t bit_order;

    int spiTransfer(unsigned char *data, int length);
};

#if (P9_17_MODE == spi) || (P9_18_MODE == spi) || (P9_21_MODE == spi) || (P9_22_MODE == spi)
extern SPIClass SPI;
#endif
#if (P9_28_MODE == spi) || (P9_29_MODE == spi) || (P9_30_MODE == spi) || (P9_31_MODE == spi) || (P9_42_MODE == spi)
extern SPIClass SPI1;
#endif
#endif