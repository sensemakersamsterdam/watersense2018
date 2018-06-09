#include "../periph/I2C.h"
#include "TOF.h"


/*******************************************************************************
 * State
 ******************************************************************************/

bool TOF::isReady;

Adafruit_VL53L0X TOF::sensor;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void TOF::setup()
{
  isReady = false;

  if (I2C::lock()) {
    isReady = sensor.begin();
    I2C::unlock();
  }

  LOGA(isReady ? "started" : "failed");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void TOF::measure()
{
  LOGA("reading a measurement... ");

  VL53L0X_RangingMeasurementData_t measure;

  if (!I2C::lock()) { return; }
  sensor.rangingTest(&measure);
  I2C::unlock();

  // phase failures have incorrect data
  if (measure.RangeStatus != 4) {
    // 0 means no proper data beacuse there is no reflection within range
    if (measure.RangeMilliMeter < 80 && measure.RangeMilliMeter != 0 ) {
      LOGT("sewer is getting full, only %d mm left", measure.RangeMilliMeter);
    }

    LOGT("distance: %d mm", measure.RangeMilliMeter);
  } else {
    LOGA("out of range");
  }
}
