/*
RTOSSewer project: sensing water quality in a variety of ways.

Copyright (C) 2018 Sensemakers Amsterdam. All rights reserved.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <Adafruit_BMP280.h>

#include "../periph/I2C.h"
#include "BMP280.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

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

  if (b) { vTaskDelay(pdMS_TO_TICKS(200)); }

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

float BMP280_measurePressure() { return sensor.readPressure(); }

float BMP280_measureTemperature() { return sensor.readTemperature(); }
