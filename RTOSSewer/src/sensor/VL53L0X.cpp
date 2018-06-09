#include <Adafruit_VL53L0X.h>
#include "../periph/I2C.h"
#include "VL53L0X.h"


/*******************************************************************************
 * State
 ******************************************************************************/

BaseType_t VL53L0X_isReady;

static Adafruit_VL53L0X sensor;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void VL53L0X_setup()
{
  VL53L0X_isReady = false;

  if (I2C_lock()) {
    VL53L0X_isReady = sensor.begin();
    I2C_unlock();
  }

  LOGS(VL53L0X_isReady ? "started" : "failed");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void VL53L0X_measure()
{
  LOGS("reading a measurement... ");

  VL53L0X_RangingMeasurementData_t measure;

  if (!I2C_lock()) { return; }

  sensor.rangingTest(&measure);

  I2C_unlock();

  // phase failures have incorrect data
  if (measure.RangeStatus != 4) {
    // 0 means no proper data beacuse there is no reflection within range
    if (measure.RangeMilliMeter < 80 && measure.RangeMilliMeter != 0 ) {
      LOGT("sewer is getting full, only %d mm left", measure.RangeMilliMeter);
    }

    LOGT("distance: %d mm", measure.RangeMilliMeter);
  } else {
    LOGS("out of range");
  }
}
