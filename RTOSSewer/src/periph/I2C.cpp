#include "I2C.h"

#include <Wire.h>


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void I2C_setup()
{
  Wire.begin();

  LOG_SHOW_SETUP_RESULT(true);

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

    if (error == 0) {
      b = true;
      LOG(VS("device found at address 0x"), VUI8H02(address));
    } else if (error == 4) {
      LOG(VS("unknow error at address 0x"), VUI8H02(address));
    }
  }

  if (b) { LOGS("scanning done"); } else { LOGS("no devices found"); }
}
