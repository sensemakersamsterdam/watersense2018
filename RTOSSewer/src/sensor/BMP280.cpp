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
  isReady = false;

  if (I2C::lock()) {
    isReady = sensor.begin(BMP280_ADDRESS);
    I2C::unlock();
  }

  LOGA(isReady ? "started" : "failed");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void BMP280::measure()
{
  LOGA("reading a measurement...");

  if (!I2C::lock()) { return; }
  float t = sensor.readTemperature();
  float p = sensor.readPressure();
  float a = sensor.readAltitude(1013.25); // this should be adjusted to your local forcase
  I2C::unlock();

  LOGT("temperature = %d.%02d *C", (int)t, FRAC02(t));
  LOGT("pressure = %d.%02d Pa",    (int)p, FRAC02(p));
  LOGT("altitude = %d.%02d m",     (int)a, FRAC02(a));
}
