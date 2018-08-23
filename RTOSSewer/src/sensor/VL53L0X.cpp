#include <VL53L0X.h>

#include "../periph/I2C.h"
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
  uint16_t i = sensor.readRangeSingleMillimeters();

  if (sensor.timeoutOccurred()) {
    LOGS("timeout");
    return 0;
  }

  return i;
}
