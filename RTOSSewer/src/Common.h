#ifndef COMMON_H
#define COMMON_H

#define PROJECT_NAME                  "RTOSSewer"
#define PROJECT_VERSION               "0.0.1"

#define USE_BOARD_ARDUINO_MEGA2560_R3 0
#define USE_BOARD_ARDUINO_UNO_R3      0
#define USE_BOARD_SODAQ_MOJA_R4       0
#define USE_BOARD_SODAQ_ONE_V3        1

#define USE_ONBOARD_LED               1
#define USE_ONBOARD_TEMPERATURE       1
#define USE_ONBOARD_VOLTAGE           1 && USE_BOARD_SODAQ_ONE_V3

#define USE_BMP280                    1
#define USE_FDC1004                   1
#define USE_VL53L0X                   1

#define USE_RTC                       1 && USE_BOARD_SODAQ_ONE_V3
#define USE_WDT                       1
#define USE_DEEPSLEEP                 0 && (USE_RTC || USE_WDT)
#define USE_LOGGER                    1

#define USE_LORA_SODAQ                USE_BOARD_SODAQ_ONE_V3
#define USE_LSM303AGR                 USE_BOARD_SODAQ_ONE_V3
#define USE_I2C                       USE_BMP280 || USE_FDC1004 || USE_LSM303AGR || USE_VL53L0X

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
