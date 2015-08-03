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

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#include "PINS.h"
#include "CommonDefines.h"

class GPIO
{
  public:
    GPIO();
    ~GPIO();
    virtual int gpioConfig(uint8_t pin, uint8_t direction);
    virtual int writeValue(uint8_t pin, uint8_t value);
    virtual uint8_t readValue(uint8_t pin);
  private:
    typedef enum {unexported, exported} gpioStatus;
    gpioStatus gpioPin[128];
    int setDirection(uint8_t pin, uint8_t direction);
    int exportPin(uint8_t pin);
    int unexportPin(uint8_t pin);
};

extern GPIO *_gpio;

GPIO* gpioInstance();

void digitalWrite(Pin pin, bool state);
uint8_t digitalRead(Pin pin);

#endif