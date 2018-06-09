#include <Wire.h>
#include "I2C.h"


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

  LOGS("started");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

BaseType_t I2C_lock()
{
  BaseType_t b = xSemaphoreTake(busMutex, 100);

  if (!b) { LOGS("resource is busy"); }

  return b;
}

void I2C_logDevices()
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
      LOGT("device found at address 0x%02X", address);
    } else if (error == 4) {
      LOGT("unknow error at address 0x%02X", address);
    }
  }

  LOGS(b ? "scanning done" : "no devices found");
}

void I2C_unlock()
{
  xSemaphoreGive(busMutex);
}
