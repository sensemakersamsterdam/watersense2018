#include <Arduino.h>
#include "TOFTask.h"
#include "Adafruit_VL53L0X.h"

static Adafruit_VL53L0X lox = Adafruit_VL53L0X();

bool TOFTask::isReady = false;

int TOFTask::poweronFlag = LOW;

void TOFTask::setup()
{
  Serial.println("TOFTask - setup");

  TOFTask::isReady = lox.begin();

  if (!TOFTask::isReady) {
    Serial.println("TOFTask - Failed to boot VL53L0X");
  }
}

void TOFTask::loop()
{
  Serial.print("TOFTask - Reading a measurement... ");

  VL53L0X_RangingMeasurementData_t measure;

  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

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

  TOFTask::poweronFlag ^= 1;
}
