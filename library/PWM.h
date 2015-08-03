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

#ifndef PWM_H
#define PWM_H

#include <stdint.h>

#include "PINS.h"
#include "CommonDefines.h"

#define DEFAULT_TIME_PERIOD 2040
#define DEFAULT_PULSE_WIDTH 0

#define PRU0             0
#define PRU1             1
#define PRU_RAM0_BASE    0x4a300000
#define PRU_IEP_BASE     0x4a32e000
#define PRU0_IRAM_BASE   0x4a334000
#define PRU1_IRAM_BASE   0x4a338000
#define PRU0_CTRL_BASE   0x4a322000
#define PRU1_CTRL_BASE   0x4a324000

// IEP Register Offset
#define GLOBAL_CFG 0x0
#define COUNT      0xc

// IEP GLOBAL_CFG Register Field Offset
#define CNT_ENABLE  0x0
#define DEFAULT_INC 0x4

// PRU_CTRL Register Offset
#define CONTROL 0x0

// PRU_CTRL CONTROL Register Field Offset
#define SOFT_RST_N 0x0
#define ENABLE     0x1

#define PIN_COUNT        13 +15

class PRU
{
  public:
  PRU();
  ~PRU();
  virtual int pruConfig(uint8_t gpioPin, uint8_t pin_mode);
  virtual void setTimePeriod (uint8_t gpioPin, uint32_t period_us);
  virtual uint32_t getTimePeriod (uint8_t gpioPin);
  virtual void setFrequency (uint8_t gpioPin, uint32_t freq_hz);
  virtual uint32_t getFrequency (uint8_t gpioPin);
  virtual void setPulseWidth (uint8_t gpioPin, uint32_t period_us);
  virtual uint32_t getPulseWidth (uint8_t gpioPin);
  virtual void setDutyPercentage (uint8_t gpioPin, uint32_t percentage);
  virtual uint32_t getDutyPercentage (uint8_t gpioPin);
  virtual void setPulseReadTimeout (uint32_t time_us);
  virtual void setFailsafePRU (uint32_t pulseWidth_us = DEFAULT_PULSE_WIDTH, uint32_t timePeriod_us = DEFAULT_TIME_PERIOD);
  virtual void resetWatchdog (long interval);

  private:
  struct pru
  {
    volatile uint32_t enable;
    volatile uint32_t mode;
    struct {
      volatile uint32_t t_on;
      volatile uint32_t t_off;
    } pwm_pin[PIN_COUNT];
    volatile uint32_t timeout;
    volatile uint32_t failsafe_t_on;
    volatile uint32_t failsafe_t_off;
    volatile uint32_t watchdog;
  };

  volatile struct pru *pru;

  uint32_t timePeriod[PIN_COUNT];

  int gpioNumToPruMap(uint8_t num);
  void pruInit();
};

extern PRU *_pru;

class PWM
{
  public:
  PWM();
  ~PWM();
  virtual int pwmConfig(uint8_t gpioPin);
  virtual void setTimePeriodns (uint8_t gpioPin, uint32_t period_ns);
  virtual void setTimePeriod (uint8_t gpioPin, uint32_t period_us);
  virtual void setFrequency (uint8_t gpioPin, uint32_t freq_hz);
  virtual void setPulseWidthns (uint8_t gpioPin, uint32_t period_ns);
  virtual void setPulseWidth (uint8_t gpioPin, uint32_t period_us);
  virtual void setDutyPercentage (uint8_t gpioPin, uint32_t percentage);

  private:
  typedef enum {unexported, exported} pwmStatus;
  typedef enum {stop = 0, start = 1} Control;
  pwmStatus pwmPin[8];
  uint32_t period[8];
  uint8_t gpioPins[8];
  int exportPin(uint8_t gpioPin);
  int unexportPin(uint8_t gpioPin);
  void pwmControl(uint8_t gpioPin, Control control);

  int gpioNumToPwmMap(uint8_t gpioNum);
};

extern PWM *_pwm;

PRU* pruInstance();
PWM* pwmInstance();

void analogWrite(Pin pin, uint8_t value);
uint32_t pulseIn(Pin pin, bool polarity, uint32_t timeout = 1000000);

void setPulseReadTimeout (uint32_t time_us);
void setFailsafePRU (uint32_t pulseWidth_us = DEFAULT_PULSE_WIDTH, uint32_t timePeriod_us = DEFAULT_TIME_PERIOD);
void* resetWatchdogPRU (void* interval);

void setTimePeriod (Pin pin, uint32_t period_us);
void setTimePeriodns (Pin pin, uint32_t period_ns);
uint32_t getTimePeriod (Pin pin);
void setFrequency (Pin pin, uint32_t freq_hz);
uint32_t getFrequency (Pin pin);
void setPulseWidth (Pin pin, uint32_t period_us);
void setPulseWidthns (Pin pin, uint32_t period_ns);
uint32_t getPulseWidth (Pin pin);
void setDutyPercentage (Pin pin, uint32_t percentage);
uint32_t getDutyPercentage (Pin pin);

#endif