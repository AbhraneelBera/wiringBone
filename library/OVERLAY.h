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

#ifndef OVERLAY_H
#define OVERLAY_H

#include <stdint.h>
#include <iostream>

#include "PINS.h"
#include "CommonDefines.h"

class OVERLAY
{
  public:
  OVERLAY();
  ~OVERLAY();
  private:
  int configOverlay(Pin pin);
  int restoreOverlay(Pin pin);
  bool capeLoaded(std::string path, std::string capeName);
  int loadCape(std::string capeName);
};

void pinMode(Pin pin, uint8_t direction);

#endif