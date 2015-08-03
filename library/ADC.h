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

#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#include "PINS.h"
#include "CommonDefines.h"

typedef enum {AIN0=0, AIN1=1, AIN2=2, AIN3=3, AIN4=4, AIN5=5, AIN6=6} adcPin;

int readADC(adcPin pin);

void analogReadResolution(uint8_t bits);

int analogRead(adcPin pin);

int analogRead(uint8_t pin);

#endif