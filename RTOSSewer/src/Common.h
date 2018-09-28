/*
RTOSSewer project: sensing water quality in a variety of ways.

Copyright (C) 2018 Sensemakers Amsterdam. All rights reserved.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COMMON_H
#define COMMON_H

#define PROJECT_NAME      "RTOSSewer"
#define PROJECT_VERSION   2018091900
#define PROTOCOL_VERSION  2
#define LORA_SF           7

#define USE_DEEPSLEEP     0
#define USE_LOGGER        1
#define USE_LORA          1

//#define PIN_VEGETRONIXAQUAPLUMB_ANALOG PIN_A0
//#define PIN_SEN0189_ANALOG             PIN_A1
//#define PIN_CONDUCTIVITY_ANALOG        PIN_A2
//#define PIN_CONDUCTIVITY_TRIG          3
//#define PIN_ONEWIREBUS                 6
//#define PIN_HCSR04_ECHO                7
//#define PIN_HCSR04_TRIG                8
//#define PIN_MB7092_ANALOG              PIN_A9
//#define PIN_MB7092_TRIG                10
//#define PIN_SENSORS_POWER              11

#include <Arduino.h>

#include "freertos/FreeRTOSv10.0.1/FreeRTOS.h"
#include "freertos/FreeRTOSv10.0.1/semphr.h"
#include "freertos/FreeRTOSv10.0.1/task.h"

#include "util/Logger.h"

#ifdef ARDUINO_ARCH_SAMD
#define Serial SerialUSB
#endif

#endif // COMMON_H
