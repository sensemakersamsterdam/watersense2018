#ifndef SEWER_H
#define SEWER_H

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "Common.h"

#if USE_I2C
#include "periph/I2C.h"
#endif

#if USE_BMP280
#include "sensor/BMP280.h"
#endif

#if USE_VL53L0X
#include "sensor/VL53L0X.h"
#endif

#if USE_BOARD_ARDUINO_MEGA2560_R3
#include "board/ARDUINO_MEGA2560_R3.h"
#elif USE_BOARD_ARDUINO_UNO_R3
#include "board/ARDUINO_UNO_R3.h"
#elif USE_BOARD_SODAQ_ONE_V3
#include "board/SODAQ_ONE_V3.h"
#endif


/*******************************************************************************
 * Public
 ******************************************************************************/

void Sewer_setup();
void Sewer_idle();


/*******************************************************************************
 * Private
 ******************************************************************************/

static void Sewer_initModules();
static void Sewer_T0(void* pvParameters);

#if USE_BMP280
static void Sewer_T1(void* pvParameters);
#endif

#if USE_VL53L0X
static void Sewer_T2(void* pvParameters);
#endif

#endif
