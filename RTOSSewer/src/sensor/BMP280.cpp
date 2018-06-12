#include "BMP280.h"

#if USE_BMP280

#include <Adafruit_BMP280.h>
#include "../periph/I2C.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BMP280_ADDRESS 0x76


/*******************************************************************************
 * State
 ******************************************************************************/

static Adafruit_BMP280 sensor;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

uint8_t BMP280_setup()
{
  uint8_t b = false;

  if (I2C_lock()) {
    b = sensor.begin(BMP280_ADDRESS);
    I2C_unlock();
  }

  #if USE_LOGGER_BMP280
  if (b) {
    LOGS("started");
  } else {
    LOGS("failed");
  }
  #endif

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void BMP280_measure()
{
  #if USE_LOGGER_BMP280
  LOGS("reading a measurement...");

  if (!I2C_lock()) { return; }

  float t = sensor.readTemperature();
  float p = sensor.readPressure();
  float a = sensor.readAltitude(1013.25); // this should be adjusted to your local forcase

  I2C_unlock();

  LOG(VS("temperature = "), VF(t), VS(" *C"));
  LOG(VS("pressure = "),    VF(p), VS(" Pa"));
  LOG(VS("altitude = "),    VF(a), VS(" m"));
  #endif
}

#endif // USE_BMP280
