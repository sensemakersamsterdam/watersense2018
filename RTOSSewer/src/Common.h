#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>

#define USE_BOARD              1
#define USE_BOARD_LED          0 && USE_BOARD

#define USE_I2C                1
#define USE_BMP280             1 && USE_I2C
#define USE_VL53L0X            1 && USE_I2C

#define USE_LOGGER             1
#define USE_LOGGER_BMP280      1 && USE_LOGGER
#define USE_LOGGER_BOARD       1 && USE_LOGGER && USE_BOARD
#define USE_LOGGER_I2C         1 && USE_LOGGER && USE_I2C
#define USE_LOGGER_I2C_DEVICES 1 && USE_LOGGER && USE_BMP280
#define USE_LOGGER_MAIN        1 && USE_LOGGER
#define USE_LOGGER_SYSINFO     1 && USE_LOGGER
#define USE_LOGGER_VL53L0X     1 && USE_LOGGER && USE_VL53L0X

#ifdef ARDUINO_SODAQ_ONE
#define Serial SerialUSB
#endif

#include "freertos/FreeRTOSv10.0.1/FreeRTOS.h"
#include "freertos/FreeRTOSv10.0.1/semphr.h"
#include "freertos/FreeRTOSv10.0.1/task.h"

#include "util/Logger.h"

#endif
