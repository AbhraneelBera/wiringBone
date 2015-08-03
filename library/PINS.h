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

#ifndef PINS_H
#define PINS_H

#include "../UserPinConfig.h"

typedef enum {disabled, gpio, pwm, pruin, pruout, i2c, uart, spi} PinModes;

typedef enum {none, hdmi, audio, emmc} VirtualCape;

extern PinModes P8_3_modes[];
extern PinModes P8_4_modes[];
extern PinModes P8_5_modes[];
extern PinModes P8_6_modes[];
extern PinModes P8_7_modes[];
extern PinModes P8_8_modes[];
extern PinModes P8_9_modes[];
extern PinModes P8_10_modes[];
extern PinModes P8_11_modes[];
extern PinModes P8_12_modes[];
extern PinModes P8_13_modes[];
extern PinModes P8_14_modes[];
extern PinModes P8_15_modes[];
extern PinModes P8_16_modes[];
extern PinModes P8_17_modes[];
extern PinModes P8_18_modes[];
extern PinModes P8_19_modes[];
extern PinModes P8_20_modes[];
extern PinModes P8_21_modes[];
extern PinModes P8_22_modes[];
extern PinModes P8_23_modes[];
extern PinModes P8_24_modes[];
extern PinModes P8_25_modes[];
extern PinModes P8_26_modes[];
extern PinModes P8_27_modes[];
extern PinModes P8_28_modes[];
extern PinModes P8_29_modes[];
extern PinModes P8_30_modes[];
extern PinModes P8_31_modes[];
extern PinModes P8_32_modes[];
extern PinModes P8_33_modes[];
extern PinModes P8_34_modes[];
extern PinModes P8_35_modes[];
extern PinModes P8_36_modes[];
extern PinModes P8_37_modes[];
extern PinModes P8_38_modes[];
extern PinModes P8_39_modes[];
extern PinModes P8_40_modes[];
extern PinModes P8_41_modes[];
extern PinModes P8_42_modes[];
extern PinModes P8_43_modes[];
extern PinModes P8_44_modes[];
extern PinModes P8_45_modes[];
extern PinModes P8_46_modes[];

extern PinModes P9_11_modes[];
extern PinModes P9_12_modes[];
extern PinModes P9_13_modes[];
extern PinModes P9_14_modes[];
extern PinModes P9_15_modes[];
extern PinModes P9_16_modes[];
extern PinModes P9_17_modes[];
extern PinModes P9_18_modes[];
extern PinModes P9_21_modes[];
extern PinModes P9_22_modes[];
extern PinModes P9_23_modes[];
extern PinModes P9_24_modes[];
extern PinModes P9_25_modes[];
extern PinModes P9_26_modes[];
extern PinModes P9_27_modes[];
extern PinModes P9_28_modes[];
extern PinModes P9_29_modes[];
extern PinModes P9_30_modes[];
extern PinModes P9_31_modes[];
extern PinModes P9_41_modes[];
extern PinModes P9_42_modes[];

typedef struct
{
  uint8_t pinNum;
  char pinName[6];
  PinModes selectedMode;
  PinModes *validModes;
  uint8_t modesCount;
  VirtualCape virtualCape;
} Pin;

extern Pin P8_3;
extern Pin P8_4;
extern Pin P8_5;
extern Pin P8_6;
extern Pin P8_7;
extern Pin P8_8;
extern Pin P8_9;
extern Pin P8_10;
extern Pin P8_11;
extern Pin P8_12;
extern Pin P8_13;
extern Pin P8_14;
extern Pin P8_15;
extern Pin P8_16;
extern Pin P8_17;
extern Pin P8_18;
extern Pin P8_19;
extern Pin P8_20;
extern Pin P8_21;
extern Pin P8_22;
extern Pin P8_23;
extern Pin P8_24;
extern Pin P8_25;
extern Pin P8_26;
extern Pin P8_27;
extern Pin P8_28;
extern Pin P8_29;
extern Pin P8_30;
extern Pin P8_31;
extern Pin P8_32;
extern Pin P8_33;
extern Pin P8_34;
extern Pin P8_35;
extern Pin P8_36;
extern Pin P8_37;
extern Pin P8_38;
extern Pin P8_39;
extern Pin P8_40;
extern Pin P8_41;
extern Pin P8_42;
extern Pin P8_43;
extern Pin P8_44;
extern Pin P8_45;
extern Pin P8_46;

extern Pin P9_11;
extern Pin P9_12;
extern Pin P9_13;
extern Pin P9_14;
extern Pin P9_15;
extern Pin P9_16;
extern Pin P9_17;
extern Pin P9_18;
extern Pin P9_21;
extern Pin P9_22;
extern Pin P9_23;
extern Pin P9_24;
extern Pin P9_25;
extern Pin P9_26;
extern Pin P9_27;
extern Pin P9_28;
extern Pin P9_29;
extern Pin P9_30;
extern Pin P9_31;
extern Pin P9_41;
extern Pin P9_42;

#endif