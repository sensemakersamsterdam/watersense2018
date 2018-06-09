#include <Adafruit_BMP280.h>
#include "../periph/I2C.h"
#include "BMP280.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BMP280_ADDRESS 0x76


/*******************************************************************************
 * State
 ******************************************************************************/

BaseType_t BMP280_isReady;

static Adafruit_BMP280 sensor;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void BMP280_setup()
{
  BMP280_isReady = false;

  if (I2C_lock()) {
    BMP280_isReady = sensor.begin(BMP280_ADDRESS);
    I2C_unlock();
  }

  LOGS(BMP280_isReady ? "started" : "failed");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void BMP280_measure()
{
  LOGS("reading a measurement...");

  if (!I2C_lock()) { return; }

  float t = sensor.readTemperature();
  float p = sensor.readPressure();
  float a = sensor.readAltitude(1013.25); // this should be adjusted to your local forcase

  I2C_unlock();

  LOGT("temperature = %d.%02d *C", (int)t, FRAC02(t));
  LOGT("pressure = %d.%02d Pa",    (int)p, FRAC02(p));
  LOGT("altitude = %d.%02d m",     (int)a, FRAC02(a));
}
