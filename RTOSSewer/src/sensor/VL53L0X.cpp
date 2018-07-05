#include "VL53L0X.h"

#if USE_VL53L0X

#include <VL53L0X.h>
#include "../periph/I2C.h"


/*******************************************************************************
 * State
 ******************************************************************************/

static VL53L0X sensor;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool VL53L0X_setup()
{
  bool b = false;

  if (I2C_lock()) {
    b = sensor.init() && sensor.last_status == 0;
    I2C_unlock();
  }

  if (b) {
    sensor.setTimeout(500);
    sensor.setMeasurementTimingBudget(200000);
  }

  #if USE_LOGGER_VL53L0X
  if (b) {
    LOGS("started");
  } else {
    LOGS("failed");
  }
  #endif

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void VL53L0X_measure()
{
  #if USE_LOGGER_VL53L0X
  LOGS("reading a measurement... ");

  if (!I2C_lock()) { return; }

  uint16_t i = sensor.readRangeSingleMillimeters();

  I2C_unlock();

  if (sensor.timeoutOccurred()) {
    LOGS("timeout");
  } else if (i > 2000)  {
    LOGS("out of range");
  } else {
    LOG(VS("distance: "), VUI16(i), VS(" mm"));
  }
  #endif
}

#endif // USE_VL53L0X
