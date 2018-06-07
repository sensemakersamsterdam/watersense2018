#ifndef TOF_SENSOR_H
#define TOF_SENSOR_H

#include "Adafruit_VL53L0X.h"
#include "../Common.h"

class TOFSensor {
public:
  static bool isReady;
  static void setup();
  static void measure();
private:
  static Adafruit_VL53L0X sensor;
  static int poweronFlag;
};

#endif
