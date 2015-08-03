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

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "PINS.h"

#include "Stream.h"

class HardwareSerial : public Stream
{
  private:
    char device[128];
    int fd;

  public:
    HardwareSerial(void);
    HardwareSerial(int);
    HardwareSerial(const char*);

    void begin(unsigned long);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write;
};

size_t serialRead(int fd, void *buff, size_t nbytes);
size_t serialWrite(int fd, void *buff, size_t nbytes);

#if (P8_37_MODE == uart) || (P8_38_MODE == uart)
extern HardwareSerial Serial3;  // UART5
#endif

#if (P9_11_MODE == uart) || (P9_13_MODE == uart)
extern HardwareSerial Serial2;  // UART4
#endif

#if (P9_21_MODE == uart) || (P9_22_MODE == uart)
extern HardwareSerial Serial1;  // UART2
#endif

#if (P9_24_MODE == uart) || (P9_26_MODE == uart)
extern HardwareSerial Serial;  // UART1
#endif

#endif
