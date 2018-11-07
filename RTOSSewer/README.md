# RTOSSewer

RTOSSewer project: sensing water quality in a variety of ways.


## License

Copyright (C) 2018 Sensemakers Amsterdam. All rights reserved.

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

See the COPYING.LESSER file for details.


## Tested on hardware:

- Board [SODAQ ONE-EU-RN2483-V3](http://support.sodaq.com/sodaq-one/sodaq-one/)
- Sensor [BMP280](https://learn.adafruit.com/adafruit-bmp280-barometric-pressure-plus-temperature-sensor-breakout/)
- Sensor [VL53L0X](https://learn.adafruit.com/adafruit-vl53l0x-micro-lidar-distance-sensor-breakout/)


## Used open source software:

### Adafruit BMP280 Sensor library 1.0.2

> *Adafruit invests time and resources providing this open source code, please support Adafruit andopen-source hardware by purchasing products from Adafruit!*  
> *Written by Kevin Townsend for Adafruit Industries. BSD license, all text above must be included in any redistribution*

*See: sources, <https://github.com/adafruit/Adafruit_BMP280_Library>*


### Adafruit Unified Sensor library 1.0.2

> *Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0*  
> *Copyright (C) 2008 The Android Open Source Project*

*Note: this library used in Adafruit BMP280 Library 1.0.2*

*See: sources, <https://github.com/adafruit/Adafruit_Sensor>*


### Dallas Temperature Sensor library 3.8.0

> *This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.*  

*See: sources, <https://github.com/milesburton/Arduino-Temperature-Control-Library>*


### FreeRTOS 10.1.1

> *Both the FreeRTOS kernel and Amazon FreeRTOS are provided under the following MIT open source license.*  
> *Copyright (C) 2017 Amazon.com, Inc. or its affiliates.*

*See: sources, <https://www.freertos.org/a00114.html>*


### OneWire library 2.3.4

> *Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do [...]*  
> *Copyright (c) 2007, Jim Studt  (original old version - many contributors since)*

*See: sources, <http://www.pjrc.com/teensy/td_libs_OneWire.html>*


### Pololu VL53L0X Sensor library 1.0.2

> *Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do [...]*  
> *Copyright (c) 2017 Pololu Corporation.*

> *Most of the functionality of this library is based on the VL53L0X API provided by ST (STSW-IMG005), and some of the explanatory comments are quoted or paraphrased from the API source code, API user manual (UM2039), and the VL53L0X datasheet. [...]*  
> *Copyright © 2016, STMicroelectronics International N.V. All rights reserved.*  

*See: sources, <https://github.com/pololu/vl53l0x-arduino>*


### RTCZero library 1.5.2

> *This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.*  
> *Copyright (c) Arduino LLC. All right reserved.*

*See: sources*


### Sodaq LSM303AGR Sensor library 2.0.0

> *Sodaq_LSM303AGR is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.*  
> *Copyright (c) 2018 SODAQ. All rights reserved.*

*See: sources, <https://github.com/SodaqMoja/Sodaq_LSM303AGR>*


### Sodaq RN2483 library 1.0.11

> *Sodaq_RN2483 is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.*  
> *Copyright (c) 2015 SODAQ. All rights reserved.*

*See: sources, <https://github.com/SodaqMoja/Sodaq_RN2483>*


### Sodaq WDT library 1.0.1

*Note: this library used in Sodaq RN2483 library 1.0.11*

*See: <https://github.com/SodaqMoja/Sodaq_wdt>*


### SparkFun MS5803-14BA Pressure Sensor 1.1.1

> *The hardware is released under Creative Commons Share-alike 3.0.*

*See: sources, <https://github.com/sparkfun/SparkFun_MS5803-14BA_Breakout_Arduino_Library>*


### SODAQ software for Arduino SAMD 1.6.18

> *This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.*  
> *Copyright (c) 2016 SODAQ. All right reserved.*

*See: sources*


## Used free tools:

### Arduino 1.8.5

> *The source code for the Java environment is released under the GPL and the C/C++ microcontroller libraries are under the LGPL*

*See: <https://www.arduino.cc/en/Main/FAQ>*


### ARM EABI GCC 4.8.3

> *arm-none-eabi-gcc (GNU Tools for ARM Embedded Processors (Arduino build)) 4.8.3 20140228 (release) [ARM/embedded-4_8-branch revision 208322]*  
> *Copyright (C) 2013 Free Software Foundation, Inc.*  
> *This is free software; see the source for copying conditions. There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.*

*See: arm-none-eabi-gcc --version*


### BOSSA 1.7.0

> *Basic Open Source SAM-BA Application (BOSSA) Version 1.7.0*  
> *Flash programmer for Atmel SAM devices.*  
> *Copyright (c) 2011-2012 ShumaTech (http://www.shumatech.com)*

*See: bossac -h*
