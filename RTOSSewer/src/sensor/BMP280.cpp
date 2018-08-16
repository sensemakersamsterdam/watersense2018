#include "BMP280.h"

#if USE_BMP280

#include <Adafruit_BMP280.h>
#include "../periph/I2C.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LOCAL_BAROMETRIC_PRESSURE 1013.25f
#define BMP280_ADDRESS_CLONE      0x76
#define BMP280_ADDRESS_ORIGINAL   0x77


/*******************************************************************************
 * State
 ******************************************************************************/

bool  BMP280_active      = false;
float BMP280_temperature = 0;
float BMP280_pressure    = 0;
float BMP280_altitude    = 0;

static Adafruit_BMP280 sensor;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void BMP280_setup()
{
  Wire.beginTransmission(BMP280_ADDRESS_CLONE);
  uint8_t error = Wire.endTransmission();

  BMP280_active = sensor.begin(error == 0 ? BMP280_ADDRESS_CLONE : BMP280_ADDRESS_ORIGINAL);

  LOG_SETUP_RESULT_TEXT(BMP280_active);
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void BMP280_measure()
{
  if (!BMP280_active) { return; }

  BMP280_temperature = sensor.readTemperature();
  BMP280_pressure    = sensor.readPressure();
  BMP280_altitude    = sensor.readAltitude(LOCAL_BAROMETRIC_PRESSURE);
}

#endif // USE_BMP280
