#include <Wire.h>

#include "I2C.h"


/*******************************************************************************
 * Public
 ******************************************************************************/

void I2C_disable()
{
  Wire.end();

  LOGS("disabled");
}

void I2C_enable()
{
  Wire.begin();

  LOGS("enabled");
}

void I2C_logDevices()
{
  LOGS("scanning...");

  bool b = false;

  I2C_enable();

  for (uint8_t address = 1; address < 127; address++) {
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

  I2C_disable();

  if (b) { LOGS("scanning done"); } else { LOGS("no devices found"); }
}
