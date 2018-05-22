#include "Adafruit_VL53L0X.h"
//First version of TimeOfFlight Part for the watersense project.  
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
#define Serial SerialUSB

void setup() {
  Serial.begin(9600);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    if (measure.RangeMilliMeter < 80 && measure.RangeMilliMeter != 0 ){ // 0 means no proper data beacuse there is no reflection within range
      Serial.print("Sewer is getting full, only ");Serial.print(measure.RangeMilliMeter); Serial.println(" mm left!");
    }
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
    
  delay(100);
}
