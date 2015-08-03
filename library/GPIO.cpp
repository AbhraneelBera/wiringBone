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

#include "CommonDefines.h"
#include "GPIO.h"
#include "OVERLAY.h"

GPIO::GPIO()
{
  for(int count = 0; count < 128; count++)
  gpioPin[count] = unexported;
}

int GPIO::exportPin(uint8_t pin)
{
  if(gpioPin[pin] == exported)
  return -1;
  FILE *fd;
  if((fd = fopen("/sys/class/gpio/export", "w")) == NULL)
  {
    perror("GPIO export failed");
    return -1;
  }
  if((fprintf(fd, "%d", pin)) < 0)
  {
    perror("GPIO export failed");
    return -1;
  }
  fclose(fd);
  return pin;
}

int GPIO::unexportPin(uint8_t pin)
{
  if(gpioPin[pin] == unexported)
  return -1;
  FILE *fd;
  if((fd = fopen("/sys/class/gpio/unexport", "w")) == NULL)
  {
    perror("GPIO unexport failed");
    return -1;
  }
  fprintf(fd, "%d", pin);
  fclose(fd);
  return pin;
}

int GPIO::setDirection(uint8_t pin, uint8_t direction)
{
  FILE *fd;
  char path[34];
  sprintf(path, "/sys/class/gpio/gpio%d/direction", pin);
  if((fd = fopen(path, "w")) == NULL)
  {
    perror("GPIO set direction failed");
    return -1;
  }
  switch(direction)
  {
    case INPUT  : fprintf(fd, "%s", "in"); break;
    case OUTPUT : fprintf(fd, "%s", "out"); break;
    default     : perror("Invalid direction"); return -1;
  }
  fclose(fd);
  return direction;
}

int GPIO::gpioConfig(uint8_t pin, uint8_t direction)
{
  if((this->exportPin(pin)) >= 0)
  gpioPin[pin] = exported;
  this->setDirection(pin,direction);
  return direction;
}

int GPIO::writeValue(uint8_t pin, uint8_t value)
{
  FILE *fd;
  char path[30];
  sprintf(path, "/sys/class/gpio/gpio%d/value", pin);
  if((fd = fopen(path, "w")) == NULL)
  {
    perror("GPIO write failed");
    return -1;
  }
  fprintf(fd, "%d", value);
  fclose(fd);
  return value;
}

uint8_t GPIO::readValue(uint8_t pin)
{
  FILE *fd;
  int value;
  char path[30];
  sprintf(path, "/sys/class/gpio/gpio%d/value", pin);
  if((fd = fopen(path, "r")) == NULL)
  {
    perror("GPIO read failed");
    return -1;
  }
  fscanf(fd, "%d", &value);
  fclose(fd);
  return (uint8_t)value;
}

GPIO::~GPIO()
{
  for(int count = 0; count < 128; count++)
  {
    if(gpioPin[count] == exported)
    {
      this->writeValue(count, 0);
      this->unexportPin(count);
    }
  }
}

GPIO *_gpio;

GPIO* gpioInstance()
{
  GPIO* instance = new GPIO();
  return instance;
}

void digitalWrite(Pin pin, bool state)
{
  _gpio->writeValue(pin.pinNum, state);
}

uint8_t digitalRead(Pin pin)
{
  return(_gpio->readValue(pin.pinNum));
}