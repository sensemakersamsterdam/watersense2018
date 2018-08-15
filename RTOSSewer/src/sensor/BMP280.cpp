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
  Wire.beginTransmission(BMP280_ADDRESS_CLONE);
  uint8_t error = Wire.endTransmission();

  bool b = sensor.begin(error == 0 ? BMP280_ADDRESS_CLONE : BMP280_ADDRESS_ORIGINAL);

  LOG_SHOW_SETUP_RESULT(b);

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void BMP280_measure()
{
  #if USE_LOGGER
  LOGS("reading a measurement...");

  float t = sensor.readTemperature();
  float p = sensor.readPressure();
  float a = sensor.readAltitude(LOCAL_BAROMETRIC_PRESSURE);

  LOG(VS("temperature = "), VF(t), VS(" *C"));
  LOG(VS("pressure = "),    VF(p), VS(" Pa"));
  LOG(VS("altitude = "),    VF(a), VS(" m"));
  #endif
}

#endif // USE_BMP280
