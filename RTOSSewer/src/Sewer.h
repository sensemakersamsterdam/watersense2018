#ifndef SEWER_H
#define SEWER_H


/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "Common.h"

#include "periph/I2C.h"
#include "sensor/BMP280.h"
#include "sensor/FDC1004.h"
#include "sensor/VL53L0X.h"

#if USE_LORA_SODAQ
#include "periph/LoRaSodaq.h"
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
static void Sewer_threadMain(void* pvParameters);

#endif // SEWER_H
