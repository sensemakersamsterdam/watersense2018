#include "I2C.h"

#if USE_I2C

#include <Wire.h>


/*******************************************************************************
 * Private declarations
 ******************************************************************************/

static void I2C_logDevices();


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void I2C_setup()
{
  Wire.begin();

  LOG_SETUP_RESULT_TEXT(true);

  #if USE_LOGGER
  I2C_logDevices();
  #endif
}


/*******************************************************************************
 * Private
 ******************************************************************************/

static void I2C_logDevices()
{
  LOGS("scanning...");

  bool b = false;

  for (uint8_t address = 1; address < 127; address++)
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.

    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();

    if (error == 0) { b = true; }

    if (error == 0 || error == 4) {
      LOG(error == 0 ? VS("device found at address 0x") : VS("unknow error at address 0x"),
          VUI8H02(address),
          address == 0x19                    ? VS(": LSM303AGR accelerometer") : (void)NULL,
          address == 0x1E                    ? VS(": LSM303AGR magnetometer")  : (void)NULL,
          address == 0x29                    ? VS(": VL53L0X")                 : (void)NULL,
          address == 0x50                    ? VS(": FDC1004")                 : (void)NULL,
          address == 0x76 || address == 0x77 ? VS(": BMP280")                  : (void)NULL
      );
    }
  }

  if (b) { LOGS("scanning done"); } else { LOGS("no devices found"); }
}

#endif // USE_I2C
