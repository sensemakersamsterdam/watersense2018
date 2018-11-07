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

#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>

#include "../periph/I2C.h"
#include "../periph/WDT.h"
#include "../util/Collection.h"
#include "MS5803.h"


/*******************************************************************************
 * State
 ******************************************************************************/

static MS5803 sensor(ADDRESS_HIGH);


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool MS5803_setup()
{
  Wire.beginTransmission(ADDRESS_HIGH);
  if (Wire.endTransmission()) { return false; }

  sensor.reset();
  sensor.begin();

  LOG_SETUP_RESULT_TEXT(true);

  return true;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

float MS5803_measurePressure()
{
  float values[MS5803_PR_ATTEMPTS];

  for (uint8_t i = 0; i < MS5803_PR_ATTEMPTS; i++) {
    WDT_reset();
    values[i] = sensor.getPressure(ADC_4096);
  }

  return MS5803_PR_CALIB_OFFSET + MS5803_PR_CALIB_COEFF * median(values, MS5803_PR_ATTEMPTS);
}

float MS5803_measureTemperature()
{
  float values[MS5803_TE_ATTEMPTS];

  for (uint8_t i = 0; i < MS5803_TE_ATTEMPTS; i++) {
    WDT_reset();
    values[i] = sensor.getTemperature(CELSIUS, ADC_512);
  }

  return MS5803_TE_CALIB_OFFSET + MS5803_TE_CALIB_COEFF * median(values, MS5803_TE_ATTEMPTS);
}
