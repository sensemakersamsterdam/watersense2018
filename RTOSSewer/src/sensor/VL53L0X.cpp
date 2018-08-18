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
    sensor.setMeasurementTimingBudget(200000);
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
