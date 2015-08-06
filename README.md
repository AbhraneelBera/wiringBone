# wiringBone
Wiring/Arduino style library for BeagleBone Black Platform
#Description:
This library contains almost all the core wiring/arduino functions along with some advanced functions.
This library is intended for quick and easy hardware control on the BeagleBone Black platform.
All the functions have similar behaviour with the wiring/arduino environment.
This library uses the power of 'beaglebone-universal-io' device tree overlays to configure the overlay at runtime. 
#Features:
1. Control 65 GPIO pins for digital input/output.
2. Generate PWM from total 30 pins (6 pins from PWMSS and 24 pins from PRU_ICSS).
3. Among 30 PWM pins, 24 pins have failsafe feature (Discussed in 'PWM' section below).
4. Capture pulse width and time period of incoming pulse train from 26 pins.
5. Control 4 UART ports and any other serial device available.
6. Control 1 I2C port.
7. Control 2 SPI ports (experimental, not tested).
8. Configure the overlay at run-time and the overlay will revert back to the default pin modes at program exit.
9. GPIO and PWM port auto clean-up at program exit (Discussed in 'Stopping the program' section below).

Use of some pins may require disabling the hdmi and emmc capes.
Refer to the pin diagrams in this link http://beagleboard.org/Support/bone101

NOTE : This library uses the 'beaglebone-universal-io' device tree overlays and the 'pasm' (PRU assembler).
Check whether beaglebone-universal-io and pasm is present in your os image by typing config-pin and pasm on the terminal.
If not present then install beaglebone-universal-io and pasm before continuing further. Refer to the links below:

https://github.com/cdsteinkuehler/beaglebone-universal-io

https://github.com/beagleboard/am335x_pru_package

Currently the library supports compiling and running sources directly on the BeagleBone Black.
#Initial Setup:
First of all clone this repository in any directory on the local BeagleBone Black file system.

To use any of the pins with wiringBone user has to modify the UserPinConfig.h file and change the pin mode of required pins. Each pin in Beaglebone Black has various modes of operation. That's why the user has to select wisely which pin to use in which mode by editing the UserPinConfig.h file. Valid modes for each pin is mentioned in the UserPinConfig.h file.

* If pins- P9.25, P9.28, P9.29, P9.31 are used hdmi-audio cape should be disabled first.

* If pins- P8.27, P8.28, P8.29, P8.30, P8.31, P8.32, P8.33, P8.34, P8.35, P8.36, P8.37, P8.38, P8.39, P8.40, P8.41, P8.42, P8.43, P8.44, P8.45, P8.46 are used hdmi cape should be disabled first.

* If pins- P8.3, P8.4, P8.5, P8.6, P8.20, P8.21, P8.22, P8.23, P8.24, P8.25 are used emmc cape should be disabled first.

To disable any of the above capes open /boot/uEnv.txt file and remove comment from the appropriate line. Save the file and reboot. For details refer to Cape Manager section in this link http://elinux.org/Beagleboard:Weather_Cape_Work-Around

The library will automatically compile source files present in the cloned directory (wiringBone directory).
If the source files are present in some other directory then that directory path needs to be included in the Makefile.mk file. Instructions are present within the Makefile.mk file.

The source files can have extensions .c, .cpp, .ino and .pde.
Multiple .c and .cpp files (including library style sources having one .c/.cpp file and one .h file) can be compiled but compiling multiple .ino or .pde is not recommended.

#Usage:
The code should follow the setup() and loop() pattern similar to the wiring/arduino sketches.
In the wiring/arduino environment some functions takes the pin number as a parameter.
But this library will use pin name instead of pin numbers.
"Pin" is defined as a structure type and all the pin names are structures in this library.
For example: In an arduino sketch one may write: 

    pinMode(2, OUTPUT); 
But here instead of pin number 2 the pin name has to be given like:

    pinMode(P8_10, OUTPUT); or pinMode(P9_14, OUTPUT);

#1) GPIO
Functions:

    void pinMode(Pin pin, uint8_t direction)
    void digitalWrite(Pin pin, bool state)
    uint8_t digitalRead(Pin pin)

direction can be INPUT or OUTPUT. state can be HIGH or LOW.

pinMode() function is only required to set the direction of pin when the pin is in gpio mode.
To use any pin in gpio mode the UserPinConfig.h file has to be modified by changing pin mode to gpio.

#2) PWM
Functions:

    void analogWrite(Pin pin, uint8_t value)
    uint32_t pulseIn(Pin pin, bool polarity)
    uint32_t pulseIn(Pin pin, bool polarity, uint32_t timeout)

polarity can be HIGH or LOW.

analogWrite() will generate PWM with time period 2040 micro seconds which is approx 490 Hz.
There are 8 pins which are controlled by the PWMSS and 24 pins which are controlled by the PRU_ICSS.
To use PWM outputs the pin modes for PWMSS pins should be changed to 'pwm' mode and PRU_ICSS pins should be changed to 'pruout' mode in the UserPinConfig.h file.

There are 26 pins which can be used with the pulseIn() function to read pulse width of PWM signal.
Default timeout will be set to 1 second.
To use this function the pin mode should be changed to 'pruin' mode in the UserPinConfig.h file.

Advanced Usage:

 For both PWMSS and PRU_ICSS:

    void setTimePeriod (Pin pin, uint32_t period_us)
    void setFrequency (Pin pin, uint32_t freq_hz)
    void setPulseWidth (Pin pin, uint32_t period_us)
    void setDutyPercentage (Pin pin, uint32_t percentage)

 Only for PWMSS:

    void setTimePeriodns (Pin pin, uint32_t period_ns)
    void setPulseWidthns (Pin pin, uint32_t period_ns)

 Only for PRU_ICSS:

    uint32_t getTimePeriod (Pin pin)
    uint32_t getFrequency (Pin pin)
    uint32_t getPulseWidth (Pin pin)
    uint32_t getDutyPercentage (Pin pin)
    void setPulseReadTimeout (uint32_t time_us)
    void setFailsafePRU (uint32_t pulseWidth_us)
    void setFailsafePRU (uint32_t pulseWidth_us, uint32_t timePeriod_us)

* 'us' refers to micro seconds and 'ns' refers to nano seconds.

The setPulseReadTimeout can be called once to set the maximum time to wait for a toggle in the input pulse.
The timeout value should be always greater than the time period of the input signal.

The PRU_ICSS pins have a failsafe feature.
In case the program stops abruptly, or considering the worst case if the system hangs then the PRU output pins will go to the failsafe mode and will generate a predefined PWM signal.
If setFailsafePRU is never called the PRU output pins will give low output (0 pulse width) in failsafe condition.
For controlling dc motors failsafe value of 0 pulse width is recommended.
For controlling servo motors or a brushless esc failsafe value of 1000 micro second pulse width is recommended.

#3) ADC
Functions:

    void analogReadResolution(uint8_t bits)
    int analogRead(adcPin pin)
    int analogRead(uint8_t pin)

"adcPin" is an enum type which has values from AIN0 to AIN6.
User can give the pin name as well as the adc pin numbr as a parameter to analogRead().
For example: 

    analogRead(AIN3); or analogRead(3);
analogRead() will return 10 bit adc values.

Advanced usage:

    int readADC(adcPin pin)

readADC will return the full 12 bit ADC readings.

#4) Time
Functions:

    void delay(uint32_t duration)
    void delayMicroseconds(uint32_t duration)
    uint32_t millis()
    uint32_t micros()

#5) UART
Functions:

    void begin(unsigned long)
    void end()
    int available(void)
    int peek(void)
    int read(void)
    void flush(void)
    size_t write(uint8_t)
    size_t write(unsigned long n)
    size_t write(long n)
    size_t write(unsigned int n)
    size_t write(int n)

Above funtions have similar behaviour with the arduino environment (Refer to documentation on Arduino website).
There are 4 uart ports available.
UART1 is defined as Serial, UART2 is defined as Serial1, UART4 is defined as Serial2 and UART5 is defined as Serial3.
Usage example: 

    Serial.begin(9600);
To use any UART pin user should change the pin mode to uart in the UserPinConfig.h file.

Advanced Usage:

Other than this any other serial device present (For Example: bluetooth serial, gadget serial etc.) can be used with this library.
To do this user will have to create an object of class HardwareSerial with device path as the parameter.
For example: to use bluetooth serial, use: 

    HardwareSerial mySerial("/dev/rfcomm0");
Now the mySerial object can be used with the library like 

    mySerial.begin(115200);

#6) I2C
Functions:

    begin()
    beginTransmission(uint8_t)
    beginTransmission(int)
    endTransmission(void)
    requestFrom(uint8_t, uint8_t)
    requestFrom(int, int)
    size_t write(uint8_t)
    size_t write(const uint8_t *, size_t)
    int available(void)
    int read(void)
    int peek(void)
    void flush(void)
    size_t write(unsigned long n)
    size_t write(long n)
    size_t write(unsigned int n)
    size_t write(int n)

Above funtions have similar behaviour with the arduino environment (Refer to documentation on Arduino website).
Though the board has 2 I2C ports, only I2C-2 will be used.
The sendStop option in I2C is not yet implemented.
BeagleBone Black can not be used in I2C slave mode.
Example usage: 

    Wire.begin();

#7) SPI
(The SPI library is experimental and not tested)

Functions:

    void begin()
    void beginTransaction(SPISettings settings)
    uint8_t transfer(uint8_t data)
    uint16_t transfer16(uint16_t data)
    void transfer(void *buf, size_t count)
    void endTransaction(void)
    void end()
    void setBitOrder(uint8_t bitOrder)
    void setDataMode(uint8_t dataMode)
    void setClockDivider(uint8_t clockDiv)

Above funtions have similar behaviour with the arduino environment (Refer to documentation on Arduino website).
There are 2 SPI ports available.
SPI-0 is defined as SPI.
Example usage: 

    SPI.begin();
To use any SPI pin user should change the pin mode to spi in the UserPinConfig.h file.

Advanced Usage:

Functions:

    void setClock(uint32_t clock_hz)

Above function can be used to manually set the SPI clock.
For Example: 

    SPI.setClock(4000000);

The SPI-1 port is defined as SPI1.
It can be used in the same manner as the SPI object.

#8) EEPROM
Functions:

    uint8_t read( int idx )
    void write( int idx, uint8_t val )
    void update( int idx, uint8_t val )
    get() (Refer to Arduino website for the funtion parameters)
    put() (Refer to Arduino website for the funtion parameters)

Above funtions have similar behaviour with the arduino environment (Refer to documentation on Arduino website).
The library will not use any hardware eeprom.
Instead it will create a virtual file in the file system and perform read write operations to it as if it is communicating with an eeprom.

#Compiling:
A shell script with name build-source is present in the wiringBone directory which will help to compile and run the code.
First make the script executable by typing 

    'chmod 755 build-source' 
in the terminal from wiringBone directory.
build-source will accept 3 cases: make, make-and-run and clean.
For Example: type 

    ./build-source make-and-run 
to compile your code and run.

#Stopping the program:
Always stop the program with ctrl+c.
The wiringBone library uses an auto clean-up feature.
The basic idea is, if the program is using a pin then that pin should be restored back to its previous state after program termination.
When the program is stopped using ctrl+c it will reset all the GPIOs being used, turn off PWM outputs, unexport the GPIO and PWM pins being used and configure the overlay back to default states.

If program is terminated with ctrl+z it will not perform the clean-up at exit.

#Known Issues:
1. After boot up if a program is run for the first time it may give a "Bus error". Nothing to worry about it. Run the program again. It will work. Sometimes the universal-io overlay takes some time to load, which is the cause of this error. Work is in progress to fix this issue.
2. Error messages may not show accurate information. Work is in progress to fix this issue.

#About
This library is developed by Abhraneel Bera. Copyright (c) 2015 All rights reserved.
