#include "BMP280Sensor.h"

#define BMP280_ADDRESS 0x76

bool BMP280Sensor::isReady = false;

Adafruit_BMP280 BMP280Sensor::sensor;

void BMP280Sensor::setup()
{
  Serial.println("BMP280: setup");

  isReady = sensor.begin(BMP280_ADDRESS);

  if (!isReady) {
    Serial.println("BMP280: failed to boot BMP280");
  }
}

void BMP280Sensor::measure()
{
  if (!isReady) {
    return;
  }

  Serial.print("BMP280: temperature = ");
  Serial.print(sensor.readTemperature());
  Serial.println(" *C");

  Serial.print("BMP280: pressure = ");
  Serial.print(sensor.readPressure());
  Serial.println(" Pa");

  Serial.print("BMP280: approx altitude = ");
  Serial.print(sensor.readAltitude(1013.25)); // this should be adjusted to your local forcase
  Serial.println(" m");
}
