#include "VL53L0X.h"

#if USE_VL53L0X

#include <Adafruit_VL53L0X.h>
#include "../periph/I2C.h"


/*******************************************************************************
 * State
 ******************************************************************************/

static Adafruit_VL53L0X sensor;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

uint8_t VL53L0X_setup()
{
  uint8_t b = false;

  if (I2C_lock()) {
    b = sensor.begin();
    I2C_unlock();
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

  VL53L0X_RangingMeasurementData_t measure;

  if (!I2C_lock()) { return; }

  sensor.rangingTest(&measure);

  I2C_unlock();

  // phase failures have incorrect data
  if (measure.RangeStatus != 4) {
    // 0 means no proper data beacuse there is no reflection within range
    if (measure.RangeMilliMeter < 80 && measure.RangeMilliMeter != 0 ) {
      LOG(VS("sewer is getting full, only "), VUI16(measure.RangeMilliMeter), VS(" mm left"));
    }

    LOG(VS("distance: "), VUI16(measure.RangeMilliMeter), VS(" mm"));
  } else {
    LOGS("out of range");
  }
  #endif
}

#endif // USE_VL53L0X
