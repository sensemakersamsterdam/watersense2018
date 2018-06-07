#include <Arduino.h>
#include "BMP280Sensor.h"

bool BMP280Sensor::isReady = false;

Adafruit_BMP280 BMP280Sensor::sensor;

void BMP280Sensor::setup()
{
  Serial.println("BMP280Sensor - Setup");

  // TODO
}

void BMP280Sensor::measure()
{
  // if (!isReady) {
  //   return;
  // }

  // TODO
}
