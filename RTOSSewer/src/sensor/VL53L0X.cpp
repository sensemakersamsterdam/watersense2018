#include "VL53L0X.h"

#if USE_VL53L0X

#include <VL53L0X.h>
#include "../periph/I2C.h"


/*******************************************************************************
 * State
 ******************************************************************************/

bool     VL53L0X_active   = false;
uint16_t VL53L0X_distance = 0;

static VL53L0X sensor;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void VL53L0X_setup()
{
  VL53L0X_active = sensor.init() && sensor.last_status == 0;

  if (VL53L0X_active) {
    sensor.setTimeout(500);
    sensor.setMeasurementTimingBudget(200000);
  }

  LOG_SETUP_RESULT_TEXT(VL53L0X_active);
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void VL53L0X_measure()
{
  if (!VL53L0X_active) { return; }

  VL53L0X_distance = sensor.readRangeSingleMillimeters();

  if (sensor.timeoutOccurred()) {
    VL53L0X_distance = 0;
    LOGS("timeout");
  }
}

#endif // USE_VL53L0X
