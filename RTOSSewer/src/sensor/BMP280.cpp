#include <Adafruit_BMP280.h>

#include "../periph/I2C.h"
#include "BMP280.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LOCAL_BAROMETRIC_PRESSURE 1013.25f
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

  LOG_SETUP_RESULT_TEXT(b);

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

float BMP280_measureAltitude() { return sensor.readAltitude(LOCAL_BAROMETRIC_PRESSURE); }

float BMP280_measurePressure() { return sensor.readPressure(); }

float BMP280_measureTemperature() { return sensor.readTemperature(); }
