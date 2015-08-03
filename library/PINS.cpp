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

#include <stdint.h>
#include "PINS.h"

PinModes P8_3_modes[]  = {gpio};
PinModes P8_4_modes[]  = {gpio};
PinModes P8_5_modes[]  = {gpio};
PinModes P8_6_modes[]  = {gpio};
PinModes P8_7_modes[]  = {gpio};
PinModes P8_8_modes[]  = {gpio};
PinModes P8_9_modes[]  = {gpio};
PinModes P8_10_modes[] = {gpio};
PinModes P8_11_modes[] = {gpio, pruout};
PinModes P8_12_modes[] = {gpio, pruout};
PinModes P8_13_modes[] = {gpio, pwm};
PinModes P8_14_modes[] = {gpio};
PinModes P8_15_modes[] = {gpio, pruin};
PinModes P8_16_modes[] = {gpio, pruin};
PinModes P8_17_modes[] = {gpio};
PinModes P8_18_modes[] = {gpio};
PinModes P8_19_modes[] = {gpio, pwm};
PinModes P8_20_modes[] = {gpio, pruout, pruin};
PinModes P8_21_modes[] = {gpio, pruout, pruin};
PinModes P8_22_modes[] = {gpio};
PinModes P8_23_modes[] = {gpio};
PinModes P8_24_modes[] = {gpio};
PinModes P8_25_modes[] = {gpio};
PinModes P8_26_modes[] = {gpio};
PinModes P8_27_modes[] = {gpio, pruout, pruin};
PinModes P8_28_modes[] = {gpio, pruout, pruin};
PinModes P8_29_modes[] = {gpio, pruout, pruin};
PinModes P8_30_modes[] = {gpio, pruout, pruin};
PinModes P8_31_modes[] = {gpio};
PinModes P8_32_modes[] = {gpio};
PinModes P8_33_modes[] = {gpio};
PinModes P8_34_modes[] = {gpio, pwm};
PinModes P8_35_modes[] = {gpio};
PinModes P8_36_modes[] = {gpio, pwm};
PinModes P8_37_modes[] = {gpio, uart};
PinModes P8_38_modes[] = {gpio, uart};
PinModes P8_39_modes[] = {gpio, pruout, pruin};
PinModes P8_40_modes[] = {gpio, pruout, pruin};
PinModes P8_41_modes[] = {gpio, pruout, pruin};
PinModes P8_42_modes[] = {gpio, pruout, pruin};
PinModes P8_43_modes[] = {gpio, pruout, pruin};
PinModes P8_44_modes[] = {gpio, pruout, pruin};
PinModes P8_45_modes[] = {gpio, pruout, pruin};
PinModes P8_46_modes[] = {gpio, pruout, pruin};

PinModes P9_11_modes[] = {gpio, uart};
PinModes P9_12_modes[] = {gpio};
PinModes P9_13_modes[] = {gpio, uart};
PinModes P9_14_modes[] = {gpio, pwm};
PinModes P9_15_modes[] = {gpio};
PinModes P9_16_modes[] = {gpio, pwm};
PinModes P9_17_modes[] = {gpio, spi, i2c};
PinModes P9_18_modes[] = {gpio, spi, i2c};
PinModes P9_21_modes[] = {gpio, pwm, uart, spi, i2c};
PinModes P9_22_modes[] = {gpio, pwm, uart, spi, i2c};
PinModes P9_23_modes[] = {gpio};
PinModes P9_24_modes[] = {gpio, pruin, uart, i2c};
PinModes P9_25_modes[] = {gpio, pruout, pruin};
PinModes P9_26_modes[] = {gpio, pruin, uart, i2c};
PinModes P9_27_modes[] = {gpio, pruout, pruin};
PinModes P9_28_modes[] = {gpio, pruout, pruin, pwm, spi};
PinModes P9_29_modes[] = {gpio, pruout, pruin, pwm, spi};
PinModes P9_30_modes[] = {gpio, pruout, pruin, spi};
PinModes P9_31_modes[] = {gpio, pruout, pruin, pwm, spi};
PinModes P9_41_modes[] = {gpio, pruout, pruin};
PinModes P9_42_modes[] = {gpio, pruout, pruin, pwm, spi};



Pin P8_3  = {38, "P8_03", P8_3_MODE,  P8_3_modes,  1, emmc};
Pin P8_4  = {39, "P8_04", P8_4_MODE,  P8_4_modes,  1, emmc};
Pin P8_5  = {34, "P8_05", P8_5_MODE,  P8_5_modes,  1, emmc};
Pin P8_6  = {35, "P8_06", P8_6_MODE,  P8_6_modes,  1, emmc};
Pin P8_7  = {66, "P8_07", P8_7_MODE,  P8_7_modes,  1, none};
Pin P8_8  = {67, "P8_08", P8_8_MODE,  P8_8_modes,  1, none};
Pin P8_9  = {69, "P8_09", P8_9_MODE,  P8_9_modes,  1, none};
Pin P8_10 = {68, "P8_10", P8_10_MODE, P8_10_modes, 1, none};
Pin P8_11 = {45, "P8_11", P8_11_MODE, P8_11_modes, 2, none};
Pin P8_12 = {44, "P8_12", P8_12_MODE, P8_12_modes, 2, none};
Pin P8_13 = {23, "P8_13", P8_13_MODE, P8_13_modes, 2, none};
Pin P8_14 = {26, "P8_14", P8_14_MODE, P8_14_modes, 1, none};
Pin P8_15 = {47, "P8_15", P8_15_MODE, P8_15_modes, 2, none};
Pin P8_16 = {46, "P8_16", P8_16_MODE, P8_16_modes, 2, none};
Pin P8_17 = {27, "P8_17", P8_17_MODE, P8_17_modes, 1, none};
Pin P8_18 = {65, "P8_18", P8_18_MODE, P8_18_modes, 1, none};
Pin P8_19 = {22, "P8_19", P8_19_MODE, P8_19_modes, 2, none};
Pin P8_20 = {63, "P8_20", P8_20_MODE, P8_20_modes, 3, emmc};
Pin P8_21 = {62, "P8_21", P8_21_MODE, P8_21_modes, 3, emmc};
Pin P8_22 = {37, "P8_22", P8_22_MODE, P8_22_modes, 1, emmc};
Pin P8_23 = {36, "P8_23", P8_23_MODE, P8_23_modes, 1, emmc};
Pin P8_24 = {33, "P8_24", P8_24_MODE, P8_24_modes, 1, emmc};
Pin P8_25 = {32, "P8_25", P8_25_MODE, P8_25_modes, 1, emmc};
Pin P8_26 = {61, "P8_26", P8_26_MODE, P8_26_modes, 1, none};
Pin P8_27 = {86, "P8_27", P8_27_MODE, P8_27_modes, 3, hdmi};
Pin P8_28 = {88, "P8_28", P8_28_MODE, P8_28_modes, 3, hdmi};
Pin P8_29 = {87, "P8_29", P8_29_MODE, P8_29_modes, 3, hdmi};
Pin P8_30 = {89, "P8_30", P8_30_MODE, P8_30_modes, 3, hdmi};
Pin P8_31 = {10, "P8_31", P8_31_MODE, P8_31_modes, 1, hdmi};
Pin P8_32 = {11, "P8_32", P8_32_MODE, P8_32_modes, 1, hdmi};
Pin P8_33 = {9,  "P8_33", P8_33_MODE, P8_33_modes, 1, hdmi};
Pin P8_34 = {81, "P8_34", P8_34_MODE, P8_34_modes, 2, hdmi};
Pin P8_35 = {8,  "P8_35", P8_35_MODE, P8_35_modes, 1, hdmi};
Pin P8_36 = {80, "P8_36", P8_36_MODE, P8_36_modes, 2, hdmi};
Pin P8_37 = {78, "P8_37", P8_37_MODE, P8_37_modes, 2, hdmi};
Pin P8_38 = {79, "P8_38", P8_38_MODE, P8_38_modes, 2, hdmi};
Pin P8_39 = {76, "P8_39", P8_39_MODE, P8_39_modes, 3, hdmi};
Pin P8_40 = {77, "P8_40", P8_40_MODE, P8_40_modes, 3, hdmi};
Pin P8_41 = {74, "P8_41", P8_41_MODE, P8_41_modes, 3, hdmi};
Pin P8_42 = {75, "P8_42", P8_42_MODE, P8_42_modes, 3, hdmi};
Pin P8_43 = {72, "P8_43", P8_43_MODE, P8_43_modes, 3, hdmi};
Pin P8_44 = {73, "P8_44", P8_44_MODE, P8_44_modes, 3, hdmi};
Pin P8_45 = {70, "P8_45", P8_45_MODE, P8_45_modes, 3, hdmi};
Pin P8_46 = {71, "P8_46", P8_46_MODE, P8_46_modes, 3, hdmi};

Pin P9_11 = {30,  "P9_11", P9_11_MODE, P9_11_modes, 2, none};
Pin P9_12 = {60,  "P9_12", P9_12_MODE, P9_12_modes, 1, none};
Pin P9_13 = {31,  "P9_13", P9_13_MODE, P9_13_modes, 2, none};
Pin P9_14 = {50,  "P9_14", P9_14_MODE, P9_14_modes, 2, none};
Pin P9_15 = {48,  "P9_15", P9_15_MODE, P9_15_modes, 1, none};
Pin P9_16 = {51,  "P9_16", P9_16_MODE, P9_16_modes, 2, none};
Pin P9_17 = {5,   "P9_17", P9_17_MODE, P9_17_modes, 3, none};
Pin P9_18 = {4,   "P9_18", P9_18_MODE, P9_18_modes, 3, none};
Pin P9_21 = {3,   "P9_21", P9_21_MODE, P9_21_modes, 5, none};
Pin P9_22 = {2,   "P9_22", P9_22_MODE, P9_22_modes, 5, none};
Pin P9_23 = {49,  "P9_23", P9_23_MODE, P9_23_modes, 1, none};
Pin P9_24 = {15,  "P9_24", P9_24_MODE, P9_24_modes, 4, none};
Pin P9_25 = {117, "P9_25", P9_25_MODE, P9_25_modes, 3, audio};
Pin P9_26 = {14,  "P9_26", P9_26_MODE, P9_26_modes, 4, none};
Pin P9_27 = {115, "P9_27", P9_27_MODE, P9_27_modes, 3, none};
Pin P9_28 = {113, "P9_28", P9_28_MODE, P9_28_modes, 5, audio};
Pin P9_29 = {111, "P9_29", P9_29_MODE, P9_29_modes, 5, audio};
Pin P9_30 = {112, "P9_30", P9_30_MODE, P9_30_modes, 4, none};
Pin P9_31 = {110, "P9_31", P9_31_MODE, P9_31_modes, 5, audio};
Pin P9_41 = {20,  "P9_41", P9_41_MODE, P9_41_modes, 3, none};
Pin P9_42 = {7,   "P9_42", P9_42_MODE, P9_42_modes, 5, none};
