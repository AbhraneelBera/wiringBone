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
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>
#include <stropts.h>
#include <asm/termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "CommonDefines.h"
#include "UART.h"
#include "OVERLAY.h"

extern int tcflush (int __fd, int __queue_selector) __THROW;

HardwareSerial::HardwareSerial(void)
{
}

HardwareSerial::HardwareSerial(int id)
{
  switch(id)
  {
    case 1: sprintf(this -> device , "/dev/ttyO1"); break;
    case 2: sprintf(this -> device , "/dev/ttyO2"); break;
    case 4: sprintf(this -> device , "/dev/ttyO4"); break;
    case 5: sprintf(this -> device , "/dev/ttyO5"); break;
    default: perror("UART initialization error");
  }
}

HardwareSerial::HardwareSerial(const char *dev)
{
  sprintf(this -> device , dev);
}

void HardwareSerial::begin(unsigned long baud)
{
  int status;
  struct termios2 terminal;

  if ((fd = open (this -> device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1)
  {
    perror("Failed to open serial device ");
    exit(-1);
  }

  fcntl (fd, F_SETFL, O_RDWR) ;

  ioctl(fd, TCGETS2, &terminal);

  terminal.c_cflag |= (CLOCAL | CREAD) ;
  terminal.c_cflag &= ~PARENB ;
  terminal.c_cflag &= ~CSTOPB ;
  terminal.c_cflag &= ~CSIZE ;
  terminal.c_cflag |= CS8 ;
  terminal.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG) ;
  terminal.c_oflag &= ~OPOST ;
  terminal.c_cflag &= ~CBAUD;
  terminal.c_cflag |= BOTHER;
  terminal.c_ispeed = baud;
  terminal.c_ospeed = baud;

  ioctl(fd, TCSETS2, &terminal);

  ioctl (fd, TIOCMGET, &status);

  status |= TIOCM_DTR ;
  status |= TIOCM_RTS ;

  ioctl (fd, TIOCMSET, &status);
}

void HardwareSerial::end()
{
  close(fd);
}


int HardwareSerial::available(void)
{
  int result ;

  if (ioctl (fd, FIONREAD, &result) == -1)
  return -1 ;

  return result ;
}

int HardwareSerial::peek(void)
{
  int c;
  FILE* fp = fdopen(fd, "r+");
  c = getc(fp);
  ungetc(c, fp);
  return c;
}

int HardwareSerial::read(void)
{
  uint8_t x ;

  if (serialRead (fd, &x, 1) != 1)
  return -1 ;

  return ((int)x) & 0xFF ;
}

void HardwareSerial::flush()
{
  ioctl (fd, TCFLSH, TCIOFLUSH) ;
}

size_t HardwareSerial::write(uint8_t c)
{
  serialWrite (fd, &c, 1) ;
  return 1;
}

size_t serialRead(int fd, void *buff, size_t nbytes)
{
  return read(fd, buff, nbytes);
}

size_t serialWrite(int fd, void *buff, size_t nbytes)
{
  return write(fd, buff, nbytes);
}

#if (P8_37_MODE == uart) || (P8_38_MODE == uart)
HardwareSerial Serial3(5);  // UART5
#endif

#if (P9_11_MODE == uart) || (P9_13_MODE == uart)
HardwareSerial Serial2(4);  // UART4
#endif

#if (P9_21_MODE == uart) || (P9_22_MODE == uart)
HardwareSerial Serial1(2);  // UART2
#endif

#if (P9_24_MODE == uart) || (P9_26_MODE == uart)
HardwareSerial Serial(1);  // UART1
#endif
