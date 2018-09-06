#ifndef COMMON_H
#define COMMON_H

#define PROJECT_NAME        "RTOSSewer"
#define PROJECT_VERSION     2018090300
#define USE_DEEPSLEEP       0
#define USE_LOGGER          1
#define USE_LORA            0

//#define PIN_ONEWIREBUS    6
//#define PIN_HCSR04_ECHO   7
//#define PIN_HCSR04_TRIG   8
//#define PIN_MB7092_ANALOG PIN_A9
//#define PIN_MB7092_TRIG   10
//#define PIN_SENSORS_POWER 11

#include <Arduino.h>

#include "freertos/FreeRTOSv10.0.1/FreeRTOS.h"
#include "freertos/FreeRTOSv10.0.1/semphr.h"
#include "freertos/FreeRTOSv10.0.1/task.h"

#include "util/Logger.h"

#ifdef ARDUINO_ARCH_SAMD
#define Serial SerialUSB
#endif

#endif // COMMON_H
