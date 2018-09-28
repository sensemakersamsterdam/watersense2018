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

#include <VL53L0X.h>

#include "../periph/I2C.h"
#include "../periph/WDT.h"
#include "../util/Collection.h"
#include "VL53L0X.h"


/*******************************************************************************
 * State
 ******************************************************************************/

static VL53L0X sensor;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool VL53L0X_setup()
{
  bool b = sensor.init() && sensor.last_status == 0;

  if (b) {
    sensor.setTimeout(500);

    // hign accuracy
    sensor.setMeasurementTimingBudget(200000);

    // lower the return signal rate limit (default is 0.25 MCPS)
    sensor.setSignalRateLimit(0.1);

     // increase laser pulse periods (defaults are 14 and 10 PCLKs)
    sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange,   18);
    sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  }

  LOG_SETUP_RESULT_TEXT(b);

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

uint16_t VL53L0X_measureDistance()
{
  uint16_t values[VL53L0X_DI_ATTEMPTS];

  for (uint8_t i = 0; i < VL53L0X_DI_ATTEMPTS; i++) {
    WDT_reset();

    uint16_t v = sensor.readRangeSingleMillimeters();

    if (sensor.timeoutOccurred()) {
      LOGS("timeout");
      return 0;
    }

    values[i] = v;
  }

  return VL53L0X_DI_CALIB_OFFSET + VL53L0X_DI_CALIB_COEFF * median(values, VL53L0X_DI_ATTEMPTS);
}
