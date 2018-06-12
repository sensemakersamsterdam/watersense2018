#ifndef COMMON_H
#define COMMON_H

#define USE_BOARD_ARDUINO_MEGA2560_R3 1
#define USE_BOARD_ARDUINO_UNO_R3      0
#define USE_BOARD_SODAQ_MOJA_R4       0
#define USE_BOARD_SODAQ_ONE_V3        0

#define USE_BOARD_LED                 1

#define USE_I2C                       1
#define USE_BMP280                    1 && USE_I2C
#define USE_VL53L0X                   1 && USE_I2C

#define USE_LOGGER                    1
#define USE_LOGGER_BMP280             1 && USE_LOGGER && USE_BMP280
#define USE_LOGGER_BOARD              1 && USE_LOGGER
#define USE_LOGGER_I2C                1 && USE_LOGGER && USE_I2C
#define USE_LOGGER_I2C_DEVICES        1 && USE_LOGGER && USE_I2C
#define USE_LOGGER_MAIN               1 && USE_LOGGER
#define USE_LOGGER_SYSINFO            1 && USE_LOGGER
#define USE_LOGGER_VL53L0X            1 && USE_LOGGER && USE_VL53L0X

#include <Arduino.h>

#ifdef ARDUINO_ARCH_SAMD
#define Serial SerialUSB
#endif

#include "freertos/FreeRTOSv10.0.1/FreeRTOS.h"
#include "freertos/FreeRTOSv10.0.1/semphr.h"
#include "freertos/FreeRTOSv10.0.1/task.h"
#include "util/Logger.h"

#if USE_BOARD_ARDUINO_MEGA2560_R3
#include "board/ARDUINO_MEGA2560_R3.h"
#elif USE_BOARD_ARDUINO_UNO_R3
#include "board/ARDUINO_UNO_R3.h"
#elif USE_BOARD_SODAQ_MOJA_R4
#include "board/SODAQ_MOJA_R4.h"
#elif USE_BOARD_SODAQ_ONE_V3
#include "board/SODAQ_ONE_V3.h"
#endif

#endif // COMMON_H
