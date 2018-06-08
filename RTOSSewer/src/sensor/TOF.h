#ifndef TOF_H
#define TOF_H

#include <Adafruit_VL53L0X.h>

#include "../Common.h"

class TOF {
public:
  static bool isReady;
  static void setup();
  static void measure();
private:
  static Adafruit_VL53L0X sensor;
};

#endif
