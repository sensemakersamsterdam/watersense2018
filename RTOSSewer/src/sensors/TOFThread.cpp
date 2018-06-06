#include <Arduino.h>
#include "TOFThread.h"
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int TOFThread::poweronFlag = LOW;

void TOFThread::setup() {
  SerialUSB.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    SerialUSB.println(F("Failed to boot VL53L0X"));
    while(1);
  }
}

void TOFThread::loop() {
 VL53L0X_RangingMeasurementData_t measure;
 SerialUSB.print("Reading a measurement... ");
 lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    if (measure.RangeMilliMeter < 80 && measure.RangeMilliMeter != 0 ){ // 0 means no proper data beacuse there is no reflection within range
      SerialUSB.print("Sewer is getting full, only ");SerialUSB.print(measure.RangeMilliMeter); SerialUSB.println(" mm left!");
    }
    SerialUSB.print("Distance (mm): "); SerialUSB.println(measure.RangeMilliMeter);
  } else {
    SerialUSB.println(" out of range ");
  }
 
  TOFThread::poweronFlag ^= 1;
}
