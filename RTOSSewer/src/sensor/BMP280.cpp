#include "../periph/I2C.h"
#include "BMP280.h"

#define BMP280_ADDRESS 0x76


/*******************************************************************************
 * State
 ******************************************************************************/

bool BMP280::isReady;

Adafruit_BMP280 BMP280::sensor;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void BMP280::setup()
{
  Serial.print("BMP280: setup... ");

  isReady = false;

  if (I2C::lock()) {
    isReady = sensor.begin(BMP280_ADDRESS);
    I2C::unlock();
  }

  Serial.println(isReady ? "done" : "failed");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void BMP280::measure()
{
  Serial.println("BMP280: reading a measurement... ");

  if (!I2C::lock()) { return; }

  float t = sensor.readTemperature();
  float p = sensor.readPressure();
  float a = sensor.readAltitude(1013.25); // this should be adjusted to your local forcase

  I2C::unlock();

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
