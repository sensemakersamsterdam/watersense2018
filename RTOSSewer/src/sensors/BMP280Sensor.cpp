#include "BMP280Sensor.h"

bool BMP280Sensor::isReady = false;

Adafruit_BMP280 BMP280Sensor::sensor;

void BMP280Sensor::setup()
{
  Serial.println("BMP280Sensor - Setup");

  isReady = sensor.begin();

  if (!isReady) {
    Serial.println("BMP280Sensor - Failed to boot BMP280");
  }
}

void BMP280Sensor::measure()
{
  if (!isReady) {
    return;
  }

  Serial.print("Temperature = ");
  Serial.print(sensor.readTemperature());
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(sensor.readPressure());
  Serial.println(" Pa");

  Serial.print("Approx altitude = ");
  Serial.print(sensor.readAltitude(1013.25)); // this should be adjusted to your local forcase
  Serial.println(" m");
}
