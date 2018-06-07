#include <Arduino.h>
#include "TOFSensor.h"

bool TOFSensor::isReady = false;

Adafruit_VL53L0X TOFSensor::sensor = Adafruit_VL53L0X();

int TOFSensor::poweronFlag = LOW;

void TOFSensor::setup()
{
  Serial.println("TOFSensor - Setup");

  isReady = sensor.begin();

  if (!isReady) {
    Serial.println("TOFSensor - Failed to boot VL53L0X");
  }
}

void TOFSensor::measure()
{
  if (!isReady) {
    return;
  }

  Serial.print("TOFSensor - Reading a measurement... ");

  VL53L0X_RangingMeasurementData_t measure;

  sensor.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  // phase failures have incorrect data
  if (measure.RangeStatus != 4) {
    // 0 means no proper data beacuse there is no reflection within range
    if (measure.RangeMilliMeter < 80 && measure.RangeMilliMeter != 0 ) {
      Serial.print("Sewer is getting full, only ");
      Serial.print(measure.RangeMilliMeter);
      Serial.println(" mm left! ");
    }

    Serial.print("Distance (mm): ");
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println("out of range ");
  }

  poweronFlag ^= 1;
}
