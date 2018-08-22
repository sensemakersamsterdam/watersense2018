#ifndef COMMON_H
#define COMMON_H

#define PROJECT_NAME    "RTOSSewer"
#define PROJECT_VERSION 2018082200
#define USE_DEEPSLEEP   0
#define USE_LOGGER      1
#define USE_LORA        1

#include <Arduino.h>

#include "freertos/FreeRTOSv10.0.1/FreeRTOS.h"
#include "freertos/FreeRTOSv10.0.1/semphr.h"
#include "freertos/FreeRTOSv10.0.1/task.h"

#include "util/Logger.h"

#ifdef ARDUINO_ARCH_SAMD
#define Serial SerialUSB
#endif

#endif // COMMON_H
