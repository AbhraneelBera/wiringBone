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
#include <sys/mman.h>
#include <pthread.h>

#include "CommonDefines.h"
#include "PWM.h"
#include "PRU0_bin.h"
#include "PRU1_bin.h"
#include "OVERLAY.h"

PRU::PRU()
{
  pthread_t thread;
  pruInit();
  pthread_create(&thread, NULL, resetWatchdogPRU, (void*)1000);
}

int PRU::gpioNumToPruMap(uint8_t num)
{
  switch(num)
  {
    case 110: return 0;
    case 111: return 1;
    case 112: return 2;
    case 113: return 3;
    case   7: return 4;
    case 115: return 5;
    case  20: return 6;
    case 117: return 7;
    case  44: return 8;
    case  45: return 9;
    case  46: return 10;
    case  47: return 11;
    case  15: return 12;
    case  70: return 13;
    case  71: return 14;
    case  72: return 15;
    case  73: return 16;
    case  74: return 17;
    case  75: return 18;
    case  76: return 19;
    case  77: return 20;
    case  86: return 21;
    case  87: return 22;
    case  88: return 23;
    case  89: return 24;
    case  62: return 25;
    case  63: return 26;
    case  14: return 27;
    default: perror("Invalid PRU pin"); return -1;
  }
}

void PRU::pruInit()
{
  unsigned int index;
  uint32_t fd;
  volatile uint32_t *iram0, *iram1;
  volatile uint8_t *iep, *ctrl0, *ctrl1;

  if((fd = open("/dev/mem",O_RDWR|O_SYNC))<1)
  perror("Device memory open failed\n");

  pru = (struct pru*) mmap(0, 0x2000,   PROT_WRITE|PROT_READ,MAP_SHARED, fd, PRU_RAM0_BASE);
  iep = (uint8_t*) mmap(0, 0x68,   PROT_WRITE|PROT_READ,MAP_SHARED, fd, PRU_IEP_BASE);
  iram0 = (uint32_t*) mmap(0, 0x2000,   PROT_WRITE|PROT_READ,MAP_SHARED, fd, PRU0_IRAM_BASE);
  iram1 = (uint32_t*) mmap(0, 0x2000,   PROT_WRITE|PROT_READ,MAP_SHARED, fd, PRU1_IRAM_BASE);
  ctrl0 = (uint8_t*) mmap(0, 0x30,   PROT_WRITE|PROT_READ,MAP_SHARED, fd, PRU0_CTRL_BASE);
  ctrl1 = (uint8_t*) mmap(0, 0x30,   PROT_WRITE|PROT_READ,MAP_SHARED, fd, PRU1_CTRL_BASE);
  if(((void *)pru== NULL)||((void *)iep== NULL)||((void *)iram0== NULL)||((void *)iram1== NULL)||((void *)ctrl0== NULL)||((void *)ctrl1== NULL))
  perror("PRU memory map open failed\n");

  close(fd);

  *(ctrl0 + CONTROL) = (1 << SOFT_RST_N);
  *(ctrl1 + CONTROL) = (1 << SOFT_RST_N);

  pru -> enable = 0x0;
  pru -> mode = 0x0;

  int count;
  for(count=0; count < PIN_COUNT; count++)
  {
    pru -> pwm_pin[count].t_on  = DEFAULT_PULSE_WIDTH * 200;
    pru -> pwm_pin[count].t_off = (DEFAULT_TIME_PERIOD * 200 - DEFAULT_PULSE_WIDTH * 200);
    timePeriod[count] = DEFAULT_TIME_PERIOD * 200;
  }

  pru -> timeout = 10 * (DEFAULT_TIME_PERIOD * 200);

  pru -> failsafe_t_on  = DEFAULT_PULSE_WIDTH * 200;
  pru -> failsafe_t_off = (DEFAULT_TIME_PERIOD * 200 - DEFAULT_PULSE_WIDTH * 200);

  for(index = 0; index < sizeof(PRU0code)/sizeof(unsigned int); index++)
  *(iram0 + index) = PRU0code[index];

  for(index = 0; index < sizeof(PRU1code)/sizeof(unsigned int); index++)
  *(iram1 + index) = PRU1code[index];

  *(iep + GLOBAL_CFG) = (1 << DEFAULT_INC);
  *(iep + COUNT) = 0x0;
  *(iep + GLOBAL_CFG) |= (1 << CNT_ENABLE);

  *(ctrl0 + CONTROL) = (1 << ENABLE);
  *(ctrl1 + CONTROL) = (1 << ENABLE);

  munmap((void*)iep, 0x68);
  munmap((void*)iram0, 0x2000);
  munmap((void*)iram1, 0x2000);
  munmap((void*)ctrl0, 0x30);
  munmap((void*)ctrl1, 0x30);
}

int PRU::pruConfig(uint8_t gpioPin, uint8_t pin_mode)
{
  uint8_t pin = gpioNumToPruMap(gpioPin);
  pru -> enable |= (1 << pin);
  pru -> mode |= (pin_mode << pin);
  return gpioPin;
}

void PRU::setTimePeriod (uint8_t gpioPin, uint32_t period_us)
{
  uint8_t pin = gpioNumToPruMap(gpioPin);
  uint32_t value;
  if(period_us > 0)
  {
    value = period_us * 200;
    timePeriod[pin] = value;
    pru -> pwm_pin[pin].t_off = (value - pru -> pwm_pin[pin].t_on);
  }
  else
  perror("Invalid time period");
}

uint32_t PRU::getTimePeriod (uint8_t gpioPin)
{
  uint8_t pin = gpioNumToPruMap(gpioPin);
  uint32_t period_us, value;
  value = (pru -> pwm_pin[pin].t_on + pru -> pwm_pin[pin].t_off);
  timePeriod[pin] = value;
  period_us = value / 200;
  return period_us;
}

void PRU::setFrequency (uint8_t gpioPin, uint32_t freq_hz)
{
  uint8_t pin = gpioNumToPruMap(gpioPin);
  uint32_t value;
  if(freq_hz > 0)
  {
    value = 200000000 / freq_hz;
    timePeriod[pin] = value;
    pru -> pwm_pin[pin].t_off = (value - pru -> pwm_pin[pin].t_on);
  }
  else
  perror("Invalid frequency");
}

uint32_t PRU::getFrequency (uint8_t gpioPin)
{
  uint8_t pin = gpioNumToPruMap(gpioPin);
  uint32_t freq_hz, value;
  value = (pru -> pwm_pin[pin].t_on + pru -> pwm_pin[pin].t_off);
  timePeriod[pin] = value;
  freq_hz = 200000000 / value;
  return freq_hz;
}

void PRU::setPulseWidth (uint8_t gpioPin, uint32_t period_us)
{
  uint8_t pin = gpioNumToPruMap(gpioPin);
  uint32_t value;
  if(period_us <= ((timePeriod[pin]) / 200))
  {
    value = period_us * 200;
    pru -> pwm_pin[pin].t_on  = value;
    pru -> pwm_pin[pin].t_off = (timePeriod[pin] - value);
  }
  else
  perror("Invalid pulse width");
}

uint32_t PRU::getPulseWidth (uint8_t gpioPin)
{
  uint8_t pin = gpioNumToPruMap(gpioPin);
  uint32_t period_us, value;
  value = pru -> pwm_pin[pin].t_on;
  period_us = value / 200;
  return period_us;
}

void PRU::setDutyPercentage (uint8_t gpioPin, uint32_t percentage)
{
  if(percentage >=0 && percentage<=100)
  {
    uint8_t pin = gpioNumToPruMap(gpioPin);
    uint32_t value = ((percentage * (timePeriod[pin])) / 100);
    pru -> pwm_pin[pin].t_on  = value;
    pru -> pwm_pin[pin].t_off = (timePeriod[pin] - value);
  }
  else
  perror("Invalid duty percentage");
}

uint32_t PRU::getDutyPercentage (uint8_t gpioPin)
{
  uint8_t pin = gpioNumToPruMap(gpioPin);
  uint32_t percentage;
  percentage = (((pru -> pwm_pin[pin].t_on) * 100) / (pru -> pwm_pin[pin].t_on + pru -> pwm_pin[pin].t_off));
  return percentage;
}

void PRU::setPulseReadTimeout (uint32_t time_us)
{
  uint32_t value;
  value = time_us * 200;
  pru -> timeout = value;
}

void PRU::setFailsafePRU (uint32_t pulseWidth_us, uint32_t timePeriod_us)
{
  pru -> failsafe_t_on  = pulseWidth_us * 200;
  pru -> failsafe_t_off = (timePeriod_us * 200 - pulseWidth_us * 200);
}

void PRU::resetWatchdog (long interval)
{
  while(1)
  {
    pru -> watchdog = 0x0;
    usleep(interval);
  }
}

PRU::~PRU()
{
  int count;
  for(count=0; count < PIN_COUNT; count++)
  {
    pru -> pwm_pin[count].t_on = DEFAULT_PULSE_WIDTH * 200;
    pru -> pwm_pin[count].t_off = (DEFAULT_TIME_PERIOD * 200 - DEFAULT_PULSE_WIDTH * 200);
  }
}

PRU *_pru;


PWM::PWM()
{
  for(int count = 0; count < 8; count++)
  pwmPin[count] = unexported;
}

int PWM::gpioNumToPwmMap(uint8_t gpioNum)
{
  switch(gpioNum)
  {
    case   2: return 0;
    case 110: return 0;
    case   3: return 1;
    case 111: return 1;
    case   7: return 2;
    case  50: return 3;
    case  80: return 3;
    case  51: return 4;
    case  81: return 4;
    case  22: return 5;
    case  70: return 5;
    case  23: return 6;
    case  71: return 6;
    case  88: return 7;
    default: perror("Invalid PWM pin"); return -1;
  }
}

int PWM::exportPin(uint8_t gpioPin)
{
  uint8_t pin = gpioNumToPwmMap(gpioPin);
  if(pwmPin[pin] == exported)
  {
    perror("PWM pin already exported");
    return -1;
  }
  FILE *fd;
  if((fd = fopen("/sys/class/pwm/export", "w")) == NULL)
  {
    perror("PWM export failed");
    return -1;
  }
  if((fprintf(fd, "%d", pin)) < 0)
  {
    perror("PWM export failed");
    return -1;
  }
  fclose(fd);
  return pin;
}

int PWM::unexportPin(uint8_t gpioPin)
{
  uint8_t pin = gpioNumToPwmMap(gpioPin);
  if(pwmPin[pin] == unexported)
  {
    perror("PWM pin already unexported");
    return -1;
  }
  FILE *fd;
  if((fd = fopen("/sys/class/pwm/unexport", "w")) == NULL)
  {
    perror("PWM unexport failed");
    return -1;
  }
  fprintf(fd, "%d", pin);
  fclose(fd);
  return pin;
}

void PWM::pwmControl(uint8_t gpioPin, Control control)
{
  uint8_t pin = gpioNumToPwmMap(gpioPin);
  char path[24];
  FILE *fd;
  sprintf(path, "/sys/class/pwm/pwm%d/run", pin);
  if((fd = fopen(path, "w")) == NULL)
  {
    perror("PWM control failed");
  }
  fprintf(fd, "%d", (uint8_t)control);
  fclose(fd);
}

int PWM::pwmConfig(uint8_t gpioPin)
{
  if((this->exportPin(gpioPin)) >= 0)
  pwmPin[gpioNumToPwmMap(gpioPin)] = exported;
  this->setTimePeriod(gpioPin, DEFAULT_TIME_PERIOD);
  this->setPulseWidth(gpioPin, DEFAULT_PULSE_WIDTH);
  this->pwmControl(gpioPin, start);
  this->gpioPins[gpioNumToPwmMap(gpioPin)] = gpioPin;
  return gpioPin;
}

void PWM::setTimePeriodns (uint8_t gpioPin, uint32_t period_ns)
{
  uint8_t pin = gpioNumToPwmMap(gpioPin);
  char path[36];
  FILE *fd;
  if(period_ns > 0)
  {
    sprintf(path, "/sys/class/pwm/pwm%d/period_ns", pin);
    if((fd = fopen(path, "w")) == NULL)
    {
      perror("PWM priod write failed");
    }
    fprintf(fd, "%d", period_ns);
    fclose(fd);
    this->period[pin] = period_ns;
  }
  else
  perror("Invalid time period");
}

void PWM::setTimePeriod (uint8_t gpioPin, uint32_t period_us)
{
  setTimePeriodns(gpioPin, period_us * 1000);
}

void PWM::setFrequency (uint8_t gpioPin, uint32_t freq_hz)
{
  setTimePeriod(gpioPin, 1000000/freq_hz);
}

void PWM::setPulseWidthns (uint8_t gpioPin, uint32_t period_ns)
{
  uint8_t pin = gpioNumToPwmMap(gpioPin);
  char path[36];
  FILE *fd;
  if(period_ns <= this->period[pin])
  {
    sprintf(path, "/sys/class/pwm/pwm%d/duty_ns", pin);
    if((fd = fopen(path, "w")) == NULL)
    {
      perror("PWM duty write failed");
    }
    fprintf(fd, "%d", period_ns);
    fclose(fd);
  }
  else
  perror("Invalid pulse width");
}

void PWM::setPulseWidth (uint8_t gpioPin, uint32_t period_us)
{
  setPulseWidthns(gpioPin, period_us * 1000);
}

void PWM::setDutyPercentage (uint8_t gpioPin, uint32_t percentage)
{
  uint8_t pin = gpioNumToPwmMap(gpioPin);
  setPulseWidthns(gpioPin, ((percentage * (this->period[pin])) / 100));
}

PWM::~PWM()
{
  for(int count = 0; count < 8; count++)
  {
    if(pwmPin[count] == exported)
    {
      this->setPulseWidth(gpioPins[count], 0);
      this->pwmControl(gpioPins[count], stop);
      this->unexportPin(gpioPins[count]);
    }
  }
}

PWM *_pwm;

PRU* pruInstance()
{
  PRU* instance = new PRU();
  return instance;
}

PWM* pwmInstance()
{
  PWM* instance = new PWM();
  return instance;
}

void analogWrite(Pin pin, uint8_t value)
{
  float duty;
  duty = (float)((value * 100) / 255);
  switch(pin.selectedMode)
  {
    case pwm   : _pwm->setDutyPercentage(pin.pinNum, (uint32_t)duty); break;
    case pruout: _pru->setDutyPercentage(pin.pinNum, (uint32_t)duty); break;
    default: perror("Invalid pwm/pru pin");
  }
}

uint32_t pulseIn(Pin pin, bool polarity, uint32_t timeout)
{
  if(pin.selectedMode == pruin)
  {
    _pru->setPulseReadTimeout(timeout);
    switch(polarity)
    {
      case LOW : return(_pru->getTimePeriod (pin.pinNum) - _pru->getPulseWidth (pin.pinNum)); break;
      case HIGH: return(_pru->getPulseWidth (pin.pinNum)); break;
      default: perror("Invalid polarity"); return 0;
    }
  }
  else
  {
    perror("Invalid pru pin");
    return 0;
  }
}

void setPulseReadTimeout (uint32_t time_us)
{
  _pru->setPulseReadTimeout(time_us);
}

void setFailsafePRU (uint32_t pulseWidth_us, uint32_t timePeriod_us)
{
  if (pulseWidth_us <= timePeriod_us)
  _pru->setFailsafePRU(pulseWidth_us, timePeriod_us);
  else
  perror("Invalid failsafe values");
}

void* resetWatchdogPRU (void* interval)
{
  _pru->resetWatchdog((long)interval);
  return (void*)0;
}

void setTimePeriod (Pin pin, uint32_t period_us)
{
  switch(pin.selectedMode)
  {
    case pwm   : _pwm->setTimePeriod(pin.pinNum, period_us); break;
    case pruout: _pru->setTimePeriod(pin.pinNum, period_us); break;
    default: perror("Invalid pwm/pru pin");
  }
}

void setTimePeriodns (Pin pin, uint32_t period_ns)
{
  switch(pin.selectedMode)
  {
    case pwm   : _pwm->setTimePeriodns(pin.pinNum, period_ns); break;
    case pruout: perror("setTimePeriodns not available for pru"); break;
    default: perror("Invalid pwm/pru pin");
  }
}

uint32_t getTimePeriod (Pin pin)
{
  switch(pin.selectedMode)
  {
    case pruin : return(_pru->getTimePeriod(pin.pinNum)); break;
    case pruout: return(_pru->getTimePeriod(pin.pinNum)); break;
    default: perror("Invalid pru pin"); return 0;
  }
}

void setFrequency (Pin pin, uint32_t freq_hz)
{
  switch(pin.selectedMode)
  {
    case pwm   : _pwm->setFrequency(pin.pinNum, freq_hz); break;
    case pruout: _pru->setFrequency(pin.pinNum, freq_hz); break;
    default: perror("Invalid pwm/pru pin");
  }
}

uint32_t getFrequency (Pin pin)
{
  switch(pin.selectedMode)
  {
    case pruin : return(_pru->getFrequency(pin.pinNum)); break;
    case pruout: return(_pru->getFrequency(pin.pinNum)); break;
    default: perror("Invalid pru pin"); return 0;
  }
}

void setPulseWidth (Pin pin, uint32_t period_us)
{
  switch(pin.selectedMode)
  {
    case pwm   : _pwm->setPulseWidth(pin.pinNum, period_us); break;
    case pruout: _pru->setPulseWidth(pin.pinNum, period_us); break;
    default: perror("Invalid pwm/pru pin");
  }
}

void setPulseWidthns (Pin pin, uint32_t period_ns)
{
  switch(pin.selectedMode)
  {
    case pwm   : _pwm->setPulseWidthns(pin.pinNum, period_ns); break;
    case pruout: perror("setPulseWidthns not available for pru"); break;
    default: perror("Invalid pwm/pru pin");
  }
}

uint32_t getPulseWidth (Pin pin)
{
  switch(pin.selectedMode)
  {
    case pruin : return(_pru->getPulseWidth(pin.pinNum)); break;
    case pruout: return(_pru->getPulseWidth(pin.pinNum)); break;
    default: perror("Invalid pru pin"); return 0;
  }
}

void setDutyPercentage (Pin pin, uint32_t percentage)
{
  switch(pin.selectedMode)
  {
    case pwm   : _pwm->setDutyPercentage(pin.pinNum, percentage); break;
    case pruout: _pru->setDutyPercentage(pin.pinNum, percentage); break;
    default: perror("Invalid pwm/pru pin");
  }
}

uint32_t getDutyPercentage (Pin pin)
{
  switch(pin.selectedMode)
  {
    case pruin : return(_pru->getDutyPercentage(pin.pinNum)); break;
    case pruout: return(_pru->getDutyPercentage(pin.pinNum)); break;
    default: perror("Invalid pru pin"); return 0;
  }
}