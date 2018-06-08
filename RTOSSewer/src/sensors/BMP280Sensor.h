#ifndef BMP280_SENSOR_H
#define BMP280_SENSOR_H

#include <Adafruit_BMP280.h>

class BMP280Sensor {
public:
  static bool isReady;
  static void setup();
  static void measure();
private:
  static Adafruit_BMP280 sensor;
};

#endif
