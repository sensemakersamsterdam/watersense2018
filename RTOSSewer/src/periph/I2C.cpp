#include "I2C.h"

#if USE_I2C

#include <Wire.h>


/*******************************************************************************
 * State
 ******************************************************************************/

static SemaphoreHandle_t busMutex;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void I2C_setup()
{
  static StaticSemaphore_t busMutexBuffer;

  Wire.begin();

  busMutex = xSemaphoreCreateMutexStatic(&busMutexBuffer);

  #if USE_LOGGER_I2C
  LOGS("started");
  #endif

  #if USE_LOGGER_I2C_DEVICES
  I2C_logDevices();
  #endif
}


/*******************************************************************************
 * Public
 ******************************************************************************/

BaseType_t I2C_lock()
{
  if (xSemaphoreTake(busMutex, 200)) { return true; }

  #if USE_LOGGER_I2C
  LOGS("resource is busy");
  #endif

  return false;
}

void I2C_unlock()
{
  xSemaphoreGive(busMutex);
}


/*******************************************************************************
 * Private
 ******************************************************************************/

#if USE_LOGGER_I2C_DEVICES
static void I2C_logDevices()
{
  LOGS("scanning...");

  bool b = false;

  for (uint8_t address = 1; address < 127; address++)
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.

    if (!I2C_lock()) { break; }

    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();

    I2C_unlock();

    if (error == 0) {
      b = true;
      LOG(VS("device found at address 0x"), VUI8H02(address));
    } else if (error == 4) {
      LOG(VS("unknow error at address 0x"), VUI8H02(address));
    }
  }

  if (b) {
    LOGS("scanning done");
  } else {
    LOGS("no devices found");
  }
}
#endif // USE_LOGGER_I2C_DEVICES

#endif // USE_I2C
