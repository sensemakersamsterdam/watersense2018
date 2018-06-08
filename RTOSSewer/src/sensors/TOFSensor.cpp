#include "TOFSensor.h"

bool TOFSensor::isReady = false;

Adafruit_VL53L0X TOFSensor::sensor;

int TOFSensor::poweronFlag = LOW;

void TOFSensor::setup()
{
  Serial.print("TOF: setup... ");

  isReady = sensor.begin();

  if (!isReady) {
    Serial.println("failed to boot VL53L0X");
  } else {
    Serial.println("done");
  }
}

void TOFSensor::measure()
{
  if (!isReady) {
    return;
  }

  Serial.print("TOF: reading a measurement... ");

  VL53L0X_RangingMeasurementData_t measure;

  sensor.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

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

  poweronFlag ^= 1;
}
