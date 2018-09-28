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

#include <OneWire.h>
#include <DallasTemperature.h>

#include "DS18B20.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PIN_ONEWIREBUS 6
#define DEVICE_INDEX   0 // device 1 => index 0


/*******************************************************************************
 * State
 ******************************************************************************/

static OneWire oneWire(PIN_ONEWIREBUS);
static DallasTemperature sensors(&oneWire);


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool DS18B20_setup()
{
  sensors.begin();

  DeviceAddress deviceAddress;
  bool b = sensors.getAddress(deviceAddress, DEVICE_INDEX);

  LOG_SETUP_RESULT_TEXT(b);

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

float DS18B20_measureTemperature()
{
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(DEVICE_INDEX);
}
