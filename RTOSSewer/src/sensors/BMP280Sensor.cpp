#include "../Common.h"
#include "BMP280Sensor.h"

/*******************************************************************************
 * State
 ******************************************************************************/

#define BMP280_ADDRESS 0x76

bool BMP280Sensor::isReady;

Adafruit_BMP280 BMP280Sensor::sensor;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void BMP280Sensor::setup()
{
  Serial.print("BMP280: setup... ");

  isReady = sensor.begin(BMP280_ADDRESS);

  Serial.println(isReady ? "done" : "failed");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void BMP280Sensor::measure()
{
  if (!isReady) {
    return;
  }

  taskENTER_CRITICAL();
  float t = sensor.readTemperature();
  float p = sensor.readPressure();
  float a = sensor.readAltitude(1013.25); // this should be adjusted to your local forcase
  taskEXIT_CRITICAL();

  Serial.print("BMP280: temperature = ");
  Serial.print(t);
  Serial.println(" *C");

  Serial.print("BMP280: pressure = ");
  Serial.print(p);
  Serial.println(" Pa");

  Serial.print("BMP280: approx altitude = ");
  Serial.print(a);
  Serial.println(" m");
}
