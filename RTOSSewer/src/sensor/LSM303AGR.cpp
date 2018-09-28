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

#include <Sodaq_LSM303AGR.h>

#include "../util/Collection.h"
#include "LSM303AGR.h"


/*******************************************************************************
 * State
 ******************************************************************************/

static Sodaq_LSM303AGR accel;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool LSM303AGR_setup()
{
  bool b = accel.checkWhoAmI();

  if (b) {
    accel.disableAccelerometer();
    accel.disableMagnetometer();
  }

  LOG_SETUP_RESULT_TEXT(b);

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

bool LSM303AGR_isActive() { return accel.checkWhoAmI(); }

int8_t LSM303AGR_measureTemperature()
{
  int8_t values[LSM303AGR_TE_ATTEMPTS];

  accel.enableAccelerometer(Sodaq_LSM303AGR::LowPowerMode, Sodaq_LSM303AGR::HrNormalLowPower100Hz,
                            Sodaq_LSM303AGR::XYZ, Sodaq_LSM303AGR::Scale2g, true);

  vTaskDelay(pdMS_TO_TICKS(30));

  for (uint8_t i = 0; i < LSM303AGR_TE_ATTEMPTS; i++) { values[i] = accel.getTemperature(); }

  accel.disableAccelerometer();

  return LSM303AGR_TE_CALIB_OFFSET + LSM303AGR_TE_CALIB_COEFF * median(values, LSM303AGR_TE_ATTEMPTS);
}
