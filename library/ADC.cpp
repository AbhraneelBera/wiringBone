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
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <iostream>
#include <fstream>

#include "CommonDefines.h"
#include "ADC.h"

#define defaultResolution 12

static uint8_t readResolution = 10;

int readADC(adcPin pin)
{
  int value;
  char path[52];
  sprintf(path, "/sys/bus/iio/devices/iio:device0/in_voltage%d_raw", (uint8_t)pin);
  std::ifstream analogFile(path);
  analogFile >> value;
  analogFile.close();
  return value;
}

void analogReadResolution(uint8_t bits)
{
  readResolution = bits;
}

int analogRead(adcPin pin)
{
  uint32_t value = readADC(pin);

  if(readResolution <= defaultResolution)
  value = value >> (defaultResolution - readResolution);
  else
  value = value << (readResolution - defaultResolution);
  return(value);
}

int analogRead(uint8_t pin)
{
  return(analogRead(static_cast<adcPin>(pin)));
}