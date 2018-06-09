#include <Wire.h>
#include "I2C.h"


/*******************************************************************************
 * State
 ******************************************************************************/

SemaphoreHandle_t I2C::busMutex;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void I2C::setup()
{
  static StaticSemaphore_t busMutexBuffer;

  Wire.begin();

  busMutex = xSemaphoreCreateMutexStatic(&busMutexBuffer);

  LOGS("started");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

bool I2C::lock()
{
  return xSemaphoreTake(busMutex, 100) == pdTRUE;
}

void I2C::scan()
{
  LOGS("scanning...");

  bool b = false;

  for (uint8_t address = 1; address < 127; address++)
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.

    if (!lock()) { break; }

    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();

    unlock();

    if (error == 0) {
      b = true;
      LOGT("device found at address 0x%02X", address);
    } else if (error == 4) {
      LOGT("unknow error at address 0x%02X", address);
    }
  }

  LOGS(b ? "scanning done" : "no devices found");
}

void I2C::unlock()
{
  xSemaphoreGive(busMutex);
}
