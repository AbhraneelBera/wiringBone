# wiringBone
Wiring/Arduino style library for BeagleBone Black Platform
#Description:
This library contains almost all the core wiring/arduino functions along with some advanced functions.
This library is intended for quick and easy hardware control on the BeagleBone Black platform.
#Features:
1. Control 65 GPIO pins for digital input/output.
2. Generate PWM from total 30 pins (6 pins with PWMSS and 24 pins with PRU_ICSS).
3. Among 30 PWM pins, 24 pins have optional failsafe feature.
4. Capture pulse width and time period of incoming pulse train from 26 pins.
5. UART ports - 4.
6. I2C ports - 1.
7. SPI ports - 2 (experimental).
8. GPIO and PWM port clean-up at program exit (Work in Progress).

Use of some pins may require disabling the hdmi and emmc capes.
Refer to the pin diagrams in this link http://beagleboard.org/Support/bone101

NOTE : This library uses the 'beaglebone-universal-io' device tree overlays and the 'pasm' (PRU assembler).
Check whether beaglebone-universal-io and pasm is present in your present os image by typing config-pin and pasm on the terminal.
If not present then install beaglebone-universal-io and pasm before continuing further.

Currently the library supports compiling and running sources directly on the BeagleBone Black.
#Usage:
(Documentationin progress)
#About
This library is developed by Abhraneel Bera. Copyright (c) 2015 All rights reserved.
