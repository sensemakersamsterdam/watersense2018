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
  bool b = sensor.init() && sensor.last_status == 0;

  if (b) {
    sensor.setTimeout(500);
    sensor.setMeasurementTimingBudget(200000);
  }

  LOG_SHOW_SETUP_RESULT(b);

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void VL53L0X_measure()
{
  #if USE_LOGGER
  LOGS("reading a measurement... ");

  uint16_t i = sensor.readRangeSingleMillimeters();

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
