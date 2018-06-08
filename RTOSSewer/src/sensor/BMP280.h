#ifndef BMP280_H
#define BMP280_H

#include <Adafruit_BMP280.h>

#include "../Common.h"

class BMP280 {
public:
  static bool isReady;
  static void setup();
  static void measure();
private:
  static Adafruit_BMP280 sensor;
};

#endif
