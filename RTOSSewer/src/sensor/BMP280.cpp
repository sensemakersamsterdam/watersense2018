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

  if (BMP280_isReady) {
    LOGS("started");
  } else {
    LOGS("failed");
  }
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

  LOG(VS("temperature = "), VF(t), VS(" *C"));
  LOG(VS("pressure = "),    VF(p), VS(" Pa"));
  LOG(VS("altitude = "),    VF(a), VS(" m"));
}
