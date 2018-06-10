#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>

#define DEBUG true

#ifdef ARDUINO_SODAQ_ONE
#define Serial SerialUSB
#endif

#include "freertos/FreeRTOSv10.0.1/FreeRTOS.h"
#include "freertos/FreeRTOSv10.0.1/semphr.h"
#include "freertos/FreeRTOSv10.0.1/task.h"

#include "util/Logger.h"

#endif
