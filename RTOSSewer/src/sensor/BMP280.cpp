#include "BMP280.h"

#if USE_BMP280

#include <Adafruit_BMP280.h>
#include "../periph/I2C.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LOCAL_BAROMETRIC_PRESSURE 1013.25
#define BMP280_ADDRESS_CLONE      0x76
#define BMP280_ADDRESS_ORIGINAL   0x77


/*******************************************************************************
 * State
 ******************************************************************************/

static Adafruit_BMP280 sensor;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool BMP280_setup()
{
  bool b = false;

  if (I2C_lock()) {
    Wire.beginTransmission(BMP280_ADDRESS_CLONE);
    int32_t error = Wire.endTransmission();

    b = sensor.begin(error == 0 ? BMP280_ADDRESS_CLONE : BMP280_ADDRESS_ORIGINAL);
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
  float a = sensor.readAltitude(LOCAL_BAROMETRIC_PRESSURE);

  I2C_unlock();

  LOG(VS("temperature = "), VF(t), VS(" *C"));
  LOG(VS("pressure = "),    VF(p), VS(" Pa"));
  LOG(VS("altitude = "),    VF(a), VS(" m"));
  #endif
}

#endif // USE_BMP280
