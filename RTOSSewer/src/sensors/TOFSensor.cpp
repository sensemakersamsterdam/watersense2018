#include "../Common.h"
#include "TOFSensor.h"

/*******************************************************************************
 * State
 ******************************************************************************/

bool TOFSensor::isReady;

Adafruit_VL53L0X TOFSensor::sensor;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void TOFSensor::setup()
{
  Serial.print("TOF: setup... ");

  isReady = sensor.begin();

  Serial.println(isReady ? "done" : "failed");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void TOFSensor::measure()
{
  if (!isReady) {
    return;
  }

  Serial.print("TOF: reading a measurement... ");

  VL53L0X_RangingMeasurementData_t measure;

  taskENTER_CRITICAL();
  sensor.rangingTest(&measure);
  taskEXIT_CRITICAL();

  // phase failures have incorrect data
  if (measure.RangeStatus != 4) {
    // 0 means no proper data beacuse there is no reflection within range
    if (measure.RangeMilliMeter < 80 && measure.RangeMilliMeter != 0 ) {
      Serial.print("sewer is getting full, only ");
      Serial.print(measure.RangeMilliMeter);
      Serial.println(" mm left! ");
    }

    Serial.print("distance (mm): ");
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println("out of range ");
  }
}
