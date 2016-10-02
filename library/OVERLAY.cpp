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
#include <wordexp.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <linux/version.h>

#include "PINS.h"
#include "CommonDefines.h"
#include "OVERLAY.h"
#include "GPIO.h"
#include "PWM.h"
#include "CLOCK.h"

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,8,13)
#define	OCPDIR "/sys/devices/platform/ocp/ocp:%s_pinmux/state"
#define	SLOTS "/sys/devices/platform/bone_capemgr/slots"
#define P9_91_OCP "/sys/devices/platform/ocp/ocp:P9_91_pinmux/state"
#define P9_92_OCP "/sys/devices/platform/ocp/ocp:P9_92_pinmux/state"
#else
#define OCPDIR "/sys/devices/ocp.*/%s_pinmux.*/state"
#define SLOTS "/sys/devices/bone_capemgr.*/slots"
#define P9_91_OCP "/sys/devices/ocp.*/P9_91_pinmux.*/state"
#define P9_92_OCP "/sys/devices/ocp.*/P9_92_pinmux.*/state"
#endif

OVERLAY::OVERLAY()
{
  loadCape("cape-universaln");
  loadCape("BB-ADC");

  _gpio = gpioInstance();
  _pru = pruInstance();
  _pwm = pwmInstance();

  configOverlay(P8_3);
  configOverlay(P8_4);
  configOverlay(P8_5);
  configOverlay(P8_6);
  configOverlay(P8_7);
  configOverlay(P8_8);
  configOverlay(P8_9);
  configOverlay(P8_10);
  configOverlay(P8_11);
  configOverlay(P8_12);
  configOverlay(P8_13);
  configOverlay(P8_14);
  configOverlay(P8_15);
  configOverlay(P8_16);
  configOverlay(P8_17);
  configOverlay(P8_18);
  configOverlay(P8_19);
  configOverlay(P8_20);
  configOverlay(P8_21);
  configOverlay(P8_22);
  configOverlay(P8_23);
  configOverlay(P8_24);
  configOverlay(P8_25);
  configOverlay(P8_26);
  configOverlay(P8_27);
  configOverlay(P8_28);
  configOverlay(P8_29);
  configOverlay(P8_30);
  configOverlay(P8_31);
  configOverlay(P8_32);
  configOverlay(P8_33);
  configOverlay(P8_34);
  configOverlay(P8_35);
  configOverlay(P8_36);
  configOverlay(P8_37);
  configOverlay(P8_38);
  configOverlay(P8_39);
  configOverlay(P8_40);
  configOverlay(P8_41);
  configOverlay(P8_42);
  configOverlay(P8_43);
  configOverlay(P8_44);
  configOverlay(P8_45);
  configOverlay(P8_46);

  configOverlay(P9_11);
  configOverlay(P9_12);
  configOverlay(P9_13);
  configOverlay(P9_14);
  configOverlay(P9_15);
  configOverlay(P9_16);
  configOverlay(P9_17);
  configOverlay(P9_18);
  configOverlay(P9_21);
  configOverlay(P9_22);
  configOverlay(P9_23);
  configOverlay(P9_24);
  configOverlay(P9_25);
  configOverlay(P9_26);
  configOverlay(P9_27);
  configOverlay(P9_28);
  configOverlay(P9_29);
  configOverlay(P9_30);
  configOverlay(P9_31);
  configOverlay(P9_41);
  configOverlay(P9_42);

  startClock();
}

int OVERLAY::configOverlay(Pin pin)
{ 
  FILE *fd;
  wordexp_t path;
  char filePath[100], mode[8];
  bool validMode = false;
  if(pin.selectedMode == disabled)
  return 0;
  for(int count = 0; count < pin.modesCount; count++)
  {
    if(pin.validModes[count] == pin.selectedMode)
    {
      validMode = true;
      break;
    }
  }
  if(validMode == false)
  {
    printf("%s:", pin.pinName);
    perror("Invalid mode selected");
    exit(-1);
  }
  if(pin.virtualCape == hdmi)
  loadCape("cape-univ-hdmi");
  if(pin.virtualCape == audio)
  loadCape("cape-univ-audio");
  if(pin.virtualCape == emmc)
  loadCape("cape-univ-emmc");
  if(pin.pinNum == 20 && (pin.selectedMode == pruin || pin.selectedMode == pruout))
  sprintf(filePath, P9_91_OCP);
  else if(pin.pinNum == 7 && (pin.selectedMode == pruin || pin.selectedMode == pruout))
  sprintf(filePath, P9_91_OCP);
  else
  sprintf(filePath, OCPDIR, pin.pinName);
  wordexp(filePath, &path, 0);
  if((fd = fopen(path.we_wordv[0], "w")) == NULL)
  {
    printf("%s:", pin.pinName);
    perror("Config overlay failed");
    exit(-1);
  }
  switch(pin.selectedMode)
  {
    case gpio  : sprintf(mode, "gpio_pd"); break;
    case pwm   : {
                   if(pin.pinNum == 113) sprintf(mode, "pwm2");
                   else sprintf(mode, "pwm");
                   _pwm->pwmConfig(pin.pinNum); break;
                 }
    case pruin : sprintf(mode, "pruin"  ); _pru->pruConfig(pin.pinNum, INPUT); break;
    case pruout: sprintf(mode, "pruout" ); _pru->pruConfig(pin.pinNum, OUTPUT); break;
    case i2c   : sprintf(mode, "i2c"    ); break;
    case uart  : sprintf(mode, "uart"   ); break;
    case spi   : {
                   if(pin.pinNum == 7) sprintf(mode, "spics");
                   else sprintf(mode, "spi");
                   break;
                 }
    default    : perror("invalid mode"); return -1;
  }
  if((fprintf(fd, "%s", mode)) < 0)
  {
    printf("%s:", pin.pinName);
    perror("Config overlay mode write failed");
    exit(-1);
  }
  fclose(fd);
  return 0;
}

int OVERLAY::restoreOverlay(Pin pin)
{
  FILE *fd;
  wordexp_t path;
  char filePath[100];
  if(pin.selectedMode == disabled)
  return 0;
  if(pin.pinNum == 20 && (pin.selectedMode == pruin || pin.selectedMode == pruout))
  sprintf(filePath, P9_91_OCP);
  else if(pin.pinNum == 7 && (pin.selectedMode == pruin || pin.selectedMode == pruout))
  sprintf(filePath, P9_91_OCP);
  else
  sprintf(filePath, OCPDIR, pin.pinName);
  wordexp(filePath, &path, 0);
  if((fd = fopen(path.we_wordv[0], "w")) == NULL)
  {
    printf("%s:", pin.pinName);
    perror("Restore overlay failed");
    return -1;
  }
  if((fprintf(fd, "%s", "default")) < 0)
  {
    printf("%s:", pin.pinName);
    perror("Restore overlay failed");
    return -1;
  }
  fclose(fd);
  return 0;
}

OVERLAY::~OVERLAY()
{
  restoreOverlay(P8_3);
  restoreOverlay(P8_4);
  restoreOverlay(P8_5);
  restoreOverlay(P8_6);
  restoreOverlay(P8_7);
  restoreOverlay(P8_8);
  restoreOverlay(P8_9);
  restoreOverlay(P8_10);
  restoreOverlay(P8_11);
  restoreOverlay(P8_12);
  restoreOverlay(P8_13);
  restoreOverlay(P8_14);
  restoreOverlay(P8_15);
  restoreOverlay(P8_16);
  restoreOverlay(P8_17);
  restoreOverlay(P8_18);
  restoreOverlay(P8_19);
  restoreOverlay(P8_20);
  restoreOverlay(P8_21);
  restoreOverlay(P8_22);
  restoreOverlay(P8_23);
  restoreOverlay(P8_24);
  restoreOverlay(P8_25);
  restoreOverlay(P8_26);
  restoreOverlay(P8_27);
  restoreOverlay(P8_28);
  restoreOverlay(P8_29);
  restoreOverlay(P8_30);
  restoreOverlay(P8_31);
  restoreOverlay(P8_32);
  restoreOverlay(P8_33);
  restoreOverlay(P8_34);
  restoreOverlay(P8_35);
  restoreOverlay(P8_36);
  restoreOverlay(P8_37);
  restoreOverlay(P8_38);
  restoreOverlay(P8_39);
  restoreOverlay(P8_40);
  restoreOverlay(P8_41);
  restoreOverlay(P8_42);
  restoreOverlay(P8_43);
  restoreOverlay(P8_44);
  restoreOverlay(P8_45);
  restoreOverlay(P8_46);

  restoreOverlay(P9_11);
  restoreOverlay(P9_12);
  restoreOverlay(P9_13);
  restoreOverlay(P9_14);
  restoreOverlay(P9_15);
  restoreOverlay(P9_16);
  restoreOverlay(P9_17);
  restoreOverlay(P9_18);
  restoreOverlay(P9_21);
  restoreOverlay(P9_22);
  restoreOverlay(P9_23);
  restoreOverlay(P9_24);
  restoreOverlay(P9_25);
  restoreOverlay(P9_26);
  restoreOverlay(P9_27);
  restoreOverlay(P9_28);
  restoreOverlay(P9_29);
  restoreOverlay(P9_30);
  restoreOverlay(P9_31);
  restoreOverlay(P9_41);
  restoreOverlay(P9_42);
  delete _gpio;
  delete _pru;
  delete _pwm;
}

bool OVERLAY::capeLoaded(std::string path, std::string capeName)
{
  std::string line;
  std::ifstream slots(path.c_str());
  if(slots.is_open())
  {
    while(getline(slots, line))
    {
      if(line.find(capeName) != std::string::npos)
      return true;
    }
  }
  return false;
}

int OVERLAY::loadCape(std::string capeName)
{
  FILE *fd;
  wordexp_t path;
  wordexp(SLOTS, &path, 0);
  if(capeLoaded(path.we_wordv[0], capeName))
  return 0;
  if((fd = fopen(path.we_wordv[0], "w")) == NULL)
  {
    perror("Cape load failed");
    return -1;
  }
  if((fprintf(fd, "%s", capeName.c_str())) < 0)
  {
    printf("%s:", capeName.c_str());
    perror("Cape load failed");
    exit(-1);
  }
  fclose(fd);
  usleep(1000*1000);
  return 0;
}

OVERLAY overlay;

void pinMode(Pin pin, uint8_t direction)
{
  if(pin.selectedMode == gpio)
  _gpio->gpioConfig(pin.pinNum, direction);
}
