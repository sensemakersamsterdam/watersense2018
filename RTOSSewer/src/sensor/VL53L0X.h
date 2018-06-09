#ifndef VL53L0X_H
#define VL53L0X_H

#include <Adafruit_VL53L0X.h>

#include "../Common.h"

class VL53L0X {
public:
  static bool isReady;
  static void setup();
  static void measure();
private:
  static Adafruit_VL53L0X sensor;
};

#endif
