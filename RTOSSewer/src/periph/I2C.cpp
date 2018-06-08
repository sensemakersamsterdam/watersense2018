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
}


/*******************************************************************************
 * Public
 ******************************************************************************/

bool I2C::lock()
{
  return xSemaphoreTake(busMutex, 100) == pdTRUE;
}

void I2C::logDevices()
{
  Serial.println("I2C: scanning...");

  int nDevices = 0;

  for(byte address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.

    if (!lock()) { break; }

    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    unlock();

    if (error == 0) {
      Serial.print("I2C: device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }

      Serial.println(address, HEX);

      nDevices++;
    } else if (error == 4) {
      Serial.print("I2C: unknow error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }

      Serial.println(address, HEX);
    }
  }

  Serial.println(nDevices ? "I2C: scanning done" : "I2C: no devices found");
}

void I2C::unlock()
{
  xSemaphoreGive(busMutex);
}
