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

// main constants
#define PROJECT_NAME               "RTOSSewer"
#define PROTOCOL_VERSION           2
#define LORA_PORT                  1
#define LORA_SF                    12
#define MAINTHREAD_DELAY_LOOP      600000
#define USE_DEEPSLEEP              1
#define USE_LOGGER                 0
#define USE_LORA                   1

// sensors parameters
// note: if the parameters changed => PROTOCOL_VERSION also should be changed
#define AQUAP_DI_ATTEMPTS          5
#define AQUAP_DI_CALIB_OFFSET      1224.11F
#define AQUAP_DI_CALIB_COEFF      -6.5059F
#define BMP280_PR_ATTEMPTS         3
#define BMP280_PR_CALIB_OFFSET     0.0F
#define BMP280_PR_CALIB_COEFF      1.0F
#define BMP280_TE_ATTEMPTS         3
#define BMP280_TE_CALIB_OFFSET     0.0F
#define BMP280_TE_CALIB_COEFF      1.0F
#define COND_CO_ATTEMPTS           5
#define COND_CO_CALIB_OFFSET       20683.0F
#define COND_CO_CALIB_COEFF       -41.129F
#define DS18B20_TE_ATTEMPTS        3
#define DS18B20_TE_CALIB_OFFSET    0.0F
#define DS18B20_TE_CALIB_COEFF     1.0F
#define HCSR04_DI_ATTEMPTS         5
#define HCSR04_DI_CALIB_OFFSET    -0.3F
#define HCSR04_DI_CALIB_COEFF      0.01733F
#define LSM303AGR_TE_ATTEMPTS      3
#define LSM303AGR_TE_CALIB_OFFSET  0.0F
#define LSM303AGR_TE_CALIB_COEFF   1.0F
#define MB7092_DI_ATTEMPTS         5
#define MB7092_DI_CALIB_OFFSET    -17.39206F
#define MB7092_DI_CALIB_COEFF      1.02825F
#define SEN0189_TB_ATTEMPTS        5
#define SEN0189_TB_CALIB_OFFSET    1000.0F
#define SEN0189_TB_CALIB_COEFF    -1.0F
#define VL53L0X_DI_ATTEMPTS        3
#define VL53L0X_DI_CALIB_OFFSET    5.31955F
#define VL53L0X_DI_CALIB_COEFF     0.07707F

// pins
#define PIN_AQUAP_ANALOG   PIN_A0
#define PIN_SEN0189_ANALOG PIN_A1
#define PIN_COND_ANALOG    PIN_A2
#define PIN_ONEWIREBUS     6
#define PIN_HCSR04_ECHO    7
#define PIN_HCSR04_TRIG    8
#define PIN_MB7092_ANALOG  PIN_A9
#define PIN_MB7092_TRIG    10
#define PIN_SENSORS_POWER  11

// required includes
#include <Arduino.h>
#include "freertos/FreeRTOSv10.0.1/FreeRTOS.h"
#include "freertos/FreeRTOSv10.0.1/semphr.h"
#include "freertos/FreeRTOSv10.0.1/task.h"
#include "util/Logger.h"

// debug serial
#ifdef ARDUINO_ARCH_SAMD
#define Serial SerialUSB
#endif

#endif // COMMON_H
