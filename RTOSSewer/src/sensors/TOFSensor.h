#ifndef TOF_SENSOR_H
#define TOF_SENSOR_H

#include <Adafruit_VL53L0X.h>

class TOFSensor {
public:
  static bool isReady;
  static void setup();
  static void measure();
private:
  static Adafruit_VL53L0X sensor;
};

#endif
