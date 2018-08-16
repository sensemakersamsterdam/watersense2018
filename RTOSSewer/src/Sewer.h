#ifndef SEWER_H
#define SEWER_H


/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "Common.h"

#include "sensor/BMP280.h"
#include "sensor/FDC1004.h"
#include "sensor/LSM303AGR.h"
#include "sensor/VL53L0X.h"

#if USE_LORA_SODAQ
#include "periph/LoRaSodaq.h"
#endif


/*******************************************************************************
 * Public
 ******************************************************************************/

void Sewer_setup();
void Sewer_idle();

#endif // SEWER_H
